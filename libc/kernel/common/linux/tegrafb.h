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
#ifndef _LINUX_TEGRAFB_H_
#define _LINUX_TEGRAFB_H_

#include <linux/types.h>
#include <asm/ioctl.h>

struct tegra_fb_flip_args {
 __u32 buff_id;
 __u32 pre_syncpt_id;
 __u32 pre_syncpt_val;
 __u32 post_syncpt_id;
 __u32 post_syncpt_val;
};

#define FBIO_TEGRA_SET_NVMAP_FD _IOW('F', 0x40, __u32)
#define FBIO_TEGRA_FLIP _IOW('F', 0x41, struct tegra_fb_flip_args)

#endif

