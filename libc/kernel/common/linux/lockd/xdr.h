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
#ifndef LOCKD_XDR_H
#define LOCKD_XDR_H
#include <linux/fs.h>
#include <linux/nfs.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/sunrpc/xdr.h>
#define NLM_MAXCOOKIELEN 32
#define NLM_MAXSTRLEN 1024
#define nlm_granted __constant_htonl(NLM_LCK_GRANTED)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define nlm_lck_denied __constant_htonl(NLM_LCK_DENIED)
#define nlm_lck_denied_nolocks __constant_htonl(NLM_LCK_DENIED_NOLOCKS)
#define nlm_lck_blocked __constant_htonl(NLM_LCK_BLOCKED)
#define nlm_lck_denied_grace_period __constant_htonl(NLM_LCK_DENIED_GRACE_PERIOD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nlm_lock {
 char * caller;
 int len;
 struct nfs_fh fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct xdr_netobj oh;
 u32 svid;
 struct file_lock fl;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nlm_cookie
{
 unsigned char data[NLM_MAXCOOKIELEN];
 unsigned int len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nlm_args {
 struct nlm_cookie cookie;
 struct nlm_lock lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 block;
 u32 reclaim;
 u32 state;
 u32 monitor;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 fsm_access;
 u32 fsm_mode;
};
typedef struct nlm_args nlm_args;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nlm_res {
 struct nlm_cookie cookie;
 u32 status;
 struct nlm_lock lock;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nlm_reboot {
 char * mon;
 int len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 state;
 u32 addr;
 u32 vers;
 u32 proto;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define NLMSVC_XDRSIZE sizeof(struct nlm_args)
#endif
