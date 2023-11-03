/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_NFACCT_MATCH_H
#define _XT_NFACCT_MATCH_H
#include <linux/netfilter/nfnetlink_acct.h>
struct nf_acct;
struct xt_nfacct_match_info {
  char name[NFACCT_NAME_MAX];
  struct nf_acct * nfacct;
};
struct xt_nfacct_match_info_v1 {
  char name[NFACCT_NAME_MAX];
  struct nf_acct * nfacct __attribute__((aligned(8)));
};
#endif
