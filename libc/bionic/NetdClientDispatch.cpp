/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "private/NetdClientDispatch.h"

#include <sys/socket.h>

#include "private/bionic_fdtrack.h"

#ifdef __i386__
#define __socketcall __attribute__((__cdecl__))
#else
#define __socketcall
#endif

extern "C" __socketcall int __accept4(int, sockaddr*, socklen_t*, int);
extern "C" __socketcall int __connect(int, const sockaddr*, socklen_t);
extern "C" __socketcall int __sendmmsg(int, const mmsghdr*, unsigned int, int);
extern "C" __socketcall ssize_t __sendmsg(int, const msghdr*, unsigned int);
extern "C" __socketcall int __sendto(int, const void*, size_t, int, const sockaddr*, socklen_t);
extern "C" __socketcall int __socket(int, int, int);

static unsigned fallBackNetIdForResolv(unsigned netId) {
    return netId;
}

static int fallBackDnsOpenProxy() {
    return -1;
}

// This structure is modified only at startup (when libc.so is loaded) and never
// afterwards, so it's okay that it's read later at runtime without a lock.
__LIBC_HIDDEN__ NetdClientDispatch __netdClientDispatch __attribute__((aligned(32))) = {
    __accept4,
    __connect,
    __sendmmsg,
    __sendmsg,
    __sendto,
    __socket,
    fallBackNetIdForResolv,
    fallBackDnsOpenProxy,
};

int accept4(int fd, sockaddr* addr, socklen_t* addr_length, int flags) {
  return FDTRACK_CREATE(__netdClientDispatch.accept4(fd, addr, addr_length, flags));
}

int connect(int fd, const sockaddr* addr, socklen_t addr_length) {
    return __netdClientDispatch.connect(fd, addr, addr_length);
}

int sendmmsg(int fd, const struct mmsghdr* msgs, unsigned int msg_count, int flags) {
    return __netdClientDispatch.sendmmsg(fd, msgs, msg_count, flags);
}

ssize_t sendmsg(int fd, const struct msghdr* msg, int flags) {
    return __netdClientDispatch.sendmsg(fd, msg, flags);
}

ssize_t sendto(int fd, const void* buf, size_t n, int flags,
               const struct sockaddr* dst_addr, socklen_t dst_addr_length) {
    return __netdClientDispatch.sendto(fd, buf, n, flags, dst_addr, dst_addr_length);
}

int socket(int domain, int type, int protocol) {
  return FDTRACK_CREATE(__netdClientDispatch.socket(domain, type, protocol));
}
