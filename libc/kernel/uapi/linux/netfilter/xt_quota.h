/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_QUOTA_H
#define _XT_QUOTA_H
#include <linux/types.h>
enum xt_quota_flags {
  XT_QUOTA_INVERT = 0x1,
};
#define XT_QUOTA_MASK 0x1
struct xt_quota_priv;
struct xt_quota_info {
  __u32 flags;
  __u32 pad;
  __aligned_u64 quota;
  struct xt_quota_priv * master;
};
#endif
