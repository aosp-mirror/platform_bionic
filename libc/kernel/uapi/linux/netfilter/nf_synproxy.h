/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _NF_SYNPROXY_H
#define _NF_SYNPROXY_H
#include <linux/types.h>
#define NF_SYNPROXY_OPT_MSS 0x01
#define NF_SYNPROXY_OPT_WSCALE 0x02
#define NF_SYNPROXY_OPT_SACK_PERM 0x04
#define NF_SYNPROXY_OPT_TIMESTAMP 0x08
#define NF_SYNPROXY_OPT_ECN 0x10
#define NF_SYNPROXY_OPT_MASK (NF_SYNPROXY_OPT_MSS | NF_SYNPROXY_OPT_WSCALE | NF_SYNPROXY_OPT_SACK_PERM | NF_SYNPROXY_OPT_TIMESTAMP)
struct nf_synproxy_info {
  __u8 options;
  __u8 wscale;
  __u16 mss;
};
#endif
