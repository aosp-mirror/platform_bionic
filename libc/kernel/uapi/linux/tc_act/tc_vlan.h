/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_TC_VLAN_H
#define __LINUX_TC_VLAN_H
#include <linux/pkt_cls.h>
#define TCA_VLAN_ACT_POP 1
#define TCA_VLAN_ACT_PUSH 2
#define TCA_VLAN_ACT_MODIFY 3
#define TCA_VLAN_ACT_POP_ETH 4
#define TCA_VLAN_ACT_PUSH_ETH 5
struct tc_vlan {
  tc_gen;
  int v_action;
};
enum {
  TCA_VLAN_UNSPEC,
  TCA_VLAN_TM,
  TCA_VLAN_PARMS,
  TCA_VLAN_PUSH_VLAN_ID,
  TCA_VLAN_PUSH_VLAN_PROTOCOL,
  TCA_VLAN_PAD,
  TCA_VLAN_PUSH_VLAN_PRIORITY,
  TCA_VLAN_PUSH_ETH_DST,
  TCA_VLAN_PUSH_ETH_SRC,
  __TCA_VLAN_MAX,
};
#define TCA_VLAN_MAX (__TCA_VLAN_MAX - 1)
#endif
