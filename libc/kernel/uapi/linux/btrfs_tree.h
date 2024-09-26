/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _BTRFS_CTREE_H_
#define _BTRFS_CTREE_H_
#include <linux/btrfs.h>
#include <linux/types.h>
#include <stddef.h>
#define BTRFS_MAGIC 0x4D5F53665248425FULL
#define BTRFS_MAX_LEVEL 8
#define BTRFS_NAME_LEN 255
#define BTRFS_LINK_MAX 65535U
#define BTRFS_ROOT_TREE_OBJECTID 1ULL
#define BTRFS_EXTENT_TREE_OBJECTID 2ULL
#define BTRFS_CHUNK_TREE_OBJECTID 3ULL
#define BTRFS_DEV_TREE_OBJECTID 4ULL
#define BTRFS_FS_TREE_OBJECTID 5ULL
#define BTRFS_ROOT_TREE_DIR_OBJECTID 6ULL
#define BTRFS_CSUM_TREE_OBJECTID 7ULL
#define BTRFS_QUOTA_TREE_OBJECTID 8ULL
#define BTRFS_UUID_TREE_OBJECTID 9ULL
#define BTRFS_FREE_SPACE_TREE_OBJECTID 10ULL
#define BTRFS_BLOCK_GROUP_TREE_OBJECTID 11ULL
#define BTRFS_RAID_STRIPE_TREE_OBJECTID 12ULL
#define BTRFS_DEV_STATS_OBJECTID 0ULL
#define BTRFS_BALANCE_OBJECTID - 4ULL
#define BTRFS_ORPHAN_OBJECTID - 5ULL
#define BTRFS_TREE_LOG_OBJECTID - 6ULL
#define BTRFS_TREE_LOG_FIXUP_OBJECTID - 7ULL
#define BTRFS_TREE_RELOC_OBJECTID - 8ULL
#define BTRFS_DATA_RELOC_TREE_OBJECTID - 9ULL
#define BTRFS_EXTENT_CSUM_OBJECTID - 10ULL
#define BTRFS_FREE_SPACE_OBJECTID - 11ULL
#define BTRFS_FREE_INO_OBJECTID - 12ULL
#define BTRFS_MULTIPLE_OBJECTIDS - 255ULL
#define BTRFS_FIRST_FREE_OBJECTID 256ULL
#define BTRFS_LAST_FREE_OBJECTID - 256ULL
#define BTRFS_FIRST_CHUNK_TREE_OBJECTID 256ULL
#define BTRFS_DEV_ITEMS_OBJECTID 1ULL
#define BTRFS_BTREE_INODE_OBJECTID 1
#define BTRFS_EMPTY_SUBVOL_DIR_OBJECTID 2
#define BTRFS_DEV_REPLACE_DEVID 0ULL
#define BTRFS_INODE_ITEM_KEY 1
#define BTRFS_INODE_REF_KEY 12
#define BTRFS_INODE_EXTREF_KEY 13
#define BTRFS_XATTR_ITEM_KEY 24
#define BTRFS_VERITY_DESC_ITEM_KEY 36
#define BTRFS_VERITY_MERKLE_ITEM_KEY 37
#define BTRFS_ORPHAN_ITEM_KEY 48
#define BTRFS_DIR_LOG_ITEM_KEY 60
#define BTRFS_DIR_LOG_INDEX_KEY 72
#define BTRFS_DIR_ITEM_KEY 84
#define BTRFS_DIR_INDEX_KEY 96
#define BTRFS_EXTENT_DATA_KEY 108
#define BTRFS_EXTENT_CSUM_KEY 128
#define BTRFS_ROOT_ITEM_KEY 132
#define BTRFS_ROOT_BACKREF_KEY 144
#define BTRFS_ROOT_REF_KEY 156
#define BTRFS_EXTENT_ITEM_KEY 168
#define BTRFS_METADATA_ITEM_KEY 169
#define BTRFS_EXTENT_OWNER_REF_KEY 172
#define BTRFS_TREE_BLOCK_REF_KEY 176
#define BTRFS_EXTENT_DATA_REF_KEY 178
#define BTRFS_SHARED_BLOCK_REF_KEY 182
#define BTRFS_SHARED_DATA_REF_KEY 184
#define BTRFS_BLOCK_GROUP_ITEM_KEY 192
#define BTRFS_FREE_SPACE_INFO_KEY 198
#define BTRFS_FREE_SPACE_EXTENT_KEY 199
#define BTRFS_FREE_SPACE_BITMAP_KEY 200
#define BTRFS_DEV_EXTENT_KEY 204
#define BTRFS_DEV_ITEM_KEY 216
#define BTRFS_CHUNK_ITEM_KEY 228
#define BTRFS_RAID_STRIPE_KEY 230
#define BTRFS_QGROUP_STATUS_KEY 240
#define BTRFS_QGROUP_INFO_KEY 242
#define BTRFS_QGROUP_LIMIT_KEY 244
#define BTRFS_QGROUP_RELATION_KEY 246
#define BTRFS_BALANCE_ITEM_KEY 248
#define BTRFS_TEMPORARY_ITEM_KEY 248
#define BTRFS_DEV_STATS_KEY 249
#define BTRFS_PERSISTENT_ITEM_KEY 249
#define BTRFS_DEV_REPLACE_KEY 250
#if BTRFS_UUID_SIZE != 16
#error "UUID items require BTRFS_UUID_SIZE == 16!"
#endif
#define BTRFS_UUID_KEY_SUBVOL 251
#define BTRFS_UUID_KEY_RECEIVED_SUBVOL 252
#define BTRFS_STRING_ITEM_KEY 253
#define BTRFS_MAX_METADATA_BLOCKSIZE 65536
#define BTRFS_CSUM_SIZE 32
enum btrfs_csum_type {
  BTRFS_CSUM_TYPE_CRC32 = 0,
  BTRFS_CSUM_TYPE_XXHASH = 1,
  BTRFS_CSUM_TYPE_SHA256 = 2,
  BTRFS_CSUM_TYPE_BLAKE2 = 3,
};
#define BTRFS_FT_UNKNOWN 0
#define BTRFS_FT_REG_FILE 1
#define BTRFS_FT_DIR 2
#define BTRFS_FT_CHRDEV 3
#define BTRFS_FT_BLKDEV 4
#define BTRFS_FT_FIFO 5
#define BTRFS_FT_SOCK 6
#define BTRFS_FT_SYMLINK 7
#define BTRFS_FT_XATTR 8
#define BTRFS_FT_MAX 9
#define BTRFS_FT_ENCRYPTED 0x80
#define BTRFS_INODE_NODATASUM (1U << 0)
#define BTRFS_INODE_NODATACOW (1U << 1)
#define BTRFS_INODE_READONLY (1U << 2)
#define BTRFS_INODE_NOCOMPRESS (1U << 3)
#define BTRFS_INODE_PREALLOC (1U << 4)
#define BTRFS_INODE_SYNC (1U << 5)
#define BTRFS_INODE_IMMUTABLE (1U << 6)
#define BTRFS_INODE_APPEND (1U << 7)
#define BTRFS_INODE_NODUMP (1U << 8)
#define BTRFS_INODE_NOATIME (1U << 9)
#define BTRFS_INODE_DIRSYNC (1U << 10)
#define BTRFS_INODE_COMPRESS (1U << 11)
#define BTRFS_INODE_ROOT_ITEM_INIT (1U << 31)
#define BTRFS_INODE_FLAG_MASK (BTRFS_INODE_NODATASUM | BTRFS_INODE_NODATACOW | BTRFS_INODE_READONLY | BTRFS_INODE_NOCOMPRESS | BTRFS_INODE_PREALLOC | BTRFS_INODE_SYNC | BTRFS_INODE_IMMUTABLE | BTRFS_INODE_APPEND | BTRFS_INODE_NODUMP | BTRFS_INODE_NOATIME | BTRFS_INODE_DIRSYNC | BTRFS_INODE_COMPRESS | BTRFS_INODE_ROOT_ITEM_INIT)
#define BTRFS_INODE_RO_VERITY (1U << 0)
#define BTRFS_INODE_RO_FLAG_MASK (BTRFS_INODE_RO_VERITY)
struct btrfs_disk_key {
  __le64 objectid;
  __u8 type;
  __le64 offset;
} __attribute__((__packed__));
struct btrfs_key {
  __u64 objectid;
  __u8 type;
  __u64 offset;
} __attribute__((__packed__));
struct btrfs_header {
  __u8 csum[BTRFS_CSUM_SIZE];
  __u8 fsid[BTRFS_FSID_SIZE];
  __le64 bytenr;
  __le64 flags;
  __u8 chunk_tree_uuid[BTRFS_UUID_SIZE];
  __le64 generation;
  __le64 owner;
  __le32 nritems;
  __u8 level;
} __attribute__((__packed__));
#define BTRFS_SYSTEM_CHUNK_ARRAY_SIZE 2048
#define BTRFS_NUM_BACKUP_ROOTS 4
struct btrfs_root_backup {
  __le64 tree_root;
  __le64 tree_root_gen;
  __le64 chunk_root;
  __le64 chunk_root_gen;
  __le64 extent_root;
  __le64 extent_root_gen;
  __le64 fs_root;
  __le64 fs_root_gen;
  __le64 dev_root;
  __le64 dev_root_gen;
  __le64 csum_root;
  __le64 csum_root_gen;
  __le64 total_bytes;
  __le64 bytes_used;
  __le64 num_devices;
  __le64 unused_64[4];
  __u8 tree_root_level;
  __u8 chunk_root_level;
  __u8 extent_root_level;
  __u8 fs_root_level;
  __u8 dev_root_level;
  __u8 csum_root_level;
  __u8 unused_8[10];
} __attribute__((__packed__));
struct btrfs_item {
  struct btrfs_disk_key key;
  __le32 offset;
  __le32 size;
} __attribute__((__packed__));
struct btrfs_leaf {
  struct btrfs_header header;
  struct btrfs_item items[];
} __attribute__((__packed__));
struct btrfs_key_ptr {
  struct btrfs_disk_key key;
  __le64 blockptr;
  __le64 generation;
} __attribute__((__packed__));
struct btrfs_node {
  struct btrfs_header header;
  struct btrfs_key_ptr ptrs[];
} __attribute__((__packed__));
struct btrfs_dev_item {
  __le64 devid;
  __le64 total_bytes;
  __le64 bytes_used;
  __le32 io_align;
  __le32 io_width;
  __le32 sector_size;
  __le64 type;
  __le64 generation;
  __le64 start_offset;
  __le32 dev_group;
  __u8 seek_speed;
  __u8 bandwidth;
  __u8 uuid[BTRFS_UUID_SIZE];
  __u8 fsid[BTRFS_UUID_SIZE];
} __attribute__((__packed__));
struct btrfs_stripe {
  __le64 devid;
  __le64 offset;
  __u8 dev_uuid[BTRFS_UUID_SIZE];
} __attribute__((__packed__));
struct btrfs_chunk {
  __le64 length;
  __le64 owner;
  __le64 stripe_len;
  __le64 type;
  __le32 io_align;
  __le32 io_width;
  __le32 sector_size;
  __le16 num_stripes;
  __le16 sub_stripes;
  struct btrfs_stripe stripe;
} __attribute__((__packed__));
struct btrfs_super_block {
  __u8 csum[BTRFS_CSUM_SIZE];
  __u8 fsid[BTRFS_FSID_SIZE];
  __le64 bytenr;
  __le64 flags;
  __le64 magic;
  __le64 generation;
  __le64 root;
  __le64 chunk_root;
  __le64 log_root;
  __le64 __unused_log_root_transid;
  __le64 total_bytes;
  __le64 bytes_used;
  __le64 root_dir_objectid;
  __le64 num_devices;
  __le32 sectorsize;
  __le32 nodesize;
  __le32 __unused_leafsize;
  __le32 stripesize;
  __le32 sys_chunk_array_size;
  __le64 chunk_root_generation;
  __le64 compat_flags;
  __le64 compat_ro_flags;
  __le64 incompat_flags;
  __le16 csum_type;
  __u8 root_level;
  __u8 chunk_root_level;
  __u8 log_root_level;
  struct btrfs_dev_item dev_item;
  char label[BTRFS_LABEL_SIZE];
  __le64 cache_generation;
  __le64 uuid_tree_generation;
  __u8 metadata_uuid[BTRFS_FSID_SIZE];
  __u64 nr_global_roots;
  __le64 reserved[27];
  __u8 sys_chunk_array[BTRFS_SYSTEM_CHUNK_ARRAY_SIZE];
  struct btrfs_root_backup super_roots[BTRFS_NUM_BACKUP_ROOTS];
  __u8 padding[565];
} __attribute__((__packed__));
#define BTRFS_FREE_SPACE_EXTENT 1
#define BTRFS_FREE_SPACE_BITMAP 2
struct btrfs_free_space_entry {
  __le64 offset;
  __le64 bytes;
  __u8 type;
} __attribute__((__packed__));
struct btrfs_free_space_header {
  struct btrfs_disk_key location;
  __le64 generation;
  __le64 num_entries;
  __le64 num_bitmaps;
} __attribute__((__packed__));
struct btrfs_raid_stride {
  __le64 devid;
  __le64 physical;
} __attribute__((__packed__));
#define BTRFS_STRIPE_RAID0 1
#define BTRFS_STRIPE_RAID1 2
#define BTRFS_STRIPE_DUP 3
#define BTRFS_STRIPE_RAID10 4
#define BTRFS_STRIPE_RAID5 5
#define BTRFS_STRIPE_RAID6 6
#define BTRFS_STRIPE_RAID1C3 7
#define BTRFS_STRIPE_RAID1C4 8
struct btrfs_stripe_extent {
  __u8 encoding;
  __u8 reserved[7];
  struct btrfs_raid_stride strides[];
} __attribute__((__packed__));
#define BTRFS_HEADER_FLAG_WRITTEN (1ULL << 0)
#define BTRFS_HEADER_FLAG_RELOC (1ULL << 1)
#define BTRFS_SUPER_FLAG_ERROR (1ULL << 2)
#define BTRFS_SUPER_FLAG_SEEDING (1ULL << 32)
#define BTRFS_SUPER_FLAG_METADUMP (1ULL << 33)
#define BTRFS_SUPER_FLAG_METADUMP_V2 (1ULL << 34)
#define BTRFS_SUPER_FLAG_CHANGING_FSID (1ULL << 35)
#define BTRFS_SUPER_FLAG_CHANGING_FSID_V2 (1ULL << 36)
struct btrfs_extent_item {
  __le64 refs;
  __le64 generation;
  __le64 flags;
} __attribute__((__packed__));
struct btrfs_extent_item_v0 {
  __le32 refs;
} __attribute__((__packed__));
#define BTRFS_EXTENT_FLAG_DATA (1ULL << 0)
#define BTRFS_EXTENT_FLAG_TREE_BLOCK (1ULL << 1)
#define BTRFS_BLOCK_FLAG_FULL_BACKREF (1ULL << 8)
#define BTRFS_BACKREF_REV_MAX 256
#define BTRFS_BACKREF_REV_SHIFT 56
#define BTRFS_BACKREF_REV_MASK (((u64) BTRFS_BACKREF_REV_MAX - 1) << BTRFS_BACKREF_REV_SHIFT)
#define BTRFS_OLD_BACKREF_REV 0
#define BTRFS_MIXED_BACKREF_REV 1
#define BTRFS_EXTENT_FLAG_SUPER (1ULL << 48)
struct btrfs_tree_block_info {
  struct btrfs_disk_key key;
  __u8 level;
} __attribute__((__packed__));
struct btrfs_extent_data_ref {
  __le64 root;
  __le64 objectid;
  __le64 offset;
  __le32 count;
} __attribute__((__packed__));
struct btrfs_shared_data_ref {
  __le32 count;
} __attribute__((__packed__));
struct btrfs_extent_owner_ref {
  __le64 root_id;
} __attribute__((__packed__));
struct btrfs_extent_inline_ref {
  __u8 type;
  __le64 offset;
} __attribute__((__packed__));
struct btrfs_dev_extent {
  __le64 chunk_tree;
  __le64 chunk_objectid;
  __le64 chunk_offset;
  __le64 length;
  __u8 chunk_tree_uuid[BTRFS_UUID_SIZE];
} __attribute__((__packed__));
struct btrfs_inode_ref {
  __le64 index;
  __le16 name_len;
} __attribute__((__packed__));
struct btrfs_inode_extref {
  __le64 parent_objectid;
  __le64 index;
  __le16 name_len;
  __u8 name[];
} __attribute__((__packed__));
struct btrfs_timespec {
  __le64 sec;
  __le32 nsec;
} __attribute__((__packed__));
struct btrfs_inode_item {
  __le64 generation;
  __le64 transid;
  __le64 size;
  __le64 nbytes;
  __le64 block_group;
  __le32 nlink;
  __le32 uid;
  __le32 gid;
  __le32 mode;
  __le64 rdev;
  __le64 flags;
  __le64 sequence;
  __le64 reserved[4];
  struct btrfs_timespec atime;
  struct btrfs_timespec ctime;
  struct btrfs_timespec mtime;
  struct btrfs_timespec otime;
} __attribute__((__packed__));
struct btrfs_dir_log_item {
  __le64 end;
} __attribute__((__packed__));
struct btrfs_dir_item {
  struct btrfs_disk_key location;
  __le64 transid;
  __le16 data_len;
  __le16 name_len;
  __u8 type;
} __attribute__((__packed__));
#define BTRFS_ROOT_SUBVOL_RDONLY (1ULL << 0)
#define BTRFS_ROOT_SUBVOL_DEAD (1ULL << 48)
struct btrfs_root_item {
  struct btrfs_inode_item inode;
  __le64 generation;
  __le64 root_dirid;
  __le64 bytenr;
  __le64 byte_limit;
  __le64 bytes_used;
  __le64 last_snapshot;
  __le64 flags;
  __le32 refs;
  struct btrfs_disk_key drop_progress;
  __u8 drop_level;
  __u8 level;
  __le64 generation_v2;
  __u8 uuid[BTRFS_UUID_SIZE];
  __u8 parent_uuid[BTRFS_UUID_SIZE];
  __u8 received_uuid[BTRFS_UUID_SIZE];
  __le64 ctransid;
  __le64 otransid;
  __le64 stransid;
  __le64 rtransid;
  struct btrfs_timespec ctime;
  struct btrfs_timespec otime;
  struct btrfs_timespec stime;
  struct btrfs_timespec rtime;
  __le64 reserved[8];
} __attribute__((__packed__));
struct btrfs_root_ref {
  __le64 dirid;
  __le64 sequence;
  __le16 name_len;
} __attribute__((__packed__));
struct btrfs_disk_balance_args {
  __le64 profiles;
  union {
    __le64 usage;
    struct {
      __le32 usage_min;
      __le32 usage_max;
    };
  };
  __le64 devid;
  __le64 pstart;
  __le64 pend;
  __le64 vstart;
  __le64 vend;
  __le64 target;
  __le64 flags;
  union {
    __le64 limit;
    struct {
      __le32 limit_min;
      __le32 limit_max;
    };
  };
  __le32 stripes_min;
  __le32 stripes_max;
  __le64 unused[6];
} __attribute__((__packed__));
struct btrfs_balance_item {
  __le64 flags;
  struct btrfs_disk_balance_args data;
  struct btrfs_disk_balance_args meta;
  struct btrfs_disk_balance_args sys;
  __le64 unused[4];
} __attribute__((__packed__));
enum {
  BTRFS_FILE_EXTENT_INLINE = 0,
  BTRFS_FILE_EXTENT_REG = 1,
  BTRFS_FILE_EXTENT_PREALLOC = 2,
  BTRFS_NR_FILE_EXTENT_TYPES = 3,
};
struct btrfs_file_extent_item {
  __le64 generation;
  __le64 ram_bytes;
  __u8 compression;
  __u8 encryption;
  __le16 other_encoding;
  __u8 type;
  __le64 disk_bytenr;
  __le64 disk_num_bytes;
  __le64 offset;
  __le64 num_bytes;
} __attribute__((__packed__));
struct btrfs_csum_item {
  __u8 csum;
} __attribute__((__packed__));
struct btrfs_dev_stats_item {
  __le64 values[BTRFS_DEV_STAT_VALUES_MAX];
} __attribute__((__packed__));
#define BTRFS_DEV_REPLACE_ITEM_CONT_READING_FROM_SRCDEV_MODE_ALWAYS 0
#define BTRFS_DEV_REPLACE_ITEM_CONT_READING_FROM_SRCDEV_MODE_AVOID 1
struct btrfs_dev_replace_item {
  __le64 src_devid;
  __le64 cursor_left;
  __le64 cursor_right;
  __le64 cont_reading_from_srcdev_mode;
  __le64 replace_state;
  __le64 time_started;
  __le64 time_stopped;
  __le64 num_write_errors;
  __le64 num_uncorrectable_read_errors;
} __attribute__((__packed__));
#define BTRFS_BLOCK_GROUP_DATA (1ULL << 0)
#define BTRFS_BLOCK_GROUP_SYSTEM (1ULL << 1)
#define BTRFS_BLOCK_GROUP_METADATA (1ULL << 2)
#define BTRFS_BLOCK_GROUP_RAID0 (1ULL << 3)
#define BTRFS_BLOCK_GROUP_RAID1 (1ULL << 4)
#define BTRFS_BLOCK_GROUP_DUP (1ULL << 5)
#define BTRFS_BLOCK_GROUP_RAID10 (1ULL << 6)
#define BTRFS_BLOCK_GROUP_RAID5 (1ULL << 7)
#define BTRFS_BLOCK_GROUP_RAID6 (1ULL << 8)
#define BTRFS_BLOCK_GROUP_RAID1C3 (1ULL << 9)
#define BTRFS_BLOCK_GROUP_RAID1C4 (1ULL << 10)
#define BTRFS_BLOCK_GROUP_RESERVED (BTRFS_AVAIL_ALLOC_BIT_SINGLE | BTRFS_SPACE_INFO_GLOBAL_RSV)
#define BTRFS_BLOCK_GROUP_TYPE_MASK (BTRFS_BLOCK_GROUP_DATA | BTRFS_BLOCK_GROUP_SYSTEM | BTRFS_BLOCK_GROUP_METADATA)
#define BTRFS_BLOCK_GROUP_PROFILE_MASK (BTRFS_BLOCK_GROUP_RAID0 | BTRFS_BLOCK_GROUP_RAID1 | BTRFS_BLOCK_GROUP_RAID1C3 | BTRFS_BLOCK_GROUP_RAID1C4 | BTRFS_BLOCK_GROUP_RAID5 | BTRFS_BLOCK_GROUP_RAID6 | BTRFS_BLOCK_GROUP_DUP | BTRFS_BLOCK_GROUP_RAID10)
#define BTRFS_BLOCK_GROUP_RAID56_MASK (BTRFS_BLOCK_GROUP_RAID5 | BTRFS_BLOCK_GROUP_RAID6)
#define BTRFS_BLOCK_GROUP_RAID1_MASK (BTRFS_BLOCK_GROUP_RAID1 | BTRFS_BLOCK_GROUP_RAID1C3 | BTRFS_BLOCK_GROUP_RAID1C4)
#define BTRFS_AVAIL_ALLOC_BIT_SINGLE (1ULL << 48)
#define BTRFS_SPACE_INFO_GLOBAL_RSV (1ULL << 49)
#define BTRFS_EXTENDED_PROFILE_MASK (BTRFS_BLOCK_GROUP_PROFILE_MASK | BTRFS_AVAIL_ALLOC_BIT_SINGLE)
struct btrfs_block_group_item {
  __le64 used;
  __le64 chunk_objectid;
  __le64 flags;
} __attribute__((__packed__));
struct btrfs_free_space_info {
  __le32 extent_count;
  __le32 flags;
} __attribute__((__packed__));
#define BTRFS_FREE_SPACE_USING_BITMAPS (1ULL << 0)
#define BTRFS_QGROUP_LEVEL_SHIFT 48
#define BTRFS_QGROUP_STATUS_FLAG_ON (1ULL << 0)
#define BTRFS_QGROUP_STATUS_FLAG_RESCAN (1ULL << 1)
#define BTRFS_QGROUP_STATUS_FLAG_INCONSISTENT (1ULL << 2)
#define BTRFS_QGROUP_STATUS_FLAG_SIMPLE_MODE (1ULL << 3)
#define BTRFS_QGROUP_STATUS_FLAGS_MASK (BTRFS_QGROUP_STATUS_FLAG_ON | BTRFS_QGROUP_STATUS_FLAG_RESCAN | BTRFS_QGROUP_STATUS_FLAG_INCONSISTENT | BTRFS_QGROUP_STATUS_FLAG_SIMPLE_MODE)
#define BTRFS_QGROUP_STATUS_VERSION 1
struct btrfs_qgroup_status_item {
  __le64 version;
  __le64 generation;
  __le64 flags;
  __le64 rescan;
  __le64 enable_gen;
} __attribute__((__packed__));
struct btrfs_qgroup_info_item {
  __le64 generation;
  __le64 rfer;
  __le64 rfer_cmpr;
  __le64 excl;
  __le64 excl_cmpr;
} __attribute__((__packed__));
struct btrfs_qgroup_limit_item {
  __le64 flags;
  __le64 max_rfer;
  __le64 max_excl;
  __le64 rsv_rfer;
  __le64 rsv_excl;
} __attribute__((__packed__));
struct btrfs_verity_descriptor_item {
  __le64 size;
  __le64 reserved[2];
  __u8 encryption;
} __attribute__((__packed__));
#endif
