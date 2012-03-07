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
#ifndef __LINUX_TEGRA_SEMA_H
#define __LINUX_TEGRA_SEMA_H
#define TEGRA_SEMA_IOCTL_MAGIC 'r'
#define TEGRA_SEMA_IOCTL_WAIT _IOW(TEGRA_SEMA_IOCTL_MAGIC, 0x30, long *)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TEGRA_SEMA_IOCTL_SIGNAL _IO(TEGRA_SEMA_IOCTL_MAGIC, 0x31)
#define TEGRA_SEMA_IOCTL_MIN_NR _IOC_NR(TEGRA_SEMA_IOCTL_WAIT)
#define TEGRA_SEMA_IOCTL_MAX_NR _IOC_NR(TEGRA_SEMA_IOCTL_SIGNAL)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
