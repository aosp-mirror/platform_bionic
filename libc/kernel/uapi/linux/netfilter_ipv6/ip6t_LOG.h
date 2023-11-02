/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _IP6T_LOG_H
#define _IP6T_LOG_H
#define IP6T_LOG_TCPSEQ 0x01
#define IP6T_LOG_TCPOPT 0x02
#define IP6T_LOG_IPOPT 0x04
#define IP6T_LOG_UID 0x08
#define IP6T_LOG_NFLOG 0x10
#define IP6T_LOG_MACDECODE 0x20
#define IP6T_LOG_MASK 0x2f
struct ip6t_log_info {
  unsigned char level;
  unsigned char logflags;
  char prefix[30];
};
#endif
