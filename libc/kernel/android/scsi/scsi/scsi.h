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
#ifndef _SCSI_SCSI_H
#define _SCSI_SCSI_H
#include <linux/types.h>
#include <scsi/scsi_proto.h>
#include <scsi/scsi_status.h>
struct ccs_modesel_head {
  __u8 _r1;
  __u8 medium;
  __u8 _r2;
  __u8 block_desc_length;
  __u8 density;
  __u8 number_blocks_hi;
  __u8 number_blocks_med;
  __u8 number_blocks_lo;
  __u8 _r3;
  __u8 block_length_hi;
  __u8 block_length_med;
  __u8 block_length_lo;
};
#define EXTENDED_MODIFY_DATA_POINTER 0x00
#define EXTENDED_SDTR 0x01
#define EXTENDED_EXTENDED_IDENTIFY 0x02
#define EXTENDED_WDTR 0x03
#define EXTENDED_PPR 0x04
#define EXTENDED_MODIFY_BIDI_DATA_PTR 0x05
#define SCSI_IOCTL_GET_IDLUN 0x5382
#define SCSI_IOCTL_PROBE_HOST 0x5385
#define SCSI_IOCTL_GET_BUS_NUMBER 0x5386
#define SCSI_IOCTL_GET_PCI 0x5387
#endif
