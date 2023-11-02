/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_BRIDGE_EBT_MARK_T_H
#define __LINUX_BRIDGE_EBT_MARK_T_H
#define MARK_SET_VALUE (0xfffffff0)
#define MARK_OR_VALUE (0xffffffe0)
#define MARK_AND_VALUE (0xffffffd0)
#define MARK_XOR_VALUE (0xffffffc0)
struct ebt_mark_t_info {
  unsigned long mark;
  int target;
};
#define EBT_MARK_TARGET "mark"
#endif
