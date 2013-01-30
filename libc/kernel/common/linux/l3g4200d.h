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
#ifndef __L3G4200D_H__
#define __L3G4200D_H__
#include <linux/ioctl.h>
#define L3G4200D_NAME "l3g4200d"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define L3G4200D_IOCTL_BASE 77
#define L3G4200D_IOCTL_SET_DELAY _IOW(L3G4200D_IOCTL_BASE, 0, int)
#define L3G4200D_IOCTL_GET_DELAY _IOR(L3G4200D_IOCTL_BASE, 1, int)
#define L3G4200D_IOCTL_SET_ENABLE _IOW(L3G4200D_IOCTL_BASE, 2, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define L3G4200D_IOCTL_GET_ENABLE _IOR(L3G4200D_IOCTL_BASE, 3, int)
#endif
