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
#ifndef _LINUX_TRACE_IRQFLAGS_H
#define _LINUX_TRACE_IRQFLAGS_H
#define trace_hardirqs_on() do { } while (0)
#define trace_hardirqs_off() do { } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define trace_softirqs_on(ip) do { } while (0)
#define trace_softirqs_off(ip) do { } while (0)
#define trace_hardirq_context(p) 0
#define trace_softirq_context(p) 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define trace_hardirqs_enabled(p) 0
#define trace_softirqs_enabled(p) 0
#define trace_hardirq_enter() do { } while (0)
#define trace_hardirq_exit() do { } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define trace_softirq_enter() do { } while (0)
#define trace_softirq_exit() do { } while (0)
#define INIT_TRACE_IRQFLAGS
#define raw_local_irq_disable() local_irq_disable()
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define raw_local_irq_enable() local_irq_enable()
#define raw_local_irq_save(flags) local_irq_save(flags)
#define raw_local_irq_restore(flags) local_irq_restore(flags)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
