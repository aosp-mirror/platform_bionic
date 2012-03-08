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
enum {
 TEGRA_CAMERA_MODULE_ISP = 0,
 TEGRA_CAMERA_MODULE_VI,
 TEGRA_CAMERA_MODULE_CSI,
};

enum {
 TEGRA_CAMERA_VI_CLK,
 TEGRA_CAMERA_VI_SENSOR_CLK,
};

struct tegra_camera_clk_info {
 uint id;
 uint clk_id;
 unsigned long rate;
};

#define TEGRA_CAMERA_IOCTL_ENABLE _IOWR('i', 1, uint)
#define TEGRA_CAMERA_IOCTL_DISABLE _IOWR('i', 2, uint)
#define TEGRA_CAMERA_IOCTL_CLK_SET_RATE   _IOWR('i', 3, struct tegra_camera_clk_info)
#define TEGRA_CAMERA_IOCTL_RESET _IOWR('i', 4, uint)

