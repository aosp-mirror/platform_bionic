/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <sys/cdefs.h>
#include <stdatomic.h>

#include "platform/bionic/fdtrack.h"

#include "bionic/pthread_internal.h"
#include "private/bionic_tls.h"
#include "private/ErrnoRestorer.h"

extern "C" _Atomic(android_fdtrack_hook_t) __android_fdtrack_hook;

// Macro to record file descriptor creation.
// e.g.:
//   int socket(int domain, int type, int protocol) {
//     return FDTRACK_CREATE_NAME("socket", __socket(domain, type, protocol));
//   }
#define FDTRACK_CREATE_NAME(name, fd_value)                       \
  ({                                                              \
    int __fd = (fd_value);                                        \
    if (__fd != -1 && __predict_false(__android_fdtrack_hook) &&  \
        !__predict_false(__get_thread()->is_vforked())) {         \
      bionic_tls& tls = __get_bionic_tls();                       \
      /* fdtrack_disabled is only true during reentrant calls. */ \
      if (!__predict_false(tls.fdtrack_disabled)) {               \
        ErrnoRestorer r;                                          \
        tls.fdtrack_disabled = true;                              \
        android_fdtrack_event event;                              \
        event.fd = __fd;                                          \
        event.type = ANDROID_FDTRACK_EVENT_TYPE_CREATE;           \
        event.data.create.function_name = name;                   \
        __android_fdtrack_hook(&event);                           \
        tls.fdtrack_disabled = false;                             \
      }                                                           \
    }                                                             \
    __fd;                                                         \
  })

// Macro to record file descriptor creation, with the current function's name.
// e.g.:
//   int socket(int domain, int type, int protocol) {
//     return FDTRACK_CREATE_NAME(__socket(domain, type, protocol));
//   }
#define FDTRACK_CREATE(fd_value) FDTRACK_CREATE_NAME(__func__, (fd_value))

// Macro to record file descriptor closure.
// Note that this does not actually close the file descriptor.
#define FDTRACK_CLOSE(fd_value)                                  \
  ({                                                             \
    int __fd = (fd_value);                                       \
    if (__fd != -1 && __predict_false(__android_fdtrack_hook) && \
        !__predict_false(__get_thread()->is_vforked())) {        \
      bionic_tls& tls = __get_bionic_tls();                      \
      if (!__predict_false(tls.fdtrack_disabled)) {              \
        int saved_errno = errno;                                 \
        tls.fdtrack_disabled = true;                             \
        android_fdtrack_event event;                             \
        event.fd = __fd;                                         \
        event.type = ANDROID_FDTRACK_EVENT_TYPE_CLOSE;           \
        __android_fdtrack_hook(&event);                          \
        tls.fdtrack_disabled = false;                            \
        errno = saved_errno;                                     \
      }                                                          \
    }                                                            \
    __fd;                                                        \
  })
