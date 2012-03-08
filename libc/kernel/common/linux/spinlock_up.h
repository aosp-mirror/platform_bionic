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
#ifndef __LINUX_SPINLOCK_UP_H
#define __LINUX_SPINLOCK_UP_H
#ifndef __LINUX_SPINLOCK_H
#error "please don't include this file directly"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define __raw_spin_is_locked(lock) ((void)(lock), 0)
#define __raw_spin_lock(lock) do { (void)(lock); } while (0)
#define __raw_spin_unlock(lock) do { (void)(lock); } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __raw_spin_trylock(lock) ({ (void)(lock); 1; })
#define __raw_read_can_lock(lock) (((void)(lock), 1))
#define __raw_write_can_lock(lock) (((void)(lock), 1))
#define __raw_spin_unlock_wait(lock)   do { cpu_relax(); } while (__raw_spin_is_locked(lock))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
