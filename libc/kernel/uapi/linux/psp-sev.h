/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __PSP_SEV_USER_H__
#define __PSP_SEV_USER_H__
#include <linux/types.h>
enum {
  SEV_FACTORY_RESET = 0,
  SEV_PLATFORM_STATUS,
  SEV_PEK_GEN,
  SEV_PEK_CSR,
  SEV_PDH_GEN,
  SEV_PDH_CERT_EXPORT,
  SEV_PEK_CERT_IMPORT,
  SEV_GET_ID,
  SEV_GET_ID2,
  SEV_MAX,
};
typedef enum {
  SEV_RET_NO_FW_CALL = - 1,
  SEV_RET_SUCCESS = 0,
  SEV_RET_INVALID_PLATFORM_STATE,
  SEV_RET_INVALID_GUEST_STATE,
  SEV_RET_INAVLID_CONFIG,
  SEV_RET_INVALID_LEN,
  SEV_RET_ALREADY_OWNED,
  SEV_RET_INVALID_CERTIFICATE,
  SEV_RET_POLICY_FAILURE,
  SEV_RET_INACTIVE,
  SEV_RET_INVALID_ADDRESS,
  SEV_RET_BAD_SIGNATURE,
  SEV_RET_BAD_MEASUREMENT,
  SEV_RET_ASID_OWNED,
  SEV_RET_INVALID_ASID,
  SEV_RET_WBINVD_REQUIRED,
  SEV_RET_DFFLUSH_REQUIRED,
  SEV_RET_INVALID_GUEST,
  SEV_RET_INVALID_COMMAND,
  SEV_RET_ACTIVE,
  SEV_RET_HWSEV_RET_PLATFORM,
  SEV_RET_HWSEV_RET_UNSAFE,
  SEV_RET_UNSUPPORTED,
  SEV_RET_INVALID_PARAM,
  SEV_RET_RESOURCE_LIMIT,
  SEV_RET_SECURE_DATA_INVALID,
  SEV_RET_INVALID_KEY = 0x27,
  SEV_RET_MAX,
} sev_ret_code;
struct sev_user_data_status {
  __u8 api_major;
  __u8 api_minor;
  __u8 state;
  __u32 flags;
  __u8 build;
  __u32 guest_count;
} __attribute__((__packed__));
#define SEV_STATUS_FLAGS_CONFIG_ES 0x0100
struct sev_user_data_pek_csr {
  __u64 address;
  __u32 length;
} __attribute__((__packed__));
struct sev_user_data_pek_cert_import {
  __u64 pek_cert_address;
  __u32 pek_cert_len;
  __u64 oca_cert_address;
  __u32 oca_cert_len;
} __attribute__((__packed__));
struct sev_user_data_pdh_cert_export {
  __u64 pdh_cert_address;
  __u32 pdh_cert_len;
  __u64 cert_chain_address;
  __u32 cert_chain_len;
} __attribute__((__packed__));
struct sev_user_data_get_id {
  __u8 socket1[64];
  __u8 socket2[64];
} __attribute__((__packed__));
struct sev_user_data_get_id2 {
  __u64 address;
  __u32 length;
} __attribute__((__packed__));
struct sev_issue_cmd {
  __u32 cmd;
  __u64 data;
  __u32 error;
} __attribute__((__packed__));
#define SEV_IOC_TYPE 'S'
#define SEV_ISSUE_CMD _IOWR(SEV_IOC_TYPE, 0x0, struct sev_issue_cmd)
#endif
