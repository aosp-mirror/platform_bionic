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
#ifndef __SOUND_TLV_H
#define __SOUND_TLV_H
#define SNDRV_CTL_TLVT_CONTAINER 0  
#define SNDRV_CTL_TLVT_DB_SCALE 1  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_CTL_TLVT_DB_LINEAR 2  
#define SNDRV_CTL_TLVT_DB_RANGE 3  
#define SNDRV_CTL_TLVT_DB_MINMAX 4  
#define SNDRV_CTL_TLVT_DB_MINMAX_MUTE 5  
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TLV_DB_SCALE_MASK 0xffff
#define TLV_DB_SCALE_MUTE 0x10000
#define TLV_DB_SCALE_ITEM(min, step, mute)   SNDRV_CTL_TLVT_DB_SCALE, 2 * sizeof(unsigned int),   (min), ((step) & TLV_DB_SCALE_MASK) | ((mute) ? TLV_DB_SCALE_MUTE : 0)
#define DECLARE_TLV_DB_SCALE(name, min, step, mute)   unsigned int name[] = { TLV_DB_SCALE_ITEM(min, step, mute) }
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TLV_DB_MINMAX_ITEM(min_dB, max_dB)   SNDRV_CTL_TLVT_DB_MINMAX, 2 * sizeof(unsigned int),   (min_dB), (max_dB)
#define TLV_DB_MINMAX_MUTE_ITEM(min_dB, max_dB)   SNDRV_CTL_TLVT_DB_MINMAX_MUTE, 2 * sizeof(unsigned int),   (min_dB), (max_dB)
#define DECLARE_TLV_DB_MINMAX(name, min_dB, max_dB)   unsigned int name[] = { TLV_DB_MINMAX_ITEM(min_dB, max_dB) }
#define DECLARE_TLV_DB_MINMAX_MUTE(name, min_dB, max_dB)   unsigned int name[] = { TLV_DB_MINMAX_MUTE_ITEM(min_dB, max_dB) }
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TLV_DB_LINEAR_ITEM(min_dB, max_dB)   SNDRV_CTL_TLVT_DB_LINEAR, 2 * sizeof(unsigned int),   (min_dB), (max_dB)
#define DECLARE_TLV_DB_LINEAR(name, min_dB, max_dB)   unsigned int name[] = { TLV_DB_LINEAR_ITEM(min_dB, max_dB) }
#define TLV_DB_RANGE_HEAD(num)   SNDRV_CTL_TLVT_DB_RANGE, 6 * (num) * sizeof(unsigned int)
#define TLV_DB_GAIN_MUTE -9999999
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif

