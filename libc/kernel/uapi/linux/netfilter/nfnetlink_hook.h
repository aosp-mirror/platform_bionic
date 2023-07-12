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
#ifndef _NFNL_HOOK_H_
#define _NFNL_HOOK_H_
enum nfnl_hook_msg_types {
  NFNL_MSG_HOOK_GET,
  NFNL_MSG_HOOK_MAX,
};
enum nfnl_hook_attributes {
  NFNLA_HOOK_UNSPEC,
  NFNLA_HOOK_HOOKNUM,
  NFNLA_HOOK_PRIORITY,
  NFNLA_HOOK_DEV,
  NFNLA_HOOK_FUNCTION_NAME,
  NFNLA_HOOK_MODULE_NAME,
  NFNLA_HOOK_CHAIN_INFO,
  __NFNLA_HOOK_MAX
};
#define NFNLA_HOOK_MAX (__NFNLA_HOOK_MAX - 1)
enum nfnl_hook_chain_info_attributes {
  NFNLA_HOOK_INFO_UNSPEC,
  NFNLA_HOOK_INFO_DESC,
  NFNLA_HOOK_INFO_TYPE,
  __NFNLA_HOOK_INFO_MAX,
};
#define NFNLA_HOOK_INFO_MAX (__NFNLA_HOOK_INFO_MAX - 1)
enum nfnl_hook_chain_desc_attributes {
  NFNLA_CHAIN_UNSPEC,
  NFNLA_CHAIN_TABLE,
  NFNLA_CHAIN_FAMILY,
  NFNLA_CHAIN_NAME,
  __NFNLA_CHAIN_MAX,
};
#define NFNLA_CHAIN_MAX (__NFNLA_CHAIN_MAX - 1)
enum nfnl_hook_chaintype {
  NFNL_HOOK_TYPE_NFTABLES = 0x1,
  NFNL_HOOK_TYPE_BPF,
};
enum nfnl_hook_bpf_attributes {
  NFNLA_HOOK_BPF_UNSPEC,
  NFNLA_HOOK_BPF_ID,
  __NFNLA_HOOK_BPF_MAX,
};
#define NFNLA_HOOK_BPF_MAX (__NFNLA_HOOK_BPF_MAX - 1)
#endif
