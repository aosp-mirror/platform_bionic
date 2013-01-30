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
#ifndef __IF_TUN_H
#define __IF_TUN_H
#include <linux/types.h>
#include <linux/if_ether.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/filter.h>
#define TUN_READQ_SIZE 500
#define TUN_TUN_DEV 0x0001
#define TUN_TAP_DEV 0x0002
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUN_TYPE_MASK 0x000f
#define TUN_FASYNC 0x0010
#define TUN_NOCHECKSUM 0x0020
#define TUN_NO_PI 0x0040
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUN_ONE_QUEUE 0x0080
#define TUN_PERSIST 0x0100
#define TUN_VNET_HDR 0x0200
#define TUNSETNOCSUM _IOW('T', 200, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUNSETDEBUG _IOW('T', 201, int)
#define TUNSETIFF _IOW('T', 202, int)
#define TUNSETPERSIST _IOW('T', 203, int)
#define TUNSETOWNER _IOW('T', 204, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUNSETLINK _IOW('T', 205, int)
#define TUNSETGROUP _IOW('T', 206, int)
#define TUNGETFEATURES _IOR('T', 207, unsigned int)
#define TUNSETOFFLOAD _IOW('T', 208, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUNSETTXFILTER _IOW('T', 209, unsigned int)
#define TUNGETIFF _IOR('T', 210, unsigned int)
#define TUNGETSNDBUF _IOR('T', 211, int)
#define TUNSETSNDBUF _IOW('T', 212, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUNATTACHFILTER _IOW('T', 213, struct sock_fprog)
#define TUNDETACHFILTER _IOW('T', 214, struct sock_fprog)
#define TUNGETVNETHDRSZ _IOR('T', 215, int)
#define TUNSETVNETHDRSZ _IOW('T', 216, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IFF_TUN 0x0001
#define IFF_TAP 0x0002
#define IFF_NO_PI 0x1000
#define IFF_ONE_QUEUE 0x2000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IFF_VNET_HDR 0x4000
#define IFF_TUN_EXCL 0x8000
#define TUN_F_CSUM 0x01
#define TUN_F_TSO4 0x02
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUN_F_TSO6 0x04
#define TUN_F_TSO_ECN 0x08
#define TUN_F_UFO 0x10
#define TUN_PKT_STRIP 0x0001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct tun_pi {
 __u16 flags;
 __be16 proto;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TUN_FLT_ALLMULTI 0x0001
struct tun_filter {
 __u16 flags;
 __u16 count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 addr[0][ETH_ALEN];
};
#endif
