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
#ifndef _LINUX_CPU_H_
#define _LINUX_CPU_H_
#include <linux/sysdev.h>
#include <linux/node.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/compiler.h>
#include <linux/cpumask.h>
#include <asm/semaphore.h>
struct cpu {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int node_id;
 int no_control;
 struct sys_device sysdev;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct notifier_block;
#define lock_cpu_hotplug() do { } while (0)
#define unlock_cpu_hotplug() do { } while (0)
#define lock_cpu_hotplug_interruptible() 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define hotcpu_notifier(fn, pri) do { } while (0)
#define register_hotcpu_notifier(nb) do { } while (0)
#define unregister_hotcpu_notifier(nb) do { } while (0)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
