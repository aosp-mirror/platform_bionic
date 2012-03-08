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
#ifndef __LINUX_SPINLOCK_TYPES_UP_H
#define __LINUX_SPINLOCK_TYPES_UP_H
#ifndef __LINUX_SPINLOCK_TYPES_H
#error "please don't include this file directly"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
typedef struct { } raw_spinlock_t;
#define __RAW_SPIN_LOCK_UNLOCKED { }
typedef struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} raw_rwlock_t;
#define __RAW_RW_LOCK_UNLOCKED { }
#endif
