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
#ifndef _UAPI__CODA_PSDEV_H
#define _UAPI__CODA_PSDEV_H
#include <linux/magic.h>
#define CODA_PSDEV_MAJOR 67
#define MAX_CODADEVS 5
#define CODA_REQ_ASYNC 0x1
#define CODA_REQ_READ 0x2
#define CODA_REQ_WRITE 0x4
#define CODA_REQ_ABORT 0x8
#endif
