/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_TEGRA_DRM_H_
#define _UAPI_TEGRA_DRM_H_
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DRM_TEGRA_GEM_CREATE_TILED (1 << 0)
#define DRM_TEGRA_GEM_CREATE_BOTTOM_UP (1 << 1)
struct drm_tegra_gem_create {
  __u64 size;
  __u32 flags;
  __u32 handle;
};
struct drm_tegra_gem_mmap {
  __u32 handle;
  __u32 pad;
  __u64 offset;
};
struct drm_tegra_syncpt_read {
  __u32 id;
  __u32 value;
};
struct drm_tegra_syncpt_incr {
  __u32 id;
  __u32 pad;
};
struct drm_tegra_syncpt_wait {
  __u32 id;
  __u32 thresh;
  __u32 timeout;
  __u32 value;
};
#define DRM_TEGRA_NO_TIMEOUT (0xffffffff)
struct drm_tegra_open_channel {
  __u32 client;
  __u32 pad;
  __u64 context;
};
struct drm_tegra_close_channel {
  __u64 context;
};
struct drm_tegra_get_syncpt {
  __u64 context;
  __u32 index;
  __u32 id;
};
struct drm_tegra_get_syncpt_base {
  __u64 context;
  __u32 syncpt;
  __u32 id;
};
struct drm_tegra_syncpt {
  __u32 id;
  __u32 incrs;
};
struct drm_tegra_cmdbuf {
  __u32 handle;
  __u32 offset;
  __u32 words;
  __u32 pad;
};
struct drm_tegra_reloc {
  struct {
    __u32 handle;
    __u32 offset;
  } cmdbuf;
  struct {
    __u32 handle;
    __u32 offset;
  } target;
  __u32 shift;
  __u32 pad;
};
struct drm_tegra_waitchk {
  __u32 handle;
  __u32 offset;
  __u32 syncpt;
  __u32 thresh;
};
struct drm_tegra_submit {
  __u64 context;
  __u32 num_syncpts;
  __u32 num_cmdbufs;
  __u32 num_relocs;
  __u32 num_waitchks;
  __u32 waitchk_mask;
  __u32 timeout;
  __u64 syncpts;
  __u64 cmdbufs;
  __u64 relocs;
  __u64 waitchks;
  __u32 fence;
  __u32 reserved[5];
};
#define DRM_TEGRA_GEM_TILING_MODE_PITCH 0
#define DRM_TEGRA_GEM_TILING_MODE_TILED 1
#define DRM_TEGRA_GEM_TILING_MODE_BLOCK 2
struct drm_tegra_gem_set_tiling {
  __u32 handle;
  __u32 mode;
  __u32 value;
  __u32 pad;
};
struct drm_tegra_gem_get_tiling {
  __u32 handle;
  __u32 mode;
  __u32 value;
  __u32 pad;
};
#define DRM_TEGRA_GEM_BOTTOM_UP (1 << 0)
#define DRM_TEGRA_GEM_FLAGS (DRM_TEGRA_GEM_BOTTOM_UP)
struct drm_tegra_gem_set_flags {
  __u32 handle;
  __u32 flags;
};
struct drm_tegra_gem_get_flags {
  __u32 handle;
  __u32 flags;
};
#define DRM_TEGRA_GEM_CREATE 0x00
#define DRM_TEGRA_GEM_MMAP 0x01
#define DRM_TEGRA_SYNCPT_READ 0x02
#define DRM_TEGRA_SYNCPT_INCR 0x03
#define DRM_TEGRA_SYNCPT_WAIT 0x04
#define DRM_TEGRA_OPEN_CHANNEL 0x05
#define DRM_TEGRA_CLOSE_CHANNEL 0x06
#define DRM_TEGRA_GET_SYNCPT 0x07
#define DRM_TEGRA_SUBMIT 0x08
#define DRM_TEGRA_GET_SYNCPT_BASE 0x09
#define DRM_TEGRA_GEM_SET_TILING 0x0a
#define DRM_TEGRA_GEM_GET_TILING 0x0b
#define DRM_TEGRA_GEM_SET_FLAGS 0x0c
#define DRM_TEGRA_GEM_GET_FLAGS 0x0d
#define DRM_IOCTL_TEGRA_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GEM_CREATE, struct drm_tegra_gem_create)
#define DRM_IOCTL_TEGRA_GEM_MMAP DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GEM_MMAP, struct drm_tegra_gem_mmap)
#define DRM_IOCTL_TEGRA_SYNCPT_READ DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_SYNCPT_READ, struct drm_tegra_syncpt_read)
#define DRM_IOCTL_TEGRA_SYNCPT_INCR DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_SYNCPT_INCR, struct drm_tegra_syncpt_incr)
#define DRM_IOCTL_TEGRA_SYNCPT_WAIT DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_SYNCPT_WAIT, struct drm_tegra_syncpt_wait)
#define DRM_IOCTL_TEGRA_OPEN_CHANNEL DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_OPEN_CHANNEL, struct drm_tegra_open_channel)
#define DRM_IOCTL_TEGRA_CLOSE_CHANNEL DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_CLOSE_CHANNEL, struct drm_tegra_close_channel)
#define DRM_IOCTL_TEGRA_GET_SYNCPT DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GET_SYNCPT, struct drm_tegra_get_syncpt)
#define DRM_IOCTL_TEGRA_SUBMIT DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_SUBMIT, struct drm_tegra_submit)
#define DRM_IOCTL_TEGRA_GET_SYNCPT_BASE DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GET_SYNCPT_BASE, struct drm_tegra_get_syncpt_base)
#define DRM_IOCTL_TEGRA_GEM_SET_TILING DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GEM_SET_TILING, struct drm_tegra_gem_set_tiling)
#define DRM_IOCTL_TEGRA_GEM_GET_TILING DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GEM_GET_TILING, struct drm_tegra_gem_get_tiling)
#define DRM_IOCTL_TEGRA_GEM_SET_FLAGS DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GEM_SET_FLAGS, struct drm_tegra_gem_set_flags)
#define DRM_IOCTL_TEGRA_GEM_GET_FLAGS DRM_IOWR(DRM_COMMAND_BASE + DRM_TEGRA_GEM_GET_FLAGS, struct drm_tegra_gem_get_flags)
#define DRM_TEGRA_CHANNEL_CAP_CACHE_COHERENT (1 << 0)
struct drm_tegra_channel_open {
  __u32 host1x_class;
  __u32 flags;
  __u32 context;
  __u32 version;
  __u32 capabilities;
  __u32 padding;
};
struct drm_tegra_channel_close {
  __u32 context;
  __u32 padding;
};
#define DRM_TEGRA_CHANNEL_MAP_READ (1 << 0)
#define DRM_TEGRA_CHANNEL_MAP_WRITE (1 << 1)
#define DRM_TEGRA_CHANNEL_MAP_READ_WRITE (DRM_TEGRA_CHANNEL_MAP_READ | DRM_TEGRA_CHANNEL_MAP_WRITE)
struct drm_tegra_channel_map {
  __u32 context;
  __u32 handle;
  __u32 flags;
  __u32 mapping;
};
struct drm_tegra_channel_unmap {
  __u32 context;
  __u32 mapping;
};
#define DRM_TEGRA_SUBMIT_RELOC_SECTOR_LAYOUT (1 << 0)
struct drm_tegra_submit_buf {
  __u32 mapping;
  __u32 flags;
  struct {
    __u64 target_offset;
    __u32 gather_offset_words;
    __u32 shift;
  } reloc;
};
#define DRM_TEGRA_SUBMIT_CMD_GATHER_UPTR 0
#define DRM_TEGRA_SUBMIT_CMD_WAIT_SYNCPT 1
#define DRM_TEGRA_SUBMIT_CMD_WAIT_SYNCPT_RELATIVE 2
struct drm_tegra_submit_cmd_gather_uptr {
  __u32 words;
  __u32 reserved[3];
};
struct drm_tegra_submit_cmd_wait_syncpt {
  __u32 id;
  __u32 value;
  __u32 reserved[2];
};
struct drm_tegra_submit_cmd {
  __u32 type;
  __u32 flags;
  union {
    struct drm_tegra_submit_cmd_gather_uptr gather_uptr;
    struct drm_tegra_submit_cmd_wait_syncpt wait_syncpt;
    __u32 reserved[4];
  };
};
struct drm_tegra_submit_syncpt {
  __u32 id;
  __u32 flags;
  __u32 increments;
  __u32 value;
};
struct drm_tegra_channel_submit {
  __u32 context;
  __u32 num_bufs;
  __u32 num_cmds;
  __u32 gather_data_words;
  __u64 bufs_ptr;
  __u64 cmds_ptr;
  __u64 gather_data_ptr;
  __u32 syncobj_in;
  __u32 syncobj_out;
  struct drm_tegra_submit_syncpt syncpt;
};
struct drm_tegra_syncpoint_allocate {
  __u32 id;
  __u32 padding;
};
struct drm_tegra_syncpoint_free {
  __u32 id;
  __u32 padding;
};
struct drm_tegra_syncpoint_wait {
  __s64 timeout_ns;
  __u32 id;
  __u32 threshold;
  __u32 value;
  __u32 padding;
};
#define DRM_IOCTL_TEGRA_CHANNEL_OPEN DRM_IOWR(DRM_COMMAND_BASE + 0x10, struct drm_tegra_channel_open)
#define DRM_IOCTL_TEGRA_CHANNEL_CLOSE DRM_IOWR(DRM_COMMAND_BASE + 0x11, struct drm_tegra_channel_close)
#define DRM_IOCTL_TEGRA_CHANNEL_MAP DRM_IOWR(DRM_COMMAND_BASE + 0x12, struct drm_tegra_channel_map)
#define DRM_IOCTL_TEGRA_CHANNEL_UNMAP DRM_IOWR(DRM_COMMAND_BASE + 0x13, struct drm_tegra_channel_unmap)
#define DRM_IOCTL_TEGRA_CHANNEL_SUBMIT DRM_IOWR(DRM_COMMAND_BASE + 0x14, struct drm_tegra_channel_submit)
#define DRM_IOCTL_TEGRA_SYNCPOINT_ALLOCATE DRM_IOWR(DRM_COMMAND_BASE + 0x20, struct drm_tegra_syncpoint_allocate)
#define DRM_IOCTL_TEGRA_SYNCPOINT_FREE DRM_IOWR(DRM_COMMAND_BASE + 0x21, struct drm_tegra_syncpoint_free)
#define DRM_IOCTL_TEGRA_SYNCPOINT_WAIT DRM_IOWR(DRM_COMMAND_BASE + 0x22, struct drm_tegra_syncpoint_wait)
#ifdef __cplusplus
}
#endif
#endif
