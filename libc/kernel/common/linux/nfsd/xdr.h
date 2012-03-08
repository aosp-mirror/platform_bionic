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
#ifndef LINUX_NFSD_H
#define LINUX_NFSD_H
#include <linux/fs.h>
#include <linux/vfs.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/nfs.h>
struct nfsd_fhandle {
 struct svc_fh fh;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfsd_sattrargs {
 struct svc_fh fh;
 struct iattr attrs;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfsd_diropargs {
 struct svc_fh fh;
 char * name;
 int len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_readargs {
 struct svc_fh fh;
 __u32 offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 count;
 struct kvec vec[RPCSVC_MAXPAGES];
 int vlen;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfsd_writeargs {
 svc_fh fh;
 __u32 offset;
 int len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct kvec vec[RPCSVC_MAXPAGES];
 int vlen;
};
struct nfsd_createargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_fh fh;
 char * name;
 int len;
 struct iattr attrs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_renameargs {
 struct svc_fh ffh;
 char * fname;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int flen;
 struct svc_fh tfh;
 char * tname;
 int tlen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_readlinkargs {
 struct svc_fh fh;
 char * buffer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_linkargs {
 struct svc_fh ffh;
 struct svc_fh tfh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char * tname;
 int tlen;
};
struct nfsd_symlinkargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_fh ffh;
 char * fname;
 int flen;
 char * tname;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int tlen;
 struct iattr attrs;
};
struct nfsd_readdirargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct svc_fh fh;
 __u32 cookie;
 __u32 count;
 u32 * buffer;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_attrstat {
 struct svc_fh fh;
 struct kstat stat;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_diropres {
 struct svc_fh fh;
 struct kstat stat;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_readlinkres {
 int len;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfsd_readres {
 struct svc_fh fh;
 unsigned long count;
 struct kstat stat;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfsd_readdirres {
 int count;
 struct readdir_cd common;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 * buffer;
 int buflen;
 u32 * offset;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfsd_statfsres {
 struct kstatfs stats;
};
union nfsd_xdrstore {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfsd_sattrargs sattr;
 struct nfsd_diropargs dirop;
 struct nfsd_readargs read;
 struct nfsd_writeargs write;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfsd_createargs create;
 struct nfsd_renameargs rename;
 struct nfsd_linkargs link;
 struct nfsd_symlinkargs symlink;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfsd_readdirargs readdir;
};
#define NFS2_SVC_XDRSIZE sizeof(union nfsd_xdrstore)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
