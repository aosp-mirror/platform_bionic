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
#ifndef _LINUX_ANDROID_ALARM_H
#define _LINUX_ANDROID_ALARM_H
#include <asm/ioctl.h>
#include <linux/time.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef enum {
 ANDROID_ALARM_RTC_WAKEUP,
 ANDROID_ALARM_RTC,
 ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ANDROID_ALARM_ELAPSED_REALTIME,
 ANDROID_ALARM_SYSTEMTIME,
 ANDROID_ALARM_TYPE_COUNT,
} android_alarm_type_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef enum {
 ANDROID_ALARM_RTC_WAKEUP_MASK = 1U << ANDROID_ALARM_RTC_WAKEUP,
 ANDROID_ALARM_RTC_MASK = 1U << ANDROID_ALARM_RTC,
 ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP_MASK = 1U << ANDROID_ALARM_ELAPSED_REALTIME_WAKEUP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ANDROID_ALARM_ELAPSED_REALTIME_MASK = 1U << ANDROID_ALARM_ELAPSED_REALTIME,
 ANDROID_ALARM_SYSTEMTIME_MASK = 1U << ANDROID_ALARM_SYSTEMTIME,
 ANDROID_ALARM_TIME_CHANGE_MASK = 1U << 16
} android_alarm_return_flags_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ANDROID_ALARM_CLEAR(type) _IO('a', 0 | ((type) << 4))
#define ANDROID_ALARM_WAIT _IO('a', 1)
#define ANDROID_ALARM_SET(type) _IOW('a', 2 | ((type) << 4), struct timespec)
#define ANDROID_ALARM_SET_AND_WAIT(type) _IOW('a', 3 | ((type) << 4), struct timespec)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ANDROID_ALARM_GET_TIME(type) _IOW('a', 4 | ((type) << 4), struct timespec)
#define ANDROID_ALARM_SET_RTC _IOW('a', 5, struct timespec)
#define ANDROID_ALARM_SET_TIMEZONE _IOW('a', 6, struct timezone)
#define ANDROID_ALARM_BASE_CMD(cmd) (cmd & ~(_IOC(0, 0, 0xf0, 0)))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ANDROID_ALARM_IOCTL_TO_TYPE(cmd) (_IOC_NR(cmd) >> 4)
#endif
