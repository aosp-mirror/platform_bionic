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
#ifndef _LINUX_GENHD_H
#define _LINUX_GENHD_H
#include <linux/types.h>
enum {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DOS_EXTENDED_PARTITION = 5,
 LINUX_EXTENDED_PARTITION = 0x85,
 WIN98_EXTENDED_PARTITION = 0x0f,
 LINUX_SWAP_PARTITION = 0x82,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 LINUX_RAID_PARTITION = 0xfd,
 SOLARIS_X86_PARTITION = LINUX_SWAP_PARTITION,
 NEW_SOLARIS_X86_PARTITION = 0xbf,
 DM6_AUX1PARTITION = 0x51,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 DM6_AUX3PARTITION = 0x53,
 DM6_PARTITION = 0x54,
 EZD_PARTITION = 0x55,
 FREEBSD_PARTITION = 0xa5,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 OPENBSD_PARTITION = 0xa6,
 NETBSD_PARTITION = 0xa9,
 BSDI_PARTITION = 0xb7,
 MINIX_PARTITION = 0x81,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 UNIXWARE_PARTITION = 0x63,
};
struct partition {
 unsigned char boot_ind;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char head;
 unsigned char sector;
 unsigned char cyl;
 unsigned char sys_ind;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char end_head;
 unsigned char end_sector;
 unsigned char end_cyl;
 unsigned int start_sect;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int nr_sects;
} __attribute__((packed));
#endif
