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
#ifndef _MD_U_H
#define _MD_U_H
#define RAID_VERSION _IOR (MD_MAJOR, 0x10, mdu_version_t)
#define GET_ARRAY_INFO _IOR (MD_MAJOR, 0x11, mdu_array_info_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define GET_DISK_INFO _IOR (MD_MAJOR, 0x12, mdu_disk_info_t)
#define PRINT_RAID_DEBUG _IO (MD_MAJOR, 0x13)
#define RAID_AUTORUN _IO (MD_MAJOR, 0x14)
#define GET_BITMAP_FILE _IOR (MD_MAJOR, 0x15, mdu_bitmap_file_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define CLEAR_ARRAY _IO (MD_MAJOR, 0x20)
#define ADD_NEW_DISK _IOW (MD_MAJOR, 0x21, mdu_disk_info_t)
#define HOT_REMOVE_DISK _IO (MD_MAJOR, 0x22)
#define SET_ARRAY_INFO _IOW (MD_MAJOR, 0x23, mdu_array_info_t)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SET_DISK_INFO _IO (MD_MAJOR, 0x24)
#define WRITE_RAID_INFO _IO (MD_MAJOR, 0x25)
#define UNPROTECT_ARRAY _IO (MD_MAJOR, 0x26)
#define PROTECT_ARRAY _IO (MD_MAJOR, 0x27)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HOT_ADD_DISK _IO (MD_MAJOR, 0x28)
#define SET_DISK_FAULTY _IO (MD_MAJOR, 0x29)
#define HOT_GENERATE_ERROR _IO (MD_MAJOR, 0x2a)
#define SET_BITMAP_FILE _IOW (MD_MAJOR, 0x2b, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RUN_ARRAY _IOW (MD_MAJOR, 0x30, mdu_param_t)
#define START_ARRAY _IO (MD_MAJOR, 0x31)
#define STOP_ARRAY _IO (MD_MAJOR, 0x32)
#define STOP_ARRAY_RO _IO (MD_MAJOR, 0x33)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RESTART_ARRAY_RW _IO (MD_MAJOR, 0x34)
typedef struct mdu_version_s {
 int major;
 int minor;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int patchlevel;
} mdu_version_t;
typedef struct mdu_array_info_s {
 int major_version;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int minor_version;
 int patch_version;
 int ctime;
 int level;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int size;
 int nr_disks;
 int raid_disks;
 int md_minor;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int not_persistent;
 int utime;
 int state;
 int active_disks;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int working_disks;
 int failed_disks;
 int spare_disks;
 int layout;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int chunk_size;
} mdu_array_info_t;
typedef struct mdu_disk_info_s {
 int number;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int major;
 int minor;
 int raid_disk;
 int state;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} mdu_disk_info_t;
typedef struct mdu_start_info_s {
 int major;
 int minor;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int raid_disk;
 int state;
} mdu_start_info_t;
typedef struct mdu_bitmap_file_s
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 char pathname[4096];
} mdu_bitmap_file_t;
typedef struct mdu_param_s
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
{
 int personality;
 int chunk_size;
 int max_fault;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} mdu_param_t;
#endif
