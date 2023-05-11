/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef SCSI_BSG_UFS_H
#define SCSI_BSG_UFS_H
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
  __be32 dword_0;
  __be32 dword_1;
  __be32 dword_2;
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
