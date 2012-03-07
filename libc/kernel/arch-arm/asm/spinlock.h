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
#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H
#if __LINUX_ARM_ARCH__ < 6
#error SMP not supported on pre-ARMv6 CPUs
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define __raw_spin_is_locked(x) ((x)->lock != 0)
#define __raw_spin_unlock_wait(lock)   do { while (__raw_spin_is_locked(lock)) cpu_relax(); } while (0)
#define __raw_spin_lock_flags(lock, flags) __raw_spin_lock(lock)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define rwlock_is_locked(x) (*((volatile unsigned int *)(x)) != 0)
#define __raw_write_can_lock(x) ((x)->lock == 0x80000000)
#define __raw_read_can_lock(x) ((x)->lock < 0x80000000)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
