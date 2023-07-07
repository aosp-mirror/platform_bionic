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
#ifndef _UAPI_LINUX_VDPA_H_
#define _UAPI_LINUX_VDPA_H_
#define VDPA_GENL_NAME "vdpa"
#define VDPA_GENL_VERSION 0x1
enum vdpa_command {
  VDPA_CMD_UNSPEC,
  VDPA_CMD_MGMTDEV_NEW,
  VDPA_CMD_MGMTDEV_GET,
  VDPA_CMD_DEV_NEW,
  VDPA_CMD_DEV_DEL,
  VDPA_CMD_DEV_GET,
  VDPA_CMD_DEV_CONFIG_GET,
  VDPA_CMD_DEV_VSTATS_GET,
};
enum vdpa_attr {
  VDPA_ATTR_UNSPEC,
  VDPA_ATTR_PAD = VDPA_ATTR_UNSPEC,
  VDPA_ATTR_MGMTDEV_BUS_NAME,
  VDPA_ATTR_MGMTDEV_DEV_NAME,
  VDPA_ATTR_MGMTDEV_SUPPORTED_CLASSES,
  VDPA_ATTR_DEV_NAME,
  VDPA_ATTR_DEV_ID,
  VDPA_ATTR_DEV_VENDOR_ID,
  VDPA_ATTR_DEV_MAX_VQS,
  VDPA_ATTR_DEV_MAX_VQ_SIZE,
  VDPA_ATTR_DEV_MIN_VQ_SIZE,
  VDPA_ATTR_DEV_NET_CFG_MACADDR,
  VDPA_ATTR_DEV_NET_STATUS,
  VDPA_ATTR_DEV_NET_CFG_MAX_VQP,
  VDPA_ATTR_DEV_NET_CFG_MTU,
  VDPA_ATTR_DEV_NEGOTIATED_FEATURES,
  VDPA_ATTR_DEV_MGMTDEV_MAX_VQS,
  VDPA_ATTR_DEV_SUPPORTED_FEATURES,
  VDPA_ATTR_DEV_QUEUE_INDEX,
  VDPA_ATTR_DEV_VENDOR_ATTR_NAME,
  VDPA_ATTR_DEV_VENDOR_ATTR_VALUE,
  VDPA_ATTR_DEV_FEATURES,
  VDPA_ATTR_MAX,
};
#endif
