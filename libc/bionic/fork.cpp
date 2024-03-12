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

#include <unistd.h>

#include <android/fdsan.h>

#include "private/bionic_defs.h"
#include "private/bionic_fdtrack.h"
#include "pthread_internal.h"

__BIONIC_WEAK_FOR_NATIVE_BRIDGE_INLINE
int __clone_for_fork() {
  pthread_internal_t* self = __get_thread();

  int result = clone(nullptr, nullptr, (CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID | SIGCHLD),
                     nullptr, nullptr, nullptr, &(self->tid));

  if (result == 0) {
    // Update the cached pid in child, since clone() will not set it directly (as
    // self->tid is updated by the kernel).
    self->set_cached_pid(gettid());
  }

  return result;
}

int _Fork() {
  return __clone_for_fork();
}

int fork() {
  __bionic_atfork_run_prepare();
  int result = _Fork();
  if (result == 0) {
    // Disable fdsan and fdtrack post-fork, so we don't falsely trigger on processes that
    // fork, close all of their fds, and then exec.
    android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_DISABLED);
    android_fdtrack_set_globally_enabled(false);

    // Reset the stack_and_tls VMA name so it doesn't end with a tid from the
    // parent process.
    __set_stack_and_tls_vma_name(true);

    __bionic_atfork_run_child();
  } else {
    __bionic_atfork_run_parent();
  }
  return result;
}
