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
#ifndef _LINUX_IRQ_H
#define _LINUX_IRQ_H
#include <linux/smp.h>
#include <linux/linkage.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/cache.h>
#include <linux/spinlock.h>
#include <linux/cpumask.h>
#include <linux/irqreturn.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <asm/irq.h>
#include <asm/ptrace.h>
#define IRQ_TYPE_NONE 0x00000000
#define IRQ_TYPE_EDGE_RISING 0x00000001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_TYPE_EDGE_FALLING 0x00000002
#define IRQ_TYPE_EDGE_BOTH (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING)
#define IRQ_TYPE_LEVEL_HIGH 0x00000004
#define IRQ_TYPE_LEVEL_LOW 0x00000008
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_TYPE_SENSE_MASK 0x0000000f
#define IRQ_TYPE_PROBE 0x00000010
#define IRQ_INPROGRESS 0x00010000
#define IRQ_DISABLED 0x00020000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_PENDING 0x00040000
#define IRQ_REPLAY 0x00080000
#define IRQ_AUTODETECT 0x00100000
#define IRQ_WAITING 0x00200000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_LEVEL 0x00400000
#define IRQ_MASKED 0x00800000
#define IRQ_PER_CPU 0x01000000
#define CHECK_IRQ_PER_CPU(var) 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_NOPROBE 0x02000000
#define IRQ_NOREQUEST 0x04000000
#define IRQ_NOAUTOEN 0x08000000
#define IRQ_DELAYED_DISABLE 0x10000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IRQ_WAKEUP 0x20000000
struct proc_dir_entry;
struct irq_chip {
 const char *name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int (*startup)(unsigned int irq);
 void (*shutdown)(unsigned int irq);
 void (*enable)(unsigned int irq);
 void (*disable)(unsigned int irq);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*ack)(unsigned int irq);
 void (*mask)(unsigned int irq);
 void (*mask_ack)(unsigned int irq);
 void (*unmask)(unsigned int irq);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*eoi)(unsigned int irq);
 void (*end)(unsigned int irq);
 void (*set_affinity)(unsigned int irq, cpumask_t dest);
 int (*retrigger)(unsigned int irq);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*set_type)(unsigned int irq, unsigned int flow_type);
 int (*set_wake)(unsigned int irq, unsigned int on);
 const char *typename;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct irq_desc {
 void fastcall (*handle_irq)(unsigned int irq,
 struct irq_desc *desc,
 struct pt_regs *regs);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct irq_chip *chip;
 void *handler_data;
 void *chip_data;
 struct irqaction *action;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int status;
 unsigned int depth;
 unsigned int wake_depth;
 unsigned int irq_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int irqs_unhandled;
 spinlock_t lock;
} ____cacheline_aligned;
#define hw_interrupt_type irq_chip
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct irq_chip hw_irq_controller;
#define no_irq_type no_irq_chip
typedef struct irq_desc irq_desc_t;
#include <asm/hw_irq.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
