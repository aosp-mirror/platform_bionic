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
#ifndef _IPT_RECENT_H
#define _IPT_RECENT_H
#define RECENT_NAME "ipt_recent"
#define RECENT_VER "v0.3.1"
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPT_RECENT_CHECK 1
#define IPT_RECENT_SET 2
#define IPT_RECENT_UPDATE 4
#define IPT_RECENT_REMOVE 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPT_RECENT_TTL 16
#define IPT_RECENT_SOURCE 0
#define IPT_RECENT_DEST 1
#define IPT_RECENT_NAME_LEN 200
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct ipt_recent_info {
 u_int32_t seconds;
 u_int32_t hit_count;
 u_int8_t check_set;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u_int8_t invert;
 char name[IPT_RECENT_NAME_LEN];
 u_int8_t side;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
