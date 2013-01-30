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
#ifndef AKM8975_H
#define AKM8975_H
#include <linux/ioctl.h>
#define AK8975_MODE_SNG_MEASURE 0x01
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AK8975_MODE_SELF_TEST 0x08
#define AK8975_MODE_FUSE_ACCESS 0x0F
#define AK8975_MODE_POWER_DOWN 0x00
#define RBUFF_SIZE 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AK8975_REG_WIA 0x00
#define AK8975_REG_INFO 0x01
#define AK8975_REG_ST1 0x02
#define AK8975_REG_HXL 0x03
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AK8975_REG_HXH 0x04
#define AK8975_REG_HYL 0x05
#define AK8975_REG_HYH 0x06
#define AK8975_REG_HZL 0x07
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AK8975_REG_HZH 0x08
#define AK8975_REG_ST2 0x09
#define AK8975_REG_CNTL 0x0A
#define AK8975_REG_RSV 0x0B
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AK8975_REG_ASTC 0x0C
#define AK8975_REG_TS1 0x0D
#define AK8975_REG_TS2 0x0E
#define AK8975_REG_I2CDIS 0x0F
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AK8975_FUSE_ASAX 0x10
#define AK8975_FUSE_ASAY 0x11
#define AK8975_FUSE_ASAZ 0x12
#define AKMIO 0xA1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ECS_IOCTL_WRITE _IOW(AKMIO, 0x02, char[5])
#define ECS_IOCTL_READ _IOWR(AKMIO, 0x03, char[5])
#define ECS_IOCTL_GETDATA _IOR(AKMIO, 0x08, char[RBUFF_SIZE])
#define ECS_IOCTL_SET_YPR _IOW(AKMIO, 0x0C, short[12])
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ECS_IOCTL_GET_OPEN_STATUS _IOR(AKMIO, 0x0D, int)
#define ECS_IOCTL_GET_CLOSE_STATUS _IOR(AKMIO, 0x0E, int)
#define ECS_IOCTL_GET_DELAY _IOR(AKMIO, 0x30, short)
#define ECS_IOCTL_APP_SET_MFLAG _IOW(AKMIO, 0x11, short)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ECS_IOCTL_APP_GET_MFLAG _IOW(AKMIO, 0x12, short)
#define ECS_IOCTL_APP_SET_AFLAG _IOW(AKMIO, 0x13, short)
#define ECS_IOCTL_APP_GET_AFLAG _IOR(AKMIO, 0x14, short)
#define ECS_IOCTL_APP_SET_DELAY _IOW(AKMIO, 0x18, short)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ECS_IOCTL_APP_GET_DELAY ECS_IOCTL_GET_DELAY
#define ECS_IOCTL_APP_SET_MVFLAG _IOW(AKMIO, 0x19, short)
#define ECS_IOCTL_APP_GET_MVFLAG _IOR(AKMIO, 0x1A, short)
#define ECS_IOCTL_APP_SET_TFLAG _IOR(AKMIO, 0x15, short)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ECS_INTR 140
struct akm8975_platform_data {
 int intr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
