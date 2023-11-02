/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_NFQ_TARGET_H
#define _XT_NFQ_TARGET_H
#include <linux/types.h>
struct xt_NFQ_info {
  __u16 queuenum;
};
struct xt_NFQ_info_v1 {
  __u16 queuenum;
  __u16 queues_total;
};
struct xt_NFQ_info_v2 {
  __u16 queuenum;
  __u16 queues_total;
  __u16 bypass;
};
struct xt_NFQ_info_v3 {
  __u16 queuenum;
  __u16 queues_total;
  __u16 flags;
#define NFQ_FLAG_BYPASS 0x01
#define NFQ_FLAG_CPU_FANOUT 0x02
#define NFQ_FLAG_MASK 0x03
};
#endif
