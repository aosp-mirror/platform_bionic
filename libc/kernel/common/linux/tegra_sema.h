/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __LINUX_TEGRA_SEMA_H
#define __LINUX_TEGRA_SEMA_H

#define TEGRA_SEMA_IOCTL_MAGIC 'r'

#define TEGRA_SEMA_IOCTL_WAIT _IOW(TEGRA_SEMA_IOCTL_MAGIC, 0x30, long *)
#define TEGRA_SEMA_IOCTL_SIGNAL _IO(TEGRA_SEMA_IOCTL_MAGIC, 0x31)

#define TEGRA_SEMA_IOCTL_MIN_NR _IOC_NR(TEGRA_SEMA_IOCTL_WAIT)
#define TEGRA_SEMA_IOCTL_MAX_NR _IOC_NR(TEGRA_SEMA_IOCTL_SIGNAL)

#endif

