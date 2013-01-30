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
#ifndef _IP_QUEUE_H
#define _IP_QUEUE_H
#include <net/if.h>
typedef struct ipq_packet_msg {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long packet_id;
 unsigned long mark;
 long timestamp_sec;
 long timestamp_usec;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned int hook;
 char indev_name[IFNAMSIZ];
 char outdev_name[IFNAMSIZ];
 unsigned short hw_protocol;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned short hw_type;
 unsigned char hw_addrlen;
 unsigned char hw_addr[8];
 size_t data_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned char payload[0];
} ipq_packet_msg_t;
typedef struct ipq_mode_msg {
 unsigned char value;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 size_t range;
} ipq_mode_msg_t;
typedef struct ipq_verdict_msg {
 unsigned int value;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 unsigned long id;
 size_t data_len;
 unsigned char payload[0];
} ipq_verdict_msg_t;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
typedef struct ipq_peer_msg {
 union {
 ipq_verdict_msg_t verdict;
 ipq_mode_msg_t mode;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 } msg;
} ipq_peer_msg_t;
enum {
 IPQ_COPY_NONE,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 IPQ_COPY_META,
 IPQ_COPY_PACKET
};
#define IPQ_COPY_MAX IPQ_COPY_PACKET
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPQM_BASE 0x10
#define IPQM_MODE (IPQM_BASE + 1)
#define IPQM_VERDICT (IPQM_BASE + 2)
#define IPQM_PACKET (IPQM_BASE + 3)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define IPQM_MAX (IPQM_BASE + 4)
#endif
