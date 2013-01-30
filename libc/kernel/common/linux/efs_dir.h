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
#ifndef __EFS_DIR_H__
#define __EFS_DIR_H__
#define EFS_DIRBSIZE_BITS EFS_BLOCKSIZE_BITS
#define EFS_DIRBSIZE (1 << EFS_DIRBSIZE_BITS)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct efs_dentry {
 __be32 inode;
 unsigned char namelen;
 char name[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define EFS_DENTSIZE (sizeof(struct efs_dentry) - 3 + 1)
#define EFS_MAXNAMELEN ((1 << (sizeof(char) * 8)) - 1)
#define EFS_DIRBLK_HEADERSIZE 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EFS_DIRBLK_MAGIC 0xbeef
struct efs_dir {
 __be16 magic;
 unsigned char firstused;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char slots;
 unsigned char space[EFS_DIRBSIZE - EFS_DIRBLK_HEADERSIZE];
};
#define EFS_MAXENTS   ((EFS_DIRBSIZE - EFS_DIRBLK_HEADERSIZE) /   (EFS_DENTSIZE + sizeof(char)))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define EFS_SLOTAT(dir, slot) EFS_REALOFF((dir)->space[slot])
#define EFS_REALOFF(offset) ((offset << 1))
#endif
