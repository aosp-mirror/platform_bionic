/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_ESP_H
#define _XT_ESP_H
#include <linux/types.h>
struct xt_esp {
  __u32 spis[2];
  __u8 invflags;
};
#define XT_ESP_INV_SPI 0x01
#define XT_ESP_INV_MASK 0x01
#endif
