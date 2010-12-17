/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __OV5650_H__
#define __OV5650_H__

#include <linux/ioctl.h>  

#define OV5650_IOCTL_SET_MODE _IOW('o', 1, struct ov5650_mode)
#define OV5650_IOCTL_SET_FRAME_LENGTH _IOW('o', 2, __u32)
#define OV5650_IOCTL_SET_COARSE_TIME _IOW('o', 3, __u32)
#define OV5650_IOCTL_SET_GAIN _IOW('o', 4, __u16)
#define OV5650_IOCTL_GET_STATUS _IOR('o', 5, __u8)
#define OV5650_IOCTL_GET_OTP _IOR('o', 6, struct ov5650_otp_data)
#define OV5650_IOCTL_TEST_PATTERN _IOW('o', 7, enum ov5650_test_pattern)

enum ov5650_test_pattern {
 TEST_PATTERN_NONE,
 TEST_PATTERN_COLORBARS,
 TEST_PATTERN_CHECKERBOARD
};

struct ov5650_otp_data {

 __u8 sensor_serial_num[6];
 __u8 part_num[8];
 __u8 lens_id[1];
 __u8 manufacture_id[2];
 __u8 factory_id[2];
 __u8 manufacture_date[9];
 __u8 manufacture_line[2];

 __u32 module_serial_num;
 __u8 focuser_liftoff[2];
 __u8 focuser_macro[2];
 __u8 reserved1[12];
 __u8 shutter_cal[16];
 __u8 reserved2[183];

 __u16 crc;
 __u8 reserved3[3];
 __u8 auto_load[2];
} __attribute__ ((packed));

struct ov5650_mode {
 int xres;
 int yres;
 __u32 frame_length;
 __u32 coarse_time;
 __u16 gain;
};

#endif


