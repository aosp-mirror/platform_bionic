/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _NF_CONNTRACK_TUPLE_COMMON_H
#define _NF_CONNTRACK_TUPLE_COMMON_H
#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nf_conntrack_common.h>
enum ip_conntrack_dir {
  IP_CT_DIR_ORIGINAL,
  IP_CT_DIR_REPLY,
  IP_CT_DIR_MAX
};
union nf_conntrack_man_proto {
  __be16 all;
  struct {
    __be16 port;
  } tcp;
  struct {
    __be16 port;
  } udp;
  struct {
    __be16 id;
  } icmp;
  struct {
    __be16 port;
  } dccp;
  struct {
    __be16 port;
  } sctp;
  struct {
    __be16 key;
  } gre;
};
#define CTINFO2DIR(ctinfo) ((ctinfo) >= IP_CT_IS_REPLY ? IP_CT_DIR_REPLY : IP_CT_DIR_ORIGINAL)
#endif
