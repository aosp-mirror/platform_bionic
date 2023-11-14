/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _PANFROST_DRM_H_
#define _PANFROST_DRM_H_
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DRM_PANFROST_SUBMIT 0x00
#define DRM_PANFROST_WAIT_BO 0x01
#define DRM_PANFROST_CREATE_BO 0x02
#define DRM_PANFROST_MMAP_BO 0x03
#define DRM_PANFROST_GET_PARAM 0x04
#define DRM_PANFROST_GET_BO_OFFSET 0x05
#define DRM_PANFROST_PERFCNT_ENABLE 0x06
#define DRM_PANFROST_PERFCNT_DUMP 0x07
#define DRM_PANFROST_MADVISE 0x08
#define DRM_IOCTL_PANFROST_SUBMIT DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_SUBMIT, struct drm_panfrost_submit)
#define DRM_IOCTL_PANFROST_WAIT_BO DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_WAIT_BO, struct drm_panfrost_wait_bo)
#define DRM_IOCTL_PANFROST_CREATE_BO DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_CREATE_BO, struct drm_panfrost_create_bo)
#define DRM_IOCTL_PANFROST_MMAP_BO DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_MMAP_BO, struct drm_panfrost_mmap_bo)
#define DRM_IOCTL_PANFROST_GET_PARAM DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_GET_PARAM, struct drm_panfrost_get_param)
#define DRM_IOCTL_PANFROST_GET_BO_OFFSET DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_GET_BO_OFFSET, struct drm_panfrost_get_bo_offset)
#define DRM_IOCTL_PANFROST_MADVISE DRM_IOWR(DRM_COMMAND_BASE + DRM_PANFROST_MADVISE, struct drm_panfrost_madvise)
#define DRM_IOCTL_PANFROST_PERFCNT_ENABLE DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_PERFCNT_ENABLE, struct drm_panfrost_perfcnt_enable)
#define DRM_IOCTL_PANFROST_PERFCNT_DUMP DRM_IOW(DRM_COMMAND_BASE + DRM_PANFROST_PERFCNT_DUMP, struct drm_panfrost_perfcnt_dump)
#define PANFROST_JD_REQ_FS (1 << 0)
struct drm_panfrost_submit {
  __u64 jc;
  __u64 in_syncs;
  __u32 in_sync_count;
  __u32 out_sync;
  __u64 bo_handles;
  __u32 bo_handle_count;
  __u32 requirements;
};
struct drm_panfrost_wait_bo {
  __u32 handle;
  __u32 pad;
  __s64 timeout_ns;
};
#define PANFROST_BO_NOEXEC 1
#define PANFROST_BO_HEAP 2
struct drm_panfrost_create_bo {
  __u32 size;
  __u32 flags;
  __u32 handle;
  __u32 pad;
  __u64 offset;
};
struct drm_panfrost_mmap_bo {
  __u32 handle;
  __u32 flags;
  __u64 offset;
};
enum drm_panfrost_param {
  DRM_PANFROST_PARAM_GPU_PROD_ID,
  DRM_PANFROST_PARAM_GPU_REVISION,
  DRM_PANFROST_PARAM_SHADER_PRESENT,
  DRM_PANFROST_PARAM_TILER_PRESENT,
  DRM_PANFROST_PARAM_L2_PRESENT,
  DRM_PANFROST_PARAM_STACK_PRESENT,
  DRM_PANFROST_PARAM_AS_PRESENT,
  DRM_PANFROST_PARAM_JS_PRESENT,
  DRM_PANFROST_PARAM_L2_FEATURES,
  DRM_PANFROST_PARAM_CORE_FEATURES,
  DRM_PANFROST_PARAM_TILER_FEATURES,
  DRM_PANFROST_PARAM_MEM_FEATURES,
  DRM_PANFROST_PARAM_MMU_FEATURES,
  DRM_PANFROST_PARAM_THREAD_FEATURES,
  DRM_PANFROST_PARAM_MAX_THREADS,
  DRM_PANFROST_PARAM_THREAD_MAX_WORKGROUP_SZ,
  DRM_PANFROST_PARAM_THREAD_MAX_BARRIER_SZ,
  DRM_PANFROST_PARAM_COHERENCY_FEATURES,
  DRM_PANFROST_PARAM_TEXTURE_FEATURES0,
  DRM_PANFROST_PARAM_TEXTURE_FEATURES1,
  DRM_PANFROST_PARAM_TEXTURE_FEATURES2,
  DRM_PANFROST_PARAM_TEXTURE_FEATURES3,
  DRM_PANFROST_PARAM_JS_FEATURES0,
  DRM_PANFROST_PARAM_JS_FEATURES1,
  DRM_PANFROST_PARAM_JS_FEATURES2,
  DRM_PANFROST_PARAM_JS_FEATURES3,
  DRM_PANFROST_PARAM_JS_FEATURES4,
  DRM_PANFROST_PARAM_JS_FEATURES5,
  DRM_PANFROST_PARAM_JS_FEATURES6,
  DRM_PANFROST_PARAM_JS_FEATURES7,
  DRM_PANFROST_PARAM_JS_FEATURES8,
  DRM_PANFROST_PARAM_JS_FEATURES9,
  DRM_PANFROST_PARAM_JS_FEATURES10,
  DRM_PANFROST_PARAM_JS_FEATURES11,
  DRM_PANFROST_PARAM_JS_FEATURES12,
  DRM_PANFROST_PARAM_JS_FEATURES13,
  DRM_PANFROST_PARAM_JS_FEATURES14,
  DRM_PANFROST_PARAM_JS_FEATURES15,
  DRM_PANFROST_PARAM_NR_CORE_GROUPS,
  DRM_PANFROST_PARAM_THREAD_TLS_ALLOC,
  DRM_PANFROST_PARAM_AFBC_FEATURES,
};
struct drm_panfrost_get_param {
  __u32 param;
  __u32 pad;
  __u64 value;
};
struct drm_panfrost_get_bo_offset {
  __u32 handle;
  __u32 pad;
  __u64 offset;
};
struct drm_panfrost_perfcnt_enable {
  __u32 enable;
  __u32 counterset;
};
struct drm_panfrost_perfcnt_dump {
  __u64 buf_ptr;
};
#define PANFROST_MADV_WILLNEED 0
#define PANFROST_MADV_DONTNEED 1
struct drm_panfrost_madvise {
  __u32 handle;
  __u32 madv;
  __u32 retained;
};
#define PANFROSTDUMP_MAJOR 1
#define PANFROSTDUMP_MINOR 0
#define PANFROSTDUMP_MAGIC 0x464E4150
#define PANFROSTDUMP_BUF_REG 0
#define PANFROSTDUMP_BUF_BOMAP (PANFROSTDUMP_BUF_REG + 1)
#define PANFROSTDUMP_BUF_BO (PANFROSTDUMP_BUF_BOMAP + 1)
#define PANFROSTDUMP_BUF_TRAILER (PANFROSTDUMP_BUF_BO + 1)
struct panfrost_dump_object_header {
  __u32 magic;
  __u32 type;
  __u32 file_size;
  __u32 file_offset;
  union {
    struct {
      __u64 jc;
      __u32 gpu_id;
      __u32 major;
      __u32 minor;
      __u64 nbos;
    } reghdr;
    struct {
      __u32 valid;
      __u64 iova;
      __u32 data[2];
    } bomap;
    __u32 sizer[496];
  };
};
struct panfrost_dump_registers {
  __u32 reg;
  __u32 value;
};
#ifdef __cplusplus
}
#endif
#endif
