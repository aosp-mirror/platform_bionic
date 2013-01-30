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
#ifndef LINUX_LOCKD_NLM_H
#define LINUX_LOCKD_NLM_H
#define NLM_OFFSET_MAX ((s32) 0x7fffffff)
#define NLM4_OFFSET_MAX ((s64) ((~(u64)0) >> 1))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum {
 NLM_LCK_GRANTED = 0,
 NLM_LCK_DENIED = 1,
 NLM_LCK_DENIED_NOLOCKS = 2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 NLM_LCK_BLOCKED = 3,
 NLM_LCK_DENIED_GRACE_PERIOD = 4,
};
#define NLM_PROGRAM 100021
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NLMPROC_NULL 0
#define NLMPROC_TEST 1
#define NLMPROC_LOCK 2
#define NLMPROC_CANCEL 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NLMPROC_UNLOCK 4
#define NLMPROC_GRANTED 5
#define NLMPROC_TEST_MSG 6
#define NLMPROC_LOCK_MSG 7
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NLMPROC_CANCEL_MSG 8
#define NLMPROC_UNLOCK_MSG 9
#define NLMPROC_GRANTED_MSG 10
#define NLMPROC_TEST_RES 11
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NLMPROC_LOCK_RES 12
#define NLMPROC_CANCEL_RES 13
#define NLMPROC_UNLOCK_RES 14
#define NLMPROC_GRANTED_RES 15
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NLMPROC_NSM_NOTIFY 16
#define NLMPROC_SHARE 20
#define NLMPROC_UNSHARE 21
#define NLMPROC_NM_LOCK 22
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NLMPROC_FREE_ALL 23
#endif
