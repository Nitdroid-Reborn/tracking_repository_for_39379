/*
 *
 *  oFono - Open Source Telephony
 *
 *  Copyright (C) 2008-2010  Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>

#include <glib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>

#include "ofono.h"

#include "simfs.h"
#include "simutil.h"
#include "storage.h"

#define SIM_CACHE_MODE 0600
#define SIM_CACHE_BASEPATH STORAGEDIR "/%s-%i"
#define SIM_CACHE_VERSION SIM_CACHE_BASEPATH "/version"
#define SIM_CACHE_PATH SIM_CACHE_BASEPATH "/%04x"
#define SIM_CACHE_HEADER_SIZE 38
#define SIM_FILE_INFO_SIZE 6

#define SIM_FS_VERSION 1

static gboolean sim_fs_op_next(gpointer user_data);
static gboolean sim_fs_op_read_record(gpointer user);
static gboolean sim_fs_op_read_block(gpointer user_data);

struct sim_fs_op {
	int id;
	enum ofono_sim_file_structure structure;
	unsigned short offset;
	int num_bytes;
	int length;
	int record_length;
	int current;
	gconstpointer cb;
	gboolean is_read;
	void *userdata;
};

static void sim_fs_op_free(struct sim_fs_op *node)
{
	g_free(node);
}

struct sim_fs {
	GQueue *op_q;
	gint op_source;
	unsigned char bitmap[32];
	int fd;
	unsigned char *buffer;
	struct ofono_sim *sim;
	const struct ofono_sim_driver *driver;
};

void sim_fs_free(struct sim_fs *fs)
{
	if (fs->op_source) {
		g_source_remove(fs->op_source);
		fs->op_source = 0;
	}

	/*
	 * Note: users of sim_fs must not assume that the callback happens
	 * for operations still in progress
	 */
	g_queue_foreach(fs->op_q, (GFunc)sim_fs_op_free, NULL);
	g_queue_free(fs->op_q);

	g_free(fs);
}

struct sim_fs *sim_fs_new(struct ofono_sim *sim,
				const struct ofono_sim_driver *driver)
{
	struct sim_fs *fs;

	fs = g_try_new0(struct sim_fs, 1);
	if (fs == NULL)
		return NULL;

	fs->sim = sim;
	fs->driver = driver;
	fs->fd = -1;

	return fs;
}

static void sim_fs_end_current(struct sim_fs *fs)
{
	struct sim_fs_op *op = g_queue_pop_head(fs->op_q);

	if (g_queue_get_length(fs->op_q) > 0)
		fs->op_source = g_idle_add(sim_fs_op_next, fs);

	if (fs->fd != -1) {
		TFR(close(fs->fd));
		fs->fd = -1;
	}

	g_free(fs->buffer);
	fs->buffer = NULL;

	memset(fs->bitmap, 0, sizeof(fs->bitmap));

	sim_fs_op_free(op);
}

static void sim_fs_op_error(struct sim_fs *fs)
{
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);

	if (op->is_read == TRUE)
		((ofono_sim_file_read_cb_t) op->cb)
			(0, 0, 0, 0, 0, op->userdata);
	else
		((ofono_sim_file_write_cb_t) op->cb)
			(0, op->userdata);

	sim_fs_end_current(fs);
}

static gboolean cache_block(struct sim_fs *fs, int block, int block_len,
				const unsigned char *data, int num_bytes)
{
	int offset;
	int bit;
	ssize_t r;
	unsigned char b;

	if (fs->fd == -1)
		return FALSE;

	if (lseek(fs->fd, block * block_len +
				SIM_CACHE_HEADER_SIZE, SEEK_SET) == (off_t) -1)
		return FALSE;

	r = TFR(write(fs->fd, data, num_bytes));

	if (r != num_bytes)
		return FALSE;

	/* update present bit for this block */
	offset = block / 8;
	bit = block % 8;

	/* lseek to correct byte (skip file info) */
	lseek(fs->fd, offset + SIM_FILE_INFO_SIZE, SEEK_SET);

	b = fs->bitmap[offset];
	b |= 1 << bit;

	r = TFR(write(fs->fd, &b, sizeof(b)));

	if (r != sizeof(b))
		return FALSE;

	fs->bitmap[offset] = b;

	return TRUE;
}

static void sim_fs_op_write_cb(const struct ofono_error *error, void *data)
{
	struct sim_fs *fs = data;
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	ofono_sim_file_write_cb_t cb = op->cb;

	if (error->type == OFONO_ERROR_TYPE_NO_ERROR)
		cb(1, op->userdata);
	else
		cb(0, op->userdata);

	sim_fs_end_current(fs);
}

static void sim_fs_op_read_block_cb(const struct ofono_error *error,
					const unsigned char *data, int len,
					void *user)
{
	struct sim_fs *fs = user;
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	int start_block;
	int end_block;
	int bufoff;
	int dataoff;
	int tocopy;

	if (error->type != OFONO_ERROR_TYPE_NO_ERROR) {
		sim_fs_op_error(fs);
		return;
	}

	start_block = op->offset / 256;
	end_block = (op->offset + (op->num_bytes - 1)) / 256;

	if (op->current == start_block) {
		bufoff = 0;
		dataoff = op->offset % 256;
		tocopy = MIN(256 - op->offset % 256, len);
	} else {
		bufoff = (op->current - start_block - 1) * 256 +
				op->offset % 256;
		dataoff = 0;
		tocopy = len;
	}

	memcpy(fs->buffer + bufoff, data + dataoff, tocopy);
	cache_block(fs, op->current, 256, data, len);

	op->current++;

	if (op->current > end_block) {
		ofono_sim_file_read_cb_t cb = op->cb;

		cb(1, op->num_bytes, 0, fs->buffer,
				op->record_length, op->userdata);

		sim_fs_end_current(fs);
	} else {
		fs->op_source = g_idle_add(sim_fs_op_read_block, fs);
	}
}

static gboolean sim_fs_op_read_block(gpointer user_data)
{
	struct sim_fs *fs = user_data;
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	int start_block;
	int end_block;
	unsigned short read_bytes;

	start_block = op->offset / 256;
	end_block = (op->offset + (op->num_bytes - 1)) / 256;

	if (op->current == start_block) {
		fs->buffer = g_try_new0(unsigned char, op->num_bytes);

		if (fs->buffer == NULL) {
			sim_fs_op_error(fs);
			return FALSE;
		}
	}

	while (fs->fd != -1 && op->current <= end_block) {
		int offset = op->current / 8;
		int bit = 1 << op->current % 8;
		int bufoff;
		int seekoff;
		int toread;

		if ((fs->bitmap[offset] & bit) == 0)
			break;

		if (op->current == start_block) {
			bufoff = 0;
			seekoff = SIM_CACHE_HEADER_SIZE + op->current * 256 +
				op->offset % 256;
			toread = MIN(256 - op->offset % 256,
					op->length - op->current * 256);
		} else {
			bufoff = (op->current - start_block - 1) * 256 +
					op->offset % 256;
			seekoff = SIM_CACHE_HEADER_SIZE + op->current * 256;
			toread = MIN(256, op->length - op->current * 256);
		}

		if (lseek(fs->fd, seekoff, SEEK_SET) == (off_t) -1)
			break;

		if (TFR(read(fs->fd, fs->buffer + bufoff, toread)) != toread)
			break;

		op->current += 1;
	}

	if (op->current > end_block) {
		ofono_sim_file_read_cb_t cb = op->cb;

		cb(1, op->num_bytes, 0, fs->buffer,
				op->record_length, op->userdata);

		sim_fs_end_current(fs);

		return FALSE;
	}

	if (fs->driver->read_file_transparent == NULL) {
		sim_fs_op_error(fs);
		return FALSE;
	}

	read_bytes = MIN(op->length - op->current * 256, 256);
	fs->driver->read_file_transparent(fs->sim, op->id,
						op->current * 256,
						read_bytes,
						sim_fs_op_read_block_cb, fs);

	return FALSE;
}

static void sim_fs_op_retrieve_cb(const struct ofono_error *error,
					const unsigned char *data, int len,
					void *user)
{
	struct sim_fs *fs = user;
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	int total = op->length / op->record_length;
	ofono_sim_file_read_cb_t cb = op->cb;

	if (error->type != OFONO_ERROR_TYPE_NO_ERROR) {
		sim_fs_op_error(fs);
		return;
	}

	cb(1, op->length, op->current, data, op->record_length, op->userdata);

	cache_block(fs, op->current - 1, op->record_length,
			data, op->record_length);

	if (op->current < total) {
		op->current += 1;
		fs->op_source = g_idle_add(sim_fs_op_read_record, fs);
	} else {
		sim_fs_end_current(fs);
	}
}

static gboolean sim_fs_op_read_record(gpointer user)
{
	struct sim_fs *fs = user;
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	const struct ofono_sim_driver *driver = fs->driver;
	int total = op->length / op->record_length;
	unsigned char buf[256];

	fs->op_source = 0;

	while (fs->fd != -1 && op->current <= total) {
		int offset = (op->current - 1) / 8;
		int bit = 1 << ((op->current - 1) % 8);
		ofono_sim_file_read_cb_t cb = op->cb;

		if ((fs->bitmap[offset] & bit) == 0)
			break;

		if (lseek(fs->fd, (op->current - 1) * op->record_length +
				SIM_CACHE_HEADER_SIZE, SEEK_SET) == (off_t) -1)
			break;

		if (TFR(read(fs->fd, buf, op->record_length)) !=
				op->record_length)
			break;

		cb(1, op->length, op->current,
				buf, op->record_length, op->userdata);

		op->current += 1;
	}

	if (op->current > total) {
		sim_fs_end_current(fs);

		return FALSE;
	}

	switch (op->structure) {
	case OFONO_SIM_FILE_STRUCTURE_FIXED:
		if (!driver->read_file_linear) {
			sim_fs_op_error(fs);
			return FALSE;
		}

		driver->read_file_linear(fs->sim, op->id, op->current,
						op->record_length,
						sim_fs_op_retrieve_cb, fs);
		break;
	case OFONO_SIM_FILE_STRUCTURE_CYCLIC:
		if (!driver->read_file_cyclic) {
			sim_fs_op_error(fs);
			return FALSE;
		}

		driver->read_file_cyclic(fs->sim, op->id, op->current,
						op->record_length,
						sim_fs_op_retrieve_cb, fs);
		break;
	default:
		ofono_error("Unrecognized file structure, this can't happen");
	}

	return FALSE;
}

static void sim_fs_op_info_cb(const struct ofono_error *error, int length,
				enum ofono_sim_file_structure structure,
				int record_length,
				const unsigned char access[3], void *data)
{
	struct sim_fs *fs = data;
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	const char *imsi = ofono_sim_get_imsi(fs->sim);
	enum ofono_sim_phase phase = ofono_sim_get_phase(fs->sim);
	enum sim_file_access update;
	enum sim_file_access invalidate;
	enum sim_file_access rehabilitate;
	unsigned char fileinfo[SIM_CACHE_HEADER_SIZE];
	gboolean cache;
	char *path;

	if (error->type != OFONO_ERROR_TYPE_NO_ERROR) {
		sim_fs_op_error(fs);
		return;
	}

	if (structure != op->structure) {
		ofono_error("Requested file structure differs from SIM: %x",
				op->id);
		sim_fs_op_error(fs);
		return;
	}

	/* TS 11.11, Section 9.3 */
	update = file_access_condition_decode(access[0] & 0xf);
	rehabilitate = file_access_condition_decode((access[2] >> 4) & 0xf);
	invalidate = file_access_condition_decode(access[2] & 0xf);

	op->structure = structure;
	op->length = length;

	/* Never cache card holder writable files */
	cache = (update == SIM_FILE_ACCESS_ADM ||
			update == SIM_FILE_ACCESS_NEVER) &&
			(invalidate == SIM_FILE_ACCESS_ADM ||
				invalidate == SIM_FILE_ACCESS_NEVER) &&
			(rehabilitate == SIM_FILE_ACCESS_ADM ||
				rehabilitate == SIM_FILE_ACCESS_NEVER);

	if (structure == OFONO_SIM_FILE_STRUCTURE_TRANSPARENT) {
		if (op->num_bytes == 0)
			op->num_bytes = op->length;

		op->record_length = length;
		op->current = op->offset / 256;
		fs->op_source = g_idle_add(sim_fs_op_read_block, fs);
	} else {
		op->record_length = record_length;
		op->current = 1;
		fs->op_source = g_idle_add(sim_fs_op_read_record, fs);
	}

	if (imsi == NULL || cache == FALSE)
		return;

	memset(fileinfo, 0, SIM_CACHE_HEADER_SIZE);

	fileinfo[0] = error->type;
	fileinfo[1] = length >> 8;
	fileinfo[2] = length & 0xff;
	fileinfo[3] = structure;
	fileinfo[4] = record_length >> 8;
	fileinfo[5] = record_length & 0xff;

	path = g_strdup_printf(SIM_CACHE_PATH, imsi, phase, op->id);
	fs->fd = TFR(open(path, O_RDWR | O_CREAT | O_TRUNC, SIM_CACHE_MODE));
	g_free(path);

	if (fs->fd == -1)
		return;

	if (TFR(write(fs->fd, fileinfo, SIM_CACHE_HEADER_SIZE)) ==
			SIM_CACHE_HEADER_SIZE)
		return;

	TFR(close(fs->fd));
	fs->fd = -1;
}

static gboolean sim_fs_op_check_cached(struct sim_fs *fs)
{
	const char *imsi = ofono_sim_get_imsi(fs->sim);
	enum ofono_sim_phase phase = ofono_sim_get_phase(fs->sim);
	struct sim_fs_op *op = g_queue_peek_head(fs->op_q);
	gboolean ret = FALSE;
	char *path;
	int fd;
	ssize_t len;
	unsigned char fileinfo[SIM_CACHE_HEADER_SIZE];
	int error_type;
	int file_length;
	enum ofono_sim_file_structure structure;
	int record_length;

	if (!imsi)
		return FALSE;

	path = g_strdup_printf(SIM_CACHE_PATH, imsi, phase, op->id);

	if (path == NULL)
		return FALSE;

	fd = TFR(open(path, O_RDWR));
	g_free(path);

	if (fd == -1) {
		if (errno != ENOENT)
			DBG("Error %i opening cache file for "
					"fileid %04x, IMSI %s",
					errno, op->id, imsi);

		return FALSE;
	}

	len = TFR(read(fd, fileinfo, SIM_CACHE_HEADER_SIZE));

	if (len != SIM_CACHE_HEADER_SIZE)
		goto error;

	error_type = fileinfo[0];
	file_length = (fileinfo[1] << 8) | fileinfo[2];
	structure = fileinfo[3];
	record_length = (fileinfo[4] << 8) | fileinfo[5];

	if (structure == OFONO_SIM_FILE_STRUCTURE_TRANSPARENT)
		record_length = file_length;

	if (record_length == 0 || file_length < record_length)
		goto error;

	op->length = file_length;
	op->record_length = record_length;
	memcpy(fs->bitmap, fileinfo + SIM_FILE_INFO_SIZE,
			SIM_CACHE_HEADER_SIZE - SIM_FILE_INFO_SIZE);
	fs->fd = fd;

	if (error_type != OFONO_ERROR_TYPE_NO_ERROR ||
			structure != op->structure)
		sim_fs_op_error(fs);

	if (structure == OFONO_SIM_FILE_STRUCTURE_TRANSPARENT) {
		if (op->num_bytes == 0)
			op->num_bytes = op->length;

		op->current = op->offset / 256;
		fs->op_source = g_idle_add(sim_fs_op_read_block, fs);
	} else {
		op->current = 1;
		fs->op_source = g_idle_add(sim_fs_op_read_record, fs);
	}

	return TRUE;

error:
	TFR(close(fd));
	return ret;
}

static gboolean sim_fs_op_next(gpointer user_data)
{
	struct sim_fs *fs = user_data;
	const struct ofono_sim_driver *driver = fs->driver;
	struct sim_fs_op *op;

	fs->op_source = 0;

	if (!fs->op_q)
		return FALSE;

	op = g_queue_peek_head(fs->op_q);

	if (op->is_read == TRUE) {
		if (sim_fs_op_check_cached(fs))
			return FALSE;

		driver->read_file_info(fs->sim, op->id, sim_fs_op_info_cb, fs);
	} else {
		switch (op->structure) {
		case OFONO_SIM_FILE_STRUCTURE_TRANSPARENT:
			driver->write_file_transparent(fs->sim, op->id, 0,
					op->length, fs->buffer,
					sim_fs_op_write_cb, fs);
			break;
		case OFONO_SIM_FILE_STRUCTURE_FIXED:
			driver->write_file_linear(fs->sim, op->id, op->current,
					op->length, fs->buffer,
					sim_fs_op_write_cb, fs);
			break;
		case OFONO_SIM_FILE_STRUCTURE_CYCLIC:
			driver->write_file_cyclic(fs->sim, op->id,
					op->length, fs->buffer,
					sim_fs_op_write_cb, fs);
			break;
		default:
			ofono_error("Unrecognized file structure, "
					"this can't happen");
		}

		g_free(fs->buffer);
		fs->buffer = NULL;
	}

	return FALSE;
}

int sim_fs_read(struct sim_fs *fs, int id,
		enum ofono_sim_file_structure expected_type,
		unsigned short offset, unsigned short num_bytes,
		ofono_sim_file_read_cb_t cb, void *data)
{
	struct sim_fs_op *op;

	if (!cb)
		return -1;

	if (!fs->driver)
		return -1;

	if (!fs->driver->read_file_info)
		return -1;

	if (!fs->op_q)
		fs->op_q = g_queue_new();

	op = g_new0(struct sim_fs_op, 1);

	op->id = id;
	op->structure = expected_type;
	op->cb = cb;
	op->userdata = data;
	op->is_read = TRUE;
	op->offset = offset;
	op->num_bytes = num_bytes;

	g_queue_push_tail(fs->op_q, op);

	if (g_queue_get_length(fs->op_q) == 1)
		fs->op_source = g_idle_add(sim_fs_op_next, fs);

	return 0;
}

int sim_fs_write(struct sim_fs *fs, int id, ofono_sim_file_write_cb_t cb,
			enum ofono_sim_file_structure structure, int record,
			const unsigned char *data, int length, void *userdata)
{
	struct sim_fs_op *op;
	gconstpointer fn = NULL;

	if (!cb)
		return -1;

	if (!fs->driver)
		return -1;

	switch (structure) {
	case OFONO_SIM_FILE_STRUCTURE_TRANSPARENT:
		fn = fs->driver->write_file_transparent;
		break;
	case OFONO_SIM_FILE_STRUCTURE_FIXED:
		fn = fs->driver->write_file_linear;
		break;
	case OFONO_SIM_FILE_STRUCTURE_CYCLIC:
		fn = fs->driver->write_file_cyclic;
		break;
	default:
		ofono_error("Unrecognized file structure, this can't happen");
	}

	if (fn == NULL)
		return -1;

	if (!fs->op_q)
		fs->op_q = g_queue_new();

	op = g_new0(struct sim_fs_op, 1);

	op->id = id;
	op->cb = cb;
	op->userdata = userdata;
	op->is_read = FALSE;
	fs->buffer = g_memdup(data, length);
	op->structure = structure;
	op->length = length;
	op->current = record;

	g_queue_push_tail(fs->op_q, op);

	if (g_queue_get_length(fs->op_q) == 1)
		fs->op_source = g_idle_add(sim_fs_op_next, fs);

	return 0;
}

static void remove_cachefile(const char *imsi, enum ofono_sim_phase phase,
				const struct dirent *file)
{
	int id;
	char *path;

	if (file->d_type != DT_REG)
		return;

	if (sscanf(file->d_name, "%4x", &id) != 1)
		return;

	path = g_strdup_printf(SIM_CACHE_PATH, imsi, phase, id);
	remove(path);
	g_free(path);
}

void sim_fs_check_version(struct sim_fs *fs)
{
	const char *imsi = ofono_sim_get_imsi(fs->sim);
	enum ofono_sim_phase phase = ofono_sim_get_phase(fs->sim);
	unsigned char version;
	struct dirent **entries;
	int len;
	char *path;

	if (read_file(&version, 1, SIM_CACHE_VERSION, imsi, phase) == 1)
		if (version == SIM_FS_VERSION)
			return;

	path = g_strdup_printf(SIM_CACHE_BASEPATH, imsi, phase);

	ofono_info("Detected old simfs version in %s, removing", path);
	len = scandir(path, &entries, NULL, alphasort);
	g_free(path);

	if (len > 0) {
		/* Remove all file ids */
		while (len--) {
			remove_cachefile(imsi, phase, entries[len]);
			g_free(entries[len]);
		}

		g_free(entries);
	}

	version = SIM_FS_VERSION;
	write_file(&version, 1, SIM_CACHE_MODE, SIM_CACHE_VERSION, imsi, phase);
}
