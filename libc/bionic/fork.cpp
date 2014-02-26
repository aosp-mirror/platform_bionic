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
#include <sys/syscall.h>

#include "private/libc_logging.h"
#include "pthread_internal.h"

int fork() {
  // POSIX mandates that the timers of a fork child process be
  // disarmed, but not destroyed. To avoid a race condition, we're
  // going to stop all timers now, and only re-start them in case
  // of error, or in the parent process
  __timer_table_start_stop(1);
  __bionic_atfork_run_prepare();

  pthread_internal_t* self = __get_thread();
  int flags = CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID | SIGCHLD;
#if defined(__x86_64__) // sys_clone's last two arguments are flipped on x86-64.
  int result = syscall(__NR_clone, flags, NULL, NULL, &(self->tid), NULL);
#else
  int result = syscall(__NR_clone, flags, NULL, NULL, NULL, &(self->tid));
#endif
  if (result == 0) {
    __bionic_atfork_run_child();
  } else {
    __timer_table_start_stop(0);
    __bionic_atfork_run_parent();
  }
  return result;
}
