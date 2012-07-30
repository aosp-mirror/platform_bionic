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
#ifndef __LINUX_MFD_MSM_ADIE_CODEC_H
#define __LINUX_MFD_MSM_ADIE_CODEC_H
#include <linux/types.h>
#define ADIE_CODEC_ACTION_ENTRY 0x1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ADIE_CODEC_ACTION_DELAY_WAIT 0x2
#define ADIE_CODEC_ACTION_STAGE_REACHED 0x3
#define ADIE_CODEC_PATH_OFF 0x0050
#define ADIE_CODEC_DIGITAL_READY 0x0100
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ADIE_CODEC_DIGITAL_ANALOG_READY 0x1000
#define ADIE_CODEC_ANALOG_OFF 0x0750
#define ADIE_CODEC_DIGITAL_OFF 0x0600
#define ADIE_CODEC_FLASH_IMAGE 0x0001
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ADIE_CODEC_RX 0
#define ADIE_CODEC_TX 1
#define ADIE_CODEC_LB 3
#define ADIE_CODEC_MAX 4
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ADIE_CODEC_PACK_ENTRY(reg, mask, val) ((val)|(mask << 8)|(reg << 16))
#define ADIE_CODEC_UNPACK_ENTRY(packed, reg, mask, val)   do {   ((reg) = ((packed >> 16) & (0xff)));   ((mask) = ((packed >> 8) & (0xff)));   ((val) = ((packed) & (0xff)));   } while (0);
struct adie_codec_action_unit {
 u32 type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 action;
};
struct adie_codec_hwsetting_entry{
 struct adie_codec_action_unit *actions;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 action_sz;
 u32 freq_plan;
 u32 osr;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct adie_codec_dev_profile {
 u32 path_type;
 u32 setting_sz;
 struct adie_codec_hwsetting_entry *settings;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct adie_codec_register {
 u8 reg;
 u8 mask;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u8 val;
};
struct adie_codec_register_image {
 struct adie_codec_register *regs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 img_sz;
};
struct adie_codec_path;
struct adie_codec_anc_data {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 size;
 u32 writes[];
};
struct adie_codec_operations {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int codec_id;
 int (*codec_open) (struct adie_codec_dev_profile *profile,
 struct adie_codec_path **path_pptr);
 int (*codec_close) (struct adie_codec_path *path_ptr);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*codec_setpath) (struct adie_codec_path *path_ptr,
 u32 freq_plan, u32 osr);
 int (*codec_proceed_stage) (struct adie_codec_path *path_ptr,
 u32 state);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 (*codec_freq_supported) (struct adie_codec_dev_profile *profile,
 u32 requested_freq);
 int (*codec_enable_sidetone) (struct adie_codec_path *rx_path_ptr,
 u32 enable);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 int (*codec_enable_anc) (struct adie_codec_path *rx_path_ptr,
 u32 enable, struct adie_codec_anc_data *calibration_writes);
 int (*codec_set_device_digital_volume) (
 struct adie_codec_path *path_ptr,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 num_channels,
 u32 vol_percentage);
 int (*codec_set_device_analog_volume) (struct adie_codec_path *path_ptr,
 u32 num_channels,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 u32 volume);
 int (*codec_set_master_mode) (struct adie_codec_path *path_ptr,
 u8 master);
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif

