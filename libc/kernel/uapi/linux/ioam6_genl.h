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
#ifndef _UAPI_LINUX_IOAM6_GENL_H
#define _UAPI_LINUX_IOAM6_GENL_H
#define IOAM6_GENL_NAME "IOAM6"
#define IOAM6_GENL_VERSION 0x1
enum {
  IOAM6_ATTR_UNSPEC,
  IOAM6_ATTR_NS_ID,
  IOAM6_ATTR_NS_DATA,
  IOAM6_ATTR_NS_DATA_WIDE,
#define IOAM6_MAX_SCHEMA_DATA_LEN (255 * 4)
  IOAM6_ATTR_SC_ID,
  IOAM6_ATTR_SC_DATA,
  IOAM6_ATTR_SC_NONE,
  IOAM6_ATTR_PAD,
  __IOAM6_ATTR_MAX,
};
#define IOAM6_ATTR_MAX (__IOAM6_ATTR_MAX - 1)
enum {
  IOAM6_CMD_UNSPEC,
  IOAM6_CMD_ADD_NAMESPACE,
  IOAM6_CMD_DEL_NAMESPACE,
  IOAM6_CMD_DUMP_NAMESPACES,
  IOAM6_CMD_ADD_SCHEMA,
  IOAM6_CMD_DEL_SCHEMA,
  IOAM6_CMD_DUMP_SCHEMAS,
  IOAM6_CMD_NS_SET_SCHEMA,
  __IOAM6_CMD_MAX,
};
#define IOAM6_CMD_MAX (__IOAM6_CMD_MAX - 1)
#endif
