/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_IPCOMP_H
#define _XT_IPCOMP_H
#include <linux/types.h>
struct xt_ipcomp {
  __u32 spis[2];
  __u8 invflags;
  __u8 hdrres;
};
#define XT_IPCOMP_INV_SPI 0x01
#define XT_IPCOMP_INV_MASK 0x01
#endif
