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
#ifndef IB_USER_IOCTL_VERBS_H
#define IB_USER_IOCTL_VERBS_H
#include <linux/types.h>
#ifndef RDMA_UAPI_PTR
#define RDMA_UAPI_PTR(_type,_name) __aligned_u64 _name
#endif
enum ib_uverbs_flow_action_esp_keymat {
  IB_UVERBS_FLOW_ACTION_ESP_KEYMAT_AES_GCM,
};
enum ib_uverbs_flow_action_esp_keymat_aes_gcm_iv_algo {
  IB_UVERBS_FLOW_ACTION_IV_ALGO_SEQ,
};
struct ib_uverbs_flow_action_esp_keymat_aes_gcm {
  __aligned_u64 iv;
  __u32 iv_algo;
  __u32 salt;
  __u32 icv_len;
  __u32 key_len;
  __u32 aes_key[256 / 32];
};
enum ib_uverbs_flow_action_esp_replay {
  IB_UVERBS_FLOW_ACTION_ESP_REPLAY_NONE,
  IB_UVERBS_FLOW_ACTION_ESP_REPLAY_BMP,
};
struct ib_uverbs_flow_action_esp_replay_bmp {
  __u32 size;
};
enum ib_uverbs_flow_action_esp_flags {
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_INLINE_CRYPTO = 0UL << 0,
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_FULL_OFFLOAD = 1UL << 0,
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_TUNNEL = 0UL << 1,
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_TRANSPORT = 1UL << 1,
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_DECRYPT = 0UL << 2,
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ENCRYPT = 1UL << 2,
  IB_UVERBS_FLOW_ACTION_ESP_FLAGS_ESN_NEW_WINDOW = 1UL << 3,
};
struct ib_uverbs_flow_action_esp_encap {
  RDMA_UAPI_PTR(void *, val_ptr);
  RDMA_UAPI_PTR(struct ib_uverbs_flow_action_esp_encap *, next_ptr);
  __u16 len;
  __u16 type;
};
struct ib_uverbs_flow_action_esp {
  __u32 spi;
  __u32 seq;
  __u32 tfc_pad;
  __u32 flags;
  __aligned_u64 hard_limit_pkts;
};
#endif
