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
#ifndef __MTD_ABI_H__
#define __MTD_ABI_H__
struct erase_info_user {
 uint32_t start;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t length;
};
struct mtd_oob_buf {
 uint32_t start;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t length;
 unsigned char __user *ptr;
};
#define MTD_ABSENT 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTD_RAM 1
#define MTD_ROM 2
#define MTD_NORFLASH 3
#define MTD_NANDFLASH 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTD_DATAFLASH 6
#define MTD_WRITEABLE 0x400
#define MTD_BIT_WRITEABLE 0x800
#define MTD_NO_ERASE 0x1000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTD_STUPID_LOCK 0x2000
#define MTD_CAP_ROM 0
#define MTD_CAP_RAM (MTD_WRITEABLE | MTD_BIT_WRITEABLE | MTD_NO_ERASE)
#define MTD_CAP_NORFLASH (MTD_WRITEABLE | MTD_BIT_WRITEABLE)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTD_CAP_NANDFLASH (MTD_WRITEABLE)
#define MTD_NANDECC_OFF 0
#define MTD_NANDECC_PLACE 1
#define MTD_NANDECC_AUTOPLACE 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTD_NANDECC_PLACEONLY 3
#define MTD_NANDECC_AUTOPL_USR 4
#define MTD_OTP_OFF 0
#define MTD_OTP_FACTORY 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTD_OTP_USER 2
struct mtd_info_user {
 uint8_t type;
 uint32_t flags;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t size;
 uint32_t erasesize;
 uint32_t writesize;
 uint32_t oobsize;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t ecctype;
 uint32_t eccsize;
};
struct region_info_user {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t offset;
 uint32_t erasesize;
 uint32_t numblocks;
 uint32_t regionindex;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct otp_info {
 uint32_t start;
 uint32_t length;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t locked;
};
#define MEMGETINFO _IOR('M', 1, struct mtd_info_user)
#define MEMERASE _IOW('M', 2, struct erase_info_user)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MEMWRITEOOB _IOWR('M', 3, struct mtd_oob_buf)
#define MEMREADOOB _IOWR('M', 4, struct mtd_oob_buf)
#define MEMLOCK _IOW('M', 5, struct erase_info_user)
#define MEMUNLOCK _IOW('M', 6, struct erase_info_user)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MEMGETREGIONCOUNT _IOR('M', 7, int)
#define MEMGETREGIONINFO _IOWR('M', 8, struct region_info_user)
#define MEMSETOOBSEL _IOW('M', 9, struct nand_oobinfo)
#define MEMGETOOBSEL _IOR('M', 10, struct nand_oobinfo)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MEMGETBADBLOCK _IOW('M', 11, loff_t)
#define MEMSETBADBLOCK _IOW('M', 12, loff_t)
#define OTPSELECT _IOR('M', 13, int)
#define OTPGETREGIONCOUNT _IOW('M', 14, int)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define OTPGETREGIONINFO _IOW('M', 15, struct otp_info)
#define OTPLOCK _IOR('M', 16, struct otp_info)
#define ECCGETLAYOUT _IOR('M', 17, struct nand_ecclayout)
#define ECCGETSTATS _IOR('M', 18, struct mtd_ecc_stats)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MTDFILEMODE _IO('M', 19)
struct nand_oobinfo {
 uint32_t useecc;
 uint32_t eccbytes;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t oobfree[8][2];
 uint32_t eccpos[32];
};
struct nand_oobfree {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t offset;
 uint32_t length;
};
#define MTD_MAX_OOBFREE_ENTRIES 8
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct nand_ecclayout {
 uint32_t eccbytes;
 uint32_t eccpos[64];
 uint32_t oobavail;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct nand_oobfree oobfree[MTD_MAX_OOBFREE_ENTRIES];
};
struct mtd_ecc_stats {
 uint32_t corrected;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t failed;
 uint32_t badblocks;
 uint32_t bbtblocks;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum mtd_file_modes {
 MTD_MODE_NORMAL = MTD_OTP_OFF,
 MTD_MODE_OTP_FACTORY = MTD_OTP_FACTORY,
 MTD_MODE_OTP_USER = MTD_OTP_USER,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 MTD_MODE_RAW,
};
#endif
