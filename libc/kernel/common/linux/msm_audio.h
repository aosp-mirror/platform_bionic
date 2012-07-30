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
#ifndef __LINUX_MSM_AUDIO_H
#define __LINUX_MSM_AUDIO_H
#include <linux/types.h>
#include <linux/ioctl.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_IOCTL_MAGIC 'a'
#define AUDIO_START _IOW(AUDIO_IOCTL_MAGIC, 0, unsigned)
#define AUDIO_STOP _IOW(AUDIO_IOCTL_MAGIC, 1, unsigned)
#define AUDIO_FLUSH _IOW(AUDIO_IOCTL_MAGIC, 2, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_GET_CONFIG _IOR(AUDIO_IOCTL_MAGIC, 3, unsigned)
#define AUDIO_SET_CONFIG _IOW(AUDIO_IOCTL_MAGIC, 4, unsigned)
#define AUDIO_GET_STATS _IOR(AUDIO_IOCTL_MAGIC, 5, unsigned)
#define AUDIO_ENABLE_AUDPP _IOW(AUDIO_IOCTL_MAGIC, 6, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_ADRC _IOW(AUDIO_IOCTL_MAGIC, 7, unsigned)
#define AUDIO_SET_EQ _IOW(AUDIO_IOCTL_MAGIC, 8, unsigned)
#define AUDIO_SET_RX_IIR _IOW(AUDIO_IOCTL_MAGIC, 9, unsigned)
#define AUDIO_SET_VOLUME _IOW(AUDIO_IOCTL_MAGIC, 10, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_PAUSE _IOW(AUDIO_IOCTL_MAGIC, 11, unsigned)
#define AUDIO_PLAY_DTMF _IOW(AUDIO_IOCTL_MAGIC, 12, unsigned)
#define AUDIO_GET_EVENT _IOR(AUDIO_IOCTL_MAGIC, 13, unsigned)
#define AUDIO_ABORT_GET_EVENT _IOW(AUDIO_IOCTL_MAGIC, 14, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_REGISTER_PMEM _IOW(AUDIO_IOCTL_MAGIC, 15, unsigned)
#define AUDIO_DEREGISTER_PMEM _IOW(AUDIO_IOCTL_MAGIC, 16, unsigned)
#define AUDIO_ASYNC_WRITE _IOW(AUDIO_IOCTL_MAGIC, 17, unsigned)
#define AUDIO_ASYNC_READ _IOW(AUDIO_IOCTL_MAGIC, 18, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_INCALL _IOW(AUDIO_IOCTL_MAGIC, 19, struct msm_voicerec_mode)
#define AUDIO_GET_NUM_SND_DEVICE _IOR(AUDIO_IOCTL_MAGIC, 20, unsigned)
#define AUDIO_GET_SND_DEVICES _IOWR(AUDIO_IOCTL_MAGIC, 21,   struct msm_snd_device_list)
#define AUDIO_ENABLE_SND_DEVICE _IOW(AUDIO_IOCTL_MAGIC, 22, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_DISABLE_SND_DEVICE _IOW(AUDIO_IOCTL_MAGIC, 23, unsigned)
#define AUDIO_ROUTE_STREAM _IOW(AUDIO_IOCTL_MAGIC, 24,   struct msm_audio_route_config)
#define AUDIO_GET_PCM_CONFIG _IOR(AUDIO_IOCTL_MAGIC, 30, unsigned)
#define AUDIO_SET_PCM_CONFIG _IOW(AUDIO_IOCTL_MAGIC, 31, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SWITCH_DEVICE _IOW(AUDIO_IOCTL_MAGIC, 32, unsigned)
#define AUDIO_SET_MUTE _IOW(AUDIO_IOCTL_MAGIC, 33, unsigned)
#define AUDIO_UPDATE_ACDB _IOW(AUDIO_IOCTL_MAGIC, 34, unsigned)
#define AUDIO_START_VOICE _IOW(AUDIO_IOCTL_MAGIC, 35, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_STOP_VOICE _IOW(AUDIO_IOCTL_MAGIC, 36, unsigned)
#define AUDIO_REINIT_ACDB _IOW(AUDIO_IOCTL_MAGIC, 39, unsigned)
#define AUDIO_OUTPORT_FLUSH _IOW(AUDIO_IOCTL_MAGIC, 40, unsigned short)
#define AUDIO_SET_ERR_THRESHOLD_VALUE _IOW(AUDIO_IOCTL_MAGIC, 41,   unsigned short)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_GET_BITSTREAM_ERROR_INFO _IOR(AUDIO_IOCTL_MAGIC, 42,   struct msm_audio_bitstream_error_info)
#define AUDIO_SET_SRS_TRUMEDIA_PARAM _IOW(AUDIO_IOCTL_MAGIC, 43, unsigned)
#define AUDIO_SET_STREAM_CONFIG _IOW(AUDIO_IOCTL_MAGIC, 80,   struct msm_audio_stream_config)
#define AUDIO_GET_STREAM_CONFIG _IOR(AUDIO_IOCTL_MAGIC, 81,   struct msm_audio_stream_config)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_GET_SESSION_ID _IOR(AUDIO_IOCTL_MAGIC, 82, unsigned short)
#define AUDIO_GET_STREAM_INFO _IOR(AUDIO_IOCTL_MAGIC, 83,   struct msm_audio_bitstream_info)
#define AUDIO_SET_PAN _IOW(AUDIO_IOCTL_MAGIC, 84, unsigned)
#define AUDIO_SET_QCONCERT_PLUS _IOW(AUDIO_IOCTL_MAGIC, 85, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_MBADRC _IOW(AUDIO_IOCTL_MAGIC, 86, unsigned)
#define AUDIO_SET_VOLUME_PATH _IOW(AUDIO_IOCTL_MAGIC, 87,   struct msm_vol_info)
#define AUDIO_SET_MAX_VOL_ALL _IOW(AUDIO_IOCTL_MAGIC, 88, unsigned)
#define AUDIO_ENABLE_AUDPRE _IOW(AUDIO_IOCTL_MAGIC, 89, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_AGC _IOW(AUDIO_IOCTL_MAGIC, 90, unsigned)
#define AUDIO_SET_NS _IOW(AUDIO_IOCTL_MAGIC, 91, unsigned)
#define AUDIO_SET_TX_IIR _IOW(AUDIO_IOCTL_MAGIC, 92, unsigned)
#define AUDIO_GET_BUF_CFG _IOW(AUDIO_IOCTL_MAGIC, 93,   struct msm_audio_buf_cfg)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_SET_BUF_CFG _IOW(AUDIO_IOCTL_MAGIC, 94,   struct msm_audio_buf_cfg)
#define AUDIO_SET_ACDB_BLK _IOW(AUDIO_IOCTL_MAGIC, 95,   struct msm_acdb_cmd_device)
#define AUDIO_GET_ACDB_BLK _IOW(AUDIO_IOCTL_MAGIC, 96,   struct msm_acdb_cmd_device)
#define AUDIO_REGISTER_ION _IOW(AUDIO_IOCTL_MAGIC, 97, unsigned)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_DEREGISTER_ION _IOW(AUDIO_IOCTL_MAGIC, 98, unsigned)
#define AUDIO_MAX_COMMON_IOCTL_NUM 100
#define HANDSET_MIC 0x01
#define HANDSET_SPKR 0x02
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HEADSET_MIC 0x03
#define HEADSET_SPKR_MONO 0x04
#define HEADSET_SPKR_STEREO 0x05
#define SPKR_PHONE_MIC 0x06
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SPKR_PHONE_MONO 0x07
#define SPKR_PHONE_STEREO 0x08
#define BT_SCO_MIC 0x09
#define BT_SCO_SPKR 0x0A
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define BT_A2DP_SPKR 0x0B
#define TTY_HEADSET_MIC 0x0C
#define TTY_HEADSET_SPKR 0x0D
#define DEFAULT_TX 0x0E
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DEFAULT_RX 0x0F
#define BT_A2DP_TX 0x10
#define HEADSET_MONO_PLUS_SPKR_MONO_RX 0x11
#define HEADSET_MONO_PLUS_SPKR_STEREO_RX 0x12
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define HEADSET_STEREO_PLUS_SPKR_MONO_RX 0x13
#define HEADSET_STEREO_PLUS_SPKR_STEREO_RX 0x14
#define I2S_RX 0x20
#define I2S_TX 0x21
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ADRC_ENABLE 0x0001
#define EQ_ENABLE 0x0002
#define IIR_ENABLE 0x0004
#define QCONCERT_PLUS_ENABLE 0x0008
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MBADRC_ENABLE 0x0010
#define SRS_ENABLE 0x0020
#define SRS_DISABLE 0x0040
#define AGC_ENABLE 0x0001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NS_ENABLE 0x0002
#define TX_IIR_ENABLE 0x0004
#define FLUENCE_ENABLE 0x0008
#define VOC_REC_UPLINK 0x00
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define VOC_REC_DOWNLINK 0x01
#define VOC_REC_BOTH 0x02
struct msm_audio_config {
 uint32_t buffer_size;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t buffer_count;
 uint32_t channel_count;
 uint32_t sample_rate;
 uint32_t type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t meta_field;
 uint32_t bits;
 uint32_t unused[3];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_audio_stream_config {
 uint32_t buffer_size;
 uint32_t buffer_count;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_audio_buf_cfg{
 uint32_t meta_info_enable;
 uint32_t frames_per_buf;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_audio_stats {
 uint32_t byte_count;
 uint32_t sample_count;
 uint32_t unused[2];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_audio_ion_info {
 int fd;
 void *vaddr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_audio_pmem_info {
 int fd;
 void *vaddr;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct msm_audio_aio_buf {
 void *buf_addr;
 uint32_t buf_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t data_len;
 void *private_data;
 unsigned short mfield_sz;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SND_IOCTL_MAGIC 's'
#define SND_MUTE_UNMUTED 0
#define SND_MUTE_MUTED 1
struct msm_mute_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t mute;
 uint32_t path;
};
struct msm_vol_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t vol;
 uint32_t path;
};
struct msm_voicerec_mode {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t rec_mode;
};
struct msm_snd_device_config {
 uint32_t device;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t ear_mute;
 uint32_t mic_mute;
};
#define SND_SET_DEVICE _IOW(SND_IOCTL_MAGIC, 2, struct msm_device_config *)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SND_METHOD_VOICE 0
struct msm_snd_volume_config {
 uint32_t device;
 uint32_t method;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t volume;
};
#define SND_SET_VOLUME _IOW(SND_IOCTL_MAGIC, 3, struct msm_snd_volume_config *)
#define SND_GET_NUM_ENDPOINTS _IOR(SND_IOCTL_MAGIC, 4, unsigned *)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_snd_endpoint {
 int id;
 char name[64];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define SND_GET_ENDPOINT _IOWR(SND_IOCTL_MAGIC, 5, struct msm_snd_endpoint *)
#define SND_AVC_CTL _IOW(SND_IOCTL_MAGIC, 6, unsigned *)
#define SND_AGC_CTL _IOW(SND_IOCTL_MAGIC, 7, unsigned *)
struct msm_audio_pcm_config {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t pcm_feedback;
 uint32_t buffer_count;
 uint32_t buffer_size;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_EVENT_SUSPEND 0
#define AUDIO_EVENT_RESUME 1
#define AUDIO_EVENT_WRITE_DONE 2
#define AUDIO_EVENT_READ_DONE 3
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_EVENT_STREAM_INFO 4
#define AUDIO_EVENT_BITSTREAM_ERROR_INFO 5
#define AUDIO_CODEC_TYPE_MP3 0
#define AUDIO_CODEC_TYPE_AAC 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_audio_bitstream_info {
 uint32_t codec_type;
 uint32_t chan_info;
 uint32_t sample_rate;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t bit_stream_info;
 uint32_t bit_rate;
 uint32_t unused[3];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_audio_bitstream_error_info {
 uint32_t dec_id;
 uint32_t err_msg_indicator;
 uint32_t err_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
union msm_audio_event_payload {
 struct msm_audio_aio_buf aio_buf;
 struct msm_audio_bitstream_info stream_info;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct msm_audio_bitstream_error_info error_info;
 int reserved;
};
struct msm_audio_event {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int event_type;
 int timeout_ms;
 union msm_audio_event_payload event_payload;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define MSM_SNDDEV_CAP_RX 0x1
#define MSM_SNDDEV_CAP_TX 0x2
#define MSM_SNDDEV_CAP_VOICE 0x4
struct msm_snd_device_info {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t dev_id;
 uint32_t dev_cap;
 char dev_name[64];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_snd_device_list {
 uint32_t num_dev;
 struct msm_snd_device_info *list;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct msm_dtmf_config {
 uint16_t path;
 uint16_t dtmf_hi;
 uint16_t dtmf_low;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t duration;
 uint16_t tx_gain;
 uint16_t rx_gain;
 uint16_t mixing;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define AUDIO_ROUTE_STREAM_VOICE_RX 0
#define AUDIO_ROUTE_STREAM_VOICE_TX 1
#define AUDIO_ROUTE_STREAM_PLAYBACK 2
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define AUDIO_ROUTE_STREAM_REC 3
struct msm_audio_route_config {
 uint32_t stream_type;
 uint32_t stream_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t dev_id;
};
#define AUDIO_MAX_EQ_BANDS 12
struct msm_audio_eq_band {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint16_t band_idx;
 uint32_t filter_type;
 uint32_t center_freq_hz;
 uint32_t filter_gain;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t q_factor;
} __attribute__ ((packed));
struct msm_audio_eq_stream_config {
 uint32_t enable;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t num_bands;
 struct msm_audio_eq_band eq_bands[AUDIO_MAX_EQ_BANDS];
} __attribute__ ((packed));
struct msm_acdb_cmd_device {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t command_id;
 uint32_t device_id;
 uint32_t network_id;
 uint32_t sample_rate_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 uint32_t interface_id;
 uint32_t algorithm_block_id;
 uint32_t total_bytes;
 uint32_t *phys_buf;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif

