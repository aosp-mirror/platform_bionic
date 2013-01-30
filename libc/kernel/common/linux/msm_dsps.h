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
#ifndef _DSPS_H_
#define _DSPS_H_
#include <linux/ioctl.h>
#define DSPS_IOCTL_MAGIC 'd'
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DSPS_IOCTL_ON _IO(DSPS_IOCTL_MAGIC, 1)
#define DSPS_IOCTL_OFF _IO(DSPS_IOCTL_MAGIC, 2)
#define DSPS_IOCTL_READ_SLOW_TIMER _IOR(DSPS_IOCTL_MAGIC, 3, unsigned int*)
#define DSPS_IOCTL_READ_FAST_TIMER _IOR(DSPS_IOCTL_MAGIC, 4, unsigned int*)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DSPS_IOCTL_RESET _IO(DSPS_IOCTL_MAGIC, 5)
#endif
