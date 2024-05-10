/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_CAN_NETLINK_H
#define _UAPI_CAN_NETLINK_H
#include <linux/types.h>
struct can_bittiming {
  __u32 bitrate;
  __u32 sample_point;
  __u32 tq;
  __u32 prop_seg;
  __u32 phase_seg1;
  __u32 phase_seg2;
  __u32 sjw;
  __u32 brp;
};
struct can_bittiming_const {
  char name[16];
  __u32 tseg1_min;
  __u32 tseg1_max;
  __u32 tseg2_min;
  __u32 tseg2_max;
  __u32 sjw_max;
  __u32 brp_min;
  __u32 brp_max;
  __u32 brp_inc;
};
struct can_clock {
  __u32 freq;
};
enum can_state {
  CAN_STATE_ERROR_ACTIVE = 0,
  CAN_STATE_ERROR_WARNING,
  CAN_STATE_ERROR_PASSIVE,
  CAN_STATE_BUS_OFF,
  CAN_STATE_STOPPED,
  CAN_STATE_SLEEPING,
  CAN_STATE_MAX
};
struct can_berr_counter {
  __u16 txerr;
  __u16 rxerr;
};
struct can_ctrlmode {
  __u32 mask;
  __u32 flags;
};
#define CAN_CTRLMODE_LOOPBACK 0x01
#define CAN_CTRLMODE_LISTENONLY 0x02
#define CAN_CTRLMODE_3_SAMPLES 0x04
#define CAN_CTRLMODE_ONE_SHOT 0x08
#define CAN_CTRLMODE_BERR_REPORTING 0x10
#define CAN_CTRLMODE_FD 0x20
#define CAN_CTRLMODE_PRESUME_ACK 0x40
#define CAN_CTRLMODE_FD_NON_ISO 0x80
#define CAN_CTRLMODE_CC_LEN8_DLC 0x100
#define CAN_CTRLMODE_TDC_AUTO 0x200
#define CAN_CTRLMODE_TDC_MANUAL 0x400
struct can_device_stats {
  __u32 bus_error;
  __u32 error_warning;
  __u32 error_passive;
  __u32 bus_off;
  __u32 arbitration_lost;
  __u32 restarts;
};
enum {
  IFLA_CAN_UNSPEC,
  IFLA_CAN_BITTIMING,
  IFLA_CAN_BITTIMING_CONST,
  IFLA_CAN_CLOCK,
  IFLA_CAN_STATE,
  IFLA_CAN_CTRLMODE,
  IFLA_CAN_RESTART_MS,
  IFLA_CAN_RESTART,
  IFLA_CAN_BERR_COUNTER,
  IFLA_CAN_DATA_BITTIMING,
  IFLA_CAN_DATA_BITTIMING_CONST,
  IFLA_CAN_TERMINATION,
  IFLA_CAN_TERMINATION_CONST,
  IFLA_CAN_BITRATE_CONST,
  IFLA_CAN_DATA_BITRATE_CONST,
  IFLA_CAN_BITRATE_MAX,
  IFLA_CAN_TDC,
  IFLA_CAN_CTRLMODE_EXT,
  __IFLA_CAN_MAX,
  IFLA_CAN_MAX = __IFLA_CAN_MAX - 1
};
enum {
  IFLA_CAN_TDC_UNSPEC,
  IFLA_CAN_TDC_TDCV_MIN,
  IFLA_CAN_TDC_TDCV_MAX,
  IFLA_CAN_TDC_TDCO_MIN,
  IFLA_CAN_TDC_TDCO_MAX,
  IFLA_CAN_TDC_TDCF_MIN,
  IFLA_CAN_TDC_TDCF_MAX,
  IFLA_CAN_TDC_TDCV,
  IFLA_CAN_TDC_TDCO,
  IFLA_CAN_TDC_TDCF,
  __IFLA_CAN_TDC,
  IFLA_CAN_TDC_MAX = __IFLA_CAN_TDC - 1
};
enum {
  IFLA_CAN_CTRLMODE_UNSPEC,
  IFLA_CAN_CTRLMODE_SUPPORTED,
  __IFLA_CAN_CTRLMODE,
  IFLA_CAN_CTRLMODE_MAX = __IFLA_CAN_CTRLMODE - 1
};
#define CAN_TERMINATION_DISABLED 0
#endif
