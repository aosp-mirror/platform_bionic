/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __UBLOCK_H_
#define __UBLOCK_H_

#include <linux/types.h>

#define UBLOCK_VERSION 0

enum {
 UBLOCK_INIT_IN = 0,
 UBLOCK_INIT_OUT = 1,
 UBLOCK_READY_IN = 2,
 UBLOCK_READY_OUT = 3,
 UBLOCK_READ_IN = 4,
 UBLOCK_READ_OUT = 5,
 UBLOCK_WRITE_IN = 6,
 UBLOCK_WRITE_OUT = 7,
};

struct ublock_in_header {
 __u32 seq;
 __u32 opcode;
};

struct ublock_out_header {
 __u32 seq;
 __u32 opcode;
};

struct ublock_init_in {
 __u32 version;
 __u32 max_buf;
 __u32 index;
};

struct ublock_init_out {
 __u32 version;
 __u32 max_buf;
 __u64 size;
};

struct ublock_ready_in {
 __u32 _unused;
};

struct ublock_ready_out {
 __u32 _unused;
};

struct ublock_read_in {
 __u64 offset;
 __u64 length;
};

struct ublock_read_out {
 __s32 status;
 __u8 data[];
};

struct ublock_write_in {
 __u64 offset;
 __u64 length;
 __u8 data[];
};

struct ublock_write_out {
 __s32 status;
};

#endif

