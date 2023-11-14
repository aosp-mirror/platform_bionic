/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef SCSI_NETLINK_FC_H
#define SCSI_NETLINK_FC_H
#include <linux/types.h>
#include <scsi/scsi_netlink.h>
#define FC_NL_ASYNC_EVENT 0x0100
#define FC_NL_MSGALIGN(len) (((len) + 7) & ~7)
struct fc_nl_event {
  struct scsi_nl_hdr snlh;
  __u64 seconds;
  __u64 vendor_id;
  __u16 host_no;
  __u16 event_datalen;
  __u32 event_num;
  __u32 event_code;
  union {
    __u32 event_data;
    __DECLARE_FLEX_ARRAY(__u8, event_data_flex);
  };
} __attribute__((aligned(sizeof(__u64))));
#endif
