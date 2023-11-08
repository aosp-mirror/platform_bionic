/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_LED_H
#define _XT_LED_H
#include <linux/types.h>
struct xt_led_info {
  char id[27];
  __u8 always_blink;
  __u32 delay;
  void * internal_data __attribute__((aligned(8)));
};
#endif
