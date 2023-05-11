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
#ifndef _UAPI_SOUND_FIREWIRE_H_INCLUDED
#define _UAPI_SOUND_FIREWIRE_H_INCLUDED
#include <linux/ioctl.h>
#include <linux/types.h>
#define SNDRV_FIREWIRE_EVENT_LOCK_STATUS 0x000010cc
#define SNDRV_FIREWIRE_EVENT_DICE_NOTIFICATION 0xd1ce004e
#define SNDRV_FIREWIRE_EVENT_EFW_RESPONSE 0x4e617475
#define SNDRV_FIREWIRE_EVENT_DIGI00X_MESSAGE 0x746e736c
#define SNDRV_FIREWIRE_EVENT_MOTU_NOTIFICATION 0x64776479
#define SNDRV_FIREWIRE_EVENT_TASCAM_CONTROL 0x7473636d
#define SNDRV_FIREWIRE_EVENT_MOTU_REGISTER_DSP_CHANGE 0x4d545244
#define SNDRV_FIREWIRE_EVENT_FF400_MESSAGE 0x4f6c6761
struct snd_firewire_event_common {
  unsigned int type;
};
struct snd_firewire_event_lock_status {
  unsigned int type;
  unsigned int status;
};
struct snd_firewire_event_dice_notification {
  unsigned int type;
  unsigned int notification;
};
#define SND_EFW_TRANSACTION_USER_SEQNUM_MAX ((__u32) ((__u16) ~0) - 1)
struct snd_efw_transaction {
  __be32 length;
  __be32 version;
  __be32 seqnum;
  __be32 category;
  __be32 command;
  __be32 status;
  __be32 params[];
};
struct snd_firewire_event_efw_response {
  unsigned int type;
  __be32 response[];
};
struct snd_firewire_event_digi00x_message {
  unsigned int type;
  __u32 message;
};
struct snd_firewire_event_motu_notification {
  unsigned int type;
  __u32 message;
};
struct snd_firewire_tascam_change {
  unsigned int index;
  __be32 before;
  __be32 after;
};
struct snd_firewire_event_tascam_control {
  unsigned int type;
  struct snd_firewire_tascam_change changes[];
};
struct snd_firewire_event_motu_register_dsp_change {
  unsigned int type;
  __u32 count;
  __u32 changes[];
};
struct snd_firewire_event_ff400_message {
  unsigned int type;
  unsigned int message_count;
  struct {
    __u32 message;
    __u32 tstamp;
  } messages[];
};
union snd_firewire_event {
  struct snd_firewire_event_common common;
  struct snd_firewire_event_lock_status lock_status;
  struct snd_firewire_event_dice_notification dice_notification;
  struct snd_firewire_event_efw_response efw_response;
  struct snd_firewire_event_digi00x_message digi00x_message;
  struct snd_firewire_event_tascam_control tascam_control;
  struct snd_firewire_event_motu_notification motu_notification;
  struct snd_firewire_event_motu_register_dsp_change motu_register_dsp_change;
  struct snd_firewire_event_ff400_message ff400_message;
};
#define SNDRV_FIREWIRE_IOCTL_GET_INFO _IOR('H', 0xf8, struct snd_firewire_get_info)
#define SNDRV_FIREWIRE_IOCTL_LOCK _IO('H', 0xf9)
#define SNDRV_FIREWIRE_IOCTL_UNLOCK _IO('H', 0xfa)
#define SNDRV_FIREWIRE_IOCTL_TASCAM_STATE _IOR('H', 0xfb, struct snd_firewire_tascam_state)
#define SNDRV_FIREWIRE_IOCTL_MOTU_REGISTER_DSP_METER _IOR('H', 0xfc, struct snd_firewire_motu_register_dsp_meter)
#define SNDRV_FIREWIRE_IOCTL_MOTU_COMMAND_DSP_METER _IOR('H', 0xfd, struct snd_firewire_motu_command_dsp_meter)
#define SNDRV_FIREWIRE_IOCTL_MOTU_REGISTER_DSP_PARAMETER _IOR('H', 0xfe, struct snd_firewire_motu_register_dsp_parameter)
#define SNDRV_FIREWIRE_TYPE_DICE 1
#define SNDRV_FIREWIRE_TYPE_FIREWORKS 2
#define SNDRV_FIREWIRE_TYPE_BEBOB 3
#define SNDRV_FIREWIRE_TYPE_OXFW 4
#define SNDRV_FIREWIRE_TYPE_DIGI00X 5
#define SNDRV_FIREWIRE_TYPE_TASCAM 6
#define SNDRV_FIREWIRE_TYPE_MOTU 7
#define SNDRV_FIREWIRE_TYPE_FIREFACE 8
struct snd_firewire_get_info {
  unsigned int type;
  unsigned int card;
  unsigned char guid[8];
  char device_name[16];
};
#define SNDRV_FIREWIRE_TASCAM_STATE_COUNT 64
struct snd_firewire_tascam_state {
  __be32 data[SNDRV_FIREWIRE_TASCAM_STATE_COUNT];
};
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_METER_INPUT_COUNT 24
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_METER_OUTPUT_COUNT 24
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_METER_COUNT (SNDRV_FIREWIRE_MOTU_REGISTER_DSP_METER_INPUT_COUNT + SNDRV_FIREWIRE_MOTU_REGISTER_DSP_METER_OUTPUT_COUNT)
struct snd_firewire_motu_register_dsp_meter {
  __u8 data[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_METER_COUNT];
};
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_COUNT 4
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_SRC_COUNT 20
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_INPUT_COUNT 10
#define SNDRV_FIREWIRE_MOTU_REGISTER_DSP_ALIGNED_INPUT_COUNT (SNDRV_FIREWIRE_MOTU_REGISTER_DSP_INPUT_COUNT + 2)
struct snd_firewire_motu_register_dsp_parameter {
  struct {
    struct {
      __u8 gain[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_SRC_COUNT];
      __u8 pan[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_SRC_COUNT];
      __u8 flag[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_SRC_COUNT];
      __u8 paired_balance[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_SRC_COUNT];
      __u8 paired_width[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_SRC_COUNT];
    } source[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_COUNT];
    struct {
      __u8 paired_volume[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_COUNT];
      __u8 paired_flag[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_MIXER_COUNT];
    } output;
  } mixer;
  struct {
    __u8 main_paired_volume;
    __u8 hp_paired_volume;
    __u8 hp_paired_assignment;
    __u8 reserved[5];
  } output;
  struct {
    __u8 boost_flag;
    __u8 nominal_level_flag;
    __u8 reserved[6];
  } line_input;
  struct {
    __u8 gain_and_invert[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_ALIGNED_INPUT_COUNT];
    __u8 flag[SNDRV_FIREWIRE_MOTU_REGISTER_DSP_ALIGNED_INPUT_COUNT];
  } input;
  __u8 reserved[64];
};
#define SNDRV_FIREWIRE_MOTU_COMMAND_DSP_METER_COUNT 400
struct snd_firewire_motu_command_dsp_meter {
  float data[SNDRV_FIREWIRE_MOTU_COMMAND_DSP_METER_COUNT];
};
#endif
