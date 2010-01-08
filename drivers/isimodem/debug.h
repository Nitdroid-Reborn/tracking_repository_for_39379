/*
 * This file is part of oFono - Open Source Telephony
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef __ISIMODEM_DEBUG_H
#define __ISIMODEM_DEBUG_H

#include "ss.h"
#include "mtc.h"
#include "sms.h"
#include "sim.h"
#include "info.h"
#include "call.h"
#include "network.h"

const char *ss_message_id_name(enum ss_message_id value);
const char *ss_subblock_name(enum ss_subblock value);

const char *mtc_isi_cause_name(enum mtc_isi_cause value);
const char *mtc_message_id_name(enum mtc_message_id value);
const char *mtc_modem_state_name(enum mtc_modem_state value);

const char *sms_gsm_cause_name(enum sms_gsm_cause value);
const char *sms_isi_cause_name(enum sms_isi_cause value);
const char *sms_message_id_name(enum sms_message_id value);
const char *sms_subblock_name(enum sms_subblock value);

const char *sim_isi_cause_name(enum sim_isi_cause value);
const char *sim_message_id_name(enum sim_message_id value);
const char *sim_subblock_name(enum sim_subblock value);

const char *info_isi_cause_name(enum info_isi_cause value);
const char *info_message_id_name(enum info_message_id value);
const char *info_subblock_name(enum info_subblock value);

const char *call_gsm_cause_name(enum call_gsm_cause value);
const char *call_isi_cause_name(enum call_isi_cause value);
const char *call_status_name(enum call_status value);
const char *call_message_id_name(enum call_message_id value);

const char *net_gsm_cause_name(enum net_gsm_cause value);
const char *net_isi_cause_name(enum net_isi_cause value);
const char *net_status_name(enum net_reg_status value);
const char *net_message_id_name(enum net_message_id value);
const char *net_subblock_name(enum net_subblock value);

void ss_debug(const void *restrict buf, size_t len, void *data);
void mtc_debug(const void *restrict buf, size_t len, void *data);
void sms_debug(const void *restrict buf, size_t len, void *data);
void sim_debug(const void *restrict buf, size_t len, void *data);
void info_debug(const void *restrict buf, size_t len, void *data);
void call_debug(const void *restrict buf, size_t len, void *data);
void net_debug(const void *restrict buf, size_t len, void *data);

const char *pn_resource_name(int value);

#endif /* __ISIMODEM_DEBUG_H */