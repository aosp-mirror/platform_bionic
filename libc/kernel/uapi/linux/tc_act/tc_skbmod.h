/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_TC_SKBMOD_H
#define __LINUX_TC_SKBMOD_H
#include <linux/pkt_cls.h>
#define SKBMOD_F_DMAC 0x1
#define SKBMOD_F_SMAC 0x2
#define SKBMOD_F_ETYPE 0x4
#define SKBMOD_F_SWAPMAC 0x8
#define SKBMOD_F_ECN 0x10
struct tc_skbmod {
  tc_gen;
  __u64 flags;
};
enum {
  TCA_SKBMOD_UNSPEC,
  TCA_SKBMOD_TM,
  TCA_SKBMOD_PARMS,
  TCA_SKBMOD_DMAC,
  TCA_SKBMOD_SMAC,
  TCA_SKBMOD_ETYPE,
  TCA_SKBMOD_PAD,
  __TCA_SKBMOD_MAX
};
#define TCA_SKBMOD_MAX (__TCA_SKBMOD_MAX - 1)
#endif
