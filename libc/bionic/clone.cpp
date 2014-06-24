/*
 * Copyright (C) 2010 The Android Open Source Project
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

#define __GNU_SOURCE 1
#include <sched.h>
#include <stdlib.h>
#include <stdarg.h>

#include "pthread_internal.h"

extern "C" pid_t __bionic_clone(uint32_t flags, void* child_stack, int* parent_tid, void* tls, int* child_tid, int (*fn)(void*), void* arg);
extern "C" __noreturn void __exit(int status);

// Called from the __bionic_clone assembler to call the thread function then exit.
extern "C" __LIBC_HIDDEN__ void __start_thread(int (*fn)(void*), void* arg) {
  int status = (*fn)(arg);
  __exit(status);
}

int clone(int (*fn)(void*), void* child_stack, int flags, void* arg, ...) {
  int* parent_tid = NULL;
  void* new_tls = NULL;
  int* child_tid = NULL;

  // Extract any optional parameters required by the flags.
  va_list args;
  va_start(args, arg);
  if ((flags & (CLONE_PARENT_SETTID|CLONE_SETTLS|CLONE_CHILD_SETTID|CLONE_CHILD_CLEARTID)) != 0) {
    parent_tid = va_arg(args, int*);
  }
  if ((flags & (CLONE_SETTLS|CLONE_CHILD_SETTID|CLONE_CHILD_CLEARTID)) != 0) {
    new_tls = va_arg(args, void*);
  }
  if ((flags & (CLONE_CHILD_SETTID|CLONE_CHILD_CLEARTID)) != 0) {
    child_tid = va_arg(args, int*);
  }
  va_end(args);

  // Align 'child_stack' to 16 bytes.
  uintptr_t child_stack_addr = reinterpret_cast<uintptr_t>(child_stack);
  child_stack_addr &= ~0xf;
  child_stack = reinterpret_cast<void*>(child_stack_addr);

  // Remember the parent pid and invalidate the cached value while we clone.
  pthread_internal_t* self = __get_thread();
  pid_t parent_pid = self->invalidate_cached_pid();

  // Actually do the clone.
  int clone_result = __bionic_clone(flags, child_stack, parent_tid, new_tls, child_tid, fn, arg);

  // We're the parent, so put our known pid back in place.
  // We leave the child without a cached pid, but:
  // 1. pthread_create gives its children their own pthread_internal_t with the correct pid.
  // 2. fork makes a clone system call directly.
  // If any other cases become important, we could use a double trampoline like __pthread_start.
  self->set_cached_pid(parent_pid);
  return clone_result;
}
