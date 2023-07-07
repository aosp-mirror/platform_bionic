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
#ifndef _UAPI_LINUX_VIRTIO_BT_H
#define _UAPI_LINUX_VIRTIO_BT_H
#include <linux/virtio_types.h>
#define VIRTIO_BT_F_VND_HCI 0
#define VIRTIO_BT_F_MSFT_EXT 1
#define VIRTIO_BT_F_AOSP_EXT 2
#define VIRTIO_BT_F_CONFIG_V2 3
enum virtio_bt_config_type {
  VIRTIO_BT_CONFIG_TYPE_PRIMARY = 0,
  VIRTIO_BT_CONFIG_TYPE_AMP = 1,
};
enum virtio_bt_config_vendor {
  VIRTIO_BT_CONFIG_VENDOR_NONE = 0,
  VIRTIO_BT_CONFIG_VENDOR_ZEPHYR = 1,
  VIRTIO_BT_CONFIG_VENDOR_INTEL = 2,
  VIRTIO_BT_CONFIG_VENDOR_REALTEK = 3,
};
struct virtio_bt_config {
  __u8 type;
  __u16 vendor;
  __u16 msft_opcode;
} __attribute__((packed));
struct virtio_bt_config_v2 {
  __u8 type;
  __u8 alignment;
  __u16 vendor;
  __u16 msft_opcode;
};
#endif
