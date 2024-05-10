/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_CONNBYTES_H
#define _XT_CONNBYTES_H
#include <linux/types.h>
enum xt_connbytes_what {
  XT_CONNBYTES_PKTS,
  XT_CONNBYTES_BYTES,
  XT_CONNBYTES_AVGPKT,
};
enum xt_connbytes_direction {
  XT_CONNBYTES_DIR_ORIGINAL,
  XT_CONNBYTES_DIR_REPLY,
  XT_CONNBYTES_DIR_BOTH,
};
struct xt_connbytes_info {
  struct {
    __aligned_u64 from;
    __aligned_u64 to;
  } count;
  __u8 what;
  __u8 direction;
};
#endif
