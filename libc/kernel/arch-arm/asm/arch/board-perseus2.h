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
#ifndef __ASM_ARCH_OMAP_PERSEUS2_H
#define __ASM_ARCH_OMAP_PERSEUS2_H
#include <asm/arch/fpga.h>
#ifndef OMAP_SDRAM_DEVICE
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMAP_SDRAM_DEVICE D256M_1X16_4B
#endif
#define MAXIRQNUM IH_BOARD_BASE
#define MAXFIQNUM MAXIRQNUM
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAXSWINUM MAXIRQNUM
#define NR_IRQS (MAXIRQNUM + 1)
#endif
