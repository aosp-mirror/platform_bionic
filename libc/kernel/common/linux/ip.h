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
#ifndef _LINUX_IP_H
#define _LINUX_IP_H
#include <linux/types.h>
#include <asm/byteorder.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPTOS_TOS_MASK 0x1E
#define IPTOS_TOS(tos) ((tos)&IPTOS_TOS_MASK)
#define IPTOS_LOWDELAY 0x10
#define IPTOS_THROUGHPUT 0x08
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPTOS_RELIABILITY 0x04
#define IPTOS_MINCOST 0x02
#define IPTOS_PREC_MASK 0xE0
#define IPTOS_PREC(tos) ((tos)&IPTOS_PREC_MASK)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPTOS_PREC_NETCONTROL 0xe0
#define IPTOS_PREC_INTERNETCONTROL 0xc0
#define IPTOS_PREC_CRITIC_ECP 0xa0
#define IPTOS_PREC_FLASHOVERRIDE 0x80
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPTOS_PREC_FLASH 0x60
#define IPTOS_PREC_IMMEDIATE 0x40
#define IPTOS_PREC_PRIORITY 0x20
#define IPTOS_PREC_ROUTINE 0x00
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_COPY 0x80
#define IPOPT_CLASS_MASK 0x60
#define IPOPT_NUMBER_MASK 0x1f
#define IPOPT_COPIED(o) ((o)&IPOPT_COPY)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_CLASS(o) ((o)&IPOPT_CLASS_MASK)
#define IPOPT_NUMBER(o) ((o)&IPOPT_NUMBER_MASK)
#define IPOPT_CONTROL 0x00
#define IPOPT_RESERVED1 0x20
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_MEASUREMENT 0x40
#define IPOPT_RESERVED2 0x60
#define IPOPT_END (0 |IPOPT_CONTROL)
#define IPOPT_NOOP (1 |IPOPT_CONTROL)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_SEC (2 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_LSRR (3 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_TIMESTAMP (4 |IPOPT_MEASUREMENT)
#define IPOPT_RR (7 |IPOPT_CONTROL)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_SID (8 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_SSRR (9 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_RA (20|IPOPT_CONTROL|IPOPT_COPY)
#define IPVERSION 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAXTTL 255
#define IPDEFTTL 64
#define IPOPT_OPTVAL 0
#define IPOPT_OLEN 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_OFFSET 2
#define IPOPT_MINOFF 4
#define MAX_IPOPTLEN 40
#define IPOPT_NOP IPOPT_NOOP
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_EOL IPOPT_END
#define IPOPT_TS IPOPT_TIMESTAMP
#define IPOPT_TS_TSONLY 0
#define IPOPT_TS_TSANDADDR 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPOPT_TS_PRESPEC 3
struct iphdr {
#ifdef __LITTLE_ENDIAN_BITFIELD
 __u8 ihl:4,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 version:4;
#elif defined (__BIG_ENDIAN_BITFIELD)
 __u8 version:4,
 ihl:4;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#else
#error "Please fix <asm/byteorder.h>"
#endif
 __u8 tos;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 tot_len;
 __be16 id;
 __be16 frag_off;
 __u8 ttl;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 protocol;
 __u16 check;
 __be32 saddr;
 __be32 daddr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ip_auth_hdr {
 __u8 nexthdr;
 __u8 hdrlen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 reserved;
 __u32 spi;
 __u32 seq_no;
 __u8 auth_data[0];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ip_esp_hdr {
 __u32 spi;
 __u32 seq_no;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 enc_data[0];
};
struct ip_comp_hdr {
 __u8 nexthdr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 flags;
 __u16 cpi;
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
