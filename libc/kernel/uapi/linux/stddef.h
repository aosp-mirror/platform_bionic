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
#ifndef _UAPI_LINUX_STDDEF_H
#define _UAPI_LINUX_STDDEF_H
#include <linux/compiler_types.h>
#ifndef __always_inline
#define __always_inline inline
#endif
#define __struct_group(TAG,NAME,ATTRS,MEMBERS...) union { struct { MEMBERS } ATTRS; struct TAG { MEMBERS } ATTRS NAME; }
#ifdef __cplusplus
#define __DECLARE_FLEX_ARRAY(T,member) T member[0]
#else
#define __DECLARE_FLEX_ARRAY(TYPE,NAME) struct { struct { } __empty_ ##NAME; TYPE NAME[]; }
#endif
#ifndef __counted_by
#define __counted_by(m)
#endif
#endif
