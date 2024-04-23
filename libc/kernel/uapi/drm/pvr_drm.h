/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef PVR_DRM_UAPI_H
#define PVR_DRM_UAPI_H
#include "drm.h"
#include <linux/const.h>
#include <linux/types.h>
#ifdef __cplusplus
extern "C" {
#endif
struct drm_pvr_obj_array {
  __u32 stride;
  __u32 count;
  __u64 array;
};
#define DRM_PVR_OBJ_ARRAY(cnt,ptr) {.stride = sizeof((ptr)[0]),.count = (cnt),.array = (__u64) (uintptr_t) (ptr) }
#define PVR_IOCTL(_ioctl,_mode,_data) _mode(DRM_COMMAND_BASE + (_ioctl), struct drm_pvr_ioctl_ ##_data ##_args)
#define DRM_IOCTL_PVR_DEV_QUERY PVR_IOCTL(0x00, DRM_IOWR, dev_query)
#define DRM_IOCTL_PVR_CREATE_BO PVR_IOCTL(0x01, DRM_IOWR, create_bo)
#define DRM_IOCTL_PVR_GET_BO_MMAP_OFFSET PVR_IOCTL(0x02, DRM_IOWR, get_bo_mmap_offset)
#define DRM_IOCTL_PVR_CREATE_VM_CONTEXT PVR_IOCTL(0x03, DRM_IOWR, create_vm_context)
#define DRM_IOCTL_PVR_DESTROY_VM_CONTEXT PVR_IOCTL(0x04, DRM_IOW, destroy_vm_context)
#define DRM_IOCTL_PVR_VM_MAP PVR_IOCTL(0x05, DRM_IOW, vm_map)
#define DRM_IOCTL_PVR_VM_UNMAP PVR_IOCTL(0x06, DRM_IOW, vm_unmap)
#define DRM_IOCTL_PVR_CREATE_CONTEXT PVR_IOCTL(0x07, DRM_IOWR, create_context)
#define DRM_IOCTL_PVR_DESTROY_CONTEXT PVR_IOCTL(0x08, DRM_IOW, destroy_context)
#define DRM_IOCTL_PVR_CREATE_FREE_LIST PVR_IOCTL(0x09, DRM_IOWR, create_free_list)
#define DRM_IOCTL_PVR_DESTROY_FREE_LIST PVR_IOCTL(0x0a, DRM_IOW, destroy_free_list)
#define DRM_IOCTL_PVR_CREATE_HWRT_DATASET PVR_IOCTL(0x0b, DRM_IOWR, create_hwrt_dataset)
#define DRM_IOCTL_PVR_DESTROY_HWRT_DATASET PVR_IOCTL(0x0c, DRM_IOW, destroy_hwrt_dataset)
#define DRM_IOCTL_PVR_SUBMIT_JOBS PVR_IOCTL(0x0d, DRM_IOW, submit_jobs)
struct drm_pvr_dev_query_gpu_info {
  __u64 gpu_id;
  __u32 num_phantoms;
  __u32 _padding_c;
};
struct drm_pvr_dev_query_runtime_info {
  __u64 free_list_min_pages;
  __u64 free_list_max_pages;
  __u32 common_store_alloc_region_size;
  __u32 common_store_partition_space_size;
  __u32 max_coeffs;
  __u32 cdm_max_local_mem_size_regs;
};
struct drm_pvr_dev_query_quirks {
  __u64 quirks;
  __u16 count;
  __u16 musthave_count;
  __u32 _padding_c;
};
struct drm_pvr_dev_query_enhancements {
  __u64 enhancements;
  __u16 count;
  __u16 _padding_a;
  __u32 _padding_c;
};
enum drm_pvr_heap_id {
  DRM_PVR_HEAP_GENERAL = 0,
  DRM_PVR_HEAP_PDS_CODE_DATA,
  DRM_PVR_HEAP_USC_CODE,
  DRM_PVR_HEAP_RGNHDR,
  DRM_PVR_HEAP_VIS_TEST,
  DRM_PVR_HEAP_TRANSFER_FRAG,
  DRM_PVR_HEAP_COUNT
};
struct drm_pvr_heap {
  __u64 base;
  __u64 size;
  __u32 flags;
  __u32 page_size_log2;
};
struct drm_pvr_dev_query_heap_info {
  struct drm_pvr_obj_array heaps;
};
enum drm_pvr_static_data_area_usage {
  DRM_PVR_STATIC_DATA_AREA_EOT = 0,
  DRM_PVR_STATIC_DATA_AREA_FENCE,
  DRM_PVR_STATIC_DATA_AREA_VDM_SYNC,
  DRM_PVR_STATIC_DATA_AREA_YUV_CSC,
};
struct drm_pvr_static_data_area {
  __u16 area_usage;
  __u16 location_heap_id;
  __u32 size;
  __u64 offset;
};
struct drm_pvr_dev_query_static_data_areas {
  struct drm_pvr_obj_array static_data_areas;
};
enum drm_pvr_dev_query {
  DRM_PVR_DEV_QUERY_GPU_INFO_GET = 0,
  DRM_PVR_DEV_QUERY_RUNTIME_INFO_GET,
  DRM_PVR_DEV_QUERY_QUIRKS_GET,
  DRM_PVR_DEV_QUERY_ENHANCEMENTS_GET,
  DRM_PVR_DEV_QUERY_HEAP_INFO_GET,
  DRM_PVR_DEV_QUERY_STATIC_DATA_AREAS_GET,
};
struct drm_pvr_ioctl_dev_query_args {
  __u32 type;
  __u32 size;
  __u64 pointer;
};
#define DRM_PVR_BO_BYPASS_DEVICE_CACHE _BITULL(0)
#define DRM_PVR_BO_PM_FW_PROTECT _BITULL(1)
#define DRM_PVR_BO_ALLOW_CPU_USERSPACE_ACCESS _BITULL(2)
#define DRM_PVR_BO_FLAGS_MASK (DRM_PVR_BO_BYPASS_DEVICE_CACHE | DRM_PVR_BO_PM_FW_PROTECT | DRM_PVR_BO_ALLOW_CPU_USERSPACE_ACCESS)
struct drm_pvr_ioctl_create_bo_args {
  __u64 size;
  __u32 handle;
  __u32 _padding_c;
  __u64 flags;
};
struct drm_pvr_ioctl_get_bo_mmap_offset_args {
  __u32 handle;
  __u32 _padding_4;
  __u64 offset;
};
struct drm_pvr_ioctl_create_vm_context_args {
  __u32 handle;
  __u32 _padding_4;
};
struct drm_pvr_ioctl_destroy_vm_context_args {
  __u32 handle;
  __u32 _padding_4;
};
struct drm_pvr_ioctl_vm_map_args {
  __u32 vm_context_handle;
  __u32 flags;
  __u64 device_addr;
  __u32 handle;
  __u32 _padding_14;
  __u64 offset;
  __u64 size;
};
struct drm_pvr_ioctl_vm_unmap_args {
  __u32 vm_context_handle;
  __u32 _padding_4;
  __u64 device_addr;
  __u64 size;
};
enum drm_pvr_ctx_priority {
  DRM_PVR_CTX_PRIORITY_LOW = - 512,
  DRM_PVR_CTX_PRIORITY_NORMAL = 0,
  DRM_PVR_CTX_PRIORITY_HIGH = 512,
};
enum drm_pvr_ctx_type {
  DRM_PVR_CTX_TYPE_RENDER = 0,
  DRM_PVR_CTX_TYPE_COMPUTE,
  DRM_PVR_CTX_TYPE_TRANSFER_FRAG,
};
struct drm_pvr_ioctl_create_context_args {
  __u32 type;
  __u32 flags;
  __s32 priority;
  __u32 handle;
  __u64 static_context_state;
  __u32 static_context_state_len;
  __u32 vm_context_handle;
  __u64 callstack_addr;
};
struct drm_pvr_ioctl_destroy_context_args {
  __u32 handle;
  __u32 _padding_4;
};
struct drm_pvr_ioctl_create_free_list_args {
  __u64 free_list_gpu_addr;
  __u32 initial_num_pages;
  __u32 max_num_pages;
  __u32 grow_num_pages;
  __u32 grow_threshold;
  __u32 vm_context_handle;
  __u32 handle;
};
struct drm_pvr_ioctl_destroy_free_list_args {
  __u32 handle;
  __u32 _padding_4;
};
struct drm_pvr_create_hwrt_geom_data_args {
  __u64 tpc_dev_addr;
  __u32 tpc_size;
  __u32 tpc_stride;
  __u64 vheap_table_dev_addr;
  __u64 rtc_dev_addr;
};
struct drm_pvr_create_hwrt_rt_data_args {
  __u64 pm_mlist_dev_addr;
  __u64 macrotile_array_dev_addr;
  __u64 region_header_dev_addr;
};
#define PVR_DRM_HWRT_FREE_LIST_LOCAL 0
#define PVR_DRM_HWRT_FREE_LIST_GLOBAL 1U
struct drm_pvr_ioctl_create_hwrt_dataset_args {
  struct drm_pvr_create_hwrt_geom_data_args geom_data_args;
  struct drm_pvr_create_hwrt_rt_data_args rt_data_args[2];
  __u32 free_list_handles[2];
  __u32 width;
  __u32 height;
  __u32 samples;
  __u32 layers;
  __u32 isp_merge_lower_x;
  __u32 isp_merge_lower_y;
  __u32 isp_merge_scale_x;
  __u32 isp_merge_scale_y;
  __u32 isp_merge_upper_x;
  __u32 isp_merge_upper_y;
  __u32 region_header_size;
  __u32 handle;
};
struct drm_pvr_ioctl_destroy_hwrt_dataset_args {
  __u32 handle;
  __u32 _padding_4;
};
#define DRM_PVR_SYNC_OP_FLAG_HANDLE_TYPE_MASK 0xf
#define DRM_PVR_SYNC_OP_FLAG_HANDLE_TYPE_SYNCOBJ 0
#define DRM_PVR_SYNC_OP_FLAG_HANDLE_TYPE_TIMELINE_SYNCOBJ 1
#define DRM_PVR_SYNC_OP_FLAG_SIGNAL _BITULL(31)
#define DRM_PVR_SYNC_OP_FLAG_WAIT 0
#define DRM_PVR_SYNC_OP_FLAGS_MASK (DRM_PVR_SYNC_OP_FLAG_HANDLE_TYPE_MASK | DRM_PVR_SYNC_OP_FLAG_SIGNAL)
struct drm_pvr_sync_op {
  __u32 handle;
  __u32 flags;
  __u64 value;
};
#define DRM_PVR_SUBMIT_JOB_GEOM_CMD_FIRST _BITULL(0)
#define DRM_PVR_SUBMIT_JOB_GEOM_CMD_LAST _BITULL(1)
#define DRM_PVR_SUBMIT_JOB_GEOM_CMD_SINGLE_CORE _BITULL(2)
#define DRM_PVR_SUBMIT_JOB_GEOM_CMD_FLAGS_MASK (DRM_PVR_SUBMIT_JOB_GEOM_CMD_FIRST | DRM_PVR_SUBMIT_JOB_GEOM_CMD_LAST | DRM_PVR_SUBMIT_JOB_GEOM_CMD_SINGLE_CORE)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_SINGLE_CORE _BITULL(0)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_DEPTHBUFFER _BITULL(1)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_STENCILBUFFER _BITULL(2)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_PREVENT_CDM_OVERLAP _BITULL(3)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_SCRATCHBUFFER _BITULL(4)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_GET_VIS_RESULTS _BITULL(5)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_PARTIAL_RENDER _BITULL(6)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_DISABLE_PIXELMERGE _BITULL(7)
#define DRM_PVR_SUBMIT_JOB_FRAG_CMD_FLAGS_MASK (DRM_PVR_SUBMIT_JOB_FRAG_CMD_SINGLE_CORE | DRM_PVR_SUBMIT_JOB_FRAG_CMD_DEPTHBUFFER | DRM_PVR_SUBMIT_JOB_FRAG_CMD_STENCILBUFFER | DRM_PVR_SUBMIT_JOB_FRAG_CMD_PREVENT_CDM_OVERLAP | DRM_PVR_SUBMIT_JOB_FRAG_CMD_SCRATCHBUFFER | DRM_PVR_SUBMIT_JOB_FRAG_CMD_GET_VIS_RESULTS | DRM_PVR_SUBMIT_JOB_FRAG_CMD_PARTIAL_RENDER | DRM_PVR_SUBMIT_JOB_FRAG_CMD_DISABLE_PIXELMERGE)
#define DRM_PVR_SUBMIT_JOB_COMPUTE_CMD_PREVENT_ALL_OVERLAP _BITULL(0)
#define DRM_PVR_SUBMIT_JOB_COMPUTE_CMD_SINGLE_CORE _BITULL(1)
#define DRM_PVR_SUBMIT_JOB_COMPUTE_CMD_FLAGS_MASK (DRM_PVR_SUBMIT_JOB_COMPUTE_CMD_PREVENT_ALL_OVERLAP | DRM_PVR_SUBMIT_JOB_COMPUTE_CMD_SINGLE_CORE)
#define DRM_PVR_SUBMIT_JOB_TRANSFER_CMD_SINGLE_CORE _BITULL(0)
#define DRM_PVR_SUBMIT_JOB_TRANSFER_CMD_FLAGS_MASK DRM_PVR_SUBMIT_JOB_TRANSFER_CMD_SINGLE_CORE
enum drm_pvr_job_type {
  DRM_PVR_JOB_TYPE_GEOMETRY = 0,
  DRM_PVR_JOB_TYPE_FRAGMENT,
  DRM_PVR_JOB_TYPE_COMPUTE,
  DRM_PVR_JOB_TYPE_TRANSFER_FRAG,
};
struct drm_pvr_hwrt_data_ref {
  __u32 set_handle;
  __u32 data_index;
};
struct drm_pvr_job {
  __u32 type;
  __u32 context_handle;
  __u32 flags;
  __u32 cmd_stream_len;
  __u64 cmd_stream;
  struct drm_pvr_obj_array sync_ops;
  struct drm_pvr_hwrt_data_ref hwrt;
};
struct drm_pvr_ioctl_submit_jobs_args {
  struct drm_pvr_obj_array jobs;
};
#ifdef __cplusplus
}
#endif
#endif
