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
#ifndef _TEGRA_AUDIO_H
#define _TEGRA_AUDIO_H
#include <linux/ioctl.h>
#define TEGRA_AUDIO_MAGIC 't'
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TEGRA_AUDIO_IN_START _IO(TEGRA_AUDIO_MAGIC, 0)
#define TEGRA_AUDIO_IN_STOP _IO(TEGRA_AUDIO_MAGIC, 1)
struct tegra_audio_in_config {
 int rate;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int stereo;
};
#define TEGRA_AUDIO_IN_SET_CONFIG _IOW(TEGRA_AUDIO_MAGIC, 2,   const struct tegra_audio_in_config *)
#define TEGRA_AUDIO_IN_GET_CONFIG _IOR(TEGRA_AUDIO_MAGIC, 3,   struct tegra_audio_in_config *)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TEGRA_AUDIO_IN_SET_NUM_BUFS _IOW(TEGRA_AUDIO_MAGIC, 4,   const unsigned int *)
#define TEGRA_AUDIO_IN_GET_NUM_BUFS _IOW(TEGRA_AUDIO_MAGIC, 5,   unsigned int *)
#define TEGRA_AUDIO_OUT_SET_NUM_BUFS _IOW(TEGRA_AUDIO_MAGIC, 6,   const unsigned int *)
#define TEGRA_AUDIO_OUT_GET_NUM_BUFS _IOW(TEGRA_AUDIO_MAGIC, 7,   unsigned int *)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TEGRA_AUDIO_OUT_FLUSH _IO(TEGRA_AUDIO_MAGIC, 10)
#define TEGRA_AUDIO_BIT_FORMAT_DEFAULT 0
#define TEGRA_AUDIO_BIT_FORMAT_DSP 1
#define TEGRA_AUDIO_SET_BIT_FORMAT _IOW(TEGRA_AUDIO_MAGIC, 11,   const unsigned int *)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define TEGRA_AUDIO_GET_BIT_FORMAT _IOR(TEGRA_AUDIO_MAGIC, 12,   unsigned int *)
#endif
