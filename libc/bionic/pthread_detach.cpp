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

#include "pthread_accessor.h"

int pthread_detach(pthread_t t) {
  {
    pthread_accessor thread(t);
    if (thread.get() == NULL) {
      return ESRCH;
    }

    if (thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) {
      return EINVAL; // Already detached.
    }

    if (thread->attr.flags & PTHREAD_ATTR_FLAG_JOINED) {
      return 0; // Already being joined; silently do nothing, like glibc.
    }

    // If the thread has not exited, we can detach it safely.
    if ((thread->attr.flags & PTHREAD_ATTR_FLAG_ZOMBIE) == 0) {
      thread->attr.flags |= PTHREAD_ATTR_FLAG_DETACHED;
      return 0;
    }
  }

  // The thread is in zombie state, use pthread_join to clean it up.
  return pthread_join(t, NULL);
}
