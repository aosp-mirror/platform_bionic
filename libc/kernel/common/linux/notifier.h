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
#ifndef _LINUX_NOTIFIER_H
#define _LINUX_NOTIFIER_H
#include <linux/errno.h>
#include <linux/mutex.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/rwsem.h>
struct notifier_block {
 int (*notifier_call)(struct notifier_block *, unsigned long, void *);
 struct notifier_block *next;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int priority;
};
struct atomic_notifier_head {
 spinlock_t lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct notifier_block *head;
};
struct blocking_notifier_head {
 struct rw_semaphore rwsem;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct notifier_block *head;
};
struct raw_notifier_head {
 struct notifier_block *head;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define ATOMIC_INIT_NOTIFIER_HEAD(name) do {   spin_lock_init(&(name)->lock);   (name)->head = NULL;   } while (0)
#define BLOCKING_INIT_NOTIFIER_HEAD(name) do {   init_rwsem(&(name)->rwsem);   (name)->head = NULL;   } while (0)
#define RAW_INIT_NOTIFIER_HEAD(name) do {   (name)->head = NULL;   } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ATOMIC_NOTIFIER_INIT(name) {   .lock = __SPIN_LOCK_UNLOCKED(name.lock),   .head = NULL }
#define BLOCKING_NOTIFIER_INIT(name) {   .rwsem = __RWSEM_INITIALIZER((name).rwsem),   .head = NULL }
#define RAW_NOTIFIER_INIT(name) {   .head = NULL }
#define ATOMIC_NOTIFIER_HEAD(name)   struct atomic_notifier_head name =   ATOMIC_NOTIFIER_INIT(name)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLOCKING_NOTIFIER_HEAD(name)   struct blocking_notifier_head name =   BLOCKING_NOTIFIER_INIT(name)
#define RAW_NOTIFIER_HEAD(name)   struct raw_notifier_head name =   RAW_NOTIFIER_INIT(name)
#endif
