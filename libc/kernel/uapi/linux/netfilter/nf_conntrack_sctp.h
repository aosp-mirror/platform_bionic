/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_NF_CONNTRACK_SCTP_H
#define _UAPI_NF_CONNTRACK_SCTP_H
#include <linux/netfilter/nf_conntrack_tuple_common.h>
enum sctp_conntrack {
  SCTP_CONNTRACK_NONE,
  SCTP_CONNTRACK_CLOSED,
  SCTP_CONNTRACK_COOKIE_WAIT,
  SCTP_CONNTRACK_COOKIE_ECHOED,
  SCTP_CONNTRACK_ESTABLISHED,
  SCTP_CONNTRACK_SHUTDOWN_SENT,
  SCTP_CONNTRACK_SHUTDOWN_RECD,
  SCTP_CONNTRACK_SHUTDOWN_ACK_SENT,
  SCTP_CONNTRACK_HEARTBEAT_SENT,
  SCTP_CONNTRACK_HEARTBEAT_ACKED,
  SCTP_CONNTRACK_MAX
};
#endif
