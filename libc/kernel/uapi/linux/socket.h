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
#ifndef _UAPI_LINUX_SOCKET_H
#define _UAPI_LINUX_SOCKET_H
#define _K_SS_MAXSIZE 128
typedef unsigned short __kernel_sa_family_t;
struct __kernel_sockaddr_storage {
  union {
    struct {
      __kernel_sa_family_t ss_family;
      char __data[_K_SS_MAXSIZE - sizeof(unsigned short)];
    };
    void * __align;
  };
};
#define SOCK_SNDBUF_LOCK 1
#define SOCK_RCVBUF_LOCK 2
#define SOCK_BUF_LOCK_MASK (SOCK_SNDBUF_LOCK | SOCK_RCVBUF_LOCK)
#define SOCK_TXREHASH_DEFAULT 255
#define SOCK_TXREHASH_DISABLED 0
#define SOCK_TXREHASH_ENABLED 1
#endif
