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
#ifndef __EFS_FS_I_H__
#define __EFS_FS_I_H__
typedef int32_t efs_block_t;
typedef uint32_t efs_ino_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EFS_DIRECTEXTENTS 12
typedef union extent_u {
 unsigned char raw[8];
 struct extent_s {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int ex_magic:8;
 unsigned int ex_bn:24;
 unsigned int ex_length:8;
 unsigned int ex_offset:24;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } cooked;
} efs_extent;
typedef struct edevs {
 __be16 odev;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 ndev;
} efs_devs;
struct efs_dinode {
 __be16 di_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 di_nlink;
 __be16 di_uid;
 __be16 di_gid;
 __be32 di_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 di_atime;
 __be32 di_mtime;
 __be32 di_ctime;
 __be32 di_gen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 di_numextents;
 u_char di_version;
 u_char di_spare;
 union di_addr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 efs_extent di_extents[EFS_DIRECTEXTENTS];
 efs_devs di_dev;
 } di_u;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct efs_inode_info {
 int numextents;
 int lastextent;
 efs_extent extents[EFS_DIRECTEXTENTS];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct inode vfs_inode;
};
#endif
