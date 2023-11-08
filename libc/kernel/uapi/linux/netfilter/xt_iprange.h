/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _LINUX_NETFILTER_XT_IPRANGE_H
#define _LINUX_NETFILTER_XT_IPRANGE_H 1
#include <linux/types.h>
#include <linux/netfilter.h>
enum {
  IPRANGE_SRC = 1 << 0,
  IPRANGE_DST = 1 << 1,
  IPRANGE_SRC_INV = 1 << 4,
  IPRANGE_DST_INV = 1 << 5,
};
struct xt_iprange_mtinfo {
  union nf_inet_addr src_min, src_max;
  union nf_inet_addr dst_min, dst_max;
  __u8 flags;
};
#endif
