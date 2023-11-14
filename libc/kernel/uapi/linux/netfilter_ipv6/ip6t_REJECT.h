/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _IP6T_REJECT_H
#define _IP6T_REJECT_H
#include <linux/types.h>
enum ip6t_reject_with {
  IP6T_ICMP6_NO_ROUTE,
  IP6T_ICMP6_ADM_PROHIBITED,
  IP6T_ICMP6_NOT_NEIGHBOUR,
  IP6T_ICMP6_ADDR_UNREACH,
  IP6T_ICMP6_PORT_UNREACH,
  IP6T_ICMP6_ECHOREPLY,
  IP6T_TCP_RESET,
  IP6T_ICMP6_POLICY_FAIL,
  IP6T_ICMP6_REJECT_ROUTE
};
struct ip6t_reject_info {
  __u32 with;
};
#endif
