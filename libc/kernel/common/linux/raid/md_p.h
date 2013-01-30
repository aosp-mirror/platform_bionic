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
#ifndef _MD_P_H
#define _MD_P_H
#define MD_RESERVED_BYTES (64 * 1024)
#define MD_RESERVED_SECTORS (MD_RESERVED_BYTES / 512)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_RESERVED_BLOCKS (MD_RESERVED_BYTES / BLOCK_SIZE)
#define MD_NEW_SIZE_SECTORS(x) ((x & ~(MD_RESERVED_SECTORS - 1)) - MD_RESERVED_SECTORS)
#define MD_NEW_SIZE_BLOCKS(x) ((x & ~(MD_RESERVED_BLOCKS - 1)) - MD_RESERVED_BLOCKS)
#define MD_SB_BYTES 4096
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_SB_WORDS (MD_SB_BYTES / 4)
#define MD_SB_BLOCKS (MD_SB_BYTES / BLOCK_SIZE)
#define MD_SB_SECTORS (MD_SB_BYTES / 512)
#define MD_SB_GENERIC_OFFSET 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_SB_PERSONALITY_OFFSET 64
#define MD_SB_DISKS_OFFSET 128
#define MD_SB_DESCRIPTOR_OFFSET 992
#define MD_SB_GENERIC_CONSTANT_WORDS 32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_SB_GENERIC_STATE_WORDS 32
#define MD_SB_GENERIC_WORDS (MD_SB_GENERIC_CONSTANT_WORDS + MD_SB_GENERIC_STATE_WORDS)
#define MD_SB_PERSONALITY_WORDS 64
#define MD_SB_DESCRIPTOR_WORDS 32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_SB_DISKS 27
#define MD_SB_DISKS_WORDS (MD_SB_DISKS*MD_SB_DESCRIPTOR_WORDS)
#define MD_SB_RESERVED_WORDS (1024 - MD_SB_GENERIC_WORDS - MD_SB_PERSONALITY_WORDS - MD_SB_DISKS_WORDS - MD_SB_DESCRIPTOR_WORDS)
#define MD_SB_EQUAL_WORDS (MD_SB_GENERIC_WORDS + MD_SB_PERSONALITY_WORDS + MD_SB_DISKS_WORDS)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_DISK_FAULTY 0
#define MD_DISK_ACTIVE 1
#define MD_DISK_SYNC 2
#define MD_DISK_REMOVED 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_DISK_WRITEMOSTLY 9
typedef struct mdp_device_descriptor_s {
 __u32 number;
 __u32 major;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 minor;
 __u32 raid_disk;
 __u32 state;
 __u32 reserved[MD_SB_DESCRIPTOR_WORDS - 5];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} mdp_disk_t;
#define MD_SB_MAGIC 0xa92b4efc
#define MD_SB_CLEAN 0
#define MD_SB_ERRORS 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_SB_BITMAP_PRESENT 8
typedef struct mdp_superblock_s {
 __u32 md_magic;
 __u32 major_version;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 minor_version;
 __u32 patch_version;
 __u32 gvalid_words;
 __u32 set_uuid0;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 ctime;
 __u32 level;
 __u32 size;
 __u32 nr_disks;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 raid_disks;
 __u32 md_minor;
 __u32 not_persistent;
 __u32 set_uuid1;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 set_uuid2;
 __u32 set_uuid3;
 __u32 gstate_creserved[MD_SB_GENERIC_CONSTANT_WORDS - 16];
 __u32 utime;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 state;
 __u32 active_disks;
 __u32 working_disks;
 __u32 failed_disks;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 spare_disks;
 __u32 sb_csum;
#ifdef __BIG_ENDIAN
 __u32 events_hi;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 events_lo;
 __u32 cp_events_hi;
 __u32 cp_events_lo;
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 events_lo;
 __u32 events_hi;
 __u32 cp_events_lo;
 __u32 cp_events_hi;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
 __u32 recovery_cp;
 __u64 reshape_position;
 __u32 new_level;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 delta_disks;
 __u32 new_layout;
 __u32 new_chunk;
 __u32 gstate_sreserved[MD_SB_GENERIC_STATE_WORDS - 18];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 layout;
 __u32 chunk_size;
 __u32 root_pv;
 __u32 root_block;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 pstate_reserved[MD_SB_PERSONALITY_WORDS - 4];
 mdp_disk_t disks[MD_SB_DISKS];
 __u32 reserved[MD_SB_RESERVED_WORDS];
 mdp_disk_t this_disk;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} mdp_super_t;
struct mdp_superblock_1 {
 __u32 magic;
 __u32 major_version;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 feature_map;
 __u32 pad0;
 __u8 set_uuid[16];
 char set_name[32];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 ctime;
 __u32 level;
 __u32 layout;
 __u64 size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 chunksize;
 __u32 raid_disks;
 __u32 bitmap_offset;
 __u32 new_level;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 reshape_position;
 __u32 delta_disks;
 __u32 new_layout;
 __u32 new_chunk;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 pad1[128-124];
 __u64 data_offset;
 __u64 data_size;
 __u64 super_offset;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 recovery_offset;
 __u32 dev_number;
 __u32 cnt_corrected_read;
 __u8 device_uuid[16];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 devflags;
#define WriteMostly1 1
 __u8 pad2[64-57];
 __u64 utime;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 events;
 __u64 resync_offset;
 __u32 sb_csum;
 __u32 max_dev;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 pad3[64-32];
 __u16 dev_roles[0];
};
#define MD_FEATURE_BITMAP_OFFSET 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MD_FEATURE_RECOVERY_OFFSET 2
#define MD_FEATURE_RESHAPE_ACTIVE 4
#define MD_FEATURE_ALL (1|2|4)
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
