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
#ifndef __LINUX_MTD_BBM_H
#define __LINUX_MTD_BBM_H
#define NAND_MAX_CHIPS 8
struct nand_bbt_descr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int options;
 int pages[NAND_MAX_CHIPS];
 int offs;
 int veroffs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t version[NAND_MAX_CHIPS];
 int len;
 int maxblocks;
 int reserved_block_code;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t *pattern;
};
#define NAND_BBT_NRBITS_MSK 0x0000000F
#define NAND_BBT_1BIT 0x00000001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NAND_BBT_2BIT 0x00000002
#define NAND_BBT_4BIT 0x00000004
#define NAND_BBT_8BIT 0x00000008
#define NAND_BBT_LASTBLOCK 0x00000010
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NAND_BBT_ABSPAGE 0x00000020
#define NAND_BBT_SEARCH 0x00000040
#define NAND_BBT_PERCHIP 0x00000080
#define NAND_BBT_VERSION 0x00000100
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NAND_BBT_CREATE 0x00000200
#define NAND_BBT_SCANALLPAGES 0x00000400
#define NAND_BBT_SCANEMPTY 0x00000800
#define NAND_BBT_WRITE 0x00001000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NAND_BBT_SAVECONTENT 0x00002000
#define NAND_BBT_SCAN2NDPAGE 0x00004000
#define NAND_BBT_SCAN_MAXBLOCKS 4
#define ONENAND_BADBLOCK_POS 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct bbm_info {
 int bbt_erase_shift;
 int badblockpos;
 int options;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t *bbt;
 int (*isbad_bbt)(struct mtd_info *mtd, loff_t ofs, int allowbbt);
 struct nand_bbt_descr *badblock_pattern;
 void *priv;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif
