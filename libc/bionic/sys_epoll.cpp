/*
 * Copyright (C) 2013 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <sys/epoll.h>

#include "private/SigSetConverter.h"
#include "private/bionic_fdtrack.h"

extern "C" int __epoll_create1(int flags);
extern "C" int __epoll_pwait(int, epoll_event*, int, int, const sigset64_t*, size_t);
extern "C" int __epoll_pwait2(int, epoll_event*, int, const __kernel_timespec*, const sigset64_t*,
                              size_t);

int epoll_create(int size) {
  if (size <= 0) {
    errno = EINVAL;
    return -1;
  }
  return FDTRACK_CREATE(__epoll_create1(0));
}

int epoll_create1(int flags) {
  return FDTRACK_CREATE(__epoll_create1(flags));
}

int epoll_pwait(int fd, epoll_event* events, int max_events, int timeout, const sigset_t* ss) {
  SigSetConverter set{ss};
  return epoll_pwait64(fd, events, max_events, timeout, set.ptr);
}

int epoll_pwait64(int fd, epoll_event* events, int max_events, int timeout, const sigset64_t* ss) {
  return __epoll_pwait(fd, events, max_events, timeout, ss, sizeof(*ss));
}

int epoll_pwait2(int fd, epoll_event* events, int max_events, const timespec* timeout,
                 const sigset_t* ss) {
  SigSetConverter set{ss};
  return epoll_pwait2_64(fd, events, max_events, timeout, set.ptr);
}

int epoll_pwait2_64(int fd, epoll_event* events, int max_events, const timespec* timeout,
                    const sigset64_t* ss) {
  // epoll_pwait2() is our first syscall that assumes a 64-bit time_t even for
  // 32-bit processes, so for ILP32 we need to convert.
  // TODO: factor this out into a TimeSpecConverter as/when we get more syscalls like this.
#if __LP64__
  const __kernel_timespec* kts_ptr = reinterpret_cast<const __kernel_timespec*>(timeout);
#else
  __kernel_timespec kts;
  const __kernel_timespec* kts_ptr = nullptr;
  if (timeout) {
    kts.tv_sec = timeout->tv_sec;
    kts.tv_nsec = timeout->tv_nsec;
    kts_ptr = &kts;
  }
#endif
  return __epoll_pwait2(fd, events, max_events, kts_ptr, ss, sizeof(*ss));
}

int epoll_wait(int fd, struct epoll_event* events, int max_events, int timeout) {
  return epoll_pwait64(fd, events, max_events, timeout, nullptr);
}
