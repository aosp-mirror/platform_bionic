/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_TCPMSS_MATCH_H
#define _XT_TCPMSS_MATCH_H
#include <linux/types.h>
struct xt_tcpmss_match_info {
  __u16 mss_min, mss_max;
  __u8 invert;
};
#endif
