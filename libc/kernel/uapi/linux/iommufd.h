/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_IOMMUFD_H
#define _UAPI_IOMMUFD_H
#include <linux/types.h>
#include <linux/ioctl.h>
#define IOMMUFD_TYPE (';')
enum {
  IOMMUFD_CMD_BASE = 0x80,
  IOMMUFD_CMD_DESTROY = IOMMUFD_CMD_BASE,
  IOMMUFD_CMD_IOAS_ALLOC,
  IOMMUFD_CMD_IOAS_ALLOW_IOVAS,
  IOMMUFD_CMD_IOAS_COPY,
  IOMMUFD_CMD_IOAS_IOVA_RANGES,
  IOMMUFD_CMD_IOAS_MAP,
  IOMMUFD_CMD_IOAS_UNMAP,
  IOMMUFD_CMD_OPTION,
  IOMMUFD_CMD_VFIO_IOAS,
  IOMMUFD_CMD_HWPT_ALLOC,
  IOMMUFD_CMD_GET_HW_INFO,
  IOMMUFD_CMD_HWPT_SET_DIRTY_TRACKING,
  IOMMUFD_CMD_HWPT_GET_DIRTY_BITMAP,
};
struct iommu_destroy {
  __u32 size;
  __u32 id;
};
#define IOMMU_DESTROY _IO(IOMMUFD_TYPE, IOMMUFD_CMD_DESTROY)
struct iommu_ioas_alloc {
  __u32 size;
  __u32 flags;
  __u32 out_ioas_id;
};
#define IOMMU_IOAS_ALLOC _IO(IOMMUFD_TYPE, IOMMUFD_CMD_IOAS_ALLOC)
struct iommu_iova_range {
  __aligned_u64 start;
  __aligned_u64 last;
};
struct iommu_ioas_iova_ranges {
  __u32 size;
  __u32 ioas_id;
  __u32 num_iovas;
  __u32 __reserved;
  __aligned_u64 allowed_iovas;
  __aligned_u64 out_iova_alignment;
};
#define IOMMU_IOAS_IOVA_RANGES _IO(IOMMUFD_TYPE, IOMMUFD_CMD_IOAS_IOVA_RANGES)
struct iommu_ioas_allow_iovas {
  __u32 size;
  __u32 ioas_id;
  __u32 num_iovas;
  __u32 __reserved;
  __aligned_u64 allowed_iovas;
};
#define IOMMU_IOAS_ALLOW_IOVAS _IO(IOMMUFD_TYPE, IOMMUFD_CMD_IOAS_ALLOW_IOVAS)
enum iommufd_ioas_map_flags {
  IOMMU_IOAS_MAP_FIXED_IOVA = 1 << 0,
  IOMMU_IOAS_MAP_WRITEABLE = 1 << 1,
  IOMMU_IOAS_MAP_READABLE = 1 << 2,
};
struct iommu_ioas_map {
  __u32 size;
  __u32 flags;
  __u32 ioas_id;
  __u32 __reserved;
  __aligned_u64 user_va;
  __aligned_u64 length;
  __aligned_u64 iova;
};
#define IOMMU_IOAS_MAP _IO(IOMMUFD_TYPE, IOMMUFD_CMD_IOAS_MAP)
struct iommu_ioas_copy {
  __u32 size;
  __u32 flags;
  __u32 dst_ioas_id;
  __u32 src_ioas_id;
  __aligned_u64 length;
  __aligned_u64 dst_iova;
  __aligned_u64 src_iova;
};
#define IOMMU_IOAS_COPY _IO(IOMMUFD_TYPE, IOMMUFD_CMD_IOAS_COPY)
struct iommu_ioas_unmap {
  __u32 size;
  __u32 ioas_id;
  __aligned_u64 iova;
  __aligned_u64 length;
};
#define IOMMU_IOAS_UNMAP _IO(IOMMUFD_TYPE, IOMMUFD_CMD_IOAS_UNMAP)
enum iommufd_option {
  IOMMU_OPTION_RLIMIT_MODE = 0,
  IOMMU_OPTION_HUGE_PAGES = 1,
};
enum iommufd_option_ops {
  IOMMU_OPTION_OP_SET = 0,
  IOMMU_OPTION_OP_GET = 1,
};
struct iommu_option {
  __u32 size;
  __u32 option_id;
  __u16 op;
  __u16 __reserved;
  __u32 object_id;
  __aligned_u64 val64;
};
#define IOMMU_OPTION _IO(IOMMUFD_TYPE, IOMMUFD_CMD_OPTION)
enum iommufd_vfio_ioas_op {
  IOMMU_VFIO_IOAS_GET = 0,
  IOMMU_VFIO_IOAS_SET = 1,
  IOMMU_VFIO_IOAS_CLEAR = 2,
};
struct iommu_vfio_ioas {
  __u32 size;
  __u32 ioas_id;
  __u16 op;
  __u16 __reserved;
};
#define IOMMU_VFIO_IOAS _IO(IOMMUFD_TYPE, IOMMUFD_CMD_VFIO_IOAS)
enum iommufd_hwpt_alloc_flags {
  IOMMU_HWPT_ALLOC_NEST_PARENT = 1 << 0,
  IOMMU_HWPT_ALLOC_DIRTY_TRACKING = 1 << 1,
};
enum iommu_hwpt_vtd_s1_flags {
  IOMMU_VTD_S1_SRE = 1 << 0,
  IOMMU_VTD_S1_EAFE = 1 << 1,
  IOMMU_VTD_S1_WPE = 1 << 2,
};
struct iommu_hwpt_vtd_s1 {
  __aligned_u64 flags;
  __aligned_u64 pgtbl_addr;
  __u32 addr_width;
  __u32 __reserved;
};
enum iommu_hwpt_data_type {
  IOMMU_HWPT_DATA_NONE,
  IOMMU_HWPT_DATA_VTD_S1,
};
struct iommu_hwpt_alloc {
  __u32 size;
  __u32 flags;
  __u32 dev_id;
  __u32 pt_id;
  __u32 out_hwpt_id;
  __u32 __reserved;
  __u32 data_type;
  __u32 data_len;
  __aligned_u64 data_uptr;
};
#define IOMMU_HWPT_ALLOC _IO(IOMMUFD_TYPE, IOMMUFD_CMD_HWPT_ALLOC)
enum iommu_hw_info_vtd_flags {
  IOMMU_HW_INFO_VTD_ERRATA_772415_SPR17 = 1 << 0,
};
struct iommu_hw_info_vtd {
  __u32 flags;
  __u32 __reserved;
  __aligned_u64 cap_reg;
  __aligned_u64 ecap_reg;
};
enum iommu_hw_info_type {
  IOMMU_HW_INFO_TYPE_NONE,
  IOMMU_HW_INFO_TYPE_INTEL_VTD,
};
enum iommufd_hw_capabilities {
  IOMMU_HW_CAP_DIRTY_TRACKING = 1 << 0,
};
struct iommu_hw_info {
  __u32 size;
  __u32 flags;
  __u32 dev_id;
  __u32 data_len;
  __aligned_u64 data_uptr;
  __u32 out_data_type;
  __u32 __reserved;
  __aligned_u64 out_capabilities;
};
#define IOMMU_GET_HW_INFO _IO(IOMMUFD_TYPE, IOMMUFD_CMD_GET_HW_INFO)
enum iommufd_hwpt_set_dirty_tracking_flags {
  IOMMU_HWPT_DIRTY_TRACKING_ENABLE = 1,
};
struct iommu_hwpt_set_dirty_tracking {
  __u32 size;
  __u32 flags;
  __u32 hwpt_id;
  __u32 __reserved;
};
#define IOMMU_HWPT_SET_DIRTY_TRACKING _IO(IOMMUFD_TYPE, IOMMUFD_CMD_HWPT_SET_DIRTY_TRACKING)
enum iommufd_hwpt_get_dirty_bitmap_flags {
  IOMMU_HWPT_GET_DIRTY_BITMAP_NO_CLEAR = 1,
};
struct iommu_hwpt_get_dirty_bitmap {
  __u32 size;
  __u32 hwpt_id;
  __u32 flags;
  __u32 __reserved;
  __aligned_u64 iova;
  __aligned_u64 length;
  __aligned_u64 page_size;
  __aligned_u64 data;
};
#define IOMMU_HWPT_GET_DIRTY_BITMAP _IO(IOMMUFD_TYPE, IOMMUFD_CMD_HWPT_GET_DIRTY_BITMAP)
#endif
