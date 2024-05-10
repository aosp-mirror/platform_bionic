/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_TC_MPLS_H
#define __LINUX_TC_MPLS_H
#include <linux/pkt_cls.h>
#define TCA_MPLS_ACT_POP 1
#define TCA_MPLS_ACT_PUSH 2
#define TCA_MPLS_ACT_MODIFY 3
#define TCA_MPLS_ACT_DEC_TTL 4
#define TCA_MPLS_ACT_MAC_PUSH 5
struct tc_mpls {
  tc_gen;
  int m_action;
};
enum {
  TCA_MPLS_UNSPEC,
  TCA_MPLS_TM,
  TCA_MPLS_PARMS,
  TCA_MPLS_PAD,
  TCA_MPLS_PROTO,
  TCA_MPLS_LABEL,
  TCA_MPLS_TC,
  TCA_MPLS_TTL,
  TCA_MPLS_BOS,
  __TCA_MPLS_MAX,
};
#define TCA_MPLS_MAX (__TCA_MPLS_MAX - 1)
#endif
