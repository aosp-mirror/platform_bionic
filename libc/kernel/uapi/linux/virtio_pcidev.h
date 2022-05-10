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
#ifndef _UAPI_LINUX_VIRTIO_PCIDEV_H
#define _UAPI_LINUX_VIRTIO_PCIDEV_H
#include <linux/types.h>
enum virtio_pcidev_ops {
  VIRTIO_PCIDEV_OP_RESERVED = 0,
  VIRTIO_PCIDEV_OP_CFG_READ,
  VIRTIO_PCIDEV_OP_CFG_WRITE,
  VIRTIO_PCIDEV_OP_MMIO_READ,
  VIRTIO_PCIDEV_OP_MMIO_WRITE,
  VIRTIO_PCIDEV_OP_MMIO_MEMSET,
  VIRTIO_PCIDEV_OP_INT,
  VIRTIO_PCIDEV_OP_MSI,
  VIRTIO_PCIDEV_OP_PME,
};
struct virtio_pcidev_msg {
  __u8 op;
  __u8 bar;
  __u16 reserved;
  __u32 size;
  __u64 addr;
  __u8 data[];
};
#endif
