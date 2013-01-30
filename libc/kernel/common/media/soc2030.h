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
#ifndef __SOC2030_H__
#define __SOC2030_H__
#include <linux/ioctl.h>
#define SOC2030_IOCTL_SET_MODE _IOWR('o', 1, struct soc2030_mode)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOC2030_IOCTL_GET_STATUS _IOC(_IOC_READ, 'o', 2, 10)
#define SOC2030_IOCTL_SET_PRIVATE _IOWR('o', 3, struct soc2030_regs)
#define SOC2030_IOCTL_GET_MODES _IO('o', 4)
#define SOC2030_IOCTL_GET_NUM_MODES _IOR('o', 5, unsigned int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOC2030_IOCTL_SET_EFFECT _IOWR('o', 6, unsigned int)
#define SOC2030_IOCTL_SET_WHITEBALANCE _IOWR('o', 7, unsigned int)
#define SOC2030_IOCTL_SET_EXP_COMP _IOWR('o', 8, int)
#define SOC2030_IOCTL_SET_LOCK _IOWR('o', 9, struct soc2030_lock)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOC2030_POLL_WAITMS 50
#define SOC2030_MAX_RETRIES 3
#define SOC2030_POLL_RETRIES 7
#define SOC2030_MAX_PRIVATE_SIZE 1024
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SOC2030_MAX_NUM_MODES 6
#define SOC_EV_MAX 2
#define SOC_EV_MIN -2
#define EXP_TARGET 0x32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 REG_TABLE_END,
 WRITE_REG_DATA,
 WRITE_REG_BIT_H,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 WRITE_REG_BIT_L,
 POLL_REG_DATA,
 POLL_REG_BIT_H,
 POLL_REG_BIT_L,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 WRITE_VAR_DATA,
 POLL_VAR_DATA,
 DELAY_MS,
 WRITE_REG_VAR1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 WRITE_REG_VAR2,
 WRITE_REG_VAR3,
 WRITE_REG_VAR4,
 READ_REG_VAR1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 READ_REG_VAR2,
 READ_REG_VAR3,
 READ_REG_VAR4,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REG_VAR1 (READ_REG_VAR1 - READ_REG_VAR1)
#define REG_VAR2 (READ_REG_VAR2 - READ_REG_VAR1)
#define REG_VAR3 (READ_REG_VAR3 - READ_REG_VAR1)
#define REG_VAR4 (READ_REG_VAR4 - READ_REG_VAR1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 EFFECT_NONE,
 EFFECT_BW,
 EFFECT_NEGATIVE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EFFECT_POSTERIZE,
 EFFECT_SEPIA,
 EFFECT_SOLARIZE,
 EFFECT_AQUA,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EFFECT_MAX,
};
enum {
 WB_AUTO,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 WB_INCANDESCENT,
 WB_FLUORESCENT,
 WB_DAYLIGHT,
 WB_CLOUDYDAYLIGHT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 WB_NIGHT,
 WB_MAX,
};
struct soc2030_regs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 op;
 __u16 addr;
 __u16 val;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct soc2030_lock {
 __u8 aelock;
 __u8 aerelock;
 __u8 awblock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 awbrelock;
 __u8 previewactive;
};
struct soc2030_mode {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int xres;
 int yres;
 int fps;
 struct soc2030_regs *regset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif
