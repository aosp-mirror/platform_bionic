/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
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
