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
#ifndef __LINUX_COMPLETION_H
#define __LINUX_COMPLETION_H
#include <linux/wait.h>
struct completion {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int done;
 wait_queue_head_t wait;
};
#define COMPLETION_INITIALIZER(work)   { 0, __WAIT_QUEUE_HEAD_INITIALIZER((work).wait) }
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define COMPLETION_INITIALIZER_ONSTACK(work)   ({ init_completion(&work); work; })
#define DECLARE_COMPLETION(work)   struct completion work = COMPLETION_INITIALIZER(work)
#define DECLARE_COMPLETION_ONSTACK(work) DECLARE_COMPLETION(work)
#define INIT_COMPLETION(x) ((x).done = 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
