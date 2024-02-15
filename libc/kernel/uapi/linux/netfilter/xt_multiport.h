/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_MULTIPORT_H
#define _XT_MULTIPORT_H
#include <linux/types.h>
enum xt_multiport_flags {
  XT_MULTIPORT_SOURCE,
  XT_MULTIPORT_DESTINATION,
  XT_MULTIPORT_EITHER
};
#define XT_MULTI_PORTS 15
struct xt_multiport {
  __u8 flags;
  __u8 count;
  __u16 ports[XT_MULTI_PORTS];
};
struct xt_multiport_v1 {
  __u8 flags;
  __u8 count;
  __u16 ports[XT_MULTI_PORTS];
  __u8 pflags[XT_MULTI_PORTS];
  __u8 invert;
};
#endif
