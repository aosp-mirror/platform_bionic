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
#ifndef MTD_PARTITIONS_H
#define MTD_PARTITIONS_H
#include <linux/types.h>
struct mtd_partition {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 char *name;
 u_int32_t size;
 u_int32_t offset;
 u_int32_t mask_flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nand_ecclayout *ecclayout;
 struct mtd_info **mtdp;
};
#define MTDPART_OFS_NXTBLK (-2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTDPART_OFS_APPEND (-1)
#define MTDPART_SIZ_FULL (0)
struct mtd_part_parser {
 struct list_head list;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct module *owner;
 const char *name;
 int (*parse_fn)(struct mtd_info *, struct mtd_partition **, unsigned long);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define put_partition_parser(p) do { module_put((p)->owner); } while(0)
#endif
