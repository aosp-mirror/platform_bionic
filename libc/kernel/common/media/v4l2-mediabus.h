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
#ifndef V4L2_MEDIABUS_H
#define V4L2_MEDIABUS_H
#include <linux/v4l2-mediabus.h>
#define V4L2_MBUS_MASTER (1 << 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_MBUS_SLAVE (1 << 1)
#define V4L2_MBUS_HSYNC_ACTIVE_HIGH (1 << 2)
#define V4L2_MBUS_HSYNC_ACTIVE_LOW (1 << 3)
#define V4L2_MBUS_VSYNC_ACTIVE_HIGH (1 << 4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_MBUS_VSYNC_ACTIVE_LOW (1 << 5)
#define V4L2_MBUS_PCLK_SAMPLE_RISING (1 << 6)
#define V4L2_MBUS_PCLK_SAMPLE_FALLING (1 << 7)
#define V4L2_MBUS_DATA_ACTIVE_HIGH (1 << 8)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_MBUS_DATA_ACTIVE_LOW (1 << 9)
#define V4L2_MBUS_FIELD_EVEN_HIGH (1 << 10)
#define V4L2_MBUS_FIELD_EVEN_LOW (1 << 11)
#define V4L2_MBUS_CSI2_1_LANE (1 << 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_MBUS_CSI2_2_LANE (1 << 1)
#define V4L2_MBUS_CSI2_3_LANE (1 << 2)
#define V4L2_MBUS_CSI2_4_LANE (1 << 3)
#define V4L2_MBUS_CSI2_CHANNEL_0 (1 << 4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_MBUS_CSI2_CHANNEL_1 (1 << 5)
#define V4L2_MBUS_CSI2_CHANNEL_2 (1 << 6)
#define V4L2_MBUS_CSI2_CHANNEL_3 (1 << 7)
#define V4L2_MBUS_CSI2_CONTINUOUS_CLOCK (1 << 8)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define V4L2_MBUS_CSI2_NONCONTINUOUS_CLOCK (1 << 9)
#define V4L2_MBUS_CSI2_LANES (V4L2_MBUS_CSI2_1_LANE | V4L2_MBUS_CSI2_2_LANE |   V4L2_MBUS_CSI2_3_LANE | V4L2_MBUS_CSI2_4_LANE)
#define V4L2_MBUS_CSI2_CHANNELS (V4L2_MBUS_CSI2_CHANNEL_0 | V4L2_MBUS_CSI2_CHANNEL_1 |   V4L2_MBUS_CSI2_CHANNEL_2 | V4L2_MBUS_CSI2_CHANNEL_3)
enum v4l2_mbus_type {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 V4L2_MBUS_PARALLEL,
 V4L2_MBUS_BT656,
 V4L2_MBUS_CSI2,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct v4l2_mbus_config {
 enum v4l2_mbus_type type;
 unsigned int flags;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
