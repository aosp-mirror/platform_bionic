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
#ifndef _LINUX_BYTEORDER_SWABB_H
#define _LINUX_BYTEORDER_SWABB_H
#define ___swahw32(x)  ({   __u32 __x = (x);   ((__u32)(   (((__u32)(__x) & (__u32)0x0000ffffUL) << 16) |   (((__u32)(__x) & (__u32)0xffff0000UL) >> 16) ));  })
#define ___swahb32(x)  ({   __u32 __x = (x);   ((__u32)(   (((__u32)(__x) & (__u32)0x00ff00ffUL) << 8) |   (((__u32)(__x) & (__u32)0xff00ff00UL) >> 8) ));  })
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ___constant_swahw32(x)   ((__u32)(   (((__u32)(x) & (__u32)0x0000ffffUL) << 16) |   (((__u32)(x) & (__u32)0xffff0000UL) >> 16) ))
#define ___constant_swahb32(x)   ((__u32)(   (((__u32)(x) & (__u32)0x00ff00ffUL) << 8) |   (((__u32)(x) & (__u32)0xff00ff00UL) >> 8) ))
#ifndef __arch__swahw32
#define __arch__swahw32(x) ___swahw32(x)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#ifndef __arch__swahb32
#define __arch__swahb32(x) ___swahb32(x)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifndef __arch__swahw32p
#define __arch__swahw32p(x) __swahw32(*(x))
#endif
#ifndef __arch__swahb32p
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __arch__swahb32p(x) __swahb32(*(x))
#endif
#ifndef __arch__swahw32s
#define __arch__swahw32s(x) do { *(x) = __swahw32p((x)); } while (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#ifndef __arch__swahb32s
#define __arch__swahb32s(x) do { *(x) = __swahb32p((x)); } while (0)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#if defined(__GNUC__) && defined(__OPTIMIZE__)
#define __swahw32(x)  (__builtin_constant_p((__u32)(x)) ?   ___swahw32((x)) :   __fswahw32((x)))
#define __swahb32(x)  (__builtin_constant_p((__u32)(x)) ?   ___swahb32((x)) :   __fswahb32((x)))
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __swahw32(x) __fswahw32(x)
#define __swahb32(x) __fswahb32(x)
#endif
#ifdef __BYTEORDER_HAS_U64__
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#endif
