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
#ifndef __LINUX_MROUTE6_H
#define __LINUX_MROUTE6_H
#include <linux/types.h>
#include <linux/sockios.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MRT6_BASE 200
#define MRT6_INIT (MRT6_BASE)
#define MRT6_DONE (MRT6_BASE+1)
#define MRT6_ADD_MIF (MRT6_BASE+2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MRT6_DEL_MIF (MRT6_BASE+3)
#define MRT6_ADD_MFC (MRT6_BASE+4)
#define MRT6_DEL_MFC (MRT6_BASE+5)
#define MRT6_VERSION (MRT6_BASE+6)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MRT6_ASSERT (MRT6_BASE+7)
#define MRT6_PIM (MRT6_BASE+8)
#define MRT6_TABLE (MRT6_BASE+9)
#define SIOCGETMIFCNT_IN6 SIOCPROTOPRIVATE
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIOCGETSGCNT_IN6 (SIOCPROTOPRIVATE+1)
#define SIOCGETRPF (SIOCPROTOPRIVATE+2)
#define MAXMIFS 32
typedef unsigned long mifbitmap_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef unsigned short mifi_t;
#define ALL_MIFS ((mifi_t)(-1))
#ifndef IF_SETSIZE
#define IF_SETSIZE 256
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
typedef __u32 if_mask;
#define NIFBITS (sizeof(if_mask) * 8)
#ifndef DIV_ROUND_UP
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DIV_ROUND_UP(x,y) (((x) + ((y) - 1)) / (y))
#endif
typedef struct if_set {
 if_mask ifs_bits[DIV_ROUND_UP(IF_SETSIZE, NIFBITS)];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} if_set;
#define IF_SET(n, p) ((p)->ifs_bits[(n)/NIFBITS] |= (1 << ((n) % NIFBITS)))
#define IF_CLR(n, p) ((p)->ifs_bits[(n)/NIFBITS] &= ~(1 << ((n) % NIFBITS)))
#define IF_ISSET(n, p) ((p)->ifs_bits[(n)/NIFBITS] & (1 << ((n) % NIFBITS)))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IF_COPY(f, t) bcopy(f, t, sizeof(*(f)))
#define IF_ZERO(p) bzero(p, sizeof(*(p)))
struct mif6ctl {
 mifi_t mif6c_mifi;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char mif6c_flags;
 unsigned char vifc_threshold;
 __u16 mif6c_pifi;
 unsigned int vifc_rate_limit;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define MIFF_REGISTER 0x1
struct mf6cctl {
 struct sockaddr_in6 mf6cc_origin;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct sockaddr_in6 mf6cc_mcastgrp;
 mifi_t mf6cc_parent;
 struct if_set mf6cc_ifset;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct sioc_sg_req6 {
 struct sockaddr_in6 src;
 struct sockaddr_in6 grp;
 unsigned long pktcnt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long bytecnt;
 unsigned long wrong_if;
};
struct sioc_mif_req6 {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mifi_t mifi;
 unsigned long icount;
 unsigned long ocount;
 unsigned long ibytes;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long obytes;
};
struct mrt6msg {
#define MRT6MSG_NOCACHE 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MRT6MSG_WRONGMIF 2
#define MRT6MSG_WHOLEPKT 3
 __u8 im6_mbz;
 __u8 im6_msgtype;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 im6_mif;
 __u32 im6_pad;
 struct in6_addr im6_src, im6_dst;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
