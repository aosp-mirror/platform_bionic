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
#ifndef _LINUX_CACHEFILES_H
#define _LINUX_CACHEFILES_H
#include <linux/types.h>
#include <linux/ioctl.h>
#define CACHEFILES_MSG_MAX_SIZE 1024
enum cachefiles_opcode {
  CACHEFILES_OP_OPEN,
  CACHEFILES_OP_CLOSE,
  CACHEFILES_OP_READ,
};
struct cachefiles_msg {
  __u32 msg_id;
  __u32 opcode;
  __u32 len;
  __u32 object_id;
  __u8 data[];
};
struct cachefiles_open {
  __u32 volume_key_size;
  __u32 cookie_key_size;
  __u32 fd;
  __u32 flags;
  __u8 data[];
};
struct cachefiles_read {
  __u64 off;
  __u64 len;
};
#define CACHEFILES_IOC_READ_COMPLETE _IOW(0x98, 1, int)
#endif
