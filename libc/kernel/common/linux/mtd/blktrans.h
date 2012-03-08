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
#ifndef __MTD_TRANS_H__
#define __MTD_TRANS_H__
#include <linux/mutex.h>
struct hd_geometry;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mtd_info;
struct mtd_blktrans_ops;
struct file;
struct inode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mtd_blktrans_dev {
 struct mtd_blktrans_ops *tr;
 struct list_head list;
 struct mtd_info *mtd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct mutex lock;
 int devnum;
 int blksize;
 unsigned long size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int readonly;
 void *blkcore_priv;
};
struct blkcore_priv;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct mtd_blktrans_ops {
 char *name;
 int major;
 int part_bits;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*readsect)(struct mtd_blktrans_dev *dev,
 unsigned long block, char *buffer);
 int (*writesect)(struct mtd_blktrans_dev *dev,
 unsigned long block, char *buffer);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*getgeo)(struct mtd_blktrans_dev *dev, struct hd_geometry *geo);
 int (*flush)(struct mtd_blktrans_dev *dev);
 int (*open)(struct mtd_blktrans_dev *dev);
 int (*release)(struct mtd_blktrans_dev *dev);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void (*add_mtd)(struct mtd_blktrans_ops *tr, struct mtd_info *mtd);
 void (*remove_dev)(struct mtd_blktrans_dev *dev);
 struct list_head devs;
 struct list_head list;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct module *owner;
 struct mtd_blkcore_priv *blkcore_priv;
};
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
