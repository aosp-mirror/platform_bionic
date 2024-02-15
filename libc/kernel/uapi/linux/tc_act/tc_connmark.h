/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __UAPI_TC_CONNMARK_H
#define __UAPI_TC_CONNMARK_H
#include <linux/types.h>
#include <linux/pkt_cls.h>
struct tc_connmark {
  tc_gen;
  __u16 zone;
};
enum {
  TCA_CONNMARK_UNSPEC,
  TCA_CONNMARK_PARMS,
  TCA_CONNMARK_TM,
  TCA_CONNMARK_PAD,
  __TCA_CONNMARK_MAX
};
#define TCA_CONNMARK_MAX (__TCA_CONNMARK_MAX - 1)
#endif
