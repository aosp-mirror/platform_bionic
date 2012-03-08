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
#ifndef __ASM_ARCH_OMAP_GPIO_SWITCH_H
#define __ASM_ARCH_OMAP_GPIO_SWITCH_H
#include <linux/types.h>
#define OMAP_GPIO_SWITCH_TYPE_COVER 0x0000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OMAP_GPIO_SWITCH_TYPE_CONNECTION 0x0001
#define OMAP_GPIO_SWITCH_TYPE_ACTIVITY 0x0002
#define OMAP_GPIO_SWITCH_FLAG_INVERTED 0x0001
#define OMAP_GPIO_SWITCH_FLAG_OUTPUT 0x0002
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct omap_gpio_switch {
 const char *name;
 s16 gpio;
 unsigned flags:4;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned type:4;
 u16 debounce_rising;
 u16 debounce_falling;
 void (* notify)(void *data, int state);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *notify_data;
};
#endif
