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

extern "C" void __restore_rt(void);
extern "C" void __restore(void);

#if defined(__LP64__)

extern "C" int __rt_sigaction(int, const struct __kernel_sigaction*, struct __kernel_sigaction*, size_t);

int sigaction(int signal, const struct sigaction* bionic_new_action, struct sigaction* bionic_old_action) {
  __kernel_sigaction kernel_new_action;
  if (bionic_new_action != NULL) {
    kernel_new_action.sa_flags = bionic_new_action->sa_flags;
    kernel_new_action.sa_handler = bionic_new_action->sa_handler;
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
                              (bionic_new_action != NULL) ? &kernel_new_action : NULL,
                              (bionic_old_action != NULL) ? &kernel_old_action : NULL,
                              sizeof(sigset_t));

  if (bionic_old_action != NULL) {
    bionic_old_action->sa_flags = kernel_old_action.sa_flags;
    bionic_old_action->sa_handler = kernel_old_action.sa_handler;
    bionic_old_action->sa_mask = kernel_old_action.sa_mask;
#if defined(SA_RESTORER)
    bionic_old_action->sa_restorer = kernel_old_action.sa_restorer;
#endif
  }

  return result;
}

#else

extern "C" int __sigaction(int, const struct sigaction*, struct sigaction*);

int sigaction(int signal, const struct sigaction* bionic_new_action, struct sigaction* bionic_old_action) {
  // The 32-bit ABI is broken. struct sigaction includes a too-small sigset_t,
  // so we have to use sigaction(2) rather than rt_sigaction(2).
  struct sigaction kernel_new_action;
  if (bionic_new_action != NULL) {
    kernel_new_action.sa_flags = bionic_new_action->sa_flags;
    kernel_new_action.sa_handler = bionic_new_action->sa_handler;
    kernel_new_action.sa_mask = bionic_new_action->sa_mask;
#if defined(SA_RESTORER)
    kernel_new_action.sa_restorer = bionic_new_action->sa_restorer;

    if (!(kernel_new_action.sa_flags & SA_RESTORER)) {
      kernel_new_action.sa_flags |= SA_RESTORER;
      kernel_new_action.sa_restorer = (kernel_new_action.sa_flags & SA_SIGINFO) ? &__restore_rt : &__restore;
    }
#endif
  }
  return __sigaction(signal, (bionic_new_action != NULL) ? &kernel_new_action : NULL, bionic_old_action);
}

#endif
