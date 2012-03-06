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
#ifndef _OMAP_CSMI_H_
#define _OMAP_CSMI_H_
#include <asm/ioctl.h>
#define OMAP_CSMI_TTY_ENABLE_ACK _IO('c', 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMAP_CSMI_TTY_DISABLE_ACK _IO('c', 1)
#define OMAP_CSMI_TTY_READ_UNACKED _IOR('c', 2, int)
#define OMAP_CSMI_TTY_ACK _IOW('c', 3, int)
#define OMAP_CSMI_TTY_WAKEUP_AND_ACK _IOW('c', 4, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
