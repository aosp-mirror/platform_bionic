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
#ifndef QAIC_ACCEL_H_
#define QAIC_ACCEL_H_
#include "drm.h"
#ifdef __cplusplus
extern "C" {
#endif
#define QAIC_MANAGE_MAX_MSG_LENGTH SZ_4K
#define QAIC_SEM_INSYNCFENCE 2
#define QAIC_SEM_OUTSYNCFENCE 1
#define QAIC_SEM_NOP 0
#define QAIC_SEM_INIT 1
#define QAIC_SEM_INC 2
#define QAIC_SEM_DEC 3
#define QAIC_SEM_WAIT_EQUAL 4
#define QAIC_SEM_WAIT_GT_EQ 5
#define QAIC_SEM_WAIT_GT_0 6
#define QAIC_TRANS_UNDEFINED 0
#define QAIC_TRANS_PASSTHROUGH_FROM_USR 1
#define QAIC_TRANS_PASSTHROUGH_TO_USR 2
#define QAIC_TRANS_PASSTHROUGH_FROM_DEV 3
#define QAIC_TRANS_PASSTHROUGH_TO_DEV 4
#define QAIC_TRANS_DMA_XFER_FROM_USR 5
#define QAIC_TRANS_DMA_XFER_TO_DEV 6
#define QAIC_TRANS_ACTIVATE_FROM_USR 7
#define QAIC_TRANS_ACTIVATE_FROM_DEV 8
#define QAIC_TRANS_ACTIVATE_TO_DEV 9
#define QAIC_TRANS_DEACTIVATE_FROM_USR 10
#define QAIC_TRANS_DEACTIVATE_FROM_DEV 11
#define QAIC_TRANS_STATUS_FROM_USR 12
#define QAIC_TRANS_STATUS_TO_USR 13
#define QAIC_TRANS_STATUS_FROM_DEV 14
#define QAIC_TRANS_STATUS_TO_DEV 15
#define QAIC_TRANS_TERMINATE_FROM_DEV 16
#define QAIC_TRANS_TERMINATE_TO_DEV 17
#define QAIC_TRANS_DMA_XFER_CONT 18
#define QAIC_TRANS_VALIDATE_PARTITION_FROM_DEV 19
#define QAIC_TRANS_VALIDATE_PARTITION_TO_DEV 20
struct qaic_manage_trans_hdr {
  __u32 type;
  __u32 len;
};
struct qaic_manage_trans_passthrough {
  struct qaic_manage_trans_hdr hdr;
  __u8 data[];
};
struct qaic_manage_trans_dma_xfer {
  struct qaic_manage_trans_hdr hdr;
  __u32 tag;
  __u32 pad;
  __u64 addr;
  __u64 size;
};
struct qaic_manage_trans_activate_to_dev {
  struct qaic_manage_trans_hdr hdr;
  __u32 queue_size;
  __u32 eventfd;
  __u32 options;
  __u32 pad;
};
struct qaic_manage_trans_activate_from_dev {
  struct qaic_manage_trans_hdr hdr;
  __u32 status;
  __u32 dbc_id;
  __u64 options;
};
struct qaic_manage_trans_deactivate {
  struct qaic_manage_trans_hdr hdr;
  __u32 dbc_id;
  __u32 pad;
};
struct qaic_manage_trans_status_to_dev {
  struct qaic_manage_trans_hdr hdr;
};
struct qaic_manage_trans_status_from_dev {
  struct qaic_manage_trans_hdr hdr;
  __u16 major;
  __u16 minor;
  __u32 status;
  __u64 status_flags;
};
struct qaic_manage_msg {
  __u32 len;
  __u32 count;
  __u64 data;
};
struct qaic_create_bo {
  __u64 size;
  __u32 handle;
  __u32 pad;
};
struct qaic_mmap_bo {
  __u32 handle;
  __u32 pad;
  __u64 offset;
};
struct qaic_sem {
  __u16 val;
  __u8 index;
  __u8 presync;
  __u8 cmd;
  __u8 flags;
  __u16 pad;
};
struct qaic_attach_slice_entry {
  __u64 size;
  struct qaic_sem sem0;
  struct qaic_sem sem1;
  struct qaic_sem sem2;
  struct qaic_sem sem3;
  __u64 dev_addr;
  __u64 db_addr;
  __u32 db_data;
  __u32 db_len;
  __u64 offset;
};
struct qaic_attach_slice_hdr {
  __u32 count;
  __u32 dbc_id;
  __u32 handle;
  __u32 dir;
  __u64 size;
};
struct qaic_attach_slice {
  struct qaic_attach_slice_hdr hdr;
  __u64 data;
};
struct qaic_execute_entry {
  __u32 handle;
  __u32 dir;
};
struct qaic_partial_execute_entry {
  __u32 handle;
  __u32 dir;
  __u64 resize;
};
struct qaic_execute_hdr {
  __u32 count;
  __u32 dbc_id;
};
struct qaic_execute {
  struct qaic_execute_hdr hdr;
  __u64 data;
};
struct qaic_wait {
  __u32 handle;
  __u32 timeout;
  __u32 dbc_id;
  __u32 pad;
};
struct qaic_perf_stats_hdr {
  __u16 count;
  __u16 pad;
  __u32 dbc_id;
};
struct qaic_perf_stats {
  struct qaic_perf_stats_hdr hdr;
  __u64 data;
};
struct qaic_perf_stats_entry {
  __u32 handle;
  __u32 queue_level_before;
  __u32 num_queue_element;
  __u32 submit_latency_us;
  __u32 device_latency_us;
  __u32 pad;
};
#define DRM_QAIC_MANAGE 0x00
#define DRM_QAIC_CREATE_BO 0x01
#define DRM_QAIC_MMAP_BO 0x02
#define DRM_QAIC_ATTACH_SLICE_BO 0x03
#define DRM_QAIC_EXECUTE_BO 0x04
#define DRM_QAIC_PARTIAL_EXECUTE_BO 0x05
#define DRM_QAIC_WAIT_BO 0x06
#define DRM_QAIC_PERF_STATS_BO 0x07
#define DRM_IOCTL_QAIC_MANAGE DRM_IOWR(DRM_COMMAND_BASE + DRM_QAIC_MANAGE, struct qaic_manage_msg)
#define DRM_IOCTL_QAIC_CREATE_BO DRM_IOWR(DRM_COMMAND_BASE + DRM_QAIC_CREATE_BO, struct qaic_create_bo)
#define DRM_IOCTL_QAIC_MMAP_BO DRM_IOWR(DRM_COMMAND_BASE + DRM_QAIC_MMAP_BO, struct qaic_mmap_bo)
#define DRM_IOCTL_QAIC_ATTACH_SLICE_BO DRM_IOW(DRM_COMMAND_BASE + DRM_QAIC_ATTACH_SLICE_BO, struct qaic_attach_slice)
#define DRM_IOCTL_QAIC_EXECUTE_BO DRM_IOW(DRM_COMMAND_BASE + DRM_QAIC_EXECUTE_BO, struct qaic_execute)
#define DRM_IOCTL_QAIC_PARTIAL_EXECUTE_BO DRM_IOW(DRM_COMMAND_BASE + DRM_QAIC_PARTIAL_EXECUTE_BO, struct qaic_execute)
#define DRM_IOCTL_QAIC_WAIT_BO DRM_IOW(DRM_COMMAND_BASE + DRM_QAIC_WAIT_BO, struct qaic_wait)
#define DRM_IOCTL_QAIC_PERF_STATS_BO DRM_IOWR(DRM_COMMAND_BASE + DRM_QAIC_PERF_STATS_BO, struct qaic_perf_stats)
#ifdef __cplusplus
}
#endif
#endif
