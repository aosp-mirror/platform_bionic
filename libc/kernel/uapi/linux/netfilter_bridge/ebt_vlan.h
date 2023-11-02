/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_BRIDGE_EBT_VLAN_H
#define __LINUX_BRIDGE_EBT_VLAN_H
#include <linux/types.h>
#define EBT_VLAN_ID 0x01
#define EBT_VLAN_PRIO 0x02
#define EBT_VLAN_ENCAP 0x04
#define EBT_VLAN_MASK (EBT_VLAN_ID | EBT_VLAN_PRIO | EBT_VLAN_ENCAP)
#define EBT_VLAN_MATCH "vlan"
struct ebt_vlan_info {
  __u16 id;
  __u8 prio;
  __be16 encap;
  __u8 bitmask;
  __u8 invflags;
};
#endif
