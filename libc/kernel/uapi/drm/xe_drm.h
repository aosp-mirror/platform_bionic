/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_XE_DRM_H_
#define _UAPI_XE_DRM_H_
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DRM_XE_DEVICE_QUERY 0x00
#define DRM_XE_GEM_CREATE 0x01
#define DRM_XE_GEM_MMAP_OFFSET 0x02
#define DRM_XE_VM_CREATE 0x03
#define DRM_XE_VM_DESTROY 0x04
#define DRM_XE_VM_BIND 0x05
#define DRM_XE_EXEC_QUEUE_CREATE 0x06
#define DRM_XE_EXEC_QUEUE_DESTROY 0x07
#define DRM_XE_EXEC_QUEUE_GET_PROPERTY 0x08
#define DRM_XE_EXEC 0x09
#define DRM_XE_WAIT_USER_FENCE 0x0a
#define DRM_IOCTL_XE_DEVICE_QUERY DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_DEVICE_QUERY, struct drm_xe_device_query)
#define DRM_IOCTL_XE_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_GEM_CREATE, struct drm_xe_gem_create)
#define DRM_IOCTL_XE_GEM_MMAP_OFFSET DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_GEM_MMAP_OFFSET, struct drm_xe_gem_mmap_offset)
#define DRM_IOCTL_XE_VM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_VM_CREATE, struct drm_xe_vm_create)
#define DRM_IOCTL_XE_VM_DESTROY DRM_IOW(DRM_COMMAND_BASE + DRM_XE_VM_DESTROY, struct drm_xe_vm_destroy)
#define DRM_IOCTL_XE_VM_BIND DRM_IOW(DRM_COMMAND_BASE + DRM_XE_VM_BIND, struct drm_xe_vm_bind)
#define DRM_IOCTL_XE_EXEC_QUEUE_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_EXEC_QUEUE_CREATE, struct drm_xe_exec_queue_create)
#define DRM_IOCTL_XE_EXEC_QUEUE_DESTROY DRM_IOW(DRM_COMMAND_BASE + DRM_XE_EXEC_QUEUE_DESTROY, struct drm_xe_exec_queue_destroy)
#define DRM_IOCTL_XE_EXEC_QUEUE_GET_PROPERTY DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_EXEC_QUEUE_GET_PROPERTY, struct drm_xe_exec_queue_get_property)
#define DRM_IOCTL_XE_EXEC DRM_IOW(DRM_COMMAND_BASE + DRM_XE_EXEC, struct drm_xe_exec)
#define DRM_IOCTL_XE_WAIT_USER_FENCE DRM_IOWR(DRM_COMMAND_BASE + DRM_XE_WAIT_USER_FENCE, struct drm_xe_wait_user_fence)
struct drm_xe_user_extension {
  __u64 next_extension;
  __u32 name;
  __u32 pad;
};
struct drm_xe_ext_set_property {
  struct drm_xe_user_extension base;
  __u32 property;
  __u32 pad;
  __u64 value;
  __u64 reserved[2];
};
struct drm_xe_engine_class_instance {
#define DRM_XE_ENGINE_CLASS_RENDER 0
#define DRM_XE_ENGINE_CLASS_COPY 1
#define DRM_XE_ENGINE_CLASS_VIDEO_DECODE 2
#define DRM_XE_ENGINE_CLASS_VIDEO_ENHANCE 3
#define DRM_XE_ENGINE_CLASS_COMPUTE 4
#define DRM_XE_ENGINE_CLASS_VM_BIND 5
  __u16 engine_class;
  __u16 engine_instance;
  __u16 gt_id;
  __u16 pad;
};
struct drm_xe_engine {
  struct drm_xe_engine_class_instance instance;
  __u64 reserved[3];
};
struct drm_xe_query_engines {
  __u32 num_engines;
  __u32 pad;
  struct drm_xe_engine engines[];
};
enum drm_xe_memory_class {
  DRM_XE_MEM_REGION_CLASS_SYSMEM = 0,
  DRM_XE_MEM_REGION_CLASS_VRAM
};
struct drm_xe_mem_region {
  __u16 mem_class;
  __u16 instance;
  __u32 min_page_size;
  __u64 total_size;
  __u64 used;
  __u64 cpu_visible_size;
  __u64 cpu_visible_used;
  __u64 reserved[6];
};
struct drm_xe_query_mem_regions {
  __u32 num_mem_regions;
  __u32 pad;
  struct drm_xe_mem_region mem_regions[];
};
struct drm_xe_query_config {
  __u32 num_params;
  __u32 pad;
#define DRM_XE_QUERY_CONFIG_REV_AND_DEVICE_ID 0
#define DRM_XE_QUERY_CONFIG_FLAGS 1
#define DRM_XE_QUERY_CONFIG_FLAG_HAS_VRAM (1 << 0)
#define DRM_XE_QUERY_CONFIG_MIN_ALIGNMENT 2
#define DRM_XE_QUERY_CONFIG_VA_BITS 3
#define DRM_XE_QUERY_CONFIG_MAX_EXEC_QUEUE_PRIORITY 4
  __u64 info[];
};
struct drm_xe_gt {
#define DRM_XE_QUERY_GT_TYPE_MAIN 0
#define DRM_XE_QUERY_GT_TYPE_MEDIA 1
  __u16 type;
  __u16 tile_id;
  __u16 gt_id;
  __u16 pad[3];
  __u32 reference_clock;
  __u64 near_mem_regions;
  __u64 far_mem_regions;
  __u16 ip_ver_major;
  __u16 ip_ver_minor;
  __u16 ip_ver_rev;
  __u16 pad2;
  __u64 reserved[7];
};
struct drm_xe_query_gt_list {
  __u32 num_gt;
  __u32 pad;
  struct drm_xe_gt gt_list[];
};
struct drm_xe_query_topology_mask {
  __u16 gt_id;
#define DRM_XE_TOPO_DSS_GEOMETRY 1
#define DRM_XE_TOPO_DSS_COMPUTE 2
#define DRM_XE_TOPO_EU_PER_DSS 4
  __u16 type;
  __u32 num_bytes;
  __u8 mask[];
};
struct drm_xe_query_engine_cycles {
  struct drm_xe_engine_class_instance eci;
  __s32 clockid;
  __u32 width;
  __u64 engine_cycles;
  __u64 cpu_timestamp;
  __u64 cpu_delta;
};
struct drm_xe_query_uc_fw_version {
#define XE_QUERY_UC_TYPE_GUC_SUBMISSION 0
#define XE_QUERY_UC_TYPE_HUC 1
  __u16 uc_type;
  __u16 pad;
  __u32 branch_ver;
  __u32 major_ver;
  __u32 minor_ver;
  __u32 patch_ver;
  __u32 pad2;
  __u64 reserved;
};
struct drm_xe_device_query {
  __u64 extensions;
#define DRM_XE_DEVICE_QUERY_ENGINES 0
#define DRM_XE_DEVICE_QUERY_MEM_REGIONS 1
#define DRM_XE_DEVICE_QUERY_CONFIG 2
#define DRM_XE_DEVICE_QUERY_GT_LIST 3
#define DRM_XE_DEVICE_QUERY_HWCONFIG 4
#define DRM_XE_DEVICE_QUERY_GT_TOPOLOGY 5
#define DRM_XE_DEVICE_QUERY_ENGINE_CYCLES 6
#define DRM_XE_DEVICE_QUERY_UC_FW_VERSION 7
  __u32 query;
  __u32 size;
  __u64 data;
  __u64 reserved[2];
};
struct drm_xe_gem_create {
  __u64 extensions;
  __u64 size;
  __u32 placement;
#define DRM_XE_GEM_CREATE_FLAG_DEFER_BACKING (1 << 0)
#define DRM_XE_GEM_CREATE_FLAG_SCANOUT (1 << 1)
#define DRM_XE_GEM_CREATE_FLAG_NEEDS_VISIBLE_VRAM (1 << 2)
  __u32 flags;
  __u32 vm_id;
  __u32 handle;
#define DRM_XE_GEM_CPU_CACHING_WB 1
#define DRM_XE_GEM_CPU_CACHING_WC 2
  __u16 cpu_caching;
  __u16 pad[3];
  __u64 reserved[2];
};
struct drm_xe_gem_mmap_offset {
  __u64 extensions;
  __u32 handle;
  __u32 flags;
  __u64 offset;
  __u64 reserved[2];
};
struct drm_xe_vm_create {
  __u64 extensions;
#define DRM_XE_VM_CREATE_FLAG_SCRATCH_PAGE (1 << 0)
#define DRM_XE_VM_CREATE_FLAG_LR_MODE (1 << 1)
#define DRM_XE_VM_CREATE_FLAG_FAULT_MODE (1 << 2)
  __u32 flags;
  __u32 vm_id;
  __u64 reserved[2];
};
struct drm_xe_vm_destroy {
  __u32 vm_id;
  __u32 pad;
  __u64 reserved[2];
};
struct drm_xe_vm_bind_op {
  __u64 extensions;
  __u32 obj;
  __u16 pat_index;
  __u16 pad;
  union {
    __u64 obj_offset;
    __u64 userptr;
  };
  __u64 range;
  __u64 addr;
#define DRM_XE_VM_BIND_OP_MAP 0x0
#define DRM_XE_VM_BIND_OP_UNMAP 0x1
#define DRM_XE_VM_BIND_OP_MAP_USERPTR 0x2
#define DRM_XE_VM_BIND_OP_UNMAP_ALL 0x3
#define DRM_XE_VM_BIND_OP_PREFETCH 0x4
  __u32 op;
#define DRM_XE_VM_BIND_FLAG_READONLY (1 << 0)
#define DRM_XE_VM_BIND_FLAG_IMMEDIATE (1 << 1)
#define DRM_XE_VM_BIND_FLAG_NULL (1 << 2)
#define DRM_XE_VM_BIND_FLAG_DUMPABLE (1 << 3)
  __u32 flags;
  __u32 prefetch_mem_region_instance;
  __u32 pad2;
  __u64 reserved[3];
};
struct drm_xe_vm_bind {
  __u64 extensions;
  __u32 vm_id;
  __u32 exec_queue_id;
  __u32 pad;
  __u32 num_binds;
  union {
    struct drm_xe_vm_bind_op bind;
    __u64 vector_of_binds;
  };
  __u32 pad2;
  __u32 num_syncs;
  __u64 syncs;
  __u64 reserved[2];
};
struct drm_xe_exec_queue_create {
#define DRM_XE_EXEC_QUEUE_EXTENSION_SET_PROPERTY 0
#define DRM_XE_EXEC_QUEUE_SET_PROPERTY_PRIORITY 0
#define DRM_XE_EXEC_QUEUE_SET_PROPERTY_TIMESLICE 1
  __u64 extensions;
  __u16 width;
  __u16 num_placements;
  __u32 vm_id;
  __u32 flags;
  __u32 exec_queue_id;
  __u64 instances;
  __u64 reserved[2];
};
struct drm_xe_exec_queue_destroy {
  __u32 exec_queue_id;
  __u32 pad;
  __u64 reserved[2];
};
struct drm_xe_exec_queue_get_property {
  __u64 extensions;
  __u32 exec_queue_id;
#define DRM_XE_EXEC_QUEUE_GET_PROPERTY_BAN 0
  __u32 property;
  __u64 value;
  __u64 reserved[2];
};
struct drm_xe_sync {
  __u64 extensions;
#define DRM_XE_SYNC_TYPE_SYNCOBJ 0x0
#define DRM_XE_SYNC_TYPE_TIMELINE_SYNCOBJ 0x1
#define DRM_XE_SYNC_TYPE_USER_FENCE 0x2
  __u32 type;
#define DRM_XE_SYNC_FLAG_SIGNAL (1 << 0)
  __u32 flags;
  union {
    __u32 handle;
    __u64 addr;
  };
  __u64 timeline_value;
  __u64 reserved[2];
};
struct drm_xe_exec {
  __u64 extensions;
  __u32 exec_queue_id;
  __u32 num_syncs;
  __u64 syncs;
  __u64 address;
  __u16 num_batch_buffer;
  __u16 pad[3];
  __u64 reserved[2];
};
struct drm_xe_wait_user_fence {
  __u64 extensions;
  __u64 addr;
#define DRM_XE_UFENCE_WAIT_OP_EQ 0x0
#define DRM_XE_UFENCE_WAIT_OP_NEQ 0x1
#define DRM_XE_UFENCE_WAIT_OP_GT 0x2
#define DRM_XE_UFENCE_WAIT_OP_GTE 0x3
#define DRM_XE_UFENCE_WAIT_OP_LT 0x4
#define DRM_XE_UFENCE_WAIT_OP_LTE 0x5
  __u16 op;
#define DRM_XE_UFENCE_WAIT_FLAG_ABSTIME (1 << 0)
  __u16 flags;
  __u32 pad;
  __u64 value;
  __u64 mask;
  __s64 timeout;
  __u32 exec_queue_id;
  __u32 pad2;
  __u64 reserved[2];
};
#ifdef __cplusplus
}
#endif
#endif
