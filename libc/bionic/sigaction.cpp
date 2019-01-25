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

#include <signal.h>
#include <string.h>

#include "private/sigrtmin.h"

extern "C" void __restore_rt(void);
extern "C" void __restore(void);

#if defined(__LP64__)

extern "C" int __rt_sigaction(int, const struct __kernel_sigaction*, struct __kernel_sigaction*, size_t);

int sigaction(int signal, const struct sigaction* bionic_new_action, struct sigaction* bionic_old_action) {
  __kernel_sigaction kernel_new_action;
  if (bionic_new_action != nullptr) {
    kernel_new_action.sa_flags = bionic_new_action->sa_flags;
    kernel_new_action.sa_handler = bionic_new_action->sa_handler;
    // Don't filter signals here; if the caller asked for everything to be blocked, we should obey.
    kernel_new_action.sa_mask = bionic_new_action->sa_mask;
#if defined(SA_RESTORER)
    kernel_new_action.sa_restorer = bionic_new_action->sa_restorer;
#if defined(__aarch64__)
    // arm64 has sa_restorer, but unwinding works best if you just let the
    // kernel supply the default restorer from [vdso]. gdb doesn't care, but
    // libgcc needs the nop that the kernel includes before the actual code.
    // (We could add that ourselves, but why bother?)
#else
    if (!(kernel_new_action.sa_flags & SA_RESTORER)) {
      kernel_new_action.sa_flags |= SA_RESTORER;
      kernel_new_action.sa_restorer = &__restore_rt;
    }
#endif
#endif
  }

  __kernel_sigaction kernel_old_action;
  int result = __rt_sigaction(signal,
                              (bionic_new_action != nullptr) ? &kernel_new_action : nullptr,
                              (bionic_old_action != nullptr) ? &kernel_old_action : nullptr,
                              sizeof(sigset_t));

  if (bionic_old_action != nullptr) {
    bionic_old_action->sa_flags = kernel_old_action.sa_flags;
    bionic_old_action->sa_handler = kernel_old_action.sa_handler;
    bionic_old_action->sa_mask = kernel_old_action.sa_mask;
#if defined(SA_RESTORER)
    bionic_old_action->sa_restorer = kernel_old_action.sa_restorer;
#endif
  }

  return result;
}

__strong_alias(sigaction64, sigaction);

#else

extern "C" int __rt_sigaction(int, const struct sigaction64*, struct sigaction64*, size_t);

int sigaction(int signal, const struct sigaction* bionic_new, struct sigaction* bionic_old) {
  // The 32-bit ABI is broken. struct sigaction includes a too-small sigset_t,
  // so we have to translate to struct sigaction64 first.
  struct sigaction64 kernel_new;
  if (bionic_new) {
    kernel_new = {};
    kernel_new.sa_flags = bionic_new->sa_flags;
    kernel_new.sa_handler = bionic_new->sa_handler;
#if defined(SA_RESTORER)
    kernel_new.sa_restorer = bionic_new->sa_restorer;
#endif
    // Don't filter signals here; if the caller asked for everything to be blocked, we should obey.
    memcpy(&kernel_new.sa_mask, &bionic_new->sa_mask, sizeof(bionic_new->sa_mask));
  }

  struct sigaction64 kernel_old;
  int result = sigaction64(signal, bionic_new ? &kernel_new : nullptr, &kernel_old);
  if (bionic_old) {
    *bionic_old = {};
    bionic_old->sa_flags = kernel_old.sa_flags;
    bionic_old->sa_handler = kernel_old.sa_handler;
#if defined(SA_RESTORER)
    bionic_old->sa_restorer = kernel_old.sa_restorer;
#endif
    memcpy(&bionic_old->sa_mask, &kernel_old.sa_mask, sizeof(bionic_old->sa_mask));
  }
  return result;
}

int sigaction64(int signal, const struct sigaction64* bionic_new, struct sigaction64* bionic_old) {
  struct sigaction64 kernel_new;
  if (bionic_new) {
    kernel_new = *bionic_new;
#if defined(SA_RESTORER)
    if (!(kernel_new.sa_flags & SA_RESTORER)) {
      kernel_new.sa_flags |= SA_RESTORER;
      kernel_new.sa_restorer = (kernel_new.sa_flags & SA_SIGINFO) ? &__restore_rt : &__restore;
    }
#endif
    // Don't filter signals here; if the caller asked for everything to be blocked, we should obey.
    kernel_new.sa_mask = kernel_new.sa_mask;
  }

  return __rt_sigaction(signal,
                        bionic_new ? &kernel_new : nullptr,
                        bionic_old,
                        sizeof(kernel_new.sa_mask));
}

#endif
