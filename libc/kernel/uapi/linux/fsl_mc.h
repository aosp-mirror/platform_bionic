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
#ifndef _UAPI_FSL_MC_H_
#define _UAPI_FSL_MC_H_
#include <linux/types.h>
#define MC_CMD_NUM_OF_PARAMS 7
struct fsl_mc_command {
  __le64 header;
  __le64 params[MC_CMD_NUM_OF_PARAMS];
};
#define FSL_MC_SEND_CMD_IOCTL_TYPE 'R'
#define FSL_MC_SEND_CMD_IOCTL_SEQ 0xE0
#define FSL_MC_SEND_MC_COMMAND _IOWR(FSL_MC_SEND_CMD_IOCTL_TYPE, FSL_MC_SEND_CMD_IOCTL_SEQ, struct fsl_mc_command)
#endif
