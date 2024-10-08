/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_FS_H
#define _UAPI_LINUX_FS_H
#include <linux/limits.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/fscrypt.h>
#include <linux/mount.h>
#undef NR_OPEN
#define INR_OPEN_CUR 1024
#define INR_OPEN_MAX 4096
#define BLOCK_SIZE_BITS 10
#define BLOCK_SIZE (1 << BLOCK_SIZE_BITS)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_DATA 3
#define SEEK_HOLE 4
#define SEEK_MAX SEEK_HOLE
#define RENAME_NOREPLACE (1 << 0)
#define RENAME_EXCHANGE (1 << 1)
#define RENAME_WHITEOUT (1 << 2)
struct file_clone_range {
  __s64 src_fd;
  __u64 src_offset;
  __u64 src_length;
  __u64 dest_offset;
};
struct fstrim_range {
  __u64 start;
  __u64 len;
  __u64 minlen;
};
struct fsuuid2 {
  __u8 len;
  __u8 uuid[16];
};
struct fs_sysfs_path {
  __u8 len;
  __u8 name[128];
};
#define FILE_DEDUPE_RANGE_SAME 0
#define FILE_DEDUPE_RANGE_DIFFERS 1
struct file_dedupe_range_info {
  __s64 dest_fd;
  __u64 dest_offset;
  __u64 bytes_deduped;
  __s32 status;
  __u32 reserved;
};
struct file_dedupe_range {
  __u64 src_offset;
  __u64 src_length;
  __u16 dest_count;
  __u16 reserved1;
  __u32 reserved2;
  struct file_dedupe_range_info info[];
};
struct files_stat_struct {
  unsigned long nr_files;
  unsigned long nr_free_files;
  unsigned long max_files;
};
struct inodes_stat_t {
  long nr_inodes;
  long nr_unused;
  long dummy[5];
};
#define NR_FILE 8192
struct fsxattr {
  __u32 fsx_xflags;
  __u32 fsx_extsize;
  __u32 fsx_nextents;
  __u32 fsx_projid;
  __u32 fsx_cowextsize;
  unsigned char fsx_pad[8];
};
#define FS_XFLAG_REALTIME 0x00000001
#define FS_XFLAG_PREALLOC 0x00000002
#define FS_XFLAG_IMMUTABLE 0x00000008
#define FS_XFLAG_APPEND 0x00000010
#define FS_XFLAG_SYNC 0x00000020
#define FS_XFLAG_NOATIME 0x00000040
#define FS_XFLAG_NODUMP 0x00000080
#define FS_XFLAG_RTINHERIT 0x00000100
#define FS_XFLAG_PROJINHERIT 0x00000200
#define FS_XFLAG_NOSYMLINKS 0x00000400
#define FS_XFLAG_EXTSIZE 0x00000800
#define FS_XFLAG_EXTSZINHERIT 0x00001000
#define FS_XFLAG_NODEFRAG 0x00002000
#define FS_XFLAG_FILESTREAM 0x00004000
#define FS_XFLAG_DAX 0x00008000
#define FS_XFLAG_COWEXTSIZE 0x00010000
#define FS_XFLAG_HASATTR 0x80000000
#define BLKROSET _IO(0x12, 93)
#define BLKROGET _IO(0x12, 94)
#define BLKRRPART _IO(0x12, 95)
#define BLKGETSIZE _IO(0x12, 96)
#define BLKFLSBUF _IO(0x12, 97)
#define BLKRASET _IO(0x12, 98)
#define BLKRAGET _IO(0x12, 99)
#define BLKFRASET _IO(0x12, 100)
#define BLKFRAGET _IO(0x12, 101)
#define BLKSECTSET _IO(0x12, 102)
#define BLKSECTGET _IO(0x12, 103)
#define BLKSSZGET _IO(0x12, 104)
#define BLKBSZGET _IOR(0x12, 112, size_t)
#define BLKBSZSET _IOW(0x12, 113, size_t)
#define BLKGETSIZE64 _IOR(0x12, 114, size_t)
#define BLKTRACESETUP _IOWR(0x12, 115, struct blk_user_trace_setup)
#define BLKTRACESTART _IO(0x12, 116)
#define BLKTRACESTOP _IO(0x12, 117)
#define BLKTRACETEARDOWN _IO(0x12, 118)
#define BLKDISCARD _IO(0x12, 119)
#define BLKIOMIN _IO(0x12, 120)
#define BLKIOOPT _IO(0x12, 121)
#define BLKALIGNOFF _IO(0x12, 122)
#define BLKPBSZGET _IO(0x12, 123)
#define BLKDISCARDZEROES _IO(0x12, 124)
#define BLKSECDISCARD _IO(0x12, 125)
#define BLKROTATIONAL _IO(0x12, 126)
#define BLKZEROOUT _IO(0x12, 127)
#define BLKGETDISKSEQ _IOR(0x12, 128, __u64)
#define BMAP_IOCTL 1
#define FIBMAP _IO(0x00, 1)
#define FIGETBSZ _IO(0x00, 2)
#define FIFREEZE _IOWR('X', 119, int)
#define FITHAW _IOWR('X', 120, int)
#define FITRIM _IOWR('X', 121, struct fstrim_range)
#define FICLONE _IOW(0x94, 9, int)
#define FICLONERANGE _IOW(0x94, 13, struct file_clone_range)
#define FIDEDUPERANGE _IOWR(0x94, 54, struct file_dedupe_range)
#define FSLABEL_MAX 256
#define FS_IOC_GETFLAGS _IOR('f', 1, long)
#define FS_IOC_SETFLAGS _IOW('f', 2, long)
#define FS_IOC_GETVERSION _IOR('v', 1, long)
#define FS_IOC_SETVERSION _IOW('v', 2, long)
#define FS_IOC_FIEMAP _IOWR('f', 11, struct fiemap)
#define FS_IOC32_GETFLAGS _IOR('f', 1, int)
#define FS_IOC32_SETFLAGS _IOW('f', 2, int)
#define FS_IOC32_GETVERSION _IOR('v', 1, int)
#define FS_IOC32_SETVERSION _IOW('v', 2, int)
#define FS_IOC_FSGETXATTR _IOR('X', 31, struct fsxattr)
#define FS_IOC_FSSETXATTR _IOW('X', 32, struct fsxattr)
#define FS_IOC_GETFSLABEL _IOR(0x94, 49, char[FSLABEL_MAX])
#define FS_IOC_SETFSLABEL _IOW(0x94, 50, char[FSLABEL_MAX])
#define FS_IOC_GETFSUUID _IOR(0x15, 0, struct fsuuid2)
#define FS_IOC_GETFSSYSFSPATH _IOR(0x15, 1, struct fs_sysfs_path)
#define FS_SECRM_FL 0x00000001
#define FS_UNRM_FL 0x00000002
#define FS_COMPR_FL 0x00000004
#define FS_SYNC_FL 0x00000008
#define FS_IMMUTABLE_FL 0x00000010
#define FS_APPEND_FL 0x00000020
#define FS_NODUMP_FL 0x00000040
#define FS_NOATIME_FL 0x00000080
#define FS_DIRTY_FL 0x00000100
#define FS_COMPRBLK_FL 0x00000200
#define FS_NOCOMP_FL 0x00000400
#define FS_ENCRYPT_FL 0x00000800
#define FS_BTREE_FL 0x00001000
#define FS_INDEX_FL 0x00001000
#define FS_IMAGIC_FL 0x00002000
#define FS_JOURNAL_DATA_FL 0x00004000
#define FS_NOTAIL_FL 0x00008000
#define FS_DIRSYNC_FL 0x00010000
#define FS_TOPDIR_FL 0x00020000
#define FS_HUGE_FILE_FL 0x00040000
#define FS_EXTENT_FL 0x00080000
#define FS_VERITY_FL 0x00100000
#define FS_EA_INODE_FL 0x00200000
#define FS_EOFBLOCKS_FL 0x00400000
#define FS_NOCOW_FL 0x00800000
#define FS_DAX_FL 0x02000000
#define FS_INLINE_DATA_FL 0x10000000
#define FS_PROJINHERIT_FL 0x20000000
#define FS_CASEFOLD_FL 0x40000000
#define FS_RESERVED_FL 0x80000000
#define FS_FL_USER_VISIBLE 0x0003DFFF
#define FS_FL_USER_MODIFIABLE 0x000380FF
#define SYNC_FILE_RANGE_WAIT_BEFORE 1
#define SYNC_FILE_RANGE_WRITE 2
#define SYNC_FILE_RANGE_WAIT_AFTER 4
#define SYNC_FILE_RANGE_WRITE_AND_WAIT (SYNC_FILE_RANGE_WRITE | SYNC_FILE_RANGE_WAIT_BEFORE | SYNC_FILE_RANGE_WAIT_AFTER)
typedef int __bitwise __kernel_rwf_t;
#define RWF_HIPRI (( __kernel_rwf_t) 0x00000001)
#define RWF_DSYNC (( __kernel_rwf_t) 0x00000002)
#define RWF_SYNC (( __kernel_rwf_t) 0x00000004)
#define RWF_NOWAIT (( __kernel_rwf_t) 0x00000008)
#define RWF_APPEND (( __kernel_rwf_t) 0x00000010)
#define RWF_NOAPPEND (( __kernel_rwf_t) 0x00000020)
#define RWF_ATOMIC (( __kernel_rwf_t) 0x00000040)
#define RWF_SUPPORTED (RWF_HIPRI | RWF_DSYNC | RWF_SYNC | RWF_NOWAIT | RWF_APPEND | RWF_NOAPPEND | RWF_ATOMIC)
#define PROCFS_IOCTL_MAGIC 'f'
#define PAGEMAP_SCAN _IOWR(PROCFS_IOCTL_MAGIC, 16, struct pm_scan_arg)
#define PAGE_IS_WPALLOWED (1 << 0)
#define PAGE_IS_WRITTEN (1 << 1)
#define PAGE_IS_FILE (1 << 2)
#define PAGE_IS_PRESENT (1 << 3)
#define PAGE_IS_SWAPPED (1 << 4)
#define PAGE_IS_PFNZERO (1 << 5)
#define PAGE_IS_HUGE (1 << 6)
#define PAGE_IS_SOFT_DIRTY (1 << 7)
struct page_region {
  __u64 start;
  __u64 end;
  __u64 categories;
};
#define PM_SCAN_WP_MATCHING (1 << 0)
#define PM_SCAN_CHECK_WPASYNC (1 << 1)
struct pm_scan_arg {
  __u64 size;
  __u64 flags;
  __u64 start;
  __u64 end;
  __u64 walk_end;
  __u64 vec;
  __u64 vec_len;
  __u64 max_pages;
  __u64 category_inverted;
  __u64 category_mask;
  __u64 category_anyof_mask;
  __u64 return_mask;
};
#define PROCMAP_QUERY _IOWR(PROCFS_IOCTL_MAGIC, 17, struct procmap_query)
enum procmap_query_flags {
  PROCMAP_QUERY_VMA_READABLE = 0x01,
  PROCMAP_QUERY_VMA_WRITABLE = 0x02,
  PROCMAP_QUERY_VMA_EXECUTABLE = 0x04,
  PROCMAP_QUERY_VMA_SHARED = 0x08,
  PROCMAP_QUERY_COVERING_OR_NEXT_VMA = 0x10,
  PROCMAP_QUERY_FILE_BACKED_VMA = 0x20,
};
struct procmap_query {
  __u64 size;
  __u64 query_flags;
  __u64 query_addr;
  __u64 vma_start;
  __u64 vma_end;
  __u64 vma_flags;
  __u64 vma_page_size;
  __u64 vma_offset;
  __u64 inode;
  __u32 dev_major;
  __u32 dev_minor;
  __u32 vma_name_size;
  __u32 build_id_size;
  __u64 vma_name_addr;
  __u64 build_id_addr;
};
#endif
