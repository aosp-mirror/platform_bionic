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
#ifndef __LINUX_BLK_TYPES_H
#define __LINUX_BLK_TYPES_H
enum rq_flag_bits {
 __REQ_WRITE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_FAILFAST_DEV,
 __REQ_FAILFAST_TRANSPORT,
 __REQ_FAILFAST_DRIVER,
 __REQ_SYNC,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_META,
 __REQ_DISCARD,
 __REQ_NOIDLE,
 __REQ_RAHEAD,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_THROTTLED,
 __REQ_SORTED,
 __REQ_SOFTBARRIER,
 __REQ_FUA,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_NOMERGE,
 __REQ_STARTED,
 __REQ_DONTPREP,
 __REQ_QUEUED,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_ELVPRIV,
 __REQ_FAILED,
 __REQ_QUIET,
 __REQ_PREEMPT,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_ALLOCED,
 __REQ_COPY_USER,
 __REQ_FLUSH,
 __REQ_FLUSH_SEQ,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __REQ_IO_STAT,
 __REQ_MIXED_MERGE,
 __REQ_SECURE,
 __REQ_NR_BITS,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define REQ_WRITE (1 << __REQ_WRITE)
#define REQ_FAILFAST_DEV (1 << __REQ_FAILFAST_DEV)
#define REQ_FAILFAST_TRANSPORT (1 << __REQ_FAILFAST_TRANSPORT)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_FAILFAST_DRIVER (1 << __REQ_FAILFAST_DRIVER)
#define REQ_SYNC (1 << __REQ_SYNC)
#define REQ_META (1 << __REQ_META)
#define REQ_DISCARD (1 << __REQ_DISCARD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_NOIDLE (1 << __REQ_NOIDLE)
#define REQ_FAILFAST_MASK   (REQ_FAILFAST_DEV | REQ_FAILFAST_TRANSPORT | REQ_FAILFAST_DRIVER)
#define REQ_COMMON_MASK   (REQ_WRITE | REQ_FAILFAST_MASK | REQ_SYNC | REQ_META | REQ_DISCARD |   REQ_NOIDLE | REQ_FLUSH | REQ_FUA | REQ_SECURE)
#define REQ_CLONE_MASK REQ_COMMON_MASK
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_RAHEAD (1 << __REQ_RAHEAD)
#define REQ_THROTTLED (1 << __REQ_THROTTLED)
#define REQ_SORTED (1 << __REQ_SORTED)
#define REQ_SOFTBARRIER (1 << __REQ_SOFTBARRIER)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_FUA (1 << __REQ_FUA)
#define REQ_NOMERGE (1 << __REQ_NOMERGE)
#define REQ_STARTED (1 << __REQ_STARTED)
#define REQ_DONTPREP (1 << __REQ_DONTPREP)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_QUEUED (1 << __REQ_QUEUED)
#define REQ_ELVPRIV (1 << __REQ_ELVPRIV)
#define REQ_FAILED (1 << __REQ_FAILED)
#define REQ_QUIET (1 << __REQ_QUIET)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_PREEMPT (1 << __REQ_PREEMPT)
#define REQ_ALLOCED (1 << __REQ_ALLOCED)
#define REQ_COPY_USER (1 << __REQ_COPY_USER)
#define REQ_FLUSH (1 << __REQ_FLUSH)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define REQ_FLUSH_SEQ (1 << __REQ_FLUSH_SEQ)
#define REQ_IO_STAT (1 << __REQ_IO_STAT)
#define REQ_MIXED_MERGE (1 << __REQ_MIXED_MERGE)
#define REQ_SECURE (1 << __REQ_SECURE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
