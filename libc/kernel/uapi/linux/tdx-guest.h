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
#ifndef _UAPI_LINUX_TDX_GUEST_H_
#define _UAPI_LINUX_TDX_GUEST_H_
#include <linux/ioctl.h>
#include <linux/types.h>
#define TDX_REPORTDATA_LEN 64
#define TDX_REPORT_LEN 1024
struct tdx_report_req {
  __u8 reportdata[TDX_REPORTDATA_LEN];
  __u8 tdreport[TDX_REPORT_LEN];
};
#define TDX_CMD_GET_REPORT0 _IOWR('T', 1, struct tdx_report_req)
#endif
