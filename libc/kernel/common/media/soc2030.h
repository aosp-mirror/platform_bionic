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
#ifndef __SOC2030_H__
#define __SOC2030_H__

#include <linux/ioctl.h>  

#define SOC2030_IOCTL_SET_MODE _IOWR('o', 1, struct soc2030_mode)
#define SOC2030_IOCTL_GET_STATUS _IOC(_IOC_READ, 'o', 2, 10)
#define SOC2030_IOCTL_SET_PRIVATE _IOWR('o', 3, struct soc2030_regs)
#define SOC2030_IOCTL_GET_MODES _IO('o', 4)
#define SOC2030_IOCTL_GET_NUM_MODES _IOR('o', 5, unsigned int)
#define SOC2030_IOCTL_SET_EFFECT _IOWR('o', 6, unsigned int)
#define SOC2030_IOCTL_SET_WHITEBALANCE _IOWR('o', 7, unsigned int)
#define SOC2030_IOCTL_SET_EXP_COMP _IOWR('o', 8, int)

#define SOC2030_POLL_WAITMS 50
#define SOC2030_MAX_RETRIES 3
#define SOC2030_POLL_RETRIES 5

#define SOC2030_MAX_PRIVATE_SIZE 1024
#define SOC2030_MAX_NUM_MODES 6

#define SOC_EV_MAX 2
#define SOC_EV_MIN -2
#define EXP_TARGET 0x32

enum {
 REG_TABLE_END,
 WRITE_REG_DATA,
 WRITE_REG_BIT_H,
 WRITE_REG_BIT_L,
 POLL_REG_DATA,
 POLL_REG_BIT_H,
 POLL_REG_BIT_L,
 WRITE_VAR_DATA,
 POLL_VAR_DATA,
 DELAY_MS,
};

enum {
 EFFECT_NONE,
 EFFECT_BW,
 EFFECT_NEGATIVE,
 EFFECT_POSTERIZE,
 EFFECT_SEPIA,
 EFFECT_SOLARIZE,
 EFFECT_AQUA,
 EFFECT_MAX,
};

enum {
 WB_AUTO,
 WB_INCANDESCENT,
 WB_FLUORESCENT,
 WB_DAYLIGHT,
 WB_CLOUDYDAYLIGHT,
 WB_NIGHT,
 WB_MAX,
};

struct soc2030_regs {
 __u8 op;
 __u16 addr;
 __u16 val;
};

struct soc2030_mode {
 int xres;
 int yres;
 int fps;
 struct soc2030_regs *regset;
};

#endif


