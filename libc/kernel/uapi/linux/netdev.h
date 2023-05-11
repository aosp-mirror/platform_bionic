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
#ifndef _UAPI_LINUX_NETDEV_H
#define _UAPI_LINUX_NETDEV_H
#define NETDEV_FAMILY_NAME "netdev"
#define NETDEV_FAMILY_VERSION 1
enum netdev_xdp_act {
  NETDEV_XDP_ACT_BASIC = 1,
  NETDEV_XDP_ACT_REDIRECT = 2,
  NETDEV_XDP_ACT_NDO_XMIT = 4,
  NETDEV_XDP_ACT_XSK_ZEROCOPY = 8,
  NETDEV_XDP_ACT_HW_OFFLOAD = 16,
  NETDEV_XDP_ACT_RX_SG = 32,
  NETDEV_XDP_ACT_NDO_XMIT_SG = 64,
  NETDEV_XDP_ACT_MASK = 127,
};
enum {
  NETDEV_A_DEV_IFINDEX = 1,
  NETDEV_A_DEV_PAD,
  NETDEV_A_DEV_XDP_FEATURES,
  __NETDEV_A_DEV_MAX,
  NETDEV_A_DEV_MAX = (__NETDEV_A_DEV_MAX - 1)
};
enum {
  NETDEV_CMD_DEV_GET = 1,
  NETDEV_CMD_DEV_ADD_NTF,
  NETDEV_CMD_DEV_DEL_NTF,
  NETDEV_CMD_DEV_CHANGE_NTF,
  __NETDEV_CMD_MAX,
  NETDEV_CMD_MAX = (__NETDEV_CMD_MAX - 1)
};
#define NETDEV_MCGRP_MGMT "mgmt"
#endif
