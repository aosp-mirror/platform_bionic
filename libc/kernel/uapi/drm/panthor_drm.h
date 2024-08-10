/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _PANTHOR_DRM_H_
#define _PANTHOR_DRM_H_
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DRM_PANTHOR_USER_MMIO_OFFSET_32BIT (1ull << 43)
#define DRM_PANTHOR_USER_MMIO_OFFSET_64BIT (1ull << 56)
#define DRM_PANTHOR_USER_MMIO_OFFSET (sizeof(unsigned long) < 8 ? DRM_PANTHOR_USER_MMIO_OFFSET_32BIT : DRM_PANTHOR_USER_MMIO_OFFSET_64BIT)
#define DRM_PANTHOR_USER_FLUSH_ID_MMIO_OFFSET (DRM_PANTHOR_USER_MMIO_OFFSET | 0)
enum drm_panthor_ioctl_id {
  DRM_PANTHOR_DEV_QUERY = 0,
  DRM_PANTHOR_VM_CREATE,
  DRM_PANTHOR_VM_DESTROY,
  DRM_PANTHOR_VM_BIND,
  DRM_PANTHOR_VM_GET_STATE,
  DRM_PANTHOR_BO_CREATE,
  DRM_PANTHOR_BO_MMAP_OFFSET,
  DRM_PANTHOR_GROUP_CREATE,
  DRM_PANTHOR_GROUP_DESTROY,
  DRM_PANTHOR_GROUP_SUBMIT,
  DRM_PANTHOR_GROUP_GET_STATE,
  DRM_PANTHOR_TILER_HEAP_CREATE,
  DRM_PANTHOR_TILER_HEAP_DESTROY,
};
#define DRM_IOCTL_PANTHOR(__access,__id,__type) DRM_IO ##__access(DRM_COMMAND_BASE + DRM_PANTHOR_ ##__id, struct drm_panthor_ ##__type)
#define DRM_IOCTL_PANTHOR_DEV_QUERY DRM_IOCTL_PANTHOR(WR, DEV_QUERY, dev_query)
#define DRM_IOCTL_PANTHOR_VM_CREATE DRM_IOCTL_PANTHOR(WR, VM_CREATE, vm_create)
#define DRM_IOCTL_PANTHOR_VM_DESTROY DRM_IOCTL_PANTHOR(WR, VM_DESTROY, vm_destroy)
#define DRM_IOCTL_PANTHOR_VM_BIND DRM_IOCTL_PANTHOR(WR, VM_BIND, vm_bind)
#define DRM_IOCTL_PANTHOR_VM_GET_STATE DRM_IOCTL_PANTHOR(WR, VM_GET_STATE, vm_get_state)
#define DRM_IOCTL_PANTHOR_BO_CREATE DRM_IOCTL_PANTHOR(WR, BO_CREATE, bo_create)
#define DRM_IOCTL_PANTHOR_BO_MMAP_OFFSET DRM_IOCTL_PANTHOR(WR, BO_MMAP_OFFSET, bo_mmap_offset)
#define DRM_IOCTL_PANTHOR_GROUP_CREATE DRM_IOCTL_PANTHOR(WR, GROUP_CREATE, group_create)
#define DRM_IOCTL_PANTHOR_GROUP_DESTROY DRM_IOCTL_PANTHOR(WR, GROUP_DESTROY, group_destroy)
#define DRM_IOCTL_PANTHOR_GROUP_SUBMIT DRM_IOCTL_PANTHOR(WR, GROUP_SUBMIT, group_submit)
#define DRM_IOCTL_PANTHOR_GROUP_GET_STATE DRM_IOCTL_PANTHOR(WR, GROUP_GET_STATE, group_get_state)
#define DRM_IOCTL_PANTHOR_TILER_HEAP_CREATE DRM_IOCTL_PANTHOR(WR, TILER_HEAP_CREATE, tiler_heap_create)
#define DRM_IOCTL_PANTHOR_TILER_HEAP_DESTROY DRM_IOCTL_PANTHOR(WR, TILER_HEAP_DESTROY, tiler_heap_destroy)
struct drm_panthor_obj_array {
  __u32 stride;
  __u32 count;
  __u64 array;
};
#define DRM_PANTHOR_OBJ_ARRAY(cnt,ptr) {.stride = sizeof((ptr)[0]),.count = (cnt),.array = (__u64) (uintptr_t) (ptr) }
enum drm_panthor_sync_op_flags {
  DRM_PANTHOR_SYNC_OP_HANDLE_TYPE_MASK = 0xff,
  DRM_PANTHOR_SYNC_OP_HANDLE_TYPE_SYNCOBJ = 0,
  DRM_PANTHOR_SYNC_OP_HANDLE_TYPE_TIMELINE_SYNCOBJ = 1,
  DRM_PANTHOR_SYNC_OP_WAIT = 0 << 31,
  DRM_PANTHOR_SYNC_OP_SIGNAL = (int) (1u << 31),
};
struct drm_panthor_sync_op {
  __u32 flags;
  __u32 handle;
  __u64 timeline_value;
};
enum drm_panthor_dev_query_type {
  DRM_PANTHOR_DEV_QUERY_GPU_INFO = 0,
  DRM_PANTHOR_DEV_QUERY_CSIF_INFO,
};
struct drm_panthor_gpu_info {
  __u32 gpu_id;
#define DRM_PANTHOR_ARCH_MAJOR(x) ((x) >> 28)
#define DRM_PANTHOR_ARCH_MINOR(x) (((x) >> 24) & 0xf)
#define DRM_PANTHOR_ARCH_REV(x) (((x) >> 20) & 0xf)
#define DRM_PANTHOR_PRODUCT_MAJOR(x) (((x) >> 16) & 0xf)
#define DRM_PANTHOR_VERSION_MAJOR(x) (((x) >> 12) & 0xf)
#define DRM_PANTHOR_VERSION_MINOR(x) (((x) >> 4) & 0xff)
#define DRM_PANTHOR_VERSION_STATUS(x) ((x) & 0xf)
  __u32 gpu_rev;
  __u32 csf_id;
#define DRM_PANTHOR_CSHW_MAJOR(x) (((x) >> 26) & 0x3f)
#define DRM_PANTHOR_CSHW_MINOR(x) (((x) >> 20) & 0x3f)
#define DRM_PANTHOR_CSHW_REV(x) (((x) >> 16) & 0xf)
#define DRM_PANTHOR_MCU_MAJOR(x) (((x) >> 10) & 0x3f)
#define DRM_PANTHOR_MCU_MINOR(x) (((x) >> 4) & 0x3f)
#define DRM_PANTHOR_MCU_REV(x) ((x) & 0xf)
  __u32 l2_features;
  __u32 tiler_features;
  __u32 mem_features;
  __u32 mmu_features;
#define DRM_PANTHOR_MMU_VA_BITS(x) ((x) & 0xff)
  __u32 thread_features;
  __u32 max_threads;
  __u32 thread_max_workgroup_size;
  __u32 thread_max_barrier_size;
  __u32 coherency_features;
  __u32 texture_features[4];
  __u32 as_present;
  __u64 shader_present;
  __u64 l2_present;
  __u64 tiler_present;
  __u32 core_features;
  __u32 pad;
};
struct drm_panthor_csif_info {
  __u32 csg_slot_count;
  __u32 cs_slot_count;
  __u32 cs_reg_count;
  __u32 scoreboard_slot_count;
  __u32 unpreserved_cs_reg_count;
  __u32 pad;
};
struct drm_panthor_dev_query {
  __u32 type;
  __u32 size;
  __u64 pointer;
};
struct drm_panthor_vm_create {
  __u32 flags;
  __u32 id;
  __u64 user_va_range;
};
struct drm_panthor_vm_destroy {
  __u32 id;
  __u32 pad;
};
enum drm_panthor_vm_bind_op_flags {
  DRM_PANTHOR_VM_BIND_OP_MAP_READONLY = 1 << 0,
  DRM_PANTHOR_VM_BIND_OP_MAP_NOEXEC = 1 << 1,
  DRM_PANTHOR_VM_BIND_OP_MAP_UNCACHED = 1 << 2,
  DRM_PANTHOR_VM_BIND_OP_TYPE_MASK = (int) (0xfu << 28),
  DRM_PANTHOR_VM_BIND_OP_TYPE_MAP = 0 << 28,
  DRM_PANTHOR_VM_BIND_OP_TYPE_UNMAP = 1 << 28,
  DRM_PANTHOR_VM_BIND_OP_TYPE_SYNC_ONLY = 2 << 28,
};
struct drm_panthor_vm_bind_op {
  __u32 flags;
  __u32 bo_handle;
  __u64 bo_offset;
  __u64 va;
  __u64 size;
  struct drm_panthor_obj_array syncs;
};
enum drm_panthor_vm_bind_flags {
  DRM_PANTHOR_VM_BIND_ASYNC = 1 << 0,
};
struct drm_panthor_vm_bind {
  __u32 vm_id;
  __u32 flags;
  struct drm_panthor_obj_array ops;
};
enum drm_panthor_vm_state {
  DRM_PANTHOR_VM_STATE_USABLE,
  DRM_PANTHOR_VM_STATE_UNUSABLE,
};
struct drm_panthor_vm_get_state {
  __u32 vm_id;
  __u32 state;
};
enum drm_panthor_bo_flags {
  DRM_PANTHOR_BO_NO_MMAP = (1 << 0),
};
struct drm_panthor_bo_create {
  __u64 size;
  __u32 flags;
  __u32 exclusive_vm_id;
  __u32 handle;
  __u32 pad;
};
struct drm_panthor_bo_mmap_offset {
  __u32 handle;
  __u32 pad;
  __u64 offset;
};
struct drm_panthor_queue_create {
  __u8 priority;
  __u8 pad[3];
  __u32 ringbuf_size;
};
enum drm_panthor_group_priority {
  PANTHOR_GROUP_PRIORITY_LOW = 0,
  PANTHOR_GROUP_PRIORITY_MEDIUM,
  PANTHOR_GROUP_PRIORITY_HIGH,
};
struct drm_panthor_group_create {
  struct drm_panthor_obj_array queues;
  __u8 max_compute_cores;
  __u8 max_fragment_cores;
  __u8 max_tiler_cores;
  __u8 priority;
  __u32 pad;
  __u64 compute_core_mask;
  __u64 fragment_core_mask;
  __u64 tiler_core_mask;
  __u32 vm_id;
  __u32 group_handle;
};
struct drm_panthor_group_destroy {
  __u32 group_handle;
  __u32 pad;
};
struct drm_panthor_queue_submit {
  __u32 queue_index;
  __u32 stream_size;
  __u64 stream_addr;
  __u32 latest_flush;
  __u32 pad;
  struct drm_panthor_obj_array syncs;
};
struct drm_panthor_group_submit {
  __u32 group_handle;
  __u32 pad;
  struct drm_panthor_obj_array queue_submits;
};
enum drm_panthor_group_state_flags {
  DRM_PANTHOR_GROUP_STATE_TIMEDOUT = 1 << 0,
  DRM_PANTHOR_GROUP_STATE_FATAL_FAULT = 1 << 1,
};
struct drm_panthor_group_get_state {
  __u32 group_handle;
  __u32 state;
  __u32 fatal_queues;
  __u32 pad;
};
struct drm_panthor_tiler_heap_create {
  __u32 vm_id;
  __u32 initial_chunk_count;
  __u32 chunk_size;
  __u32 max_chunks;
  __u32 target_in_flight;
  __u32 handle;
  __u64 tiler_heap_ctx_gpu_va;
  __u64 first_heap_chunk_gpu_va;
};
struct drm_panthor_tiler_heap_destroy {
  __u32 handle;
  __u32 pad;
};
#ifdef __cplusplus
}
#endif
#endif
