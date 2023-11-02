/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
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
