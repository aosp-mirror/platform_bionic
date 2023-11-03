/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __IPV6HEADER_H
#define __IPV6HEADER_H
#include <linux/types.h>
struct ip6t_ipv6header_info {
  __u8 matchflags;
  __u8 invflags;
  __u8 modeflag;
};
#define MASK_HOPOPTS 128
#define MASK_DSTOPTS 64
#define MASK_ROUTING 32
#define MASK_FRAGMENT 16
#define MASK_AH 8
#define MASK_ESP 4
#define MASK_NONE 2
#define MASK_PROTO 1
#endif
