/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef _TEGRA_AUDIO_H
#define _TEGRA_AUDIO_H

#include <linux/ioctl.h>

#define TEGRA_AUDIO_MAGIC 't'

#define TEGRA_AUDIO_IN_START _IO(TEGRA_AUDIO_MAGIC, 0)
#define TEGRA_AUDIO_IN_STOP _IO(TEGRA_AUDIO_MAGIC, 1)

struct tegra_audio_in_config {
 int rate;
 int stereo;
};

#define TEGRA_AUDIO_IN_SET_CONFIG _IOW(TEGRA_AUDIO_MAGIC, 2,   const struct tegra_audio_in_config *)
#define TEGRA_AUDIO_IN_GET_CONFIG _IOR(TEGRA_AUDIO_MAGIC, 3,   struct tegra_audio_in_config *)

struct tegra_audio_buf_config {
 unsigned size;
 unsigned threshold;
 unsigned chunk;
};

#define TEGRA_AUDIO_IN_SET_BUF_CONFIG _IOW(TEGRA_AUDIO_MAGIC, 4,   const struct tegra_audio_buf_config *)
#define TEGRA_AUDIO_IN_GET_BUF_CONFIG _IOR(TEGRA_AUDIO_MAGIC, 5,   struct tegra_audio_buf_config *)

#define TEGRA_AUDIO_OUT_SET_BUF_CONFIG _IOW(TEGRA_AUDIO_MAGIC, 6,   const struct tegra_audio_buf_config *)
#define TEGRA_AUDIO_OUT_GET_BUF_CONFIG _IOR(TEGRA_AUDIO_MAGIC, 7,   struct tegra_audio_buf_config *)

struct tegra_audio_error_counts {
 unsigned late_dma;
 unsigned full_empty;
};

#define TEGRA_AUDIO_IN_GET_ERROR_COUNT _IOR(TEGRA_AUDIO_MAGIC, 8,   struct tegra_audio_error_counts *)

#define TEGRA_AUDIO_OUT_GET_ERROR_COUNT _IOR(TEGRA_AUDIO_MAGIC, 9,   struct tegra_audio_error_counts *)

struct tegra_audio_out_preload {
 void *data;
 size_t len;
 size_t len_written;
};

#define TEGRA_AUDIO_OUT_PRELOAD_FIFO _IOWR(TEGRA_AUDIO_MAGIC, 10,   struct tegra_audio_out_preload *)

#endif

