/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_TEE_TARGET_H
#define _XT_TEE_TARGET_H
#include <linux/netfilter.h>
struct xt_tee_tginfo {
  union nf_inet_addr gw;
  char oif[16];
  struct xt_tee_priv * priv __attribute__((aligned(8)));
};
#endif
