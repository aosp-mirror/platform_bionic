/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _IPT_ECN_TARGET_H
#define _IPT_ECN_TARGET_H
#include <linux/types.h>
#include <linux/netfilter/xt_DSCP.h>
#define IPT_ECN_IP_MASK (~XT_DSCP_MASK)
#define IPT_ECN_OP_SET_IP 0x01
#define IPT_ECN_OP_SET_ECE 0x10
#define IPT_ECN_OP_SET_CWR 0x20
#define IPT_ECN_OP_MASK 0xce
struct ipt_ECN_info {
  __u8 operation;
  __u8 ip_ect;
  union {
    struct {
      __u8 ece : 1, cwr : 1;
    } tcp;
  } proto;
};
#endif
