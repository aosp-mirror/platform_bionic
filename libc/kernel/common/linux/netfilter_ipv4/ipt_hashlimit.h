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
#ifndef _IPT_HASHLIMIT_H
#define _IPT_HASHLIMIT_H
#define IPT_HASHLIMIT_SCALE 10000
struct ipt_hashlimit_htable;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPT_HASHLIMIT_HASH_DIP 0x0001
#define IPT_HASHLIMIT_HASH_DPT 0x0002
#define IPT_HASHLIMIT_HASH_SIP 0x0004
#define IPT_HASHLIMIT_HASH_SPT 0x0008
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct hashlimit_cfg {
 u_int32_t mode;
 u_int32_t avg;
 u_int32_t burst;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u_int32_t size;
 u_int32_t max;
 u_int32_t gc_interval;
 u_int32_t expire;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct ipt_hashlimit_info {
 char name [IFNAMSIZ];
 struct hashlimit_cfg cfg;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct ipt_hashlimit_htable *hinfo;
 union {
 void *ptr;
 struct ipt_hashlimit_info *master;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } u;
};
#endif
