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
#ifndef _IPX_H_
#define _IPX_H_
#include <linux/sockios.h>
#include <linux/socket.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPX_NODE_LEN 6
#define IPX_MTU 576
struct sockaddr_ipx {
 sa_family_t sipx_family;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 sipx_port;
 __u32 sipx_network;
 unsigned char sipx_node[IPX_NODE_LEN];
 __u8 sipx_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char sipx_zero;
};
#define sipx_special sipx_port
#define sipx_action sipx_zero
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPX_DLTITF 0
#define IPX_CRTITF 1
struct ipx_route_definition {
 __u32 ipx_network;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 ipx_router_network;
 unsigned char ipx_router_node[IPX_NODE_LEN];
};
struct ipx_interface_definition {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 ipx_network;
 unsigned char ipx_device[16];
 unsigned char ipx_dlink_type;
#define IPX_FRAME_NONE 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPX_FRAME_SNAP 1
#define IPX_FRAME_8022 2
#define IPX_FRAME_ETHERII 3
#define IPX_FRAME_8023 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPX_FRAME_TR_8022 5
 unsigned char ipx_special;
#define IPX_SPECIAL_NONE 0
#define IPX_PRIMARY 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPX_INTERNAL 2
 unsigned char ipx_node[IPX_NODE_LEN];
};
struct ipx_config_data {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char ipxcfg_auto_select_primary;
 unsigned char ipxcfg_auto_create_interfaces;
};
struct ipx_route_def {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 ipx_network;
 __u32 ipx_router_network;
#define IPX_ROUTE_NO_ROUTER 0
 unsigned char ipx_router_node[IPX_NODE_LEN];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char ipx_device[16];
 unsigned short ipx_flags;
#define IPX_RT_SNAP 8
#define IPX_RT_8022 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPX_RT_BLUEBOOK 2
#define IPX_RT_ROUTED 1
};
#define SIOCAIPXITFCRT (SIOCPROTOPRIVATE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SIOCAIPXPRISLT (SIOCPROTOPRIVATE + 1)
#define SIOCIPXCFGDATA (SIOCPROTOPRIVATE + 2)
#define SIOCIPXNCPCONN (SIOCPROTOPRIVATE + 3)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
