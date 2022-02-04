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
#ifndef _UAPI_AMT_H_
#define _UAPI_AMT_H_
enum ifla_amt_mode {
  AMT_MODE_GATEWAY = 0,
  AMT_MODE_RELAY,
  __AMT_MODE_MAX,
};
#define AMT_MODE_MAX (__AMT_MODE_MAX - 1)
enum {
  IFLA_AMT_UNSPEC,
  IFLA_AMT_MODE,
  IFLA_AMT_RELAY_PORT,
  IFLA_AMT_GATEWAY_PORT,
  IFLA_AMT_LINK,
  IFLA_AMT_LOCAL_IP,
  IFLA_AMT_REMOTE_IP,
  IFLA_AMT_DISCOVERY_IP,
  IFLA_AMT_MAX_TUNNELS,
  __IFLA_AMT_MAX,
};
#define IFLA_AMT_MAX (__IFLA_AMT_MAX - 1)
#endif
