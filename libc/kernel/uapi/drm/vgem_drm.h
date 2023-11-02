/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_VGEM_DRM_H_
#define _UAPI_VGEM_DRM_H_
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DRM_VGEM_FENCE_ATTACH 0x1
#define DRM_VGEM_FENCE_SIGNAL 0x2
#define DRM_IOCTL_VGEM_FENCE_ATTACH DRM_IOWR(DRM_COMMAND_BASE + DRM_VGEM_FENCE_ATTACH, struct drm_vgem_fence_attach)
#define DRM_IOCTL_VGEM_FENCE_SIGNAL DRM_IOW(DRM_COMMAND_BASE + DRM_VGEM_FENCE_SIGNAL, struct drm_vgem_fence_signal)
struct drm_vgem_fence_attach {
  __u32 handle;
  __u32 flags;
#define VGEM_FENCE_WRITE 0x1
  __u32 out_fence;
  __u32 pad;
};
struct drm_vgem_fence_signal {
  __u32 fence;
  __u32 flags;
};
#ifdef __cplusplus
}
#endif
#endif
