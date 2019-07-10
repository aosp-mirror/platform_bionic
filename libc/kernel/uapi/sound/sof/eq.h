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
#ifndef __INCLUDE_UAPI_SOUND_SOF_USER_EQ_H__
#define __INCLUDE_UAPI_SOUND_SOF_USER_EQ_H__
#define SOF_EQ_FIR_IDX_SWITCH 0
#define SOF_EQ_FIR_MAX_SIZE 4096
#define SOF_EQ_FIR_MAX_LENGTH 192
#define SOF_EQ_FIR_MAX_RESPONSES 8
struct sof_eq_fir_config {
  uint32_t size;
  uint16_t channels_in_config;
  uint16_t number_of_responses;
  uint32_t reserved[4];
  int16_t data[];
} __packed;
struct sof_eq_fir_coef_data {
  int16_t length;
  int16_t out_shift;
  uint32_t reserved[4];
  int16_t coef[];
} __packed;
#define SOF_EQ_FIR_COEF_NHEADER (sizeof(struct sof_eq_fir_coef_data) / sizeof(int16_t))
#define SOF_EQ_IIR_IDX_SWITCH 0
#define SOF_EQ_IIR_MAX_SIZE 1024
#define SOF_EQ_IIR_MAX_RESPONSES 8
struct sof_eq_iir_config {
  uint32_t size;
  uint32_t channels_in_config;
  uint32_t number_of_responses;
  uint32_t reserved[4];
  int32_t data[];
} __packed;
struct sof_eq_iir_header_df2t {
  uint32_t num_sections;
  uint32_t num_sections_in_series;
  uint32_t reserved[4];
  int32_t biquads[];
} __packed;
struct sof_eq_iir_biquad_df2t {
  int32_t a2;
  int32_t a1;
  int32_t b2;
  int32_t b1;
  int32_t b0;
  int32_t output_shift;
  int32_t output_gain;
} __packed;
#define SOF_EQ_IIR_DF2T_BIQUADS_MAX 11
#define SOF_EQ_IIR_NHEADER_DF2T (sizeof(struct sof_eq_iir_header_df2t) / sizeof(int32_t))
#define SOF_EQ_IIR_NBIQUAD_DF2T (sizeof(struct sof_eq_iir_biquad_df2t) / sizeof(int32_t))
#endif
