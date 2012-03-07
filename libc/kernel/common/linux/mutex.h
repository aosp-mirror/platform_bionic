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
#ifndef __LINUX_MUTEX_H
#define __LINUX_MUTEX_H
#include <linux/list.h>
#include <linux/spinlock_types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/linkage.h>
#include <linux/lockdep.h>
#include <asm/atomic.h>
struct mutex {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 atomic_t count;
 spinlock_t wait_lock;
 struct list_head wait_list;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mutex_waiter {
 struct list_head list;
 struct task_struct *task;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __DEBUG_MUTEX_INITIALIZER(lockname)
#define mutex_init(mutex)  do {   static struct lock_class_key __key;     __mutex_init((mutex), #mutex, &__key);  } while (0)
#define mutex_destroy(mutex) do { } while (0)
#define __DEP_MAP_MUTEX_INITIALIZER(lockname)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __MUTEX_INITIALIZER(lockname)   { .count = ATOMIC_INIT(1)   , .wait_lock = SPIN_LOCK_UNLOCKED   , .wait_list = LIST_HEAD_INIT(lockname.wait_list)   __DEBUG_MUTEX_INITIALIZER(lockname)   __DEP_MAP_MUTEX_INITIALIZER(lockname) }
#define DEFINE_MUTEX(mutexname)   struct mutex mutexname = __MUTEX_INITIALIZER(mutexname)
#define mutex_lock_nested(lock, subclass) mutex_lock(lock)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
