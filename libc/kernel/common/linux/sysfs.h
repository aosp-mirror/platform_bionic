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
#ifndef _SYSFS_H_
#define _SYSFS_H_
#include <asm/atomic.h>
struct kobject;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct module;
struct attribute {
 const char * name;
 struct module * owner;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 mode_t mode;
};
struct attribute_group {
 const char * name;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct attribute ** attrs;
};
#define __ATTR(_name,_mode,_show,_store) {   .attr = {.name = __stringify(_name), .mode = _mode, .owner = THIS_MODULE },   .show = _show,   .store = _store,  }
#define __ATTR_RO(_name) {   .attr = { .name = __stringify(_name), .mode = 0444, .owner = THIS_MODULE },   .show = _name##_show,  }
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __ATTR_NULL { .attr = { .name = NULL } }
#define attr_name(_attr) (_attr).attr.name
struct vm_area_struct;
struct bin_attribute {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct attribute attr;
 size_t size;
 void *private;
 ssize_t (*read)(struct kobject *, char *, loff_t, size_t);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ssize_t (*write)(struct kobject *, char *, loff_t, size_t);
 int (*mmap)(struct kobject *, struct bin_attribute *attr,
 struct vm_area_struct *vma);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct sysfs_ops {
 ssize_t (*show)(struct kobject *, struct attribute *,char *);
 ssize_t (*store)(struct kobject *,struct attribute *,const char *, size_t);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct sysfs_dirent {
 atomic_t s_count;
 struct list_head s_sibling;
 struct list_head s_children;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void * s_element;
 int s_type;
 umode_t s_mode;
 struct dentry * s_dentry;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct iattr * s_iattr;
 atomic_t s_event;
};
#define SYSFS_ROOT 0x0001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SYSFS_DIR 0x0002
#define SYSFS_KOBJ_ATTR 0x0004
#define SYSFS_KOBJ_BIN_ATTR 0x0008
#define SYSFS_KOBJ_DEVICE 0x0010
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SYSFS_KOBJ_LINK 0x0020
#define SYSFS_NOT_PINNED (SYSFS_KOBJ_ATTR | SYSFS_KOBJ_BIN_ATTR | SYSFS_KOBJ_DEVICE | SYSFS_KOBJ_LINK)
#endif
