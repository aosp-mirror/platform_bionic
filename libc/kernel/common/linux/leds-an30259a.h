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
#ifndef _LEDS_AN30259A_H
#define _LEDS_AN30259A_H
#include <linux/ioctl.h>
#include <linux/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define LED_LIGHT_OFF 0
#define LED_LIGHT_ON 1
#define LED_LIGHT_PULSE 2
#define LED_LIGHT_SLOPE 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct an30259a_pr_control {
 __u32 color;
 __u32 state;
 __u16 start_delay;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 time_slope_up_1;
 __u16 time_slope_up_2;
 __u16 time_on;
 __u16 time_slope_down_1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 time_slope_down_2;
 __u16 time_off;
 __u8 mid_brightness;
} __packed;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AN30259A_PR_SET_LED _IOW('S', 42, struct an30259a_pr_control)
#define AN30259A_PR_SET_LEDS _IOW('S', 43, struct an30259a_pr_control[3])
#define AN30259A_PR_SET_IMAX _IOW('S', 44, __u8)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
