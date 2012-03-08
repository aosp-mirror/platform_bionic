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
#ifndef __LINUX_TEGRA_AVP_H
#define __LINUX_TEGRA_AVP_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define TEGRA_AVP_LIB_MAX_NAME 32
#define TEGRA_AVP_LIB_MAX_ARGS 220  

struct tegra_avp_lib {
 char name[TEGRA_AVP_LIB_MAX_NAME];
 void __user *args;
 size_t args_len;
 int greedy;
 unsigned long handle;
};

#define TEGRA_AVP_IOCTL_MAGIC 'r'

#define TEGRA_AVP_IOCTL_LOAD_LIB _IOWR(TEGRA_AVP_IOCTL_MAGIC, 0x40, struct tegra_avp_lib)
#define TEGRA_AVP_IOCTL_UNLOAD_LIB _IOW(TEGRA_AVP_IOCTL_MAGIC, 0x41, unsigned long)

#define TEGRA_AVP_IOCTL_MIN_NR _IOC_NR(TEGRA_AVP_IOCTL_LOAD_LIB)
#define TEGRA_AVP_IOCTL_MAX_NR _IOC_NR(TEGRA_AVP_IOCTL_UNLOAD_LIB)

#endif

