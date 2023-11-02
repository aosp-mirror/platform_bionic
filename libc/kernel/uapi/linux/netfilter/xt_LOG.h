/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _XT_LOG_H
#define _XT_LOG_H
#define XT_LOG_TCPSEQ 0x01
#define XT_LOG_TCPOPT 0x02
#define XT_LOG_IPOPT 0x04
#define XT_LOG_UID 0x08
#define XT_LOG_NFLOG 0x10
#define XT_LOG_MACDECODE 0x20
#define XT_LOG_MASK 0x2f
struct xt_log_info {
  unsigned char level;
  unsigned char logflags;
  char prefix[30];
};
#endif
