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
#ifndef __INCLUDE_UAPI_SOUND_SOF_USER_HEADER_H__
#define __INCLUDE_UAPI_SOUND_SOF_USER_HEADER_H__
struct sof_abi_hdr {
  uint32_t magic;
  uint32_t type;
  uint32_t size;
  uint32_t abi;
  uint32_t reserved[4];
  uint32_t data[0];
} __packed;
#endif
