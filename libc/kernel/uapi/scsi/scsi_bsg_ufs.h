/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef SCSI_BSG_UFS_H
#define SCSI_BSG_UFS_H
#include <asm/byteorder.h>
#include <linux/types.h>
#define UFS_CDB_SIZE 16
#define UIC_CMD_SIZE (sizeof(__u32) * 4)
enum ufs_bsg_msg_code {
  UPIU_TRANSACTION_UIC_CMD = 0x1F,
  UPIU_TRANSACTION_ARPMB_CMD,
};
enum ufs_rpmb_op_type {
  UFS_RPMB_WRITE_KEY = 0x01,
  UFS_RPMB_READ_CNT = 0x02,
  UFS_RPMB_WRITE = 0x03,
  UFS_RPMB_READ = 0x04,
  UFS_RPMB_READ_RESP = 0x05,
  UFS_RPMB_SEC_CONF_WRITE = 0x06,
  UFS_RPMB_SEC_CONF_READ = 0x07,
  UFS_RPMB_PURGE_ENABLE = 0x08,
  UFS_RPMB_PURGE_STATUS_READ = 0x09,
};
struct utp_upiu_header {
  union {
    struct {
      __be32 dword_0;
      __be32 dword_1;
      __be32 dword_2;
    };
    struct {
      __u8 transaction_code;
      __u8 flags;
      __u8 lun;
      __u8 task_tag;
#ifdef __BIG_ENDIAN
      __u8 iid : 4;
      __u8 command_set_type : 4;
#elif defined(__LITTLE_ENDIAN)
      __u8 command_set_type : 4;
      __u8 iid : 4;
#else
#error 
#endif
      union {
        __u8 tm_function;
        __u8 query_function;
      } __attribute__((packed));
      __u8 response;
      __u8 status;
      __u8 ehs_length;
      __u8 device_information;
      __be16 data_segment_length;
    };
  };
};
struct utp_upiu_query {
  __u8 opcode;
  __u8 idn;
  __u8 index;
  __u8 selector;
  __be16 reserved_osf;
  __be16 length;
  __be32 value;
  __be32 reserved[2];
};
struct utp_upiu_query_v4_0 {
  __u8 opcode;
  __u8 idn;
  __u8 index;
  __u8 selector;
  __u8 osf3;
  __u8 osf4;
  __be16 osf5;
  __be32 osf6;
  __be32 osf7;
  __be32 reserved;
};
struct utp_upiu_cmd {
  __be32 exp_data_transfer_len;
  __u8 cdb[UFS_CDB_SIZE];
};
struct utp_upiu_req {
  struct utp_upiu_header header;
  union {
    struct utp_upiu_cmd sc;
    struct utp_upiu_query qr;
    struct utp_upiu_query uc;
  };
};
struct ufs_arpmb_meta {
  __be16 req_resp_type;
  __u8 nonce[16];
  __be32 write_counter;
  __be16 addr_lun;
  __be16 block_count;
  __be16 result;
} __attribute__((__packed__));
struct ufs_ehs {
  __u8 length;
  __u8 ehs_type;
  __be16 ehssub_type;
  struct ufs_arpmb_meta meta;
  __u8 mac_key[32];
} __attribute__((__packed__));
struct ufs_bsg_request {
  __u32 msgcode;
  struct utp_upiu_req upiu_req;
};
struct ufs_bsg_reply {
  int result;
  __u32 reply_payload_rcv_len;
  struct utp_upiu_req upiu_rsp;
};
struct ufs_rpmb_request {
  struct ufs_bsg_request bsg_request;
  struct ufs_ehs ehs_req;
};
struct ufs_rpmb_reply {
  struct ufs_bsg_reply bsg_reply;
  struct ufs_ehs ehs_rsp;
};
#endif
