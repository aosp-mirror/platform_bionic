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
#ifndef __MTD_FLASHCHIP_H__
#define __MTD_FLASHCHIP_H__
#include <linux/sched.h>
typedef enum {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FL_READY,
 FL_STATUS,
 FL_CFI_QUERY,
 FL_JEDEC_QUERY,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FL_ERASING,
 FL_ERASE_SUSPENDING,
 FL_ERASE_SUSPENDED,
 FL_WRITING,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FL_WRITING_TO_BUFFER,
 FL_OTP_WRITE,
 FL_WRITE_SUSPENDING,
 FL_WRITE_SUSPENDED,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FL_PM_SUSPENDED,
 FL_SYNCING,
 FL_UNLOADING,
 FL_LOCKING,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FL_UNLOCKING,
 FL_POINT,
 FL_XIP_WHILE_ERASING,
 FL_XIP_WHILE_WRITING,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FL_UNKNOWN
} flstate_t;
struct flchip {
 unsigned long start;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int ref_point_counter;
 flstate_t state;
 flstate_t oldstate;
 unsigned int write_suspended:1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int erase_suspended:1;
 unsigned long in_progress_block_addr;
 spinlock_t *mutex;
 spinlock_t _spinlock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 wait_queue_head_t wq;
 int word_write_time;
 int buffer_write_time;
 int erase_time;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void *priv;
};
struct flchip_shared {
 spinlock_t lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct flchip *writing;
 struct flchip *erasing;
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
