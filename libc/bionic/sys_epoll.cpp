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
  SigSetConverter set;
  sigset64_t* ss_ptr = nullptr;
  if (ss != nullptr) {
    set = {};
    set.sigset = *ss;
    ss_ptr = &set.sigset64;
  }
  return epoll_pwait64(fd, events, max_events, timeout, ss_ptr);
}

int epoll_pwait64(int fd, epoll_event* events, int max_events, int timeout, const sigset64_t* ss) {
  return __epoll_pwait(fd, events, max_events, timeout, ss, sizeof(*ss));
}

int epoll_wait(int fd, struct epoll_event* events, int max_events, int timeout) {
  return epoll_pwait64(fd, events, max_events, timeout, nullptr);
}
