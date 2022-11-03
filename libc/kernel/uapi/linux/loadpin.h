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
#ifndef _UAPI_LINUX_LOOP_LOADPIN_H
#define _UAPI_LINUX_LOOP_LOADPIN_H
#define LOADPIN_IOC_MAGIC 'L'
#define LOADPIN_IOC_SET_TRUSTED_VERITY_DIGESTS _IOW(LOADPIN_IOC_MAGIC, 0x00, unsigned int)
#endif
