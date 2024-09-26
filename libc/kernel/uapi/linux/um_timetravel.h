/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_UM_TIMETRAVEL_H
#define _UAPI_LINUX_UM_TIMETRAVEL_H
#include <linux/types.h>
struct um_timetravel_msg {
  __u32 op;
  __u32 seq;
  __u64 time;
};
enum um_timetravel_ops {
  UM_TIMETRAVEL_ACK = 0,
  UM_TIMETRAVEL_START = 1,
  UM_TIMETRAVEL_REQUEST = 2,
  UM_TIMETRAVEL_WAIT = 3,
  UM_TIMETRAVEL_GET = 4,
  UM_TIMETRAVEL_UPDATE = 5,
  UM_TIMETRAVEL_RUN = 6,
  UM_TIMETRAVEL_FREE_UNTIL = 7,
  UM_TIMETRAVEL_GET_TOD = 8,
};
#endif
