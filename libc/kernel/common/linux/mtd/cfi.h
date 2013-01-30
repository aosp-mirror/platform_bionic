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
#ifndef __MTD_CFI_H__
#define __MTD_CFI_H__
#include <linux/delay.h>
#include <linux/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <linux/interrupt.h>
#include <linux/mtd/flashchip.h>
#include <linux/mtd/map.h>
#include <linux/mtd/cfi_endian.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define cfi_interleave_is_1(cfi) (0)
#define cfi_interleave_is_2(cfi) (0)
#define cfi_interleave_is_4(cfi) (0)
#define cfi_interleave_is_8(cfi) (0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define CFI_DEVICETYPE_X8 (8 / 8)
#define CFI_DEVICETYPE_X16 (16 / 8)
#define CFI_DEVICETYPE_X32 (32 / 8)
#define CFI_DEVICETYPE_X64 (64 / 8)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct cfi_ident {
 uint8_t qry[3];
 uint16_t P_ID;
 uint16_t P_ADR;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t A_ID;
 uint16_t A_ADR;
 uint8_t VccMin;
 uint8_t VccMax;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t VppMin;
 uint8_t VppMax;
 uint8_t WordWriteTimeoutTyp;
 uint8_t BufWriteTimeoutTyp;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t BlockEraseTimeoutTyp;
 uint8_t ChipEraseTimeoutTyp;
 uint8_t WordWriteTimeoutMax;
 uint8_t BufWriteTimeoutMax;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t BlockEraseTimeoutMax;
 uint8_t ChipEraseTimeoutMax;
 uint8_t DevSize;
 uint16_t InterfaceDesc;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t MaxBufWriteSize;
 uint8_t NumEraseRegions;
 uint32_t EraseRegionInfo[0];
} __attribute__((packed));
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct cfi_extquery {
 uint8_t pri[3];
 uint8_t MajorVersion;
 uint8_t MinorVersion;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} __attribute__((packed));
struct cfi_pri_intelext {
 uint8_t pri[3];
 uint8_t MajorVersion;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t MinorVersion;
 uint32_t FeatureSupport;
 uint8_t SuspendCmdSupport;
 uint16_t BlkStatusRegMask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t VccOptimal;
 uint8_t VppOptimal;
 uint8_t NumProtectionFields;
 uint16_t ProtRegAddr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t FactProtRegSize;
 uint8_t UserProtRegSize;
 uint8_t extra[0];
} __attribute__((packed));
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct cfi_intelext_otpinfo {
 uint32_t ProtRegAddr;
 uint16_t FactGroups;
 uint8_t FactProtRegSize;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t UserGroups;
 uint8_t UserProtRegSize;
} __attribute__((packed));
struct cfi_intelext_blockinfo {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t NumIdentBlocks;
 uint16_t BlockSize;
 uint16_t MinBlockEraseCycles;
 uint8_t BitsPerCell;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t BlockCap;
} __attribute__((packed));
struct cfi_intelext_regioninfo {
 uint16_t NumIdentPartitions;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t NumOpAllowed;
 uint8_t NumOpAllowedSimProgMode;
 uint8_t NumOpAllowedSimEraMode;
 uint8_t NumBlockTypes;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct cfi_intelext_blockinfo BlockTypes[1];
} __attribute__((packed));
struct cfi_intelext_programming_regioninfo {
 uint8_t ProgRegShift;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t Reserved1;
 uint8_t ControlValid;
 uint8_t Reserved2;
 uint8_t ControlInvalid;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t Reserved3;
} __attribute__((packed));
struct cfi_pri_amdstd {
 uint8_t pri[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t MajorVersion;
 uint8_t MinorVersion;
 uint8_t SiliconRevision;
 uint8_t EraseSuspend;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t BlkProt;
 uint8_t TmpBlkUnprotect;
 uint8_t BlkProtUnprot;
 uint8_t SimultaneousOps;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t BurstMode;
 uint8_t PageMode;
 uint8_t VppMin;
 uint8_t VppMax;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t TopBottom;
} __attribute__((packed));
struct cfi_pri_atmel {
 uint8_t pri[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t MajorVersion;
 uint8_t MinorVersion;
 uint8_t Features;
 uint8_t BottomBoot;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t BurstMode;
 uint8_t PageMode;
} __attribute__((packed));
struct cfi_pri_query {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t NumFields;
 uint32_t ProtField[1];
} __attribute__((packed));
struct cfi_bri_query {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint8_t PageModeReadCap;
 uint8_t NumFields;
 uint32_t ConfField[1];
} __attribute__((packed));
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define P_ID_NONE 0x0000
#define P_ID_INTEL_EXT 0x0001
#define P_ID_AMD_STD 0x0002
#define P_ID_INTEL_STD 0x0003
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define P_ID_AMD_EXT 0x0004
#define P_ID_WINBOND 0x0006
#define P_ID_ST_ADV 0x0020
#define P_ID_MITSUBISHI_STD 0x0100
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define P_ID_MITSUBISHI_EXT 0x0101
#define P_ID_SST_PAGE 0x0102
#define P_ID_INTEL_PERFORMANCE 0x0200
#define P_ID_INTEL_DATA 0x0210
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define P_ID_RESERVED 0xffff
#define CFI_MODE_CFI 1
#define CFI_MODE_JEDEC 0
struct cfi_private {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t cmdset;
 void *cmdset_priv;
 int interleave;
 int device_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int cfi_mode;
 int addr_unlock1;
 int addr_unlock2;
 struct mtd_info *(*cmdset_setup)(struct map_info *);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct cfi_ident *cfiq;
 int mfr, id;
 int numchips;
 unsigned long chipshift;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 const char *im_name;
 struct flchip chips[0];
};
#if BITS_PER_LONG >= 64
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#define CMD(x) cfi_build_cmd((x), map, cfi)
#if BITS_PER_LONG >= 64
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MERGESTATUS(x) cfi_merge_status((x), map, cfi)
struct cfi_extquery *cfi_read_pri(struct map_info *map, uint16_t adr, uint16_t size,
 const char* name);
struct cfi_fixup {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t mfr;
 uint16_t id;
 void (*fixup)(struct mtd_info *mtd, void* param);
 void* param;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define CFI_MFR_ANY 0xffff
#define CFI_ID_ANY 0xffff
#define CFI_MFR_AMD 0x0001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define CFI_MFR_ATMEL 0x001F
#define CFI_MFR_ST 0x0020
typedef int (*varsize_frob_t)(struct map_info *map, struct flchip *chip,
 unsigned long adr, int len, void *thunk);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
