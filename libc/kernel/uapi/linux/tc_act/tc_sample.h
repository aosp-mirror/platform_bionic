/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_TC_SAMPLE_H
#define __LINUX_TC_SAMPLE_H
#include <linux/types.h>
#include <linux/pkt_cls.h>
#include <linux/if_ether.h>
struct tc_sample {
  tc_gen;
};
enum {
  TCA_SAMPLE_UNSPEC,
  TCA_SAMPLE_TM,
  TCA_SAMPLE_PARMS,
  TCA_SAMPLE_RATE,
  TCA_SAMPLE_TRUNC_SIZE,
  TCA_SAMPLE_PSAMPLE_GROUP,
  TCA_SAMPLE_PAD,
  __TCA_SAMPLE_MAX
};
#define TCA_SAMPLE_MAX (__TCA_SAMPLE_MAX - 1)
#endif
