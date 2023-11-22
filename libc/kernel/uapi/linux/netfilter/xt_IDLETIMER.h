/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_IDLETIMER_H
#define _XT_IDLETIMER_H
#include <linux/types.h>
#define MAX_IDLETIMER_LABEL_SIZE 28
#define XT_IDLETIMER_ALARM 0x01
struct idletimer_tg_info {
  __u32 timeout;
  char label[MAX_IDLETIMER_LABEL_SIZE];
  struct idletimer_tg * timer __attribute__((aligned(8)));
};
struct idletimer_tg_info_v1 {
  __u32 timeout;
  char label[MAX_IDLETIMER_LABEL_SIZE];
  __u8 send_nl_msg;
  __u8 timer_type;
  struct idletimer_tg * timer __attribute__((aligned(8)));
};
#endif
