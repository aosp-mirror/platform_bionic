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
#endif
