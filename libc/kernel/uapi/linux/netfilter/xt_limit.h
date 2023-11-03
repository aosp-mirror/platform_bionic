/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_RATE_H
#define _XT_RATE_H
#include <linux/types.h>
#define XT_LIMIT_SCALE 10000
struct xt_limit_priv;
struct xt_rateinfo {
  __u32 avg;
  __u32 burst;
  unsigned long prev;
  __u32 credit;
  __u32 credit_cap, cost;
  struct xt_limit_priv * master;
};
#endif
