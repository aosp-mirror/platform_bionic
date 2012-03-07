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
#ifndef LINUX_NFSD_STATS_H
#define LINUX_NFSD_STATS_H
#include <linux/nfs4.h>
struct nfsd_stats {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int rchits;
 unsigned int rcmisses;
 unsigned int rcnocache;
 unsigned int fh_stale;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int fh_lookup;
 unsigned int fh_anon;
 unsigned int fh_nocache_dir;
 unsigned int fh_nocache_nondir;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int io_read;
 unsigned int io_write;
 unsigned int th_cnt;
 unsigned int th_usage[10];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int th_fullcnt;
 unsigned int ra_size;
 unsigned int ra_depth[11];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NFSD_USAGE_WRAP (HZ*1000000)
#endif
