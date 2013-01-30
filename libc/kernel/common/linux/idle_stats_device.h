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
#ifndef __ARCH_ARM_MACH_MSM_IDLE_STATS_DEVICE_H
#define __ARCH_ARM_MACH_MSM_IDLE_STATS_DEVICE_H
#include <linux/types.h>
#include <linux/ioctl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSM_IDLE_STATS_EVENT_NONE 0
#define MSM_IDLE_STATS_EVENT_BUSY_TIMER_EXPIRED 1
#define MSM_IDLE_STATS_EVENT_BUSY_TIMER_EXPIRED_RESET 2
#define MSM_IDLE_STATS_EVENT_COLLECTION_NEARLY_FULL 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSM_IDLE_STATS_EVENT_COLLECTION_FULL 8
#define MSM_IDLE_STATS_EVENT_IDLE_TIMER_EXPIRED 16
#define MSM_IDLE_STATS_NR_MAX_INTERVALS 200
struct msm_idle_pulse {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __s64 busy_start_time;
 __u32 busy_interval;
 __u32 wait_interval;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_idle_read_stats {
 __u32 event;
 __s64 return_timestamp;
 __u32 busy_timer_remaining;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 nr_collected;
 struct msm_idle_pulse pulse_chain[MSM_IDLE_STATS_NR_MAX_INTERVALS];
};
struct msm_idle_write_stats {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 busy_timer;
 __u32 next_busy_timer;
 __u32 max_samples;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSM_IDLE_STATS_IOC_MAGIC 0xD8
#define MSM_IDLE_STATS_IOC_READ_STATS   _IOWR(MSM_IDLE_STATS_IOC_MAGIC, 1, struct msm_idle_read_stats)
#define MSM_IDLE_STATS_IOC_WRITE_STATS   _IOWR(MSM_IDLE_STATS_IOC_MAGIC, 2, struct msm_idle_write_stats)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
