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
#ifndef _IPV6_H
#define _IPV6_H
#include <linux/types.h>
#include <linux/in6.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <asm/byteorder.h>
#define IPV6_MIN_MTU 1280
struct in6_pktinfo {
 struct in6_addr ipi6_addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int ipi6_ifindex;
};
struct ip6_mtuinfo {
 struct sockaddr_in6 ip6m_addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 ip6m_mtu;
};
struct in6_ifreq {
 struct in6_addr ifr6_addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 ifr6_prefixlen;
 int ifr6_ifindex;
};
#define IPV6_SRCRT_STRICT 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPV6_SRCRT_TYPE_0 0
#define IPV6_SRCRT_TYPE_2 2
struct ipv6_rt_hdr {
 __u8 nexthdr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 hdrlen;
 __u8 type;
 __u8 segments_left;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ipv6_opt_hdr {
 __u8 nexthdr;
 __u8 hdrlen;
} __attribute__((packed));
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ipv6_destopt_hdr ipv6_opt_hdr
#define ipv6_hopopt_hdr ipv6_opt_hdr
struct rt0_hdr {
 struct ipv6_rt_hdr rt_hdr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 reserved;
 struct in6_addr addr[0];
#define rt0_type rt_hdr.type
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct rt2_hdr {
 struct ipv6_rt_hdr rt_hdr;
 __u32 reserved;
 struct in6_addr addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define rt2_type rt_hdr.type
};
struct ipv6_destopt_hao {
 __u8 type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 length;
 struct in6_addr addr;
} __attribute__((packed));
struct ipv6hdr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __LITTLE_ENDIAN_BITFIELD
 __u8 priority:4,
 version:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 version:4,
 priority:4;
#else
#error "Please fix <asm/byteorder.h>"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
 __u8 flow_lbl[3];
 __be16 payload_len;
 __u8 nexthdr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 hop_limit;
 struct in6_addr saddr;
 struct in6_addr daddr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 DEVCONF_FORWARDING = 0,
 DEVCONF_HOPLIMIT,
 DEVCONF_MTU6,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_ACCEPT_RA,
 DEVCONF_ACCEPT_REDIRECTS,
 DEVCONF_AUTOCONF,
 DEVCONF_DAD_TRANSMITS,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_RTR_SOLICITS,
 DEVCONF_RTR_SOLICIT_INTERVAL,
 DEVCONF_RTR_SOLICIT_DELAY,
 DEVCONF_USE_TEMPADDR,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_TEMP_VALID_LFT,
 DEVCONF_TEMP_PREFERED_LFT,
 DEVCONF_REGEN_MAX_RETRY,
 DEVCONF_MAX_DESYNC_FACTOR,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_MAX_ADDRESSES,
 DEVCONF_FORCE_MLD_VERSION,
 DEVCONF_ACCEPT_RA_DEFRTR,
 DEVCONF_ACCEPT_RA_PINFO,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_ACCEPT_RA_RTR_PREF,
 DEVCONF_RTR_PROBE_INTERVAL,
 DEVCONF_ACCEPT_RA_RT_INFO_MAX_PLEN,
 DEVCONF_PROXY_NDP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_OPTIMISTIC_DAD,
 DEVCONF_ACCEPT_SOURCE_ROUTE,
 DEVCONF_MC_FORWARDING,
 DEVCONF_DISABLE_IPV6,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DEVCONF_ACCEPT_DAD,
 DEVCONF_FORCE_TLLAO,
 DEVCONF_MAX
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
