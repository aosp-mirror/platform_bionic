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
#ifndef _MSM_RMNET_H_
#define _MSM_RMNET_H_
#define RMNET_MODE_NONE (0x00)
#define RMNET_MODE_LLP_ETH (0x01)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RMNET_MODE_LLP_IP (0x02)
#define RMNET_MODE_QOS (0x04)
#define RMNET_MODE_MASK (RMNET_MODE_LLP_ETH |   RMNET_MODE_LLP_IP |   RMNET_MODE_QOS)
#define RMNET_IS_MODE_QOS(mode)   ((mode & RMNET_MODE_QOS) == RMNET_MODE_QOS)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RMNET_IS_MODE_IP(mode)   ((mode & RMNET_MODE_LLP_IP) == RMNET_MODE_LLP_IP)
enum rmnet_ioctl_cmds_e {
 RMNET_IOCTL_SET_LLP_ETHERNET = 0x000089F1,
 RMNET_IOCTL_SET_LLP_IP = 0x000089F2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 RMNET_IOCTL_GET_LLP = 0x000089F3,
 RMNET_IOCTL_SET_QOS_ENABLE = 0x000089F4,
 RMNET_IOCTL_SET_QOS_DISABLE = 0x000089F5,
 RMNET_IOCTL_GET_QOS = 0x000089F6,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 RMNET_IOCTL_GET_OPMODE = 0x000089F7,
 RMNET_IOCTL_OPEN = 0x000089F8,
 RMNET_IOCTL_CLOSE = 0x000089F9,
 RMNET_IOCTL_MAX
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define QMI_QOS_HDR_S __attribute((__packed__)) qmi_qos_hdr_s
struct QMI_QOS_HDR_S {
 unsigned char version;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char flags;
 unsigned long flow_id;
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */

