/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __INCLUDE_UAPI_SOUND_SOF_ABI_H__
#define __INCLUDE_UAPI_SOUND_SOF_ABI_H__
#include <linux/types.h>
#define SOF_ABI_MAJOR 3
#define SOF_ABI_MINOR 23
#define SOF_ABI_PATCH 0
#define SOF_ABI_MAJOR_SHIFT 24
#define SOF_ABI_MAJOR_MASK 0xff
#define SOF_ABI_MINOR_SHIFT 12
#define SOF_ABI_MINOR_MASK 0xfff
#define SOF_ABI_PATCH_SHIFT 0
#define SOF_ABI_PATCH_MASK 0xfff
#define SOF_ABI_VER(major,minor,patch) (((major) << SOF_ABI_MAJOR_SHIFT) | ((minor) << SOF_ABI_MINOR_SHIFT) | ((patch) << SOF_ABI_PATCH_SHIFT))
#define SOF_ABI_VERSION_MAJOR(version) (((version) >> SOF_ABI_MAJOR_SHIFT) & SOF_ABI_MAJOR_MASK)
#define SOF_ABI_VERSION_MINOR(version) (((version) >> SOF_ABI_MINOR_SHIFT) & SOF_ABI_MINOR_MASK)
#define SOF_ABI_VERSION_PATCH(version) (((version) >> SOF_ABI_PATCH_SHIFT) & SOF_ABI_PATCH_MASK)
#define SOF_ABI_VERSION_INCOMPATIBLE(sof_ver,client_ver) (SOF_ABI_VERSION_MAJOR((sof_ver)) != SOF_ABI_VERSION_MAJOR((client_ver)))
#define SOF_ABI_VERSION SOF_ABI_VER(SOF_ABI_MAJOR, SOF_ABI_MINOR, SOF_ABI_PATCH)
#define SOF_ABI_MAGIC 0x00464F53
#define SOF_IPC4_ABI_MAGIC 0x34464F53
#endif
