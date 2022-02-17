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
#ifndef _UAPI_LINUX_IOPRIO_H
#define _UAPI_LINUX_IOPRIO_H
#define IOPRIO_CLASS_SHIFT 13
#define IOPRIO_CLASS_MASK 0x07
#define IOPRIO_PRIO_MASK ((1UL << IOPRIO_CLASS_SHIFT) - 1)
#define IOPRIO_PRIO_CLASS(ioprio) (((ioprio) >> IOPRIO_CLASS_SHIFT) & IOPRIO_CLASS_MASK)
#define IOPRIO_PRIO_DATA(ioprio) ((ioprio) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class,data) ((((class) & IOPRIO_CLASS_MASK) << IOPRIO_CLASS_SHIFT) | ((data) & IOPRIO_PRIO_MASK))
enum {
  IOPRIO_CLASS_NONE,
  IOPRIO_CLASS_RT,
  IOPRIO_CLASS_BE,
  IOPRIO_CLASS_IDLE,
};
#define IOPRIO_NR_LEVELS 8
#define IOPRIO_BE_NR IOPRIO_NR_LEVELS
enum {
  IOPRIO_WHO_PROCESS = 1,
  IOPRIO_WHO_PGRP,
  IOPRIO_WHO_USER,
};
#define IOPRIO_NORM 4
#define IOPRIO_BE_NORM IOPRIO_NORM
#endif
