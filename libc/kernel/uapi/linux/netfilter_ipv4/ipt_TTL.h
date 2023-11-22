/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _IPT_TTL_H
#define _IPT_TTL_H
#include <linux/types.h>
enum {
  IPT_TTL_SET = 0,
  IPT_TTL_INC,
  IPT_TTL_DEC
};
#define IPT_TTL_MAXMODE IPT_TTL_DEC
struct ipt_TTL_info {
  __u8 mode;
  __u8 ttl;
};
#endif
