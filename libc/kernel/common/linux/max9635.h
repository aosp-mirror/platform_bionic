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
#ifndef _LINUX_MAX9635_H__
#define _LINUX_MAX9635_H__

#define MAX9635_NAME "MAX9635_als"
#define FOPS_MAX9635_NAME "MAX9635"

#define MAX9635_IO 0xA3

#define MAX9635_IOCTL_GET_ENABLE _IOR(MAX9635_IO, 0x00, char)
#define MAX9635_IOCTL_SET_ENABLE _IOW(MAX9635_IO, 0x01, char)

#endif
