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
#ifndef __UAPI_IVPU_DRM_H__
#define __UAPI_IVPU_DRM_H__
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DRM_IVPU_DRIVER_MAJOR 1
#define DRM_IVPU_DRIVER_MINOR 0
#define DRM_IVPU_GET_PARAM 0x00
#define DRM_IVPU_SET_PARAM 0x01
#define DRM_IVPU_BO_CREATE 0x02
#define DRM_IVPU_BO_INFO 0x03
#define DRM_IVPU_SUBMIT 0x05
#define DRM_IVPU_BO_WAIT 0x06
#define DRM_IOCTL_IVPU_GET_PARAM DRM_IOWR(DRM_COMMAND_BASE + DRM_IVPU_GET_PARAM, struct drm_ivpu_param)
#define DRM_IOCTL_IVPU_SET_PARAM DRM_IOW(DRM_COMMAND_BASE + DRM_IVPU_SET_PARAM, struct drm_ivpu_param)
#define DRM_IOCTL_IVPU_BO_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_IVPU_BO_CREATE, struct drm_ivpu_bo_create)
#define DRM_IOCTL_IVPU_BO_INFO DRM_IOWR(DRM_COMMAND_BASE + DRM_IVPU_BO_INFO, struct drm_ivpu_bo_info)
#define DRM_IOCTL_IVPU_SUBMIT DRM_IOW(DRM_COMMAND_BASE + DRM_IVPU_SUBMIT, struct drm_ivpu_submit)
#define DRM_IOCTL_IVPU_BO_WAIT DRM_IOWR(DRM_COMMAND_BASE + DRM_IVPU_BO_WAIT, struct drm_ivpu_bo_wait)
#define DRM_IVPU_PARAM_DEVICE_ID 0
#define DRM_IVPU_PARAM_DEVICE_REVISION 1
#define DRM_IVPU_PARAM_PLATFORM_TYPE 2
#define DRM_IVPU_PARAM_CORE_CLOCK_RATE 3
#define DRM_IVPU_PARAM_NUM_CONTEXTS 4
#define DRM_IVPU_PARAM_CONTEXT_BASE_ADDRESS 5
#define DRM_IVPU_PARAM_CONTEXT_PRIORITY 6
#define DRM_IVPU_PARAM_CONTEXT_ID 7
#define DRM_IVPU_PARAM_FW_API_VERSION 8
#define DRM_IVPU_PARAM_ENGINE_HEARTBEAT 9
#define DRM_IVPU_PARAM_UNIQUE_INFERENCE_ID 10
#define DRM_IVPU_PARAM_TILE_CONFIG 11
#define DRM_IVPU_PARAM_SKU 12
#define DRM_IVPU_PARAM_CAPABILITIES 13
#define DRM_IVPU_PLATFORM_TYPE_SILICON 0
#define DRM_IVPU_CONTEXT_PRIORITY_IDLE 0
#define DRM_IVPU_CONTEXT_PRIORITY_NORMAL 1
#define DRM_IVPU_CONTEXT_PRIORITY_FOCUS 2
#define DRM_IVPU_CONTEXT_PRIORITY_REALTIME 3
#define DRM_IVPU_CAP_METRIC_STREAMER 1
#define DRM_IVPU_CAP_DMA_MEMORY_RANGE 2
struct drm_ivpu_param {
  __u32 param;
  __u32 index;
  __u64 value;
};
#define DRM_IVPU_BO_SHAVE_MEM 0x00000001
#define DRM_IVPU_BO_HIGH_MEM DRM_IVPU_BO_SHAVE_MEM
#define DRM_IVPU_BO_MAPPABLE 0x00000002
#define DRM_IVPU_BO_DMA_MEM 0x00000004
#define DRM_IVPU_BO_CACHED 0x00000000
#define DRM_IVPU_BO_UNCACHED 0x00010000
#define DRM_IVPU_BO_WC 0x00020000
#define DRM_IVPU_BO_CACHE_MASK 0x00030000
#define DRM_IVPU_BO_FLAGS (DRM_IVPU_BO_HIGH_MEM | DRM_IVPU_BO_MAPPABLE | DRM_IVPU_BO_DMA_MEM | DRM_IVPU_BO_CACHE_MASK)
struct drm_ivpu_bo_create {
  __u64 size;
  __u32 flags;
  __u32 handle;
  __u64 vpu_addr;
};
struct drm_ivpu_bo_info {
  __u32 handle;
  __u32 flags;
  __u64 vpu_addr;
  __u64 mmap_offset;
  __u64 size;
};
#define DRM_IVPU_ENGINE_COMPUTE 0
#define DRM_IVPU_ENGINE_COPY 1
struct drm_ivpu_submit {
  __u64 buffers_ptr;
  __u32 buffer_count;
  __u32 engine;
  __u32 flags;
  __u32 commands_offset;
};
#define DRM_IVPU_JOB_STATUS_SUCCESS 0
struct drm_ivpu_bo_wait {
  __u32 handle;
  __u32 flags;
  __s64 timeout_ns;
  __u32 job_status;
  __u32 pad;
};
#ifdef __cplusplus
}
#endif
#endif
