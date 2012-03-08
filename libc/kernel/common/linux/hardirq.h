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
#ifndef LINUX_HARDIRQ_H
#define LINUX_HARDIRQ_H
#include <linux/preempt.h>
#include <linux/smp_lock.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/lockdep.h>
#include <asm/hardirq.h>
#include <asm/system.h>
#define PREEMPT_BITS 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOFTIRQ_BITS 8
#ifndef HARDIRQ_BITS
#define HARDIRQ_BITS 12
#if 1 << HARDIRQ_BITS < NR_IRQS
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#error HARDIRQ_BITS is too low!
#endif
#endif
#define PREEMPT_SHIFT 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOFTIRQ_SHIFT (PREEMPT_SHIFT + PREEMPT_BITS)
#define HARDIRQ_SHIFT (SOFTIRQ_SHIFT + SOFTIRQ_BITS)
#define __IRQ_MASK(x) ((1UL << (x))-1)
#define PREEMPT_MASK (__IRQ_MASK(PREEMPT_BITS) << PREEMPT_SHIFT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOFTIRQ_MASK (__IRQ_MASK(SOFTIRQ_BITS) << SOFTIRQ_SHIFT)
#define HARDIRQ_MASK (__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)
#define PREEMPT_OFFSET (1UL << PREEMPT_SHIFT)
#define SOFTIRQ_OFFSET (1UL << SOFTIRQ_SHIFT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HARDIRQ_OFFSET (1UL << HARDIRQ_SHIFT)
#if PREEMPT_ACTIVE < 1 << HARDIRQ_SHIFT + HARDIRQ_BITS
#error PREEMPT_ACTIVE is too low!
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define hardirq_count() (preempt_count() & HARDIRQ_MASK)
#define softirq_count() (preempt_count() & SOFTIRQ_MASK)
#define irq_count() (preempt_count() & (HARDIRQ_MASK | SOFTIRQ_MASK))
#define in_irq() (hardirq_count())
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define in_softirq() (softirq_count())
#define in_interrupt() (irq_count())
#define in_atomic() ((preempt_count() & ~PREEMPT_ACTIVE) != 0)
#define preemptible() 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_EXIT_OFFSET HARDIRQ_OFFSET
#define synchronize_irq(irq) barrier()
struct task_struct;
#define irq_enter()   do {   account_system_vtime(current);   add_preempt_count(HARDIRQ_OFFSET);   trace_hardirq_enter();   } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __irq_exit()   do {   trace_hardirq_exit();   account_system_vtime(current);   sub_preempt_count(HARDIRQ_OFFSET);   } while (0)
#define nmi_enter() do { lockdep_off(); irq_enter(); } while (0)
#define nmi_exit() do { __irq_exit(); lockdep_on(); } while (0)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
