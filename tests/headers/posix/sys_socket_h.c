// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include "header_checks.h"

static void sys_socket_h() {
  TYPE(socklen_t);
  TYPE(sa_family_t);

  TYPE(struct sockaddr);
  STRUCT_MEMBER(struct sockaddr, sa_family_t, sa_family);
  STRUCT_MEMBER_ARRAY(struct sockaddr, char/*[]*/, sa_data);

  TYPE(struct sockaddr_storage);
  STRUCT_MEMBER(struct sockaddr_storage, sa_family_t, ss_family);

  TYPE(struct msghdr);
  STRUCT_MEMBER(struct msghdr, void*, msg_name);
  STRUCT_MEMBER(struct msghdr, socklen_t, msg_namelen);
  STRUCT_MEMBER(struct msghdr, struct iovec*, msg_iov);
#if defined(__BIONIC__) || defined(__GLIBC__)
  STRUCT_MEMBER(struct msghdr, size_t, msg_iovlen);
#else
  STRUCT_MEMBER(struct msghdr, int, msg_iovlen);
#endif
  STRUCT_MEMBER(struct msghdr, void*, msg_control);
#if defined(__BIONIC__) || defined(__GLIBC__)
  STRUCT_MEMBER(struct msghdr, size_t, msg_controllen);
#else
  STRUCT_MEMBER(struct msghdr, socklen_t, msg_controllen);
#endif
  STRUCT_MEMBER(struct msghdr, int, msg_flags);

  TYPE(struct iovec);

  TYPE(struct cmsghdr);
#if defined(__BIONIC__) || defined(__GLIBC__)
  STRUCT_MEMBER(struct cmsghdr, size_t, cmsg_len);
#else
  STRUCT_MEMBER(struct cmsghdr, socklen_t, cmsg_len);
#endif
  STRUCT_MEMBER(struct cmsghdr, int, cmsg_level);
  STRUCT_MEMBER(struct cmsghdr, int, cmsg_type);

  MACRO(SCM_RIGHTS);

#if !defined(CMSG_DATA)
#error CMSG_DATA
#endif
#if !defined(CMSG_NXTHDR)
#error CMSG_NXTHDR
#endif
#if !defined(CMSG_FIRSTHDR)
#error CMSG_FIRSTHDR
#endif

  TYPE(struct linger);
  STRUCT_MEMBER(struct linger, int, l_onoff);
  STRUCT_MEMBER(struct linger, int, l_linger);

  MACRO(SOCK_DGRAM);
  MACRO(SOCK_RAW);
  MACRO(SOCK_SEQPACKET);
  MACRO(SOCK_STREAM);

  MACRO(SOL_SOCKET);

  MACRO(SO_ACCEPTCONN);
  MACRO(SO_BROADCAST);
  MACRO(SO_DEBUG);
  MACRO(SO_DONTROUTE);
  MACRO(SO_ERROR);
  MACRO(SO_KEEPALIVE);
  MACRO(SO_LINGER);
  MACRO(SO_OOBINLINE);
  MACRO(SO_RCVBUF);
  MACRO(SO_RCVLOWAT);
  MACRO(SO_RCVTIMEO);
  MACRO(SO_REUSEADDR);
  MACRO(SO_SNDBUF);
  MACRO(SO_SNDLOWAT);
  MACRO(SO_SNDTIMEO);
  MACRO(SO_TYPE);

  MACRO(SOMAXCONN);

  MACRO(MSG_CTRUNC);
  MACRO(MSG_DONTROUTE);
  MACRO(MSG_EOR);
  MACRO(MSG_OOB);
  MACRO(MSG_NOSIGNAL);
  MACRO(MSG_PEEK);
  MACRO(MSG_TRUNC);
  MACRO(MSG_WAITALL);

  MACRO(AF_INET);
  MACRO(AF_INET6);
  MACRO(AF_UNIX);
  MACRO_VALUE(AF_UNSPEC, 0);

  MACRO(SHUT_RD);
  MACRO(SHUT_RDWR);
  MACRO(SHUT_WR);

  TYPE(size_t);
  TYPE(ssize_t);

  FUNCTION(accept, int (*f)(int, struct sockaddr*, socklen_t*));
  FUNCTION(bind, int (*f)(int, const struct sockaddr*, socklen_t));
  FUNCTION(connect, int (*f)(int, const struct sockaddr*, socklen_t));
  FUNCTION(getpeername, int (*f)(int, struct sockaddr*, socklen_t*));
  FUNCTION(getsockname, int (*f)(int, struct sockaddr*, socklen_t*));
  FUNCTION(getsockopt, int (*f)(int, int, int, void*, socklen_t*));
  FUNCTION(listen, int (*f)(int, int));
  FUNCTION(recv, ssize_t (*f)(int, void*, size_t, int));
  FUNCTION(recvfrom, ssize_t (*f)(int, void*, size_t, int, struct sockaddr*, socklen_t*));
  FUNCTION(send, ssize_t (*f)(int, const void*, size_t, int));
  FUNCTION(sendmsg, ssize_t (*f)(int, const struct msghdr*, int));
  FUNCTION(sendto, ssize_t (*f)(int, const void*, size_t, int, const struct sockaddr*, socklen_t));
  FUNCTION(setsockopt, int (*f)(int, int, int, const void*, socklen_t));
  FUNCTION(shutdown, int (*f)(int, int));
#if !defined(__BIONIC__) // Obsolete, plus ioctl disallowed by SELinux.
  FUNCTION(sockatmark, int (*f)(int));
#endif
  FUNCTION(socket, int (*f)(int, int, int));
  FUNCTION(socketpair, int (*f)(int, int, int, int[2]));
}
