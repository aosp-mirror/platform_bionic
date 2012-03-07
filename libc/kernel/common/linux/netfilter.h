/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __LINUX_NETFILTER_H
#define __LINUX_NETFILTER_H
#include <linux/types.h>
#include <linux/compiler.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NF_DROP 0
#define NF_ACCEPT 1
#define NF_STOLEN 2
#define NF_QUEUE 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NF_REPEAT 4
#define NF_STOP 5
#define NF_MAX_VERDICT NF_STOP
#define NF_VERDICT_MASK 0x0000ffff
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NF_VERDICT_BITS 16
#define NF_VERDICT_QMASK 0xffff0000
#define NF_VERDICT_QBITS 16
#define NF_QUEUE_NR(x) ((((x) << NF_VERDICT_BITS) & NF_VERDICT_QMASK) | NF_QUEUE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NFC_UNKNOWN 0x4000
#define NFC_ALTERED 0x8000
enum nf_inet_hooks {
 NF_INET_PRE_ROUTING,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 NF_INET_LOCAL_IN,
 NF_INET_FORWARD,
 NF_INET_LOCAL_OUT,
 NF_INET_POST_ROUTING,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 NF_INET_NUMHOOKS
};
enum {
 NFPROTO_UNSPEC = 0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 NFPROTO_IPV4 = 2,
 NFPROTO_ARP = 3,
 NFPROTO_BRIDGE = 7,
 NFPROTO_IPV6 = 10,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 NFPROTO_DECNET = 12,
 NFPROTO_NUMPROTO,
};
union nf_inet_addr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 all[4];
 __be32 ip;
 __be32 ip6[4];
 struct in_addr in;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct in6_addr in6;
};
#endif
