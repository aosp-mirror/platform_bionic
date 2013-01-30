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
#ifndef _LINUX_SWAP_H
#define _LINUX_SWAP_H
#include <linux/spinlock.h>
#include <linux/linkage.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/mmzone.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <asm/atomic.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <asm/page.h>
#define SWAP_FLAG_PREFER 0x8000
#define SWAP_FLAG_PRIO_MASK 0x7fff
#define SWAP_FLAG_PRIO_SHIFT 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAX_SWAPFILES_SHIFT 5
#define MAX_SWAPFILES (1 << MAX_SWAPFILES_SHIFT)
union swap_header {
 struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char reserved[PAGE_SIZE - 10];
 char magic[10];
 } magic;
 struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char bootbits[1024];
 __u32 version;
 __u32 last_page;
 __u32 nr_badpages;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char sws_uuid[16];
 unsigned char sws_volume[16];
 __u32 padding[117];
 __u32 badpages[1];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } info;
};
typedef struct {
 unsigned long val;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} swp_entry_t;
struct reclaim_state {
 unsigned long reclaimed_slab;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
