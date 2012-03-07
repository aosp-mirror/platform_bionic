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
#ifndef __LINUX_SPINLOCK_TYPES_H
#define __LINUX_SPINLOCK_TYPES_H
#include <linux/lockdep.h>
#include <linux/spinlock_types_up.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct {
 raw_spinlock_t raw_lock;
} spinlock_t;
#define SPINLOCK_MAGIC 0xdead4ead
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct {
 raw_rwlock_t raw_lock;
} rwlock_t;
#define RWLOCK_MAGIC 0xdeaf1eed
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SPINLOCK_OWNER_INIT ((void *)-1L)
#define SPIN_DEP_MAP_INIT(lockname)
#define RW_DEP_MAP_INIT(lockname)
#define __SPIN_LOCK_UNLOCKED(lockname)   (spinlock_t) { .raw_lock = __RAW_SPIN_LOCK_UNLOCKED,   SPIN_DEP_MAP_INIT(lockname) }
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __RW_LOCK_UNLOCKED(lockname)   (rwlock_t) { .raw_lock = __RAW_RW_LOCK_UNLOCKED,   RW_DEP_MAP_INIT(lockname) }
#define SPIN_LOCK_UNLOCKED __SPIN_LOCK_UNLOCKED(old_style_spin_init)
#define RW_LOCK_UNLOCKED __RW_LOCK_UNLOCKED(old_style_rw_init)
#define DEFINE_SPINLOCK(x) spinlock_t x = __SPIN_LOCK_UNLOCKED(x)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DEFINE_RWLOCK(x) rwlock_t x = __RW_LOCK_UNLOCKED(x)
#endif
