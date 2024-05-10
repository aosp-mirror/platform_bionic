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
#include <signal.h>

#include <platform/bionic/reserved_signals.h>

#include "private/SigSetConverter.h"

extern "C" int __rt_sigprocmask(int, const sigset64_t*, sigset64_t*, size_t);

//
// These need to be kept separate from pthread_sigmask, sigblock, sigsetmask,
// sighold, and sigset because libsigchain only intercepts sigprocmask so we
// can't allow clang to decide to inline sigprocmask.
//

int sigprocmask(int how,
                const sigset_t* bionic_new_set,
                sigset_t* bionic_old_set) __attribute__((__noinline__)) {
  SigSetConverter new_set{bionic_new_set};
  SigSetConverter old_set{bionic_old_set};
  int rc = sigprocmask64(how, new_set.ptr, old_set.ptr);
  if (rc == 0 && bionic_old_set != nullptr) {
    old_set.copy_out();
  }
  return rc;
}

int sigprocmask64(int how,
                  const sigset64_t* new_set,
                  sigset64_t* old_set) __attribute__((__noinline__)) {
  // how is only checked for validity if new_set is provided.
  if (new_set && how != SIG_BLOCK && how != SIG_UNBLOCK && how != SIG_SETMASK) {
    errno = EINVAL;
    return -1;
  }

  sigset64_t mutable_new_set;
  sigset64_t* mutable_new_set_ptr = nullptr;
  if (new_set) {
    mutable_new_set = filter_reserved_signals(*new_set, how);
    mutable_new_set_ptr = &mutable_new_set;
  }
  return __rt_sigprocmask(how, mutable_new_set_ptr, old_set, sizeof(*new_set));
}
