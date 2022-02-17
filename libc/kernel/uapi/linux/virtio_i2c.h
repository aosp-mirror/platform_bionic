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
#ifndef _UAPI_LINUX_VIRTIO_I2C_H
#define _UAPI_LINUX_VIRTIO_I2C_H
#include <linux/const.h>
#include <linux/types.h>
#define VIRTIO_I2C_F_ZERO_LENGTH_REQUEST 0
#define VIRTIO_I2C_FLAGS_FAIL_NEXT _BITUL(0)
#define VIRTIO_I2C_FLAGS_M_RD _BITUL(1)
struct virtio_i2c_out_hdr {
  __le16 addr;
  __le16 padding;
  __le32 flags;
};
struct virtio_i2c_in_hdr {
  __u8 status;
};
#define VIRTIO_I2C_MSG_OK 0
#define VIRTIO_I2C_MSG_ERR 1
#endif
