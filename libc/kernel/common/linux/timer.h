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
#ifndef _LINUX_TIMER_H
#define _LINUX_TIMER_H
#include <linux/list.h>
#include <linux/spinlock.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/stddef.h>
struct tvec_t_base_s;
struct timer_list {
 struct list_head entry;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long expires;
 void (*function)(unsigned long);
 unsigned long data;
 struct tvec_t_base_s *base;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define TIMER_INITIALIZER(_function, _expires, _data) {   .function = (_function),   .expires = (_expires),   .data = (_data),   .base = &boot_tvec_bases,   }
#define DEFINE_TIMER(_name, _function, _expires, _data)   struct timer_list _name =   TIMER_INITIALIZER(_function, _expires, _data)
#define try_to_del_timer_sync(t) del_timer(t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define del_timer_sync(t) del_timer(t)
#define del_singleshot_timer_sync(t) del_timer_sync(t)
struct hrtimer;
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
