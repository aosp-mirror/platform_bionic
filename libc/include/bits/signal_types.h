/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <limits.h>
#include <linux/signal.h>
#include <sys/types.h>

/* The arm and x86 kernel header files don't define _NSIG. */
#ifndef _KERNEL__NSIG
#define _KERNEL__NSIG 64
#endif

/* Userspace's NSIG is the kernel's _NSIG + 1. */
#define _NSIG (_KERNEL__NSIG + 1)
#define NSIG _NSIG

typedef int sig_atomic_t;

typedef __sighandler_t sig_t; /* BSD compatibility. */
typedef __sighandler_t sighandler_t; /* glibc compatibility. */

/* sigset_t is already large enough on LP64, but LP32's sigset_t
 * is just `unsigned long`.
 */
#if defined(__LP64__)
typedef sigset_t sigset64_t;
#else
typedef struct { unsigned long __bits[_KERNEL__NSIG/(8*sizeof(long))]; } sigset64_t;
#endif

/* The kernel's struct sigaction doesn't match the POSIX one. */

#if defined(__LP64__)

/* For 64-bit, that's the only problem, and we only need two structs
 * for source compatibility with 32-bit. */

#define __SIGACTION_BODY \
  int sa_flags; \
  union { \
    sighandler_t sa_handler; \
    void (*sa_sigaction)(int, struct siginfo*, void*); \
  }; \
  sigset_t sa_mask; \
  void (*sa_restorer)(void); \

struct sigaction { __SIGACTION_BODY };
struct sigaction64 { __SIGACTION_BODY };

#undef __SIGACTION_BODY

#else

/* For 32-bit, Android's ABIs used a too-small sigset_t that doesn't
 * support RT signals, so we need two different structs.
 */

/* The arm32 kernel headers also pollute the namespace with these,
 * but our header scrubber doesn't know how to remove #defines. */
#undef sa_handler
#undef sa_sigaction

struct sigaction {
  union {
    sighandler_t sa_handler;
    void (*sa_sigaction)(int, struct siginfo*, void*);
  };
  sigset_t sa_mask;
  int sa_flags;
  void (*sa_restorer)(void);
};

struct sigaction64 {
  union {
    sighandler_t sa_handler;
    void (*sa_sigaction)(int, struct siginfo*, void*);
  };
  int sa_flags;
  void (*sa_restorer)(void);
  sigset64_t sa_mask;
};

#endif
