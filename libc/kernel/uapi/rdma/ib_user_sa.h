/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef IB_USER_SA_H
#define IB_USER_SA_H
#include <linux/types.h>
enum {
  IB_PATH_GMP = 1,
  IB_PATH_PRIMARY = (1 << 1),
  IB_PATH_ALTERNATE = (1 << 2),
  IB_PATH_OUTBOUND = (1 << 3),
  IB_PATH_INBOUND = (1 << 4),
  IB_PATH_INBOUND_REVERSE = (1 << 5),
  IB_PATH_BIDIRECTIONAL = IB_PATH_OUTBOUND | IB_PATH_INBOUND_REVERSE
};
struct ib_path_rec_data {
  __u32 flags;
  __u32 reserved;
  __u32 path_rec[16];
};
struct ib_user_path_rec {
  __u8 dgid[16];
  __u8 sgid[16];
  __be16 dlid;
  __be16 slid;
  __u32 raw_traffic;
  __be32 flow_label;
  __u32 reversible;
  __u32 mtu;
  __be16 pkey;
  __u8 hop_limit;
  __u8 traffic_class;
  __u8 numb_path;
  __u8 sl;
  __u8 mtu_selector;
  __u8 rate_selector;
  __u8 rate;
  __u8 packet_life_time_selector;
  __u8 packet_life_time;
  __u8 preference;
};
#endif
