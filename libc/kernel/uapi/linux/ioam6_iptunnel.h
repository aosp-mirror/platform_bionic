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
#ifndef _UAPI_LINUX_IOAM6_IPTUNNEL_H
#define _UAPI_LINUX_IOAM6_IPTUNNEL_H
enum {
  __IOAM6_IPTUNNEL_MODE_MIN,
  IOAM6_IPTUNNEL_MODE_INLINE,
  IOAM6_IPTUNNEL_MODE_ENCAP,
  IOAM6_IPTUNNEL_MODE_AUTO,
  __IOAM6_IPTUNNEL_MODE_MAX,
};
#define IOAM6_IPTUNNEL_MODE_MIN (__IOAM6_IPTUNNEL_MODE_MIN + 1)
#define IOAM6_IPTUNNEL_MODE_MAX (__IOAM6_IPTUNNEL_MODE_MAX - 1)
enum {
  IOAM6_IPTUNNEL_UNSPEC,
  IOAM6_IPTUNNEL_MODE,
  IOAM6_IPTUNNEL_DST,
  IOAM6_IPTUNNEL_TRACE,
#define IOAM6_IPTUNNEL_FREQ_MIN 1
#define IOAM6_IPTUNNEL_FREQ_MAX 1000000
  IOAM6_IPTUNNEL_FREQ_K,
  IOAM6_IPTUNNEL_FREQ_N,
  __IOAM6_IPTUNNEL_MAX,
};
#define IOAM6_IPTUNNEL_MAX (__IOAM6_IPTUNNEL_MAX - 1)
#endif
