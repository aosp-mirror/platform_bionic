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

#include "private/bionic_futex.h"
#include "pthread_accessor.h"

int pthread_join(pthread_t t, void** return_value) {
  if (t == pthread_self()) {
    return EDEADLK;
  }

  pid_t tid;
  volatile int* tid_ptr;
  {
    pthread_accessor thread(t);
    if (thread.get() == NULL) {
      return ESRCH;
    }

    if ((thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) != 0) {
      return EINVAL;
    }

    if ((thread->attr.flags & PTHREAD_ATTR_FLAG_JOINED) != 0) {
      return EINVAL;
    }

    // Okay, looks like we can signal our intention to join.
    thread->attr.flags |= PTHREAD_ATTR_FLAG_JOINED;
    tid = thread->tid;
    tid_ptr = &thread->tid;
  }

  // We set the PTHREAD_ATTR_FLAG_JOINED flag with the lock held,
  // so no one is going to remove this thread except us.

  // Wait for the thread to actually exit, if it hasn't already.
  while (*tid_ptr != 0) {
    __futex_wait(tid_ptr, tid, NULL);
  }

  // Take the lock again so we can pull the thread's return value
  // and remove the thread from the list.
  pthread_accessor thread(t);

  if (return_value) {
    *return_value = thread->return_value;
  }

  _pthread_internal_remove_locked(thread.get());
  return 0;
}
