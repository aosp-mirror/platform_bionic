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
#include <drm/drm.h>
struct drm_exynos_gem_create {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint64_t size;
 unsigned int flags;
 unsigned int handle;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_exynos_gem_map_off {
 unsigned int handle;
 unsigned int pad;
 uint64_t offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_exynos_gem_mmap {
 unsigned int handle;
 unsigned int pad;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint64_t size;
 uint64_t mapped;
};
struct drm_exynos_gem_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int handle;
 unsigned int flags;
 uint64_t size;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_exynos_vidi_connection {
 unsigned int connection;
 unsigned int extensions;
 uint64_t edid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum e_drm_exynos_gem_mem_type {
 EXYNOS_BO_CONTIG = 0 << 0,
 EXYNOS_BO_NONCONTIG = 1 << 0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EXYNOS_BO_NONCACHABLE = 0 << 1,
 EXYNOS_BO_CACHABLE = 1 << 1,
 EXYNOS_BO_WC = 1 << 2,
 EXYNOS_BO_MASK = EXYNOS_BO_NONCONTIG | EXYNOS_BO_CACHABLE |
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EXYNOS_BO_WC
};
struct drm_exynos_g2d_get_ver {
 __u32 major;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 minor;
};
struct drm_exynos_g2d_cmd {
 __u32 offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 data;
};
enum drm_exynos_g2d_buf_type {
 G2D_BUF_USERPTR = 1 << 31,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum drm_exynos_g2d_event_type {
 G2D_EVENT_NOT,
 G2D_EVENT_NONSTOP,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 G2D_EVENT_STOP,
};
struct drm_exynos_g2d_userptr {
 unsigned long userptr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long size;
};
struct drm_exynos_g2d_set_cmdlist {
 __u64 cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 cmd_buf;
 __u32 cmd_nr;
 __u32 cmd_buf_nr;
 __u64 event_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 user_data;
};
struct drm_exynos_g2d_exec {
 __u64 async;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum drm_exynos_ops_id {
 EXYNOS_DRM_OPS_SRC,
 EXYNOS_DRM_OPS_DST,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EXYNOS_DRM_OPS_MAX,
};
struct drm_exynos_sz {
 __u32 hsize;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 vsize;
};
struct drm_exynos_pos {
 __u32 x;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 y;
 __u32 w;
 __u32 h;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum drm_exynos_flip {
 EXYNOS_DRM_FLIP_NONE = (0 << 0),
 EXYNOS_DRM_FLIP_VERTICAL = (1 << 0),
 EXYNOS_DRM_FLIP_HORIZONTAL = (1 << 1),
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EXYNOS_DRM_FLIP_BOTH = EXYNOS_DRM_FLIP_VERTICAL |
 EXYNOS_DRM_FLIP_HORIZONTAL,
};
enum drm_exynos_degree {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EXYNOS_DRM_DEGREE_0,
 EXYNOS_DRM_DEGREE_90,
 EXYNOS_DRM_DEGREE_180,
 EXYNOS_DRM_DEGREE_270,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum drm_exynos_planer {
 EXYNOS_DRM_PLANAR_Y,
 EXYNOS_DRM_PLANAR_CB,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 EXYNOS_DRM_PLANAR_CR,
 EXYNOS_DRM_PLANAR_MAX,
};
struct drm_exynos_ipp_prop_list {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 version;
 __u32 ipp_id;
 __u32 count;
 __u32 writeback;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 flip;
 __u32 degree;
 __u32 csc;
 __u32 crop;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 scale;
 __u32 refresh_min;
 __u32 refresh_max;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct drm_exynos_sz crop_min;
 struct drm_exynos_sz crop_max;
 struct drm_exynos_sz scale_min;
 struct drm_exynos_sz scale_max;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_exynos_ipp_config {
 enum drm_exynos_ops_id ops_id;
 enum drm_exynos_flip flip;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 enum drm_exynos_degree degree;
 __u32 fmt;
 struct drm_exynos_sz sz;
 struct drm_exynos_pos pos;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum drm_exynos_ipp_cmd {
 IPP_CMD_NONE,
 IPP_CMD_M2M,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IPP_CMD_WB,
 IPP_CMD_OUTPUT,
 IPP_CMD_MAX,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct drm_exynos_ipp_property {
 struct drm_exynos_ipp_config config[EXYNOS_DRM_OPS_MAX];
 enum drm_exynos_ipp_cmd cmd;
 __u32 ipp_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 prop_id;
 __u32 refresh_rate;
};
enum drm_exynos_ipp_buf_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IPP_BUF_ENQUEUE,
 IPP_BUF_DEQUEUE,
};
struct drm_exynos_ipp_queue_buf {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 enum drm_exynos_ops_id ops_id;
 enum drm_exynos_ipp_buf_type buf_type;
 __u32 prop_id;
 __u32 buf_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 handle[EXYNOS_DRM_PLANAR_MAX];
 __u32 reserved;
 __u64 user_data;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum drm_exynos_ipp_ctrl {
 IPP_CTRL_PLAY,
 IPP_CTRL_STOP,
 IPP_CTRL_PAUSE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IPP_CTRL_RESUME,
 IPP_CTRL_MAX,
};
struct drm_exynos_ipp_cmd_ctrl {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 prop_id;
 enum drm_exynos_ipp_ctrl ctrl;
};
#define DRM_EXYNOS_GEM_CREATE 0x00
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_EXYNOS_GEM_MAP_OFFSET 0x01
#define DRM_EXYNOS_GEM_MMAP 0x02
#define DRM_EXYNOS_GEM_GET 0x04
#define DRM_EXYNOS_VIDI_CONNECTION 0x07
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_EXYNOS_G2D_GET_VER 0x20
#define DRM_EXYNOS_G2D_SET_CMDLIST 0x21
#define DRM_EXYNOS_G2D_EXEC 0x22
#define DRM_EXYNOS_IPP_GET_PROPERTY 0x30
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_EXYNOS_IPP_SET_PROPERTY 0x31
#define DRM_EXYNOS_IPP_QUEUE_BUF 0x32
#define DRM_EXYNOS_IPP_CMD_CTRL 0x33
#define DRM_IOCTL_EXYNOS_GEM_CREATE DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_GEM_CREATE, struct drm_exynos_gem_create)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_EXYNOS_GEM_MAP_OFFSET DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_GEM_MAP_OFFSET, struct drm_exynos_gem_map_off)
#define DRM_IOCTL_EXYNOS_GEM_MMAP DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_GEM_MMAP, struct drm_exynos_gem_mmap)
#define DRM_IOCTL_EXYNOS_GEM_GET DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_GEM_GET, struct drm_exynos_gem_info)
#define DRM_IOCTL_EXYNOS_VIDI_CONNECTION DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_VIDI_CONNECTION, struct drm_exynos_vidi_connection)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_EXYNOS_G2D_GET_VER DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_G2D_GET_VER, struct drm_exynos_g2d_get_ver)
#define DRM_IOCTL_EXYNOS_G2D_SET_CMDLIST DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_G2D_SET_CMDLIST, struct drm_exynos_g2d_set_cmdlist)
#define DRM_IOCTL_EXYNOS_G2D_EXEC DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_G2D_EXEC, struct drm_exynos_g2d_exec)
#define DRM_IOCTL_EXYNOS_IPP_GET_PROPERTY DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_IPP_GET_PROPERTY, struct drm_exynos_ipp_prop_list)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_IOCTL_EXYNOS_IPP_SET_PROPERTY DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_IPP_SET_PROPERTY, struct drm_exynos_ipp_property)
#define DRM_IOCTL_EXYNOS_IPP_QUEUE_BUF DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_IPP_QUEUE_BUF, struct drm_exynos_ipp_queue_buf)
#define DRM_IOCTL_EXYNOS_IPP_CMD_CTRL DRM_IOWR(DRM_COMMAND_BASE +   DRM_EXYNOS_IPP_CMD_CTRL, struct drm_exynos_ipp_cmd_ctrl)
#define DRM_EXYNOS_G2D_EVENT 0x80000000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DRM_EXYNOS_IPP_EVENT 0x80000001
struct drm_exynos_g2d_event {
 struct drm_event base;
 __u64 user_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 tv_sec;
 __u32 tv_usec;
 __u32 cmdlist_no;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct drm_exynos_ipp_event {
 struct drm_event base;
 __u64 user_data;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 tv_sec;
 __u32 tv_usec;
 __u32 prop_id;
 __u32 reserved;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 buf_id[EXYNOS_DRM_OPS_MAX];
};
#endif
