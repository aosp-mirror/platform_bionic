/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_TIME_H
#define _XT_TIME_H 1
#include <linux/types.h>
struct xt_time_info {
  __u32 date_start;
  __u32 date_stop;
  __u32 daytime_start;
  __u32 daytime_stop;
  __u32 monthdays_match;
  __u8 weekdays_match;
  __u8 flags;
};
enum {
  XT_TIME_LOCAL_TZ = 1 << 0,
  XT_TIME_CONTIGUOUS = 1 << 1,
  XT_TIME_ALL_MONTHDAYS = 0xFFFFFFFE,
  XT_TIME_ALL_WEEKDAYS = 0xFE,
  XT_TIME_MIN_DAYTIME = 0,
  XT_TIME_MAX_DAYTIME = 24 * 60 * 60 - 1,
};
#define XT_TIME_ALL_FLAGS (XT_TIME_LOCAL_TZ | XT_TIME_CONTIGUOUS)
#endif
