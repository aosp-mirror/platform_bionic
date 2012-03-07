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
#ifndef _LINUX_KTIME_H
#define _LINUX_KTIME_H
#include <linux/time.h>
#include <linux/jiffies.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef union {
 s64 tv64;
#if BITS_PER_LONG != (64 && !defined(CONFIG_KTIME_SCALAR))
 struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __BIG_ENDIAN
 s32 sec, nsec;
#else
 s32 nsec, sec;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
 } tv;
#endif
} ktime_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define KTIME_MAX ((s64)~((u64)1 << 63))
#define KTIME_SEC_MAX (KTIME_MAX / NSEC_PER_SEC)
#if BITS_PER_LONG == 64
#if BITS_PER_LONG == 64
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define ktime_sub(lhs, rhs)   ({ (ktime_t){ .tv64 = (lhs).tv64 - (rhs).tv64 }; })
#define ktime_add(lhs, rhs)   ({ (ktime_t){ .tv64 = (lhs).tv64 + (rhs).tv64 }; })
#define ktime_add_ns(kt, nsval)   ({ (ktime_t){ .tv64 = (kt).tv64 + (nsval) }; })
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ktime_to_timespec(kt) ns_to_timespec((kt).tv64)
#define ktime_to_timeval(kt) ns_to_timeval((kt).tv64)
#define ktime_to_ns(kt) ((kt).tv64)
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define KTIME_REALTIME_RES (ktime_t){ .tv64 = TICK_NSEC }
#define KTIME_MONOTONIC_RES (ktime_t){ .tv64 = TICK_NSEC }
#define ktime_get_real_ts(ts) getnstimeofday(ts)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
