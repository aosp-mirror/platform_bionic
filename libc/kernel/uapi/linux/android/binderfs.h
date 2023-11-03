/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_BINDERFS_H
#define _UAPI_LINUX_BINDERFS_H
#include <linux/android/binder.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#define BINDERFS_MAX_NAME 255
struct binderfs_device {
  char name[BINDERFS_MAX_NAME + 1];
  __u32 major;
  __u32 minor;
};
#define BINDER_CTL_ADD _IOWR('b', 1, struct binderfs_device)
#endif
