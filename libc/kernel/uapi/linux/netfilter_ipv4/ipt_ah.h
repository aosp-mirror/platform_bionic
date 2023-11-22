/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _IPT_AH_H
#define _IPT_AH_H
#include <linux/types.h>
struct ipt_ah {
  __u32 spis[2];
  __u8 invflags;
};
#define IPT_AH_INV_SPI 0x01
#define IPT_AH_INV_MASK 0x01
#endif
