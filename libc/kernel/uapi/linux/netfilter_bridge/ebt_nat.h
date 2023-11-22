/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_BRIDGE_EBT_NAT_H
#define __LINUX_BRIDGE_EBT_NAT_H
#include <linux/if_ether.h>
#define NAT_ARP_BIT (0x00000010)
struct ebt_nat_info {
  unsigned char mac[ETH_ALEN];
  int target;
};
#define EBT_SNAT_TARGET "snat"
#define EBT_DNAT_TARGET "dnat"
#endif
