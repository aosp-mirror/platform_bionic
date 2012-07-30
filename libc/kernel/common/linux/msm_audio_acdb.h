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
#ifndef __MSM_AUDIO_ACDB_H
#define __MSM_AUDIO_ACDB_H
#include <linux/msm_audio.h>
#define AUDIO_SET_VOCPROC_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+0), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_VOCPROC_STREAM_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+1), unsigned)
#define AUDIO_SET_VOCPROC_VOL_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+2), unsigned)
#define AUDIO_SET_AUDPROC_RX_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+3), unsigned)
#define AUDIO_SET_AUDPROC_RX_STREAM_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+4), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_AUDPROC_RX_VOL_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+5), unsigned)
#define AUDIO_SET_AUDPROC_TX_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+6), unsigned)
#define AUDIO_SET_AUDPROC_TX_STREAM_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+7), unsigned)
#define AUDIO_SET_AUDPROC_TX_VOL_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+8), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_SIDETONE_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+9), unsigned)
#define AUDIO_SET_ANC_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+10), unsigned)
#define AUDIO_SET_VOICE_RX_TOPOLOGY _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+11), unsigned)
#define AUDIO_SET_VOICE_TX_TOPOLOGY _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+12), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_ADM_RX_TOPOLOGY _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+13), unsigned)
#define AUDIO_SET_ADM_TX_TOPOLOGY _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+14), unsigned)
#define AUDIO_SET_ASM_TOPOLOGY _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+15), unsigned)
#define AUDIO_SET_AFE_TX_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+16), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_AFE_RX_CAL _IOW(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_COMMON_IOCTL_NUM+17), unsigned)
#define AUDIO_MAX_ACDB_IOCTL (AUDIO_MAX_COMMON_IOCTL_NUM+30)
struct cal_block {
 uint32_t cal_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t cal_offset;
};
struct sidetone_cal {
 uint16_t enable;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t gain;
};
#define AUDIO_GET_RTAC_ADM_INFO _IOR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+1), unsigned)
#define AUDIO_GET_RTAC_VOICE_INFO _IOR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+2), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_GET_RTAC_ADM_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+3), unsigned)
#define AUDIO_SET_RTAC_ADM_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+4), unsigned)
#define AUDIO_GET_RTAC_ASM_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+5), unsigned)
#define AUDIO_SET_RTAC_ASM_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+6), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_GET_RTAC_CVS_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+7), unsigned)
#define AUDIO_SET_RTAC_CVS_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+8), unsigned)
#define AUDIO_GET_RTAC_CVP_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+9), unsigned)
#define AUDIO_SET_RTAC_CVP_CAL _IOWR(AUDIO_IOCTL_MAGIC,   (AUDIO_MAX_ACDB_IOCTL+10), unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_MAX_RTAC_IOCTL (AUDIO_MAX_ACDB_IOCTL+20)
#endif

