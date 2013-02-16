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

#include "pthread_internal.h"

#include "bionic_tls.h"
#include "ScopedPthreadMutexLocker.h"

__LIBC_HIDDEN__ pthread_internal_t* gThreadList = NULL;
__LIBC_HIDDEN__ pthread_mutex_t gThreadListLock = PTHREAD_MUTEX_INITIALIZER;

void _pthread_internal_remove_locked(pthread_internal_t* thread) {
  if (thread->next != NULL) {
    thread->next->prev = thread->prev;
  }
  if (thread->prev != NULL) {
    thread->prev->next = thread->next;
  } else {
    gThreadList = thread->next;
  }

  // The main thread is not heap-allocated. See __libc_init_tls for the declaration,
  // and __libc_init_common for the point where it's added to the thread list.
  if (thread->allocated_on_heap) {
    free(thread);
  }
}

__LIBC_ABI_PRIVATE__ void _pthread_internal_add(pthread_internal_t* thread) {
  ScopedPthreadMutexLocker locker(&gThreadListLock);

  // We insert at the head.
  thread->next = gThreadList;
  thread->prev = NULL;
  if (thread->next != NULL) {
    thread->next->prev = thread;
  }
  gThreadList = thread;
}

__LIBC_ABI_PRIVATE__ pthread_internal_t* __get_thread(void) {
  void** tls = reinterpret_cast<void**>(const_cast<void*>(__get_tls()));
  return reinterpret_cast<pthread_internal_t*>(tls[TLS_SLOT_THREAD_ID]);
}
