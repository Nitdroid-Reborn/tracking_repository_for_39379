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

enum sim_fileid {
	SIM_EFPL_FILEID = 0x2f05,
	SIM_EF_ICCID_FILEID = 0x2fe2,
	SIM_EFIMG_FILEID = 0x4F20,
	SIM_EFLI_FILEID = 0x6f05,
	SIM_EF_CPHS_MWIS_FILEID = 0x6f11,
	SIM_EF_CPHS_INFORMATION_FILEID = 0x6f16,
	SIM_EF_CPHS_MBDN_FILEID = 0x6f17,
	SIM_EFUST_FILEID = 0x6f38,
	SIM_EFSST_FILEID = 0x6f38, /* same as EFust */
	SIM_EFMSISDN_FILEID = 0x6f40,
	SIM_EFSPN_FILEID = 0x6f46,
	SIM_EFSDN_FILEID = 0x6f49,
	SIM_EFEST_FILEID = 0x6f56,
	SIM_EFAD_FILEID = 0x6fad,
	SIM_EFPHASE_FILEID = 0x6fae,
	SIM_EFPNN_FILEID = 0x6fc5,
	SIM_EFOPL_FILEID = 0x6fc6,
	SIM_EFMBDN_FILEID = 0x6fc7,
	SIM_EFMBI_FILEID = 0x6fc9,
	SIM_EFMWIS_FILEID = 0x6fca,
	SIM_EFSPDI_FILEID = 0x6fcd,
	SIM_EFECC_FILEID = 0x6fb7,
	SIM_EFCBMIR_FILEID = 0x6f50,
	SIM_EFCBMI_FILEID = 0x6f45,
	SIM_EFCBMID_FILEID = 0x6f48,
};

/* 51.011 Section 9.3 */
enum sim_file_access {
	SIM_FILE_ACCESS_ALWAYS = 0,
	SIM_FILE_ACCESS_CHV1 = 1,
	SIM_FILE_ACCESS_CHV2 = 2,
	SIM_FILE_ACCESS_RESERVED = 3,
	SIM_FILE_ACCESS_ADM = 4,
	SIM_FILE_ACCESS_NEVER = 15,
};

/* 131.102 Section 4.2.8 */
enum sim_ust_service {
	SIM_UST_SERVICE_LOCAL_PHONE_BOOK		= 0,
	SIM_UST_SERVICE_FDN				= 1,
	SIM_UST_SERVICE_EXT_2				= 2,
	SIM_UST_SERVICE_SDN				= 3,
	SIM_UST_SERVICE_EXT_3				= 4,
	SIM_UST_SERVICE_BDN				= 5,
	SIM_UST_SERVICE_EXT_4				= 6,
	SIM_UST_SERVICE_OCI_OCT				= 7,
	SIM_UST_SERVICE_ICI_ICT				= 8,
	SIM_UST_SERVICE_SMS				= 9,
	SIM_UST_SERVICE_SMSR				= 10,
	SIM_UST_SERVICE_SMSP				= 11,
	SIM_UST_SERVICE_AOC				= 12,
	SIM_UST_SERVICE_CCP2				= 13,
	SIM_UST_SERVICE_CBS_ID				= 14,
	SIM_UST_SERVICE_CBS_ID_RANGE			= 15,
	SIM_UST_SERVICE_GROUP_ID_LEVEL_1		= 16,
	SIM_UST_SERVICE_GROUP_ID_LEVEL_2		= 17,
	SIM_UST_SERVICE_PROVIDER_NAME			= 18,
	SIM_UST_SERVICE_USER_PLMN			= 19,
	SIM_UST_SERVICE_MSISDN				= 20,
	SIM_UST_SERVICE_IMG				= 21,
	SIM_UST_SERVICE_SOLSA				= 22,
	SIM_UST_SERVICE_PRECEDENCE_PREEMPTION		= 23,
	SIM_UST_SERVICE_EMLPP				= 24,
	SIM_UST_SERVICE_GSM_ACCESS			= 26,
	SIM_UST_SERVICE_DATA_DOWNLOAD_SMS_PP		= 27,
	SIM_UST_SERVICE_DATA_DOWNLOAD_SMS_CB		= 28,
	SIM_UST_SERVICE_CALL_CONTROL_USIM		= 29,
	SIM_UST_SERVICE_MO_SMS_USIM			= 30,
	SIM_UST_SERVICE_RUN_AT_COMMAND			= 31,
	SIM_UST_SERVICE_ENABLED_SERVICE_TABLE		= 33,
	SIM_UST_SERVICE_ACL				= 34,
	SIM_UST_SERVICE_DEPERSONALISATION_CTRL_KEY	= 35,
	SIM_UST_SERVICE_NETWORK_LIST			= 36,
	SIM_UST_SERVICE_GSM_SECURITY_CONTEXT		= 37,
	SIM_UST_SERVICE_CPBCCH				= 38,
	SIM_UST_SERVICE_INVESTIGATION_SCAN		= 39,
	SIM_UST_SERVICE_MEXE				= 40,
	SIM_UST_SERVICE_OPERATOR_PLMN			= 41,
	SIM_UST_SERVICE_HPLMN				= 42,
	SIM_UST_SERVICE_EXT_5				= 43,
	SIM_UST_SERVICE_PLMN_NETWORK_NAME		= 44,
	SIM_UST_SERVICE_OPERATOR_PLMN_LIST		= 45,
	SIM_UST_SERVICE_MAILBOX_DIALLING_NUMBERS	= 46,
	SIM_UST_SERVICE_MWIS				= 47,
	SIM_UST_SERVICE_CFIS				= 48,
	SIM_UST_SERVICE_PROVIDER_DISPLAY_INFO		= 50,
	SIM_UST_SERVICE_MMS				= 51,
	SIM_UST_SERVICE_EXT_8				= 52,
	SIM_UST_SERVICE_CALL_CONTROL_GPRS_USIM		= 53,
	SIM_UST_SERVICE_MMS_USER_CONN_PARAM		= 54,
	SIM_UST_SERVICE_NIA				= 55,
	SIM_UST_SERVICE_EFVGCS_EFVGCSS			= 56,
	SIM_UST_SERVICE_EFVBS_EFVBSS			= 57,
	SIM_UST_SERVICE_PSEUDONYM			= 58,
	SIM_UST_SERVICE_USER_PLMN_I_WLAN		= 59,
	SIM_UST_SERVICE_OPERATOR_PLMN_I_WLAN		= 60,
	SIM_UST_SERVICE_USER_WSID			= 61,
	SIM_UST_SERVICE_OPERATOR_WSID			= 62,
	SIM_UST_SERVICE_VGCS_SECURITY			= 63,
	SIM_UST_SERVICE_VBS_SECURITY			= 64,
	SIM_UST_SERVICE_WLAN_REAUTH_ID			= 65,
	SIM_UST_SERVICE_MMS_STORAGE			= 66,
	SIM_UST_SERVICE_GBA				= 67,
	SIM_UST_SERVICE_MBMS_SECURITY			= 68,
	SIM_UST_SERVICE_USSD_APPLICATION_MODE		= 69,
	SIM_UST_SERVICE_EQUIVALENT_HPLMN		= 70,
	SIM_UST_SERVICE_ADDITIONAL_TERMINAL_PROFILE	= 71,
	SIM_UST_SERVICE_EQUIVALENT_HPLMN_IND		= 72,
	SIM_UST_SERVICE_LAST_RPLMN_IND			= 73,
	SIM_UST_SERVICE_OMA_BCAST_SC_PROFILE		= 74,
	SIM_UST_SERVICE_BGA_LOCAL_KEY			= 75,
	SIM_UST_SERVICE_TERMINAL_APPLICATIONS		= 76,
	SIM_UST_SERVICE_PROVIDER_NAME_ICON		= 77,
	SIM_UST_SERVICE_PLMN_NETWORK_NAME_ICON		= 78,
	SIM_UST_SERVICE_CONN_PARAM_USIM_IP		= 79,
	SIM_UST_SERVICE_HOME_I_WLAN_ID_LIST		= 80,
	SIM_UST_SERVICE_I_WLAN_EQUIVALENT_HPLMN_IND	= 81,
	SIM_UST_SERVICE_I_WLAN_HPLMN_PRIORITY_IND	= 82,
	SIM_UST_SERVICE_I_WLAN_LAST_PLMN		= 83,
	SIM_UST_SERVICE_EPS_INFO			= 84,
	SIM_UST_SERVICE_CSG_IND				= 85,
	SIM_UST_SERVICE_CALL_CONTROL_EPS_PDN_USIM	= 86,
	SIM_UST_SERVICE_HPLMN_DIRECT_ACCESS		= 87,
	SIM_UST_SERVICE_ECALL_DATA			= 88,
	SIM_UST_SERVICE_OPERATOR_CSG			= 89
};

/* 131.102 Section 4.2.47 */
enum sim_est_service {
	SIM_EST_SERVICE_FDN				= 0,
	SIM_EST_SERVICE_BDN				= 1,
	SIM_EST_SERVICE_ACL				= 2
};

/* 51.011 Section 10.3.7 */
enum sim_sst_service {
	SIM_SST_SERVICE_CHV1_DISABLE			= 0,
	SIM_SST_SERVICE_ADN				= 1,
	SIM_SST_SERVICE_FDN				= 2,
	SIM_SST_SERVICE_SMS				= 3,
	SIM_SST_SERVICE_AOC				= 4,
	SIM_SST_SERVICE_CCP				= 5,
	SIM_SST_SERVICE_PLMN_SELECTOR			= 6,
	SIM_SST_SERVICE_MSISDN				= 8,
	SIM_SST_SERVICE_EXT_1				= 9,
	SIM_SST_SERVICE_EXT_2				= 10,
	SIM_SST_SERVICE_SMSP				= 11,
	SIM_SST_SERVICE_LND				= 12,
	SIM_SST_SERVICE_CBS_ID				= 13,
	SIM_SST_SERVICE_GROUP_ID_LEVEL_1		= 14,
	SIM_SST_SERVICE_GROUP_ID_LEVEL_2		= 15,
	SIM_SST_SERVICE_PROVIDER_NAME			= 16,
	SIM_SST_SERVICE_SDN				= 17,
	SIM_SST_SERVICE_EXT_3				= 18,
	SIM_SST_SERVICE_EFVGCS_EFVGCSS			= 20,
	SIM_SST_SERVICE_EFVBS_EFVBSS			= 21,
	SIM_SST_SERVICE_PRECEDENCE_PREEMPTION		= 22,
	SIM_SST_SERVICE_EMLPP				= 23,
	SIM_SST_SERVICE_DATA_DOWNLOAD_SMS_CB		= 24,
	SIM_SST_SERVICE_DATA_DOWNLOAD_SMS_PP		= 25,
	SIM_SST_SERVICE_MENU_SELECTION			= 26,
	SIM_SST_SERVICE_CALL_CONTROL			= 27,
	SIM_SST_SERVICE_PROACTIVE_SIM			= 28,
	SIM_SST_SERVICE_CBS_ID_RANGE			= 29,
	SIM_SST_SERVICE_BDN				= 30,
	SIM_SST_SERVICE_EXT_4				= 31,
	SIM_SST_SERVICE_DEPERSONALISATION_CTRL_KEY	= 32,
	SIM_SST_SERVICE_NETWORK_LIST			= 33,
	SIM_SST_SERVICE_SMSR				= 34,
	SIM_SST_SERVICE_NIA				= 35,
	SIM_SST_SERVICE_MO_SMS_SIM			= 36,
	SIM_SST_SERVICE_GPRS				= 37,
	SIM_SST_SERVICE_IMG				= 38,
	SIM_SST_SERVICE_SOLSA				= 39,
	SIM_SST_SERVICE_USSD_CALL_CONTROL		= 40,
	SIM_SST_SERVICE_RUN_AT_COMMAND			= 41,
	SIM_SST_SERVICE_USER_PLMN			= 42,
	SIM_SST_SERVICE_OPERATOR_PLMN			= 43,
	SIM_SST_SERVICE_HPLMN				= 44,
	SIM_SST_SERVICE_CPBCCH				= 45,
	SIM_SST_SERVICE_INVESTIGATION_SCAN		= 46,
	SIM_SST_SERVICE_EXT_CCP				= 47,
	SIM_SST_SERVICE_MEXE				= 48,
	SIM_SST_SERVICE_RPLMN				= 49,
	SIM_SST_SERVICE_PLMN_NETWORK_NAME		= 50,
	SIM_SST_SERVICE_OPERATOR_PLMN_LIST		= 51,
	SIM_SST_SERVICE_MAILBOX_DIALLING_NUMBERS	= 52,
	SIM_SST_SERVICE_MWIS				= 53,
	SIM_SST_SERVICE_CFIS				= 54,
	SIM_SST_SERVICE_PROVIDER_DISPLAY_INFO		= 55
};

#define SIM_EFSPN_DC_HOME_PLMN_BIT 0x1
#define SIM_EFSPN_DC_ROAMING_SPN_BIT 0x2

enum ber_tlv_data_type {
	BER_TLV_DATA_TYPE_UNIVERSAL = 0,
	BER_TLV_DATA_TYPE_APPLICATION = 1,
	BER_TLV_DATA_TYPE_CONTEXT_SPECIFIC = 2,
	BER_TLV_DATA_TYPE_PRIVATE = 3,
};

enum ber_tlv_data_encoding_type {
	BER_TLV_DATA_ENCODING_TYPE_PRIMITIVE = 0,
	BER_TLV_DATA_ENCODING_TYPE_CONSTRUCTED = 1,
};

struct sim_eons_operator_info {
	char *longname;
	gboolean long_ci;
	char *shortname;
	gboolean short_ci;
	char *info;
};

struct sim_ef_info {
	unsigned short id;
	unsigned short parent;
	unsigned char file_type;
	unsigned char size;
	enum sim_file_access perm_read;
	enum sim_file_access perm_update;
};

struct simple_tlv_iter {
	unsigned int max;
	unsigned int pos;
	const unsigned char *pdu;
	unsigned char tag;
	unsigned short len;
	const unsigned char *data;
};

struct comprehension_tlv_iter {
	unsigned int max;
	unsigned int pos;
	const unsigned char *pdu;
	unsigned short tag;
	gboolean cr;
	unsigned int len;
	const unsigned char *data;
};

struct ber_tlv_iter {
	unsigned int max;
	unsigned int pos;
	const unsigned char *pdu;
	unsigned int tag;
	enum ber_tlv_data_type class;
	enum ber_tlv_data_encoding_type encoding;
	unsigned int len;
	const unsigned char *data;
};

struct ber_tlv_builder {
	unsigned int max;
	unsigned int pos;
	unsigned char *pdu;
	struct ber_tlv_builder *parent;

	unsigned int tag;
	enum ber_tlv_data_type class;
	enum ber_tlv_data_encoding_type encoding;
	unsigned int len;
};

struct comprehension_tlv_builder {
	unsigned int max;
	unsigned int pos;
	unsigned char *pdu;
	unsigned int len;
	struct ber_tlv_builder *parent;
};

void simple_tlv_iter_init(struct simple_tlv_iter *iter,
				const unsigned char *pdu, unsigned int len);
gboolean simple_tlv_iter_next(struct simple_tlv_iter *iter);
unsigned char simple_tlv_iter_get_tag(struct simple_tlv_iter *iter);
unsigned short simple_tlv_iter_get_length(struct simple_tlv_iter *iter);
const unsigned char *simple_tlv_iter_get_data(struct simple_tlv_iter *iter);

void comprehension_tlv_iter_init(struct comprehension_tlv_iter *iter,
					const unsigned char *pdu,
					unsigned int len);
gboolean comprehension_tlv_iter_next(struct comprehension_tlv_iter *iter);
unsigned short comprehension_tlv_iter_get_tag(struct comprehension_tlv_iter *i);
gboolean comprehension_tlv_get_cr(struct comprehension_tlv_iter *iter);
unsigned int comprehension_tlv_iter_get_length(
					struct comprehension_tlv_iter *iter);
const unsigned char *comprehension_tlv_iter_get_data(
					struct comprehension_tlv_iter *iter);

void comprehension_tlv_iter_copy(struct comprehension_tlv_iter *from,
					struct comprehension_tlv_iter *to);

gboolean comprehension_tlv_builder_init(
				struct comprehension_tlv_builder *builder,
				unsigned char *pdu, unsigned int size);
gboolean comprehension_tlv_builder_next(
				struct comprehension_tlv_builder *builder,
				gboolean cr, unsigned short tag);
gboolean comprehension_tlv_builder_set_length(
				struct comprehension_tlv_builder *builder,
				unsigned int len);
unsigned char *comprehension_tlv_builder_get_data(
				struct comprehension_tlv_builder *builder);

void ber_tlv_iter_init(struct ber_tlv_iter *iter, const unsigned char *pdu,
			unsigned int len);
/*
 * Returns the tag value of the TLV.  Note that the tag value can be either
 * short (0-30) or long
 */
unsigned int ber_tlv_iter_get_tag(struct ber_tlv_iter *iter);

enum ber_tlv_data_type ber_tlv_iter_get_class(struct ber_tlv_iter *iter);
enum ber_tlv_data_encoding_type
	ber_tlv_iter_get_encoding(struct ber_tlv_iter *iter);

/*
 * This will return the short tag along with class and encoding information.
 * This is more convenient to use for TLV contents of SIM Elementary Files
 * and SIM toolkit since these elements only use short tags.  In case of an
 * error (e.g. not a short tag) a zero is returned.  According to ISO 7816,
 * a tag value of '00' is invalid.
 */
unsigned char ber_tlv_iter_get_short_tag(struct ber_tlv_iter *iter);
unsigned int ber_tlv_iter_get_length(struct ber_tlv_iter *iter);

const unsigned char *ber_tlv_iter_get_data(struct ber_tlv_iter *iter);

gboolean ber_tlv_iter_next(struct ber_tlv_iter *iter);
void ber_tlv_iter_recurse(struct ber_tlv_iter *iter,
				struct ber_tlv_iter *recurse);
void ber_tlv_iter_recurse_simple(struct ber_tlv_iter *iter,
					struct simple_tlv_iter *container);
void ber_tlv_iter_recurse_comprehension(struct ber_tlv_iter *iter,
					struct comprehension_tlv_iter *recurse);

gboolean ber_tlv_builder_init(struct ber_tlv_builder *builder,
				unsigned char *pdu, unsigned int size);
gboolean ber_tlv_builder_next(struct ber_tlv_builder *builder,
				enum ber_tlv_data_type class,
				enum ber_tlv_data_encoding_type encoding,
				unsigned int new_tag);
gboolean ber_tlv_builder_set_length(struct ber_tlv_builder *builder,
					unsigned int len);
unsigned char *ber_tlv_builder_get_data(struct ber_tlv_builder *builder);
gboolean ber_tlv_builder_recurse(struct ber_tlv_builder *builder,
					struct ber_tlv_builder *recurse);
gboolean ber_tlv_builder_recurse_comprehension(struct ber_tlv_builder *builder,
				struct comprehension_tlv_builder *recurse);
void ber_tlv_builder_optimize(struct ber_tlv_builder *builder,
				unsigned char **pdu, unsigned int *len);

struct sim_eons *sim_eons_new(int pnn_records);
void sim_eons_add_pnn_record(struct sim_eons *eons, int record,
				const guint8 *tlv, int length);
gboolean sim_eons_pnn_is_empty(struct sim_eons *eons);
void sim_eons_add_opl_record(struct sim_eons *eons,
				const guint8 *contents, int length);
void sim_eons_optimize(struct sim_eons *eons);
const struct sim_eons_operator_info *sim_eons_lookup_with_lac(
						struct sim_eons *eons,
						const char *mcc,
						const char *mnc,
						guint16 lac);
const struct sim_eons_operator_info *sim_eons_lookup(struct sim_eons *eons,
						const char *mcc,
						const char *mnc);
void sim_eons_free(struct sim_eons *eons);

void sim_parse_mcc_mnc(const guint8 *bcd, char *mcc, char *mnc);
void sim_encode_mcc_mnc(guint8 *out, const char *mcc, const char *mnc);
struct sim_spdi *sim_spdi_new(const guint8 *tlv, int length);
gboolean sim_spdi_lookup(struct sim_spdi *spdi,
				const char *mcc, const char *mnc);
void sim_spdi_free(struct sim_spdi *spdi);

static inline enum sim_file_access file_access_condition_decode(int bcd)
{
	if (bcd >= 4 && bcd <= 14)
		return SIM_FILE_ACCESS_ADM;
	return bcd;
}

void sim_extract_bcd_number(const unsigned char *buf, int len, char *out);
void sim_encode_bcd_number(const char *number, unsigned char *out);

gboolean sim_adn_parse(const unsigned char *data, int length,
			struct ofono_phone_number *ph, char **identifier);
void sim_adn_build(unsigned char *data, int length,
			const struct ofono_phone_number *ph,
			const char *identifier);

struct sim_ef_info *sim_ef_db_lookup(unsigned short efid);

gboolean sim_parse_3g_get_response(const unsigned char *data, int len,
					int *file_len, int *record_len,
					int *structure, unsigned char *access,
					unsigned short *efid);

gboolean sim_parse_2g_get_response(const unsigned char *response, int len,
					int *file_len, int *record_len,
					int *structure, unsigned char *access);

gboolean sim_ust_is_available(unsigned char *service_ust, unsigned char len,
						enum sim_ust_service index);
gboolean sim_est_is_active(unsigned char *service_est, unsigned char len,
						enum sim_est_service index);
gboolean sim_sst_is_available(unsigned char *service_sst, unsigned char len,
						enum sim_sst_service index);
gboolean sim_sst_is_active(unsigned char *service_sst, unsigned char len,
						enum sim_sst_service index);
