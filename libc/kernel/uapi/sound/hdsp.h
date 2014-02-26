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
#ifndef __SOUND_HDSP_H
#define __SOUND_HDSP_H
#include <linux/types.h>
#define HDSP_MATRIX_MIXER_SIZE 2048
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum HDSP_IO_Type {
 Digiface,
 Multiface,
 H9652,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 H9632,
 RPM,
 Undefined,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct hdsp_peak_rms {
 __u32 input_peaks[26];
 __u32 playback_peaks[26];
 __u32 output_peaks[28];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u64 input_rms[26];
 __u64 playback_rms[26];
 __u64 output_rms[26];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_HDSP_IOCTL_GET_PEAK_RMS _IOR('H', 0x40, struct hdsp_peak_rms)
struct hdsp_config_info {
 unsigned char pref_sync_ref;
 unsigned char wordclock_sync_check;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char spdif_sync_check;
 unsigned char adatsync_sync_check;
 unsigned char adat_sync_check[3];
 unsigned char spdif_in;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char spdif_out;
 unsigned char spdif_professional;
 unsigned char spdif_emphasis;
 unsigned char spdif_nonaudio;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int spdif_sample_rate;
 unsigned int system_sample_rate;
 unsigned int autosync_sample_rate;
 unsigned char system_clock_mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char clock_source;
 unsigned char autosync_ref;
 unsigned char line_out;
 unsigned char passthru;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char da_gain;
 unsigned char ad_gain;
 unsigned char phone_gain;
 unsigned char xlr_breakout_cable;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char analog_extension_board;
};
#define SNDRV_HDSP_IOCTL_GET_CONFIG_INFO _IOR('H', 0x41, struct hdsp_config_info)
struct hdsp_firmware {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 void __user *firmware_data;
};
#define SNDRV_HDSP_IOCTL_UPLOAD_FIRMWARE _IOW('H', 0x42, struct hdsp_firmware)
struct hdsp_version {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 enum HDSP_IO_Type io_type;
 unsigned short firmware_rev;
};
#define SNDRV_HDSP_IOCTL_GET_VERSION _IOR('H', 0x43, struct hdsp_version)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct hdsp_mixer {
 unsigned short matrix[HDSP_MATRIX_MIXER_SIZE];
};
#define SNDRV_HDSP_IOCTL_GET_MIXER _IOR('H', 0x44, struct hdsp_mixer)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct hdsp_9632_aeb {
 int aebi;
 int aebo;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SNDRV_HDSP_IOCTL_GET_9632_AEB _IOR('H', 0x45, struct hdsp_9632_aeb)
typedef enum HDSP_IO_Type HDSP_IO_Type;
typedef struct hdsp_peak_rms hdsp_peak_rms_t;
typedef struct hdsp_config_info hdsp_config_info_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct hdsp_firmware hdsp_firmware_t;
typedef struct hdsp_version hdsp_version_t;
typedef struct hdsp_mixer hdsp_mixer_t;
typedef struct hdsp_9632_aeb hdsp_9632_aeb_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
