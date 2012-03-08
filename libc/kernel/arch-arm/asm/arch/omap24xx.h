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
#ifndef __ASM_ARCH_OMAP24XX_H
#define __ASM_ARCH_OMAP24XX_H
#define L4_24XX_BASE 0x48000000
#define L3_24XX_BASE 0x68000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMAP24XX_IC_BASE (L4_24XX_BASE + 0xfe000)
#define VA_IC_BASE IO_ADDRESS(OMAP24XX_IC_BASE)
#define OMAP24XX_IVA_INTC_BASE 0x40000000
#define IRQ_SIR_IRQ 0x0040
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMAP24XX_32KSYNCT_BASE (L4_24XX_BASE + 0x4000)
#define OMAP24XX_PRCM_BASE (L4_24XX_BASE + 0x8000)
#define OMAP24XX_SDRC_BASE (L3_24XX_BASE + 0x9000)
#define OMAP242X_CONTROL_STATUS (L4_24XX_BASE + 0x2f8)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
