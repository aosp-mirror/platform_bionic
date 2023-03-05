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
#ifndef MANA_ABI_USER_H
#define MANA_ABI_USER_H
#include <linux/types.h>
#include <rdma/ib_user_ioctl_verbs.h>
#define MANA_IB_UVERBS_ABI_VERSION 1
struct mana_ib_create_cq {
  __aligned_u64 buf_addr;
};
struct mana_ib_create_qp {
  __aligned_u64 sq_buf_addr;
  __u32 sq_buf_size;
  __u32 port;
};
struct mana_ib_create_qp_resp {
  __u32 sqid;
  __u32 cqid;
  __u32 tx_vp_offset;
  __u32 reserved;
};
struct mana_ib_create_wq {
  __aligned_u64 wq_buf_addr;
  __u32 wq_buf_size;
  __u32 reserved;
};
enum mana_ib_rx_hash_function_flags {
  MANA_IB_RX_HASH_FUNC_TOEPLITZ = 1 << 0,
};
struct mana_ib_create_qp_rss {
  __aligned_u64 rx_hash_fields_mask;
  __u8 rx_hash_function;
  __u8 reserved[7];
  __u32 rx_hash_key_len;
  __u8 rx_hash_key[40];
  __u32 port;
};
struct rss_resp_entry {
  __u32 cqid;
  __u32 wqid;
};
struct mana_ib_create_qp_rss_resp {
  __aligned_u64 num_entries;
  struct rss_resp_entry entries[64];
};
#endif
