/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_NSFS_H
#define __LINUX_NSFS_H
#include <linux/ioctl.h>
#define NSIO 0xb7
#define NS_GET_USERNS _IO(NSIO, 0x1)
#define NS_GET_PARENT _IO(NSIO, 0x2)
#define NS_GET_NSTYPE _IO(NSIO, 0x3)
#define NS_GET_OWNER_UID _IO(NSIO, 0x4)
#endif
