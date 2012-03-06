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
#ifndef _LINUX_PROC_FS_H
#define _LINUX_PROC_FS_H
#include <linux/slab.h>
#include <linux/fs.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/spinlock.h>
#include <asm/atomic.h>
#define FIRST_PROCESS_ENTRY 256
enum {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 PROC_ROOT_INO = 1,
};
#define PROC_SUPER_MAGIC 0x9fa0
typedef int (read_proc_t)(char *page, char **start, off_t off,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int count, int *eof, void *data);
typedef int (write_proc_t)(struct file *file, const char __user *buffer,
 unsigned long count, void *data);
typedef int (get_info_t)(char *, char **, off_t, int);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct proc_dir_entry {
 unsigned int low_ino;
 unsigned short namelen;
 const char *name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mode_t mode;
 nlink_t nlink;
 uid_t uid;
 gid_t gid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 loff_t size;
 struct inode_operations * proc_iops;
 const struct file_operations * proc_fops;
 get_info_t *get_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct module *owner;
 struct proc_dir_entry *next, *parent, *subdir;
 void *data;
 read_proc_t *read_proc;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 write_proc_t *write_proc;
 atomic_t count;
 int deleted;
 void *set;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct kcore_list {
 struct kcore_list *next;
 unsigned long addr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t size;
};
struct vmcore {
 struct list_head list;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long long paddr;
 unsigned long long size;
 loff_t offset;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define proc_root_driver NULL
#define proc_net NULL
#define proc_bus NULL
#define proc_net_fops_create(name, mode, fops) ({ (void)(mode), NULL; })
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define proc_net_create(name, mode, info) ({ (void)(mode), NULL; })
#define remove_proc_entry(name, parent) do {} while (0)
struct tty_driver;
struct proc_inode {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct pid *pid;
 int fd;
 union {
 int (*proc_get_link)(struct inode *, struct dentry **, struct vfsmount **);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*proc_read)(struct task_struct *task, char *page);
 } op;
 struct proc_dir_entry *pde;
 struct inode vfs_inode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct proc_maps_private {
 struct pid *pid;
 struct task_struct *task;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct vm_area_struct *tail_vma;
};
#endif
