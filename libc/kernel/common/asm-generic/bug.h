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
#ifndef _ASM_GENERIC_BUG_H
#define _ASM_GENERIC_BUG_H
#include <linux/compiler.h>
#ifndef HAVE_ARCH_BUG
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BUG()
#endif
#ifndef HAVE_ARCH_BUG_ON
#define BUG_ON(condition) do { if (condition) ; } while(0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#ifndef HAVE_ARCH_WARN_ON
#define WARN_ON(condition) do { if (condition) ; } while(0)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define WARN_ON_ONCE(condition)  ({   static int __warn_once = 1;   int __ret = 0;     if (unlikely((condition) && __warn_once)) {   __warn_once = 0;   WARN_ON(1);   __ret = 1;   }   __ret;  })
#define WARN_ON_SMP(x) do { } while (0)
#endif
