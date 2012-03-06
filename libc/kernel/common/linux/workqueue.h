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
#ifndef _LINUX_WORKQUEUE_H
#define _LINUX_WORKQUEUE_H
#include <linux/timer.h>
#include <linux/linkage.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/bitops.h>
struct workqueue_struct;
struct work_struct {
 unsigned long pending;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct list_head entry;
 void (*func)(void *);
 void *data;
 void *wq_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct timer_list timer;
};
struct execute_work {
 struct work_struct work;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define __WORK_INITIALIZER(n, f, d) {   .entry = { &(n).entry, &(n).entry },   .func = (f),   .data = (d),   .timer = TIMER_INITIALIZER(NULL, 0, 0),   }
#define DECLARE_WORK(n, f, d)   struct work_struct n = __WORK_INITIALIZER(n, f, d)
#define PREPARE_WORK(_work, _func, _data)   do {   (_work)->func = _func;   (_work)->data = _data;   } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define INIT_WORK(_work, _func, _data)   do {   INIT_LIST_HEAD(&(_work)->entry);   (_work)->pending = 0;   PREPARE_WORK((_work), (_func), (_data));   init_timer(&(_work)->timer);   } while (0)
#define create_workqueue(name) __create_workqueue((name), 0)
#define create_singlethread_workqueue(name) __create_workqueue((name), 1)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
