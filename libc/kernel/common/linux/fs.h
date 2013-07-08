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
#ifndef _LINUX_FS_H
#define _LINUX_FS_H
#include <linux/limits.h>
#include <linux/ioctl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/blk_types.h>
#include <linux/types.h>
#undef NR_OPEN
#define INR_OPEN_CUR 1024
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define INR_OPEN_MAX 4096
#define BLOCK_SIZE_BITS 10
#define BLOCK_SIZE (1<<BLOCK_SIZE_BITS)
#define SEEK_SET 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_DATA 3
#define SEEK_HOLE 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SEEK_MAX SEEK_HOLE
struct fstrim_range {
 __u64 start;
 __u64 len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 minlen;
};
struct files_stat_struct {
 unsigned long nr_files;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long nr_free_files;
 unsigned long max_files;
};
struct inodes_stat_t {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int nr_inodes;
 int nr_unused;
 int dummy[5];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NR_FILE 8192
#define MAY_EXEC 0x00000001
#define MAY_WRITE 0x00000002
#define MAY_READ 0x00000004
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAY_APPEND 0x00000008
#define MAY_ACCESS 0x00000010
#define MAY_OPEN 0x00000020
#define MAY_CHDIR 0x00000040
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MAY_NOT_BLOCK 0x00000080
#define FMODE_READ ((__force fmode_t)0x1)
#define FMODE_WRITE ((__force fmode_t)0x2)
#define FMODE_LSEEK ((__force fmode_t)0x4)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FMODE_PREAD ((__force fmode_t)0x8)
#define FMODE_PWRITE ((__force fmode_t)0x10)
#define FMODE_EXEC ((__force fmode_t)0x20)
#define FMODE_NDELAY ((__force fmode_t)0x40)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FMODE_EXCL ((__force fmode_t)0x80)
#define FMODE_WRITE_IOCTL ((__force fmode_t)0x100)
#define FMODE_32BITHASH ((__force fmode_t)0x200)
#define FMODE_64BITHASH ((__force fmode_t)0x400)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FMODE_NOCMTIME ((__force fmode_t)0x800)
#define FMODE_RANDOM ((__force fmode_t)0x1000)
#define FMODE_UNSIGNED_OFFSET ((__force fmode_t)0x2000)
#define FMODE_PATH ((__force fmode_t)0x4000)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FMODE_NONOTIFY ((__force fmode_t)0x1000000)
#define RW_MASK REQ_WRITE
#define RWA_MASK REQ_RAHEAD
#define READ 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define WRITE RW_MASK
#define READA RWA_MASK
#define READ_SYNC (READ | REQ_SYNC)
#define WRITE_SYNC (WRITE | REQ_SYNC | REQ_NOIDLE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define WRITE_ODIRECT (WRITE | REQ_SYNC)
#define WRITE_FLUSH (WRITE | REQ_SYNC | REQ_NOIDLE | REQ_FLUSH)
#define WRITE_FUA (WRITE | REQ_SYNC | REQ_NOIDLE | REQ_FUA)
#define WRITE_FLUSH_FUA (WRITE | REQ_SYNC | REQ_NOIDLE | REQ_FLUSH | REQ_FUA)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SEL_IN 1
#define SEL_OUT 2
#define SEL_EX 4
#define FS_REQUIRES_DEV 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_BINARY_MOUNTDATA 2
#define FS_HAS_SUBTYPE 4
#define FS_REVAL_DOT 16384
#define FS_RENAME_DOES_D_MOVE 32768
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_RDONLY 1
#define MS_NOSUID 2
#define MS_NODEV 4
#define MS_NOEXEC 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_SYNCHRONOUS 16
#define MS_REMOUNT 32
#define MS_MANDLOCK 64
#define MS_DIRSYNC 128
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_NOATIME 1024
#define MS_NODIRATIME 2048
#define MS_BIND 4096
#define MS_MOVE 8192
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_REC 16384
#define MS_VERBOSE 32768
#define MS_SILENT 32768
#define MS_POSIXACL (1<<16)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_UNBINDABLE (1<<17)
#define MS_PRIVATE (1<<18)
#define MS_SLAVE (1<<19)
#define MS_SHARED (1<<20)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_RELATIME (1<<21)
#define MS_KERNMOUNT (1<<22)
#define MS_I_VERSION (1<<23)
#define MS_STRICTATIME (1<<24)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_NOSEC (1<<28)
#define MS_BORN (1<<29)
#define MS_ACTIVE (1<<30)
#define MS_NOUSER (1<<31)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MS_RMT_MASK (MS_RDONLY|MS_SYNCHRONOUS|MS_MANDLOCK|MS_I_VERSION)
#define MS_MGC_VAL 0xC0ED0000
#define MS_MGC_MSK 0xffff0000
#define S_SYNC 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define S_NOATIME 2
#define S_APPEND 4
#define S_IMMUTABLE 8
#define S_DEAD 16
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define S_NOQUOTA 32
#define S_DIRSYNC 64
#define S_NOCMTIME 128
#define S_SWAPFILE 256
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define S_PRIVATE 512
#define S_IMA 1024
#define S_AUTOMOUNT 2048
#define S_NOSEC 4096
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __IS_FLG(inode,flg) ((inode)->i_sb->s_flags & (flg))
#define IS_RDONLY(inode) ((inode)->i_sb->s_flags & MS_RDONLY)
#define IS_SYNC(inode) (__IS_FLG(inode, MS_SYNCHRONOUS) ||   ((inode)->i_flags & S_SYNC))
#define IS_DIRSYNC(inode) (__IS_FLG(inode, MS_SYNCHRONOUS|MS_DIRSYNC) ||   ((inode)->i_flags & (S_SYNC|S_DIRSYNC)))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IS_MANDLOCK(inode) __IS_FLG(inode, MS_MANDLOCK)
#define IS_NOATIME(inode) __IS_FLG(inode, MS_RDONLY|MS_NOATIME)
#define IS_I_VERSION(inode) __IS_FLG(inode, MS_I_VERSION)
#define IS_NOQUOTA(inode) ((inode)->i_flags & S_NOQUOTA)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IS_APPEND(inode) ((inode)->i_flags & S_APPEND)
#define IS_IMMUTABLE(inode) ((inode)->i_flags & S_IMMUTABLE)
#define IS_POSIXACL(inode) __IS_FLG(inode, MS_POSIXACL)
#define IS_DEADDIR(inode) ((inode)->i_flags & S_DEAD)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IS_NOCMTIME(inode) ((inode)->i_flags & S_NOCMTIME)
#define IS_SWAPFILE(inode) ((inode)->i_flags & S_SWAPFILE)
#define IS_PRIVATE(inode) ((inode)->i_flags & S_PRIVATE)
#define IS_IMA(inode) ((inode)->i_flags & S_IMA)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IS_AUTOMOUNT(inode) ((inode)->i_flags & S_AUTOMOUNT)
#define IS_NOSEC(inode) ((inode)->i_flags & S_NOSEC)
#define BLKROSET _IO(0x12,93)
#define BLKROGET _IO(0x12,94)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKRRPART _IO(0x12,95)
#define BLKGETSIZE _IO(0x12,96)
#define BLKFLSBUF _IO(0x12,97)
#define BLKRASET _IO(0x12,98)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKRAGET _IO(0x12,99)
#define BLKFRASET _IO(0x12,100)
#define BLKFRAGET _IO(0x12,101)
#define BLKSECTSET _IO(0x12,102)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKSECTGET _IO(0x12,103)
#define BLKSSZGET _IO(0x12,104)
#define BLKBSZGET _IOR(0x12,112,size_t)
#define BLKBSZSET _IOW(0x12,113,size_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKGETSIZE64 _IOR(0x12,114,size_t)
#define BLKTRACESETUP _IOWR(0x12,115,struct blk_user_trace_setup)
#define BLKTRACESTART _IO(0x12,116)
#define BLKTRACESTOP _IO(0x12,117)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKTRACETEARDOWN _IO(0x12,118)
#define BLKDISCARD _IO(0x12,119)
#define BLKIOMIN _IO(0x12,120)
#define BLKIOOPT _IO(0x12,121)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKALIGNOFF _IO(0x12,122)
#define BLKPBSZGET _IO(0x12,123)
#define BLKDISCARDZEROES _IO(0x12,124)
#define BLKSECDISCARD _IO(0x12,125)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLKROTATIONAL _IO(0x12,126)
#define BMAP_IOCTL 1
#define FIBMAP _IO(0x00,1)
#define FIGETBSZ _IO(0x00,2)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FIFREEZE _IOWR('X', 119, int)
#define FITHAW _IOWR('X', 120, int)
#define FITRIM _IOWR('X', 121, struct fstrim_range)
#define FS_IOC_GETFLAGS _IOR('f', 1, long)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_IOC_SETFLAGS _IOW('f', 2, long)
#define FS_IOC_GETVERSION _IOR('v', 1, long)
#define FS_IOC_SETVERSION _IOW('v', 2, long)
#define FS_IOC_FIEMAP _IOWR('f', 11, struct fiemap)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_IOC32_GETFLAGS _IOR('f', 1, int)
#define FS_IOC32_SETFLAGS _IOW('f', 2, int)
#define FS_IOC32_GETVERSION _IOR('v', 1, int)
#define FS_IOC32_SETVERSION _IOW('v', 2, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_SECRM_FL 0x00000001
#define FS_UNRM_FL 0x00000002
#define FS_COMPR_FL 0x00000004
#define FS_SYNC_FL 0x00000008
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_IMMUTABLE_FL 0x00000010
#define FS_APPEND_FL 0x00000020
#define FS_NODUMP_FL 0x00000040
#define FS_NOATIME_FL 0x00000080
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_DIRTY_FL 0x00000100
#define FS_COMPRBLK_FL 0x00000200
#define FS_NOCOMP_FL 0x00000400
#define FS_ECOMPR_FL 0x00000800
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_BTREE_FL 0x00001000
#define FS_INDEX_FL 0x00001000
#define FS_IMAGIC_FL 0x00002000
#define FS_JOURNAL_DATA_FL 0x00004000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_NOTAIL_FL 0x00008000
#define FS_DIRSYNC_FL 0x00010000
#define FS_TOPDIR_FL 0x00020000
#define FS_EXTENT_FL 0x00080000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_DIRECTIO_FL 0x00100000
#define FS_NOCOW_FL 0x00800000
#define FS_RESERVED_FL 0x80000000
#define FS_FL_USER_VISIBLE 0x0003DFFF
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FS_FL_USER_MODIFIABLE 0x000380FF
#define SYNC_FILE_RANGE_WAIT_BEFORE 1
#define SYNC_FILE_RANGE_WRITE 2
#define SYNC_FILE_RANGE_WAIT_AFTER 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
