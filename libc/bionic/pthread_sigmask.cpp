/*
 * Copyright (C) 2008 The Android Open Source Project
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
#include <pthread.h>
#include <signal.h>

#include "private/ErrnoRestorer.h"
#include "private/kernel_sigset_t.h"

extern "C" int __rt_sigprocmask(int, const kernel_sigset_t*, kernel_sigset_t*, size_t);

int pthread_sigmask(int how, const sigset_t* iset, sigset_t* oset) {
  ErrnoRestorer errno_restorer;

  // 'in_set_ptr' is the second parameter to __rt_sigprocmask. It must be NULL
  // if 'set' is NULL to ensure correct semantics (which in this case would
  // be to ignore 'how' and return the current signal set into 'oset').
  kernel_sigset_t in_set;
  kernel_sigset_t* in_set_ptr = NULL;
  if (iset != NULL) {
    in_set.set(iset);
    in_set_ptr = &in_set;
  }

  kernel_sigset_t out_set;
  if (__rt_sigprocmask(how, in_set_ptr, &out_set, sizeof(out_set)) == -1) {
    return errno;
  }

  if (oset != NULL) {
    *oset = out_set.bionic;
  }

  return 0;
}
