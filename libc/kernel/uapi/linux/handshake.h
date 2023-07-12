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
#ifndef _UAPI_LINUX_HANDSHAKE_H
#define _UAPI_LINUX_HANDSHAKE_H
#define HANDSHAKE_FAMILY_NAME "handshake"
#define HANDSHAKE_FAMILY_VERSION 1
enum handshake_handler_class {
  HANDSHAKE_HANDLER_CLASS_NONE,
  HANDSHAKE_HANDLER_CLASS_TLSHD,
  HANDSHAKE_HANDLER_CLASS_MAX,
};
enum handshake_msg_type {
  HANDSHAKE_MSG_TYPE_UNSPEC,
  HANDSHAKE_MSG_TYPE_CLIENTHELLO,
  HANDSHAKE_MSG_TYPE_SERVERHELLO,
};
enum handshake_auth {
  HANDSHAKE_AUTH_UNSPEC,
  HANDSHAKE_AUTH_UNAUTH,
  HANDSHAKE_AUTH_PSK,
  HANDSHAKE_AUTH_X509,
};
enum {
  HANDSHAKE_A_X509_CERT = 1,
  HANDSHAKE_A_X509_PRIVKEY,
  __HANDSHAKE_A_X509_MAX,
  HANDSHAKE_A_X509_MAX = (__HANDSHAKE_A_X509_MAX - 1)
};
enum {
  HANDSHAKE_A_ACCEPT_SOCKFD = 1,
  HANDSHAKE_A_ACCEPT_HANDLER_CLASS,
  HANDSHAKE_A_ACCEPT_MESSAGE_TYPE,
  HANDSHAKE_A_ACCEPT_TIMEOUT,
  HANDSHAKE_A_ACCEPT_AUTH_MODE,
  HANDSHAKE_A_ACCEPT_PEER_IDENTITY,
  HANDSHAKE_A_ACCEPT_CERTIFICATE,
  HANDSHAKE_A_ACCEPT_PEERNAME,
  __HANDSHAKE_A_ACCEPT_MAX,
  HANDSHAKE_A_ACCEPT_MAX = (__HANDSHAKE_A_ACCEPT_MAX - 1)
};
enum {
  HANDSHAKE_A_DONE_STATUS = 1,
  HANDSHAKE_A_DONE_SOCKFD,
  HANDSHAKE_A_DONE_REMOTE_AUTH,
  __HANDSHAKE_A_DONE_MAX,
  HANDSHAKE_A_DONE_MAX = (__HANDSHAKE_A_DONE_MAX - 1)
};
enum {
  HANDSHAKE_CMD_READY = 1,
  HANDSHAKE_CMD_ACCEPT,
  HANDSHAKE_CMD_DONE,
  __HANDSHAKE_CMD_MAX,
  HANDSHAKE_CMD_MAX = (__HANDSHAKE_CMD_MAX - 1)
};
#define HANDSHAKE_MCGRP_NONE "none"
#define HANDSHAKE_MCGRP_TLSHD "tlshd"
#endif
