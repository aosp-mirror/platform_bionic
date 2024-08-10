/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __HDA_TPLG_INTERFACE_H__
#define __HDA_TPLG_INTERFACE_H__
#include <linux/types.h>
#define SKL_CONTROL_TYPE_BYTE_TLV 0x100
#define SKL_CONTROL_TYPE_MIC_SELECT 0x102
#define SKL_CONTROL_TYPE_MULTI_IO_SELECT 0x103
#define SKL_CONTROL_TYPE_MULTI_IO_SELECT_DMIC 0x104
#define HDA_SST_CFG_MAX 900
#define MAX_IN_QUEUE 8
#define MAX_OUT_QUEUE 8
#define SKL_UUID_STR_SZ 40
enum skl_event_types {
  SKL_EVENT_NONE = 0,
  SKL_MIXER_EVENT,
  SKL_MUX_EVENT,
  SKL_VMIXER_EVENT,
  SKL_PGA_EVENT
};
enum skl_ch_cfg {
  SKL_CH_CFG_MONO = 0,
  SKL_CH_CFG_STEREO = 1,
  SKL_CH_CFG_2_1 = 2,
  SKL_CH_CFG_3_0 = 3,
  SKL_CH_CFG_3_1 = 4,
  SKL_CH_CFG_QUATRO = 5,
  SKL_CH_CFG_4_0 = 6,
  SKL_CH_CFG_5_0 = 7,
  SKL_CH_CFG_5_1 = 8,
  SKL_CH_CFG_DUAL_MONO = 9,
  SKL_CH_CFG_I2S_DUAL_STEREO_0 = 10,
  SKL_CH_CFG_I2S_DUAL_STEREO_1 = 11,
  SKL_CH_CFG_7_1 = 12,
  SKL_CH_CFG_4_CHANNEL = SKL_CH_CFG_7_1,
  SKL_CH_CFG_INVALID
};
enum skl_module_type {
  SKL_MODULE_TYPE_MIXER = 0,
  SKL_MODULE_TYPE_COPIER,
  SKL_MODULE_TYPE_UPDWMIX,
  SKL_MODULE_TYPE_SRCINT,
  SKL_MODULE_TYPE_ALGO,
  SKL_MODULE_TYPE_BASE_OUTFMT,
  SKL_MODULE_TYPE_KPB,
  SKL_MODULE_TYPE_MIC_SELECT,
};
enum skl_core_affinity {
  SKL_AFFINITY_CORE_0 = 0,
  SKL_AFFINITY_CORE_1,
  SKL_AFFINITY_CORE_MAX
};
enum skl_pipe_conn_type {
  SKL_PIPE_CONN_TYPE_NONE = 0,
  SKL_PIPE_CONN_TYPE_FE,
  SKL_PIPE_CONN_TYPE_BE
};
enum skl_hw_conn_type {
  SKL_CONN_NONE = 0,
  SKL_CONN_SOURCE = 1,
  SKL_CONN_SINK = 2
};
enum skl_dev_type {
  SKL_DEVICE_BT = 0x0,
  SKL_DEVICE_DMIC = 0x1,
  SKL_DEVICE_I2S = 0x2,
  SKL_DEVICE_SLIMBUS = 0x3,
  SKL_DEVICE_HDALINK = 0x4,
  SKL_DEVICE_HDAHOST = 0x5,
  SKL_DEVICE_NONE
};
enum skl_interleaving {
  SKL_INTERLEAVING_PER_CHANNEL = 0,
  SKL_INTERLEAVING_PER_SAMPLE = 1,
};
enum skl_sample_type {
  SKL_SAMPLE_TYPE_INT_MSB = 0,
  SKL_SAMPLE_TYPE_INT_LSB = 1,
  SKL_SAMPLE_TYPE_INT_SIGNED = 2,
  SKL_SAMPLE_TYPE_INT_UNSIGNED = 3,
  SKL_SAMPLE_TYPE_FLOAT = 4
};
enum module_pin_type {
  SKL_PIN_TYPE_HOMOGENEOUS,
  SKL_PIN_TYPE_HETEROGENEOUS,
};
enum skl_module_param_type {
  SKL_PARAM_DEFAULT = 0,
  SKL_PARAM_INIT,
  SKL_PARAM_SET,
  SKL_PARAM_BIND
};
struct skl_dfw_algo_data {
  __u32 set_params : 2;
  __u32 rsvd : 30;
  __u32 param_id;
  __u32 max;
  char params[];
} __attribute__((__packed__));
enum skl_tkn_dir {
  SKL_DIR_IN,
  SKL_DIR_OUT
};
enum skl_tuple_type {
  SKL_TYPE_TUPLE,
  SKL_TYPE_DATA
};
#endif
