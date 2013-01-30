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
#ifndef __KXTF9_H__
#define __KXTF9_H__
#include <linux/ioctl.h>
#define KXTF9_IOCTL_BASE 77
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define KXTF9_IOCTL_SET_DELAY _IOW(KXTF9_IOCTL_BASE, 0, int)
#define KXTF9_IOCTL_GET_DELAY _IOR(KXTF9_IOCTL_BASE, 1, int)
#define KXTF9_IOCTL_SET_ENABLE _IOW(KXTF9_IOCTL_BASE, 2, int)
#define KXTF9_IOCTL_GET_ENABLE _IOR(KXTF9_IOCTL_BASE, 3, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define KXTF9_IOCTL_SET_G_RANGE _IOW(KXTF9_IOCTL_BASE, 4, int)
#define KXTF9_IOCTL_SET_TILT_ENABLE _IOW(KXTF9_IOCTL_BASE, 5, int)
#define KXTF9_IOCTL_SET_TAP_ENABLE _IOW(KXTF9_IOCTL_BASE, 6, int)
#define KXTF9_IOCTL_SET_WAKE_ENABLE _IOW(KXTF9_IOCTL_BASE, 7, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define KXTF9_IOCTL_SET_PM_MODE _IOW(KXTF9_IOCTL_BASE, 8, int)
#define KXTF9_IOCTL_SELF_TEST _IOW(KXTF9_IOCTL_BASE, 9, int)
#define KXTF9_IOCTL_SET_SENSITIVITY _IOW(KXTF9_IOCTL_BASE, 10, int)
#define RES_12BIT 0x40
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define KXTF9_G_2G 0x00
#define KXTF9_G_4G 0x08
#define KXTF9_G_8G 0x10
#define TPE 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define WUFE 0x02
#define TDTE 0x04
#define OTP1_6 0x00
#define OTP6_3 0x20
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OTP12_5 0x40
#define OTP50 0x60
#define OWUF25 0x00
#define OWUF50 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OWUF100 0x02
#define OWUF200 0x03
#define OTDT50 0x00
#define OTDT100 0x04
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OTDT200 0x08
#define OTDT400 0x0C
#define IEN 0x20
#define IEA 0x10
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IEL 0x08
#define IEU 0x04
#define ODR800 0x06
#define ODR400 0x05
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ODR200 0x04
#define ODR100 0x03
#define ODR50 0x02
#define ODR25 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ODR12_5 0x00
#define SENSITIVITY_REGS 0x07
#endif
