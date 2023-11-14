/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_XT_CONNLABEL_H
#define _UAPI_XT_CONNLABEL_H
#include <linux/types.h>
#define XT_CONNLABEL_MAXBIT 127
enum xt_connlabel_mtopts {
  XT_CONNLABEL_OP_INVERT = 1 << 0,
  XT_CONNLABEL_OP_SET = 1 << 1,
};
struct xt_connlabel_mtinfo {
  __u16 bit;
  __u16 options;
};
#endif
