/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_CAN_J1939_H_
#define _UAPI_CAN_J1939_H_
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/can.h>
#define J1939_MAX_UNICAST_ADDR 0xfd
#define J1939_IDLE_ADDR 0xfe
#define J1939_NO_ADDR 0xff
#define J1939_NO_NAME 0
#define J1939_PGN_REQUEST 0x0ea00
#define J1939_PGN_ADDRESS_CLAIMED 0x0ee00
#define J1939_PGN_ADDRESS_COMMANDED 0x0fed8
#define J1939_PGN_PDU1_MAX 0x3ff00
#define J1939_PGN_MAX 0x3ffff
#define J1939_NO_PGN 0x40000
typedef __u32 pgn_t;
typedef __u8 priority_t;
typedef __u64 name_t;
#define SOL_CAN_J1939 (SOL_CAN_BASE + CAN_J1939)
enum {
  SO_J1939_FILTER = 1,
  SO_J1939_PROMISC = 2,
  SO_J1939_SEND_PRIO = 3,
  SO_J1939_ERRQUEUE = 4,
};
enum {
  SCM_J1939_DEST_ADDR = 1,
  SCM_J1939_DEST_NAME = 2,
  SCM_J1939_PRIO = 3,
  SCM_J1939_ERRQUEUE = 4,
};
enum {
  J1939_NLA_PAD,
  J1939_NLA_BYTES_ACKED,
  J1939_NLA_TOTAL_SIZE,
  J1939_NLA_PGN,
  J1939_NLA_SRC_NAME,
  J1939_NLA_DEST_NAME,
  J1939_NLA_SRC_ADDR,
  J1939_NLA_DEST_ADDR,
};
enum {
  J1939_EE_INFO_NONE,
  J1939_EE_INFO_TX_ABORT,
  J1939_EE_INFO_RX_RTS,
  J1939_EE_INFO_RX_DPO,
  J1939_EE_INFO_RX_ABORT,
};
struct j1939_filter {
  name_t name;
  name_t name_mask;
  pgn_t pgn;
  pgn_t pgn_mask;
  __u8 addr;
  __u8 addr_mask;
};
#define J1939_FILTER_MAX 512
#endif
