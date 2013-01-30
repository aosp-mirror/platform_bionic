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
#ifndef __MTD_NFTL_H__
#define __MTD_NFTL_H__
#include <linux/mtd/mtd.h>
#include <linux/mtd/blktrans.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <mtd/nftl-user.h>
#define BLOCK_NIL 0xffff
#define BLOCK_FREE 0xfffe
#define BLOCK_NOTEXPLORED 0xfffd
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BLOCK_RESERVED 0xfffc
struct NFTLrecord {
 struct mtd_blktrans_dev mbd;
 __u16 MediaUnit, SpareMediaUnit;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u32 EraseSize;
 struct NFTLMediaHeader MediaHdr;
 int usecount;
 unsigned char heads;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char sectors;
 unsigned short cylinders;
 __u16 numvunits;
 __u16 lastEUN;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 numfreeEUNs;
 __u16 LastFreeEUN;
 int head,sect,cyl;
 __u16 *EUNtable;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u16 *ReplUnitTable;
 unsigned int nb_blocks;
 unsigned int nb_boot_blocks;
 struct erase_info instr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nand_ecclayout oobinfo;
};
#ifndef NFTL_MAJOR
#define NFTL_MAJOR 93
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define MAX_NFTLS 16
#define MAX_SECTORS_PER_UNIT 64
#define NFTL_PARTN_BITS 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
