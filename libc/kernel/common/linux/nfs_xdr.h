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
#ifndef _LINUX_NFS_XDR_H
#define _LINUX_NFS_XDR_H
#include <linux/sunrpc/xprt.h>
#include <linux/nfsacl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NFS_MAX_FILE_IO_SIZE (1048576U)
#define NFS_DEF_FILE_IO_SIZE (4096U)
#define NFS_MIN_FILE_IO_SIZE (1024U)
struct nfs_fsid {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint64_t major;
 uint64_t minor;
};
struct nfs_fattr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned short valid;
 __u64 pre_size;
 struct timespec pre_mtime;
 struct timespec pre_ctime;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 enum nfs_ftype type;
 __u32 mode;
 __u32 nlink;
 __u32 uid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 gid;
 __u64 size;
 union {
 struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 blocksize;
 __u32 blocks;
 } nfs2;
 struct {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 used;
 } nfs3;
 } du;
 dev_t rdev;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fsid fsid;
 __u64 fileid;
 struct timespec atime;
 struct timespec mtime;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct timespec ctime;
 __u32 bitmap[2];
 __u64 change_attr;
 __u64 pre_change_attr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long time_start;
};
#define NFS_ATTR_WCC 0x0001
#define NFS_ATTR_FATTR 0x0002
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NFS_ATTR_FATTR_V3 0x0004
#define NFS_ATTR_FATTR_V4 0x0008
#define NFS_ATTR_FATTR_V4_REFERRAL 0x0010
struct nfs_fsinfo {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr *fattr;
 __u32 rtmax;
 __u32 rtpref;
 __u32 rtmult;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 wtmax;
 __u32 wtpref;
 __u32 wtmult;
 __u32 dtpref;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 maxfilesize;
 __u32 lease_time;
};
struct nfs_fsstat {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr *fattr;
 __u64 tbytes;
 __u64 fbytes;
 __u64 abytes;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 tfiles;
 __u64 ffiles;
 __u64 afiles;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs2_fsstat {
 __u32 tsize;
 __u32 bsize;
 __u32 blocks;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 bfree;
 __u32 bavail;
};
struct nfs_pathconf {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr *fattr;
 __u32 max_link;
 __u32 max_namelen;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs4_change_info {
 u32 atomic;
 u64 before;
 u64 after;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_seqid;
struct nfs_openargs {
 const struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_seqid * seqid;
 int open_flags;
 __u64 clientid;
 __u32 id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 union {
 struct iattr * attrs;
 nfs4_verifier verifier;
 nfs4_stateid delegation;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int delegation_type;
 } u;
 const struct qstr * name;
 const struct nfs_server *server;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const u32 * bitmask;
 __u32 claim;
};
struct nfs_openres {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 nfs4_stateid stateid;
 struct nfs_fh fh;
 struct nfs4_change_info cinfo;
 __u32 rflags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr * f_attr;
 struct nfs_fattr * dir_attr;
 const struct nfs_server *server;
 int delegation_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 nfs4_stateid delegation;
 __u32 do_recall;
 __u64 maxsize;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_open_confirmargs {
 const struct nfs_fh * fh;
 nfs4_stateid * stateid;
 struct nfs_seqid * seqid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_open_confirmres {
 nfs4_stateid stateid;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_closeargs {
 struct nfs_fh * fh;
 nfs4_stateid * stateid;
 struct nfs_seqid * seqid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int open_flags;
 const u32 * bitmask;
};
struct nfs_closeres {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 nfs4_stateid stateid;
 struct nfs_fattr * fattr;
 const struct nfs_server *server;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_lowner {
 __u64 clientid;
 u32 id;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_lock_args {
 struct nfs_fh * fh;
 struct file_lock * fl;
 struct nfs_seqid * lock_seqid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 nfs4_stateid * lock_stateid;
 struct nfs_seqid * open_seqid;
 nfs4_stateid * open_stateid;
 struct nfs_lowner lock_owner;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char block : 1;
 unsigned char reclaim : 1;
 unsigned char new_lock_owner : 1;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_lock_res {
 nfs4_stateid stateid;
};
struct nfs_locku_args {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fh;
 struct file_lock * fl;
 struct nfs_seqid * seqid;
 nfs4_stateid * stateid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_locku_res {
 nfs4_stateid stateid;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_lockt_args {
 struct nfs_fh * fh;
 struct file_lock * fl;
 struct nfs_lowner lock_owner;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_lockt_res {
 struct file_lock * denied;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs4_delegreturnargs {
 const struct nfs_fh *fhandle;
 const nfs4_stateid *stateid;
 const u32 * bitmask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs4_delegreturnres {
 struct nfs_fattr * fattr;
 const struct nfs_server *server;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_readargs {
 struct nfs_fh * fh;
 struct nfs_open_context *context;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 offset;
 __u32 count;
 unsigned int pgbase;
 struct page ** pages;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_readres {
 struct nfs_fattr * fattr;
 __u32 count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int eof;
};
struct nfs_writeargs {
 struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_open_context *context;
 __u64 offset;
 __u32 count;
 enum nfs3_stable_how stable;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int pgbase;
 struct page ** pages;
 const u32 * bitmask;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_writeverf {
 enum nfs3_stable_how committed;
 __u32 verifier[2];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_writeres {
 struct nfs_fattr * fattr;
 struct nfs_writeverf * verf;
 __u32 count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const struct nfs_server *server;
};
struct nfs_entry {
 __u64 ino;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 cookie,
 prev_cookie;
 const char * name;
 unsigned int len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int eof;
 struct nfs_fh * fh;
 struct nfs_fattr * fattr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_sattrargs {
 struct nfs_fh * fh;
 struct iattr * sattr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_diropargs {
 struct nfs_fh * fh;
 const char * name;
 unsigned int len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs_createargs {
 struct nfs_fh * fh;
 const char * name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int len;
 struct iattr * sattr;
};
struct nfs_renameargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fromfh;
 const char * fromname;
 unsigned int fromlen;
 struct nfs_fh * tofh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const char * toname;
 unsigned int tolen;
};
struct nfs_setattrargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fh;
 nfs4_stateid stateid;
 struct iattr * iap;
 const struct nfs_server * server;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const u32 * bitmask;
};
struct nfs_setaclargs {
 struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t acl_len;
 unsigned int acl_pgbase;
 struct page ** acl_pages;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_getaclargs {
 struct nfs_fh * fh;
 size_t acl_len;
 unsigned int acl_pgbase;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page ** acl_pages;
};
struct nfs_setattrres {
 struct nfs_fattr * fattr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const struct nfs_server * server;
};
struct nfs_linkargs {
 struct nfs_fh * fromfh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * tofh;
 const char * toname;
 unsigned int tolen;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_symlinkargs {
 struct nfs_fh * fromfh;
 const char * fromname;
 unsigned int fromlen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const char * topath;
 unsigned int tolen;
 struct iattr * sattr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs_readdirargs {
 struct nfs_fh * fh;
 __u32 cookie;
 unsigned int count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page ** pages;
};
struct nfs3_getaclargs {
 struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int mask;
 struct page ** pages;
};
struct nfs3_setaclargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct inode * inode;
 int mask;
 struct posix_acl * acl_access;
 struct posix_acl * acl_default;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page ** pages;
};
struct nfs_diropok {
 struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr * fattr;
};
struct nfs_readlinkargs {
 struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int pgbase;
 unsigned int pglen;
 struct page ** pages;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs3_sattrargs {
 struct nfs_fh * fh;
 struct iattr * sattr;
 unsigned int guard;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct timespec guardtime;
};
struct nfs3_diropargs {
 struct nfs_fh * fh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const char * name;
 unsigned int len;
};
struct nfs3_accessargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fh;
 __u32 access;
};
struct nfs3_createargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fh;
 const char * name;
 unsigned int len;
 struct iattr * sattr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 enum nfs3_createmode createmode;
 __u32 verifier[2];
};
struct nfs3_mkdirargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fh;
 const char * name;
 unsigned int len;
 struct iattr * sattr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs3_symlinkargs {
 struct nfs_fh * fromfh;
 const char * fromname;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int fromlen;
 const char * topath;
 unsigned int tolen;
 struct iattr * sattr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs3_mknodargs {
 struct nfs_fh * fh;
 const char * name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int len;
 enum nfs3_ftype type;
 struct iattr * sattr;
 dev_t rdev;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs3_renameargs {
 struct nfs_fh * fromfh;
 const char * fromname;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int fromlen;
 struct nfs_fh * tofh;
 const char * toname;
 unsigned int tolen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct nfs3_linkargs {
 struct nfs_fh * fromfh;
 struct nfs_fh * tofh;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const char * toname;
 unsigned int tolen;
};
struct nfs3_readdirargs {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fh * fh;
 __u64 cookie;
 __u32 verf[2];
 int plus;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int count;
 struct page ** pages;
};
struct nfs3_diropres {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr * dir_attr;
 struct nfs_fh * fh;
 struct nfs_fattr * fattr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs3_accessres {
 struct nfs_fattr * fattr;
 __u32 access;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nfs3_readlinkargs {
 struct nfs_fh * fh;
 unsigned int pgbase;
 unsigned int pglen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page ** pages;
};
struct nfs3_renameres {
 struct nfs_fattr * fromattr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr * toattr;
};
struct nfs3_linkres {
 struct nfs_fattr * dir_attr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr * fattr;
};
struct nfs3_readdirres {
 struct nfs_fattr * dir_attr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 * verf;
 int plus;
};
struct nfs3_getaclres {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr * fattr;
 int mask;
 unsigned int acl_access_count;
 unsigned int acl_default_count;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct posix_acl * acl_access;
 struct posix_acl * acl_default;
};
struct nfs_page;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NFS_PAGEVEC_SIZE (8U)
struct nfs_read_data {
 int flags;
 struct rpc_task task;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct inode *inode;
 struct rpc_cred *cred;
 struct nfs_fattr fattr;
 struct list_head pages;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_page *req;
 struct page **pagevec;
 unsigned int npages;
 struct nfs_readargs args;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_readres res;
 struct page *page_array[NFS_PAGEVEC_SIZE];
};
struct nfs_write_data {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int flags;
 struct rpc_task task;
 struct inode *inode;
 struct rpc_cred *cred;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nfs_fattr fattr;
 struct nfs_writeverf verf;
 struct list_head pages;
 struct nfs_page *req;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page **pagevec;
 unsigned int npages;
 struct nfs_writeargs args;
 struct nfs_writeres res;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct page *page_array[NFS_PAGEVEC_SIZE];
};
struct nfs_access_entry;
struct nfs_rpc_ops {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int version;
 struct dentry_operations *dentry_ops;
 struct inode_operations *dir_inode_ops;
 struct inode_operations *file_inode_ops;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*getroot) (struct nfs_server *, struct nfs_fh *,
 struct nfs_fsinfo *);
 int (*getattr) (struct nfs_server *, struct nfs_fh *,
 struct nfs_fattr *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*setattr) (struct dentry *, struct nfs_fattr *,
 struct iattr *);
 int (*lookup) (struct inode *, struct qstr *,
 struct nfs_fh *, struct nfs_fattr *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*access) (struct inode *, struct nfs_access_entry *);
 int (*readlink)(struct inode *, struct page *, unsigned int,
 unsigned int);
 int (*read) (struct nfs_read_data *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*write) (struct nfs_write_data *);
 int (*commit) (struct nfs_write_data *);
 int (*create) (struct inode *, struct dentry *,
 struct iattr *, int, struct nameidata *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*remove) (struct inode *, struct qstr *);
 int (*unlink_setup) (struct rpc_message *,
 struct dentry *, struct qstr *);
 int (*unlink_done) (struct dentry *, struct rpc_task *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*rename) (struct inode *, struct qstr *,
 struct inode *, struct qstr *);
 int (*link) (struct inode *, struct inode *, struct qstr *);
 int (*symlink) (struct inode *, struct qstr *, struct qstr *,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct iattr *, struct nfs_fh *,
 struct nfs_fattr *);
 int (*mkdir) (struct inode *, struct dentry *, struct iattr *);
 int (*rmdir) (struct inode *, struct qstr *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*readdir) (struct dentry *, struct rpc_cred *,
 u64, struct page *, unsigned int, int);
 int (*mknod) (struct inode *, struct dentry *, struct iattr *,
 dev_t);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*statfs) (struct nfs_server *, struct nfs_fh *,
 struct nfs_fsstat *);
 int (*fsinfo) (struct nfs_server *, struct nfs_fh *,
 struct nfs_fsinfo *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*pathconf) (struct nfs_server *, struct nfs_fh *,
 struct nfs_pathconf *);
 u32 * (*decode_dirent)(u32 *, struct nfs_entry *, int plus);
 void (*read_setup) (struct nfs_read_data *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*read_done) (struct rpc_task *, struct nfs_read_data *);
 void (*write_setup) (struct nfs_write_data *, int how);
 int (*write_done) (struct rpc_task *, struct nfs_write_data *);
 void (*commit_setup) (struct nfs_write_data *, int how);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*commit_done) (struct rpc_task *, struct nfs_write_data *);
 int (*file_open) (struct inode *, struct file *);
 int (*file_release) (struct inode *, struct file *);
 int (*lock)(struct file *, int, struct file_lock *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*clear_acl_cache)(struct inode *);
};
#define NFS_CALL(op, inode, args) NFS_PROTO(inode)->op args
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
