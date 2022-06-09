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
#ifndef __UAPI_MCTP_H
#define __UAPI_MCTP_H
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/netdevice.h>
typedef __u8 mctp_eid_t;
struct mctp_addr {
  mctp_eid_t s_addr;
};
struct sockaddr_mctp {
  __kernel_sa_family_t smctp_family;
  __u16 __smctp_pad0;
  unsigned int smctp_network;
  struct mctp_addr smctp_addr;
  __u8 smctp_type;
  __u8 smctp_tag;
  __u8 __smctp_pad1;
};
struct sockaddr_mctp_ext {
  struct sockaddr_mctp smctp_base;
  int smctp_ifindex;
  __u8 smctp_halen;
  __u8 __smctp_pad0[3];
  __u8 smctp_haddr[MAX_ADDR_LEN];
};
#define MCTP_NET_ANY 0x0
#define MCTP_ADDR_NULL 0x00
#define MCTP_ADDR_ANY 0xff
#define MCTP_TAG_MASK 0x07
#define MCTP_TAG_OWNER 0x08
#define MCTP_TAG_PREALLOC 0x10
#define MCTP_OPT_ADDR_EXT 1
#define SIOCMCTPALLOCTAG (SIOCPROTOPRIVATE + 0)
#define SIOCMCTPDROPTAG (SIOCPROTOPRIVATE + 1)
struct mctp_ioc_tag_ctl {
  mctp_eid_t peer_addr;
  __u8 tag;
  __u16 flags;
};
#endif
