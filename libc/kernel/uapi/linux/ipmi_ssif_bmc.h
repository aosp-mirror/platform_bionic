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
#ifndef _UAPI_LINUX_IPMI_SSIF_BMC_H
#define _UAPI_LINUX_IPMI_SSIF_BMC_H
#include <linux/types.h>
#define IPMI_SSIF_PAYLOAD_MAX 254
struct ipmi_ssif_msg {
  unsigned int len;
  __u8 payload[IPMI_SSIF_PAYLOAD_MAX];
};
#endif
