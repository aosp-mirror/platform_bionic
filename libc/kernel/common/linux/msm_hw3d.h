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
#ifndef _MSM_HW3D_H_
#define _MSM_HW3D_H_
#include <linux/fs.h>
#include <linux/ioctl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct hw3d_region;
#define HW3D_IOCTL_MAGIC 'h'
#define HW3D_WAIT_FOR_REVOKE _IO(HW3D_IOCTL_MAGIC, 0x80)
#define HW3D_WAIT_FOR_INTERRUPT _IO(HW3D_IOCTL_MAGIC, 0x81)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HW3D_GET_REGIONS   _IOR(HW3D_IOCTL_MAGIC, 0x82, struct hw3d_region *)
#define HW3D_REGION_OFFSET(id) ((((uint32_t)(id)) & 0xf) << 28)
#define HW3D_REGION_ID(addr) (((uint32_t)(addr) >> 28) & 0xf)
#define HW3D_OFFSET_IN_REGION(addr) ((uint32_t)(addr) & ~(0xfUL << 28))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 HW3D_EBI = 0,
 HW3D_SMI = 1,
 HW3D_REGS = 2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 HW3D_NUM_REGIONS = HW3D_REGS + 1,
};
struct hw3d_region {
 unsigned long phys;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long map_offset;
 unsigned long len;
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
