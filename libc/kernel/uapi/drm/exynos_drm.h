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
#ifndef _UAPI_EXYNOS_DRM_H_
#define _UAPI_EXYNOS_DRM_H_
#include "drm.h"
#ifdef __cplusplus
#endif
struct drm_exynos_gem_create {
  __u64 size;
  __u32 flags;
  __u32 handle;
};
struct drm_exynos_gem_map {
  __u32 handle;
  __u32 reserved;
  __u64 offset;
};
struct drm_exynos_gem_info {
  __u32 handle;
  __u32 flags;
  __u64 size;
};
struct drm_exynos_vidi_connection {
  __u32 connection;
  __u32 extensions;
  __u64 edid;
};
enum e_drm_exynos_gem_mem_type {
  EXYNOS_BO_CONTIG = 0 << 0,
  EXYNOS_BO_NONCONTIG = 1 << 0,
  EXYNOS_BO_NONCACHABLE = 0 << 1,
  EXYNOS_BO_CACHABLE = 1 << 1,
  EXYNOS_BO_WC = 1 << 2,
  EXYNOS_BO_MASK = EXYNOS_BO_NONCONTIG | EXYNOS_BO_CACHABLE | EXYNOS_BO_WC
};
struct drm_exynos_g2d_get_ver {
  __u32 major;
  __u32 minor;
};
struct drm_exynos_g2d_cmd {
  __u32 offset;
  __u32 data;
};
enum drm_exynos_g2d_buf_type {
  G2D_BUF_USERPTR = 1 << 31,
};
enum drm_exynos_g2d_event_type {
  G2D_EVENT_NOT,
  G2D_EVENT_NONSTOP,
  G2D_EVENT_STOP,
};
struct drm_exynos_g2d_userptr {
  unsigned long userptr;
  unsigned long size;
};
struct drm_exynos_g2d_set_cmdlist {
  __u64 cmd;
  __u64 cmd_buf;
  __u32 cmd_nr;
  __u32 cmd_buf_nr;
  __u64 event_type;
  __u64 user_data;
};
struct drm_exynos_g2d_exec {
  __u64 async;
};
#define DRM_EXYNOS_GEM_CREATE 0x00
#define DRM_EXYNOS_GEM_MAP 0x01
#define DRM_EXYNOS_GEM_GET 0x04
#define DRM_EXYNOS_VIDI_CONNECTION 0x07
#define DRM_EXYNOS_G2D_GET_VER 0x20
#define DRM_EXYNOS_G2D_SET_CMDLIST 0x21
#define DRM_EXYNOS_G2D_EXEC 0x22
#define DRM_IOCTL_EXYNOS_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_GEM_CREATE, struct drm_exynos_gem_create)
#define DRM_IOCTL_EXYNOS_GEM_MAP DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_GEM_MAP, struct drm_exynos_gem_map)
#define DRM_IOCTL_EXYNOS_GEM_GET DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_GEM_GET, struct drm_exynos_gem_info)
#define DRM_IOCTL_EXYNOS_VIDI_CONNECTION DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_VIDI_CONNECTION, struct drm_exynos_vidi_connection)
#define DRM_IOCTL_EXYNOS_G2D_GET_VER DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_G2D_GET_VER, struct drm_exynos_g2d_get_ver)
#define DRM_IOCTL_EXYNOS_G2D_SET_CMDLIST DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_G2D_SET_CMDLIST, struct drm_exynos_g2d_set_cmdlist)
#define DRM_IOCTL_EXYNOS_G2D_EXEC DRM_IOWR(DRM_COMMAND_BASE + DRM_EXYNOS_G2D_EXEC, struct drm_exynos_g2d_exec)
#define DRM_EXYNOS_G2D_EVENT 0x80000000
struct drm_exynos_g2d_event {
  struct drm_event base;
  __u64 user_data;
  __u32 tv_sec;
  __u32 tv_usec;
  __u32 cmdlist_no;
  __u32 reserved;
};
#ifdef __cplusplus
#endif
#endif
