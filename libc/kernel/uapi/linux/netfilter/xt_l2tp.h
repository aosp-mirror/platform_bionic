/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _LINUX_NETFILTER_XT_L2TP_H
#define _LINUX_NETFILTER_XT_L2TP_H
#include <linux/types.h>
enum xt_l2tp_type {
  XT_L2TP_TYPE_CONTROL,
  XT_L2TP_TYPE_DATA,
};
struct xt_l2tp_info {
  __u32 tid;
  __u32 sid;
  __u8 version;
  __u8 type;
  __u8 flags;
};
enum {
  XT_L2TP_TID = (1 << 0),
  XT_L2TP_SID = (1 << 1),
  XT_L2TP_VERSION = (1 << 2),
  XT_L2TP_TYPE = (1 << 3),
};
#endif
