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
#ifndef _UAPI_LINUX_SURFACE_AGGREGATOR_CDEV_H
#define _UAPI_LINUX_SURFACE_AGGREGATOR_CDEV_H
#include <linux/ioctl.h>
#include <linux/types.h>
enum ssam_cdev_request_flags {
  SSAM_CDEV_REQUEST_HAS_RESPONSE = 0x01,
  SSAM_CDEV_REQUEST_UNSEQUENCED = 0x02,
};
struct ssam_cdev_request {
  __u8 target_category;
  __u8 target_id;
  __u8 command_id;
  __u8 instance_id;
  __u16 flags;
  __s16 status;
  struct {
    __u64 data;
    __u16 length;
    __u8 __pad[6];
  } payload;
  struct {
    __u64 data;
    __u16 length;
    __u8 __pad[6];
  } response;
} __attribute__((__packed__));
#define SSAM_CDEV_REQUEST _IOWR(0xA5, 1, struct ssam_cdev_request)
#endif
