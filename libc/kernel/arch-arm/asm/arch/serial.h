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
#ifndef __ASM_ARCH_SERIAL_H
#define __ASM_ARCH_SERIAL_H
#define OMAP_MAX_NR_PORTS 3
#define OMAP1510_BASE_BAUD (12000000/16)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMAP16XX_BASE_BAUD (48000000/16)
#define is_omap_port(p) ({int __ret = 0;   if (p == IO_ADDRESS(OMAP_UART1_BASE) ||   p == IO_ADDRESS(OMAP_UART2_BASE) ||   p == IO_ADDRESS(OMAP_UART3_BASE))   __ret = 1;   __ret;   })
#endif
