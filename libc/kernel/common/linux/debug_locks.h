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
#ifndef __LINUX_DEBUG_LOCKING_H
#define __LINUX_DEBUG_LOCKING_H
struct task_struct;
#define _RET_IP_ (unsigned long)__builtin_return_address(0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define _THIS_IP_ ({ __label__ __here; __here: (unsigned long)&&__here; })
#define DEBUG_LOCKS_WARN_ON(c)  ({   int __ret = 0;     if (unlikely(c)) {   if (debug_locks_off())   WARN_ON(1);   __ret = 1;   }   __ret;  })
#define SMP_DEBUG_LOCKS_WARN_ON(c) do { } while (0)
#define locking_selftest() do { } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
