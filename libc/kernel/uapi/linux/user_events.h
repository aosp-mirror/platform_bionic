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
#ifndef _UAPI_LINUX_USER_EVENTS_H
#define _UAPI_LINUX_USER_EVENTS_H
#include <linux/types.h>
#include <linux/ioctl.h>
#define USER_EVENTS_SYSTEM "user_events"
#define USER_EVENTS_PREFIX "u:"
#define DYN_LOC(offset,size) ((size) << 16 | (offset))
struct user_reg {
  __u32 size;
  __u8 enable_bit;
  __u8 enable_size;
  __u16 flags;
  __u64 enable_addr;
  __u64 name_args;
  __u32 write_index;
} __attribute__((__packed__));
struct user_unreg {
  __u32 size;
  __u8 disable_bit;
  __u8 __reserved;
  __u16 __reserved2;
  __u64 disable_addr;
} __attribute__((__packed__));
#define DIAG_IOC_MAGIC '*'
#define DIAG_IOCSREG _IOWR(DIAG_IOC_MAGIC, 0, struct user_reg *)
#define DIAG_IOCSDEL _IOW(DIAG_IOC_MAGIC, 1, char *)
#define DIAG_IOCSUNREG _IOW(DIAG_IOC_MAGIC, 2, struct user_unreg *)
#endif
