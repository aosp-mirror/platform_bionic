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
#ifndef _UAPI_I915_DRM_H_
#define _UAPI_I915_DRM_H_
#include <drm/drm.h>
#define I915_NR_TEX_REGIONS 255
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_LOG_MIN_TEX_REGION_SIZE 14
typedef struct _drm_i915_init {
 enum {
 I915_INIT_DMA = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 I915_CLEANUP_DMA = 0x02,
 I915_RESUME_DMA = 0x03
 } func;
 unsigned int mmio_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int sarea_priv_offset;
 unsigned int ring_start;
 unsigned int ring_end;
 unsigned int ring_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int front_offset;
 unsigned int back_offset;
 unsigned int depth_offset;
 unsigned int w;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int h;
 unsigned int pitch;
 unsigned int pitch_bits;
 unsigned int back_pitch;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int depth_pitch;
 unsigned int cpp;
 unsigned int chipset;
} drm_i915_init_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct _drm_i915_sarea {
 struct drm_tex_region texList[I915_NR_TEX_REGIONS + 1];
 int last_upload;
 int last_enqueue;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int last_dispatch;
 int ctxOwner;
 int texAge;
 int pf_enabled;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int pf_active;
 int pf_current_page;
 int perf_boxes;
 int width, height;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 drm_handle_t front_handle;
 int front_offset;
 int front_size;
 drm_handle_t back_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int back_offset;
 int back_size;
 drm_handle_t depth_handle;
 int depth_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int depth_size;
 drm_handle_t tex_handle;
 int tex_offset;
 int tex_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int log_tex_granularity;
 int pitch;
 int rotation;
 int rotated_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int rotated_size;
 int rotated_pitch;
 int virtualX, virtualY;
 unsigned int front_tiled;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int back_tiled;
 unsigned int depth_tiled;
 unsigned int rotated_tiled;
 unsigned int rotated2_tiled;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int pipeA_x;
 int pipeA_y;
 int pipeA_w;
 int pipeA_h;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int pipeB_x;
 int pipeB_y;
 int pipeB_w;
 int pipeB_h;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 drm_handle_t unused_handle;
 __u32 unused1, unused2, unused3;
 __u32 front_bo_handle;
 __u32 back_bo_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 unused_bo_handle;
 __u32 depth_bo_handle;
} drm_i915_sarea_t;
#define planeA_x pipeA_x
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define planeA_y pipeA_y
#define planeA_w pipeA_w
#define planeA_h pipeA_h
#define planeB_x pipeB_x
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define planeB_y pipeB_y
#define planeB_w pipeB_w
#define planeB_h pipeB_h
#define I915_BOX_RING_EMPTY 0x1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_BOX_FLIP 0x2
#define I915_BOX_WAIT 0x4
#define I915_BOX_TEXTURE_LOAD 0x8
#define I915_BOX_LOST_CONTEXT 0x10
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_INIT 0x00
#define DRM_I915_FLUSH 0x01
#define DRM_I915_FLIP 0x02
#define DRM_I915_BATCHBUFFER 0x03
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_IRQ_EMIT 0x04
#define DRM_I915_IRQ_WAIT 0x05
#define DRM_I915_GETPARAM 0x06
#define DRM_I915_SETPARAM 0x07
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_ALLOC 0x08
#define DRM_I915_FREE 0x09
#define DRM_I915_INIT_HEAP 0x0a
#define DRM_I915_CMDBUFFER 0x0b
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_DESTROY_HEAP 0x0c
#define DRM_I915_SET_VBLANK_PIPE 0x0d
#define DRM_I915_GET_VBLANK_PIPE 0x0e
#define DRM_I915_VBLANK_SWAP 0x0f
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_HWS_ADDR 0x11
#define DRM_I915_GEM_INIT 0x13
#define DRM_I915_GEM_EXECBUFFER 0x14
#define DRM_I915_GEM_PIN 0x15
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GEM_UNPIN 0x16
#define DRM_I915_GEM_BUSY 0x17
#define DRM_I915_GEM_THROTTLE 0x18
#define DRM_I915_GEM_ENTERVT 0x19
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GEM_LEAVEVT 0x1a
#define DRM_I915_GEM_CREATE 0x1b
#define DRM_I915_GEM_PREAD 0x1c
#define DRM_I915_GEM_PWRITE 0x1d
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GEM_MMAP 0x1e
#define DRM_I915_GEM_SET_DOMAIN 0x1f
#define DRM_I915_GEM_SW_FINISH 0x20
#define DRM_I915_GEM_SET_TILING 0x21
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GEM_GET_TILING 0x22
#define DRM_I915_GEM_GET_APERTURE 0x23
#define DRM_I915_GEM_MMAP_GTT 0x24
#define DRM_I915_GET_PIPE_FROM_CRTC_ID 0x25
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GEM_MADVISE 0x26
#define DRM_I915_OVERLAY_PUT_IMAGE 0x27
#define DRM_I915_OVERLAY_ATTRS 0x28
#define DRM_I915_GEM_EXECBUFFER2 0x29
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GET_SPRITE_COLORKEY 0x2a
#define DRM_I915_SET_SPRITE_COLORKEY 0x2b
#define DRM_I915_GEM_WAIT 0x2c
#define DRM_I915_GEM_CONTEXT_CREATE 0x2d
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_GEM_CONTEXT_DESTROY 0x2e
#define DRM_I915_GEM_SET_CACHING 0x2f
#define DRM_I915_GEM_GET_CACHING 0x30
#define DRM_I915_REG_READ 0x31
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_INIT DRM_IOW( DRM_COMMAND_BASE + DRM_I915_INIT, drm_i915_init_t)
#define DRM_IOCTL_I915_FLUSH DRM_IO ( DRM_COMMAND_BASE + DRM_I915_FLUSH)
#define DRM_IOCTL_I915_FLIP DRM_IO ( DRM_COMMAND_BASE + DRM_I915_FLIP)
#define DRM_IOCTL_I915_BATCHBUFFER DRM_IOW( DRM_COMMAND_BASE + DRM_I915_BATCHBUFFER, drm_i915_batchbuffer_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_IRQ_EMIT DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_IRQ_EMIT, drm_i915_irq_emit_t)
#define DRM_IOCTL_I915_IRQ_WAIT DRM_IOW( DRM_COMMAND_BASE + DRM_I915_IRQ_WAIT, drm_i915_irq_wait_t)
#define DRM_IOCTL_I915_GETPARAM DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GETPARAM, drm_i915_getparam_t)
#define DRM_IOCTL_I915_SETPARAM DRM_IOW( DRM_COMMAND_BASE + DRM_I915_SETPARAM, drm_i915_setparam_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_ALLOC DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_ALLOC, drm_i915_mem_alloc_t)
#define DRM_IOCTL_I915_FREE DRM_IOW( DRM_COMMAND_BASE + DRM_I915_FREE, drm_i915_mem_free_t)
#define DRM_IOCTL_I915_INIT_HEAP DRM_IOW( DRM_COMMAND_BASE + DRM_I915_INIT_HEAP, drm_i915_mem_init_heap_t)
#define DRM_IOCTL_I915_CMDBUFFER DRM_IOW( DRM_COMMAND_BASE + DRM_I915_CMDBUFFER, drm_i915_cmdbuffer_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_DESTROY_HEAP DRM_IOW( DRM_COMMAND_BASE + DRM_I915_DESTROY_HEAP, drm_i915_mem_destroy_heap_t)
#define DRM_IOCTL_I915_SET_VBLANK_PIPE DRM_IOW( DRM_COMMAND_BASE + DRM_I915_SET_VBLANK_PIPE, drm_i915_vblank_pipe_t)
#define DRM_IOCTL_I915_GET_VBLANK_PIPE DRM_IOR( DRM_COMMAND_BASE + DRM_I915_GET_VBLANK_PIPE, drm_i915_vblank_pipe_t)
#define DRM_IOCTL_I915_VBLANK_SWAP DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_VBLANK_SWAP, drm_i915_vblank_swap_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_HWS_ADDR DRM_IOW(DRM_COMMAND_BASE + DRM_I915_HWS_ADDR, struct drm_i915_gem_init)
#define DRM_IOCTL_I915_GEM_INIT DRM_IOW(DRM_COMMAND_BASE + DRM_I915_GEM_INIT, struct drm_i915_gem_init)
#define DRM_IOCTL_I915_GEM_EXECBUFFER DRM_IOW(DRM_COMMAND_BASE + DRM_I915_GEM_EXECBUFFER, struct drm_i915_gem_execbuffer)
#define DRM_IOCTL_I915_GEM_EXECBUFFER2 DRM_IOW(DRM_COMMAND_BASE + DRM_I915_GEM_EXECBUFFER2, struct drm_i915_gem_execbuffer2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_GEM_PIN DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_PIN, struct drm_i915_gem_pin)
#define DRM_IOCTL_I915_GEM_UNPIN DRM_IOW(DRM_COMMAND_BASE + DRM_I915_GEM_UNPIN, struct drm_i915_gem_unpin)
#define DRM_IOCTL_I915_GEM_BUSY DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_BUSY, struct drm_i915_gem_busy)
#define DRM_IOCTL_I915_GEM_SET_CACHING DRM_IOW(DRM_COMMAND_BASE + DRM_I915_GEM_SET_CACHING, struct drm_i915_gem_caching)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_GEM_GET_CACHING DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_GET_CACHING, struct drm_i915_gem_caching)
#define DRM_IOCTL_I915_GEM_THROTTLE DRM_IO ( DRM_COMMAND_BASE + DRM_I915_GEM_THROTTLE)
#define DRM_IOCTL_I915_GEM_ENTERVT DRM_IO(DRM_COMMAND_BASE + DRM_I915_GEM_ENTERVT)
#define DRM_IOCTL_I915_GEM_LEAVEVT DRM_IO(DRM_COMMAND_BASE + DRM_I915_GEM_LEAVEVT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_CREATE, struct drm_i915_gem_create)
#define DRM_IOCTL_I915_GEM_PREAD DRM_IOW (DRM_COMMAND_BASE + DRM_I915_GEM_PREAD, struct drm_i915_gem_pread)
#define DRM_IOCTL_I915_GEM_PWRITE DRM_IOW (DRM_COMMAND_BASE + DRM_I915_GEM_PWRITE, struct drm_i915_gem_pwrite)
#define DRM_IOCTL_I915_GEM_MMAP DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_MMAP, struct drm_i915_gem_mmap)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_GEM_MMAP_GTT DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_MMAP_GTT, struct drm_i915_gem_mmap_gtt)
#define DRM_IOCTL_I915_GEM_SET_DOMAIN DRM_IOW (DRM_COMMAND_BASE + DRM_I915_GEM_SET_DOMAIN, struct drm_i915_gem_set_domain)
#define DRM_IOCTL_I915_GEM_SW_FINISH DRM_IOW (DRM_COMMAND_BASE + DRM_I915_GEM_SW_FINISH, struct drm_i915_gem_sw_finish)
#define DRM_IOCTL_I915_GEM_SET_TILING DRM_IOWR (DRM_COMMAND_BASE + DRM_I915_GEM_SET_TILING, struct drm_i915_gem_set_tiling)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_GEM_GET_TILING DRM_IOWR (DRM_COMMAND_BASE + DRM_I915_GEM_GET_TILING, struct drm_i915_gem_get_tiling)
#define DRM_IOCTL_I915_GEM_GET_APERTURE DRM_IOR (DRM_COMMAND_BASE + DRM_I915_GEM_GET_APERTURE, struct drm_i915_gem_get_aperture)
#define DRM_IOCTL_I915_GET_PIPE_FROM_CRTC_ID DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GET_PIPE_FROM_CRTC_ID, struct drm_i915_get_pipe_from_crtc_id)
#define DRM_IOCTL_I915_GEM_MADVISE DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_MADVISE, struct drm_i915_gem_madvise)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_OVERLAY_PUT_IMAGE DRM_IOW(DRM_COMMAND_BASE + DRM_I915_OVERLAY_PUT_IMAGE, struct drm_intel_overlay_put_image)
#define DRM_IOCTL_I915_OVERLAY_ATTRS DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_OVERLAY_ATTRS, struct drm_intel_overlay_attrs)
#define DRM_IOCTL_I915_SET_SPRITE_COLORKEY DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_SET_SPRITE_COLORKEY, struct drm_intel_sprite_colorkey)
#define DRM_IOCTL_I915_GET_SPRITE_COLORKEY DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_SET_SPRITE_COLORKEY, struct drm_intel_sprite_colorkey)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_I915_GEM_WAIT DRM_IOWR(DRM_COMMAND_BASE + DRM_I915_GEM_WAIT, struct drm_i915_gem_wait)
#define DRM_IOCTL_I915_GEM_CONTEXT_CREATE DRM_IOWR (DRM_COMMAND_BASE + DRM_I915_GEM_CONTEXT_CREATE, struct drm_i915_gem_context_create)
#define DRM_IOCTL_I915_GEM_CONTEXT_DESTROY DRM_IOW (DRM_COMMAND_BASE + DRM_I915_GEM_CONTEXT_DESTROY, struct drm_i915_gem_context_destroy)
#define DRM_IOCTL_I915_REG_READ DRM_IOWR (DRM_COMMAND_BASE + DRM_I915_REG_READ, struct drm_i915_reg_read)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct drm_i915_batchbuffer {
 int start;
 int used;
 int DR1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int DR4;
 int num_cliprects;
 struct drm_clip_rect __user *cliprects;
} drm_i915_batchbuffer_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct _drm_i915_cmdbuffer {
 char __user *buf;
 int sz;
 int DR1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int DR4;
 int num_cliprects;
 struct drm_clip_rect __user *cliprects;
} drm_i915_cmdbuffer_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct drm_i915_irq_emit {
 int __user *irq_seq;
} drm_i915_irq_emit_t;
typedef struct drm_i915_irq_wait {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int irq_seq;
} drm_i915_irq_wait_t;
#define I915_PARAM_IRQ_ACTIVE 1
#define I915_PARAM_ALLOW_BATCHBUFFER 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_PARAM_LAST_DISPATCH 3
#define I915_PARAM_CHIPSET_ID 4
#define I915_PARAM_HAS_GEM 5
#define I915_PARAM_NUM_FENCES_AVAIL 6
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_PARAM_HAS_OVERLAY 7
#define I915_PARAM_HAS_PAGEFLIPPING 8
#define I915_PARAM_HAS_EXECBUF2 9
#define I915_PARAM_HAS_BSD 10
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_PARAM_HAS_BLT 11
#define I915_PARAM_HAS_RELAXED_FENCING 12
#define I915_PARAM_HAS_COHERENT_RINGS 13
#define I915_PARAM_HAS_EXEC_CONSTANTS 14
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_PARAM_HAS_RELAXED_DELTA 15
#define I915_PARAM_HAS_GEN7_SOL_RESET 16
#define I915_PARAM_HAS_LLC 17
#define I915_PARAM_HAS_ALIASING_PPGTT 18
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_PARAM_HAS_WAIT_TIMEOUT 19
#define I915_PARAM_HAS_SEMAPHORES 20
#define I915_PARAM_HAS_PRIME_VMAP_FLUSH 21
#define I915_PARAM_RSVD_FOR_FUTURE_USE 22
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_PARAM_HAS_SECURE_BATCHES 23
#define I915_PARAM_HAS_PINNED_BATCHES 24
#define I915_PARAM_HAS_EXEC_NO_RELOC 25
#define I915_PARAM_HAS_EXEC_HANDLE_LUT 26
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct drm_i915_getparam {
 int param;
 int __user *value;
} drm_i915_getparam_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_SETPARAM_USE_MI_BATCHBUFFER_START 1
#define I915_SETPARAM_TEX_LRU_LOG_GRANULARITY 2
#define I915_SETPARAM_ALLOW_BATCHBUFFER 3
#define I915_SETPARAM_NUM_USED_FENCES 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct drm_i915_setparam {
 int param;
 int value;
} drm_i915_setparam_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_MEM_REGION_AGP 1
typedef struct drm_i915_mem_alloc {
 int region;
 int alignment;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int size;
 int __user *region_offset;
} drm_i915_mem_alloc_t;
typedef struct drm_i915_mem_free {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int region;
 int region_offset;
} drm_i915_mem_free_t;
typedef struct drm_i915_mem_init_heap {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int region;
 int size;
 int start;
} drm_i915_mem_init_heap_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct drm_i915_mem_destroy_heap {
 int region;
} drm_i915_mem_destroy_heap_t;
#define DRM_I915_VBLANK_PIPE_A 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_I915_VBLANK_PIPE_B 2
typedef struct drm_i915_vblank_pipe {
 int pipe;
} drm_i915_vblank_pipe_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct drm_i915_vblank_swap {
 drm_drawable_t drawable;
 enum drm_vblank_seq_type seqtype;
 unsigned int sequence;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} drm_i915_vblank_swap_t;
typedef struct drm_i915_hws_addr {
 __u64 addr;
} drm_i915_hws_addr_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_init {
 __u64 gtt_start;
 __u64 gtt_end;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_create {
 __u64 size;
 __u32 handle;
 __u32 pad;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_gem_pread {
 __u32 handle;
 __u32 pad;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 offset;
 __u64 size;
 __u64 data_ptr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_pwrite {
 __u32 handle;
 __u32 pad;
 __u64 offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 size;
 __u64 data_ptr;
};
struct drm_i915_gem_mmap {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle;
 __u32 pad;
 __u64 offset;
 __u64 size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 addr_ptr;
};
struct drm_i915_gem_mmap_gtt {
 __u32 handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pad;
 __u64 offset;
};
struct drm_i915_gem_set_domain {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle;
 __u32 read_domains;
 __u32 write_domain;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_sw_finish {
 __u32 handle;
};
struct drm_i915_gem_relocation_entry {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 target_handle;
 __u32 delta;
 __u64 offset;
 __u64 presumed_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 read_domains;
 __u32 write_domain;
};
#define I915_GEM_DOMAIN_CPU 0x00000001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_GEM_DOMAIN_RENDER 0x00000002
#define I915_GEM_DOMAIN_SAMPLER 0x00000004
#define I915_GEM_DOMAIN_COMMAND 0x00000008
#define I915_GEM_DOMAIN_INSTRUCTION 0x00000010
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_GEM_DOMAIN_VERTEX 0x00000020
#define I915_GEM_DOMAIN_GTT 0x00000040
struct drm_i915_gem_exec_object {
 __u32 handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 relocation_count;
 __u64 relocs_ptr;
 __u64 alignment;
 __u64 offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_gem_execbuffer {
 __u64 buffers_ptr;
 __u32 buffer_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 batch_start_offset;
 __u32 batch_len;
 __u32 DR1;
 __u32 DR4;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 num_cliprects;
 __u64 cliprects_ptr;
};
struct drm_i915_gem_exec_object2 {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle;
 __u32 relocation_count;
 __u64 relocs_ptr;
 __u64 alignment;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 offset;
#define EXEC_OBJECT_NEEDS_FENCE (1<<0)
#define EXEC_OBJECT_NEEDS_GTT (1<<1)
#define EXEC_OBJECT_WRITE (1<<2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __EXEC_OBJECT_UNKNOWN_FLAGS -(EXEC_OBJECT_WRITE<<1)
 __u64 flags;
 __u64 rsvd1;
 __u64 rsvd2;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_gem_execbuffer2 {
 __u64 buffers_ptr;
 __u32 buffer_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 batch_start_offset;
 __u32 batch_len;
 __u32 DR1;
 __u32 DR4;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 num_cliprects;
 __u64 cliprects_ptr;
#define I915_EXEC_RING_MASK (7<<0)
#define I915_EXEC_DEFAULT (0<<0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_EXEC_RENDER (1<<0)
#define I915_EXEC_BSD (2<<0)
#define I915_EXEC_BLT (3<<0)
#define I915_EXEC_CONSTANTS_MASK (3<<6)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_EXEC_CONSTANTS_REL_GENERAL (0<<6)
#define I915_EXEC_CONSTANTS_ABSOLUTE (1<<6)
#define I915_EXEC_CONSTANTS_REL_SURFACE (2<<6)
 __u64 flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 rsvd1;
 __u64 rsvd2;
};
#define I915_EXEC_GEN7_SOL_RESET (1<<8)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_EXEC_SECURE (1<<9)
#define I915_EXEC_IS_PINNED (1<<10)
#define I915_EXEC_NO_RELOC (1<<11)
#define I915_EXEC_HANDLE_LUT (1<<12)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __I915_EXEC_UNKNOWN_FLAGS -(I915_EXEC_HANDLE_LUT<<1)
#define I915_EXEC_CONTEXT_ID_MASK (0xffffffff)
#define i915_execbuffer2_set_context_id(eb2, context)   (eb2).rsvd1 = context & I915_EXEC_CONTEXT_ID_MASK
#define i915_execbuffer2_get_context_id(eb2)   ((eb2).rsvd1 & I915_EXEC_CONTEXT_ID_MASK)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_pin {
 __u32 handle;
 __u32 pad;
 __u64 alignment;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 offset;
};
struct drm_i915_gem_unpin {
 __u32 handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pad;
};
struct drm_i915_gem_busy {
 __u32 handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 busy;
};
#define I915_CACHING_NONE 0
#define I915_CACHING_CACHED 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_caching {
 __u32 handle;
 __u32 caching;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_TILING_NONE 0
#define I915_TILING_X 1
#define I915_TILING_Y 2
#define I915_BIT_6_SWIZZLE_NONE 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_BIT_6_SWIZZLE_9 1
#define I915_BIT_6_SWIZZLE_9_10 2
#define I915_BIT_6_SWIZZLE_9_11 3
#define I915_BIT_6_SWIZZLE_9_10_11 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_BIT_6_SWIZZLE_UNKNOWN 5
#define I915_BIT_6_SWIZZLE_9_17 6
#define I915_BIT_6_SWIZZLE_9_10_17 7
struct drm_i915_gem_set_tiling {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle;
 __u32 tiling_mode;
 __u32 stride;
 __u32 swizzle_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_gem_get_tiling {
 __u32 handle;
 __u32 tiling_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 swizzle_mode;
};
struct drm_i915_gem_get_aperture {
 __u64 aper_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 aper_available_size;
};
struct drm_i915_get_pipe_from_crtc_id {
 __u32 crtc_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pipe;
};
#define I915_MADV_WILLNEED 0
#define I915_MADV_DONTNEED 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __I915_MADV_PURGED 2
struct drm_i915_gem_madvise {
 __u32 handle;
 __u32 madv;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 retained;
};
#define I915_OVERLAY_TYPE_MASK 0xff
#define I915_OVERLAY_YUV_PLANAR 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_OVERLAY_YUV_PACKED 0x02
#define I915_OVERLAY_RGB 0x03
#define I915_OVERLAY_DEPTH_MASK 0xff00
#define I915_OVERLAY_RGB24 0x1000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_OVERLAY_RGB16 0x2000
#define I915_OVERLAY_RGB15 0x3000
#define I915_OVERLAY_YUV422 0x0100
#define I915_OVERLAY_YUV411 0x0200
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_OVERLAY_YUV420 0x0300
#define I915_OVERLAY_YUV410 0x0400
#define I915_OVERLAY_SWAP_MASK 0xff0000
#define I915_OVERLAY_NO_SWAP 0x000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_OVERLAY_UV_SWAP 0x010000
#define I915_OVERLAY_Y_SWAP 0x020000
#define I915_OVERLAY_Y_AND_UV_SWAP 0x030000
#define I915_OVERLAY_FLAGS_MASK 0xff000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_OVERLAY_ENABLE 0x01000000
struct drm_intel_overlay_put_image {
 __u32 flags;
 __u32 bo_handle;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 stride_Y;
 __u16 stride_UV;
 __u32 offset_Y;
 __u32 offset_U;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 offset_V;
 __u16 src_width;
 __u16 src_height;
 __u16 src_scan_width;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 src_scan_height;
 __u32 crtc_id;
 __u16 dst_x;
 __u16 dst_y;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 dst_width;
 __u16 dst_height;
};
#define I915_OVERLAY_UPDATE_ATTRS (1<<0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_OVERLAY_UPDATE_GAMMA (1<<1)
struct drm_intel_overlay_attrs {
 __u32 flags;
 __u32 color_key;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __s32 brightness;
 __u32 contrast;
 __u32 saturation;
 __u32 gamma0;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 gamma1;
 __u32 gamma2;
 __u32 gamma3;
 __u32 gamma4;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 gamma5;
};
#define I915_SET_COLORKEY_NONE (1<<0)
#define I915_SET_COLORKEY_DESTINATION (1<<1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define I915_SET_COLORKEY_SOURCE (1<<2)
struct drm_intel_sprite_colorkey {
 __u32 plane_id;
 __u32 min_value;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 channel_mask;
 __u32 max_value;
 __u32 flags;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_i915_gem_wait {
 __u32 bo_handle;
 __u32 flags;
 __s64 timeout_ns;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_gem_context_create {
 __u32 ctx_id;
 __u32 pad;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_gem_context_destroy {
 __u32 ctx_id;
 __u32 pad;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_i915_reg_read {
 __u64 offset;
 __u64 val;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif
