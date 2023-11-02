/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_CT_H
#define _XT_CT_H
#include <linux/types.h>
enum {
  XT_CT_NOTRACK = 1 << 0,
  XT_CT_NOTRACK_ALIAS = 1 << 1,
  XT_CT_ZONE_DIR_ORIG = 1 << 2,
  XT_CT_ZONE_DIR_REPL = 1 << 3,
  XT_CT_ZONE_MARK = 1 << 4,
  XT_CT_MASK = XT_CT_NOTRACK | XT_CT_NOTRACK_ALIAS | XT_CT_ZONE_DIR_ORIG | XT_CT_ZONE_DIR_REPL | XT_CT_ZONE_MARK,
};
struct xt_ct_target_info {
  __u16 flags;
  __u16 zone;
  __u32 ct_events;
  __u32 exp_events;
  char helper[16];
  struct nf_conn * ct __attribute__((aligned(8)));
};
struct xt_ct_target_info_v1 {
  __u16 flags;
  __u16 zone;
  __u32 ct_events;
  __u32 exp_events;
  char helper[16];
  char timeout[32];
  struct nf_conn * ct __attribute__((aligned(8)));
};
#endif
