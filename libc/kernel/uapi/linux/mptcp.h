/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef _UAPI_MPTCP_H
#define _UAPI_MPTCP_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <linux/const.h>
#include <linux/types.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/socket.h>
#define MPTCP_SUBFLOW_FLAG_MCAP_REM _BITUL(0)
#define MPTCP_SUBFLOW_FLAG_MCAP_LOC _BITUL(1)
#define MPTCP_SUBFLOW_FLAG_JOIN_REM _BITUL(2)
#define MPTCP_SUBFLOW_FLAG_JOIN_LOC _BITUL(3)
#define MPTCP_SUBFLOW_FLAG_BKUP_REM _BITUL(4)
#define MPTCP_SUBFLOW_FLAG_BKUP_LOC _BITUL(5)
#define MPTCP_SUBFLOW_FLAG_FULLY_ESTABLISHED _BITUL(6)
#define MPTCP_SUBFLOW_FLAG_CONNECTED _BITUL(7)
#define MPTCP_SUBFLOW_FLAG_MAPVALID _BITUL(8)
enum {
  MPTCP_SUBFLOW_ATTR_UNSPEC,
  MPTCP_SUBFLOW_ATTR_TOKEN_REM,
  MPTCP_SUBFLOW_ATTR_TOKEN_LOC,
  MPTCP_SUBFLOW_ATTR_RELWRITE_SEQ,
  MPTCP_SUBFLOW_ATTR_MAP_SEQ,
  MPTCP_SUBFLOW_ATTR_MAP_SFSEQ,
  MPTCP_SUBFLOW_ATTR_SSN_OFFSET,
  MPTCP_SUBFLOW_ATTR_MAP_DATALEN,
  MPTCP_SUBFLOW_ATTR_FLAGS,
  MPTCP_SUBFLOW_ATTR_ID_REM,
  MPTCP_SUBFLOW_ATTR_ID_LOC,
  MPTCP_SUBFLOW_ATTR_PAD,
  __MPTCP_SUBFLOW_ATTR_MAX
};
#define MPTCP_SUBFLOW_ATTR_MAX (__MPTCP_SUBFLOW_ATTR_MAX - 1)
#define MPTCP_PM_NAME "mptcp_pm"
#define MPTCP_PM_CMD_GRP_NAME "mptcp_pm_cmds"
#define MPTCP_PM_EV_GRP_NAME "mptcp_pm_events"
#define MPTCP_PM_VER 0x1
enum {
  MPTCP_PM_ATTR_UNSPEC,
  MPTCP_PM_ATTR_ADDR,
  MPTCP_PM_ATTR_RCV_ADD_ADDRS,
  MPTCP_PM_ATTR_SUBFLOWS,
  MPTCP_PM_ATTR_TOKEN,
  MPTCP_PM_ATTR_LOC_ID,
  MPTCP_PM_ATTR_ADDR_REMOTE,
  __MPTCP_PM_ATTR_MAX
};
#define MPTCP_PM_ATTR_MAX (__MPTCP_PM_ATTR_MAX - 1)
enum {
  MPTCP_PM_ADDR_ATTR_UNSPEC,
  MPTCP_PM_ADDR_ATTR_FAMILY,
  MPTCP_PM_ADDR_ATTR_ID,
  MPTCP_PM_ADDR_ATTR_ADDR4,
  MPTCP_PM_ADDR_ATTR_ADDR6,
  MPTCP_PM_ADDR_ATTR_PORT,
  MPTCP_PM_ADDR_ATTR_FLAGS,
  MPTCP_PM_ADDR_ATTR_IF_IDX,
  __MPTCP_PM_ADDR_ATTR_MAX
};
#define MPTCP_PM_ADDR_ATTR_MAX (__MPTCP_PM_ADDR_ATTR_MAX - 1)
#define MPTCP_PM_ADDR_FLAG_SIGNAL (1 << 0)
#define MPTCP_PM_ADDR_FLAG_SUBFLOW (1 << 1)
#define MPTCP_PM_ADDR_FLAG_BACKUP (1 << 2)
#define MPTCP_PM_ADDR_FLAG_FULLMESH (1 << 3)
#define MPTCP_PM_ADDR_FLAG_IMPLICIT (1 << 4)
enum {
  MPTCP_PM_CMD_UNSPEC,
  MPTCP_PM_CMD_ADD_ADDR,
  MPTCP_PM_CMD_DEL_ADDR,
  MPTCP_PM_CMD_GET_ADDR,
  MPTCP_PM_CMD_FLUSH_ADDRS,
  MPTCP_PM_CMD_SET_LIMITS,
  MPTCP_PM_CMD_GET_LIMITS,
  MPTCP_PM_CMD_SET_FLAGS,
  MPTCP_PM_CMD_ANNOUNCE,
  MPTCP_PM_CMD_REMOVE,
  MPTCP_PM_CMD_SUBFLOW_CREATE,
  MPTCP_PM_CMD_SUBFLOW_DESTROY,
  __MPTCP_PM_CMD_AFTER_LAST
};
#define MPTCP_INFO_FLAG_FALLBACK _BITUL(0)
#define MPTCP_INFO_FLAG_REMOTE_KEY_RECEIVED _BITUL(1)
struct mptcp_info {
  __u8 mptcpi_subflows;
  __u8 mptcpi_add_addr_signal;
  __u8 mptcpi_add_addr_accepted;
  __u8 mptcpi_subflows_max;
  __u8 mptcpi_add_addr_signal_max;
  __u8 mptcpi_add_addr_accepted_max;
  __u32 mptcpi_flags;
  __u32 mptcpi_token;
  __u64 mptcpi_write_seq;
  __u64 mptcpi_snd_una;
  __u64 mptcpi_rcv_nxt;
  __u8 mptcpi_local_addr_used;
  __u8 mptcpi_local_addr_max;
  __u8 mptcpi_csum_enabled;
  __u32 mptcpi_retransmits;
  __u64 mptcpi_bytes_retrans;
  __u64 mptcpi_bytes_sent;
  __u64 mptcpi_bytes_received;
  __u64 mptcpi_bytes_acked;
};
enum mptcp_event_type {
  MPTCP_EVENT_UNSPEC = 0,
  MPTCP_EVENT_CREATED = 1,
  MPTCP_EVENT_ESTABLISHED = 2,
  MPTCP_EVENT_CLOSED = 3,
  MPTCP_EVENT_ANNOUNCED = 6,
  MPTCP_EVENT_REMOVED = 7,
  MPTCP_EVENT_SUB_ESTABLISHED = 10,
  MPTCP_EVENT_SUB_CLOSED = 11,
  MPTCP_EVENT_SUB_PRIORITY = 13,
  MPTCP_EVENT_LISTENER_CREATED = 15,
  MPTCP_EVENT_LISTENER_CLOSED = 16,
};
enum mptcp_event_attr {
  MPTCP_ATTR_UNSPEC = 0,
  MPTCP_ATTR_TOKEN,
  MPTCP_ATTR_FAMILY,
  MPTCP_ATTR_LOC_ID,
  MPTCP_ATTR_REM_ID,
  MPTCP_ATTR_SADDR4,
  MPTCP_ATTR_SADDR6,
  MPTCP_ATTR_DADDR4,
  MPTCP_ATTR_DADDR6,
  MPTCP_ATTR_SPORT,
  MPTCP_ATTR_DPORT,
  MPTCP_ATTR_BACKUP,
  MPTCP_ATTR_ERROR,
  MPTCP_ATTR_FLAGS,
  MPTCP_ATTR_TIMEOUT,
  MPTCP_ATTR_IF_IDX,
  MPTCP_ATTR_RESET_REASON,
  MPTCP_ATTR_RESET_FLAGS,
  MPTCP_ATTR_SERVER_SIDE,
  __MPTCP_ATTR_AFTER_LAST
};
#define MPTCP_ATTR_MAX (__MPTCP_ATTR_AFTER_LAST - 1)
#define MPTCP_RST_EUNSPEC 0
#define MPTCP_RST_EMPTCP 1
#define MPTCP_RST_ERESOURCE 2
#define MPTCP_RST_EPROHIBIT 3
#define MPTCP_RST_EWQ2BIG 4
#define MPTCP_RST_EBADPERF 5
#define MPTCP_RST_EMIDDLEBOX 6
struct mptcp_subflow_data {
  __u32 size_subflow_data;
  __u32 num_subflows;
  __u32 size_kernel;
  __u32 size_user;
} __attribute__((aligned(8)));
struct mptcp_subflow_addrs {
  union {
    __kernel_sa_family_t sa_family;
    struct sockaddr sa_local;
    struct sockaddr_in sin_local;
    struct sockaddr_in6 sin6_local;
    struct __kernel_sockaddr_storage ss_local;
  };
  union {
    struct sockaddr sa_remote;
    struct sockaddr_in sin_remote;
    struct sockaddr_in6 sin6_remote;
    struct __kernel_sockaddr_storage ss_remote;
  };
};
struct mptcp_subflow_info {
  __u32 id;
  struct mptcp_subflow_addrs addrs;
};
struct mptcp_full_info {
  __u32 size_tcpinfo_kernel;
  __u32 size_tcpinfo_user;
  __u32 size_sfinfo_kernel;
  __u32 size_sfinfo_user;
  __u32 num_subflows;
  __u32 size_arrays_user;
  __aligned_u64 subflow_info;
  __aligned_u64 tcp_info;
  struct mptcp_info mptcp_info;
};
#define MPTCP_INFO 1
#define MPTCP_TCPINFO 2
#define MPTCP_SUBFLOW_ADDRS 3
#define MPTCP_FULL_INFO 4
#endif
