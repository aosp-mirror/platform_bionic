/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __UAPI_TC_IFE_H
#define __UAPI_TC_IFE_H
#include <linux/types.h>
#include <linux/pkt_cls.h>
#include <linux/ife.h>
#define IFE_ENCODE 1
#define IFE_DECODE 0
struct tc_ife {
  tc_gen;
  __u16 flags;
};
enum {
  TCA_IFE_UNSPEC,
  TCA_IFE_PARMS,
  TCA_IFE_TM,
  TCA_IFE_DMAC,
  TCA_IFE_SMAC,
  TCA_IFE_TYPE,
  TCA_IFE_METALST,
  TCA_IFE_PAD,
  __TCA_IFE_MAX
};
#define TCA_IFE_MAX (__TCA_IFE_MAX - 1)
#endif
