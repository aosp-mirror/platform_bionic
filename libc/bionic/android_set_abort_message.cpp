/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <android/set_abort_message.h>

#include <pthread.h>
#include <sys/mman.h>

#include "private/ScopedPthreadMutexLocker.h"

static pthread_mutex_t g_abort_msg_lock = PTHREAD_MUTEX_INITIALIZER;

struct abort_msg_t {
  size_t size;
  char msg[0];
};

abort_msg_t** __abort_message_ptr; // Accessible to __libc_init_common.

void android_set_abort_message(const char* msg) {
  ScopedPthreadMutexLocker locker(&g_abort_msg_lock);

  if (__abort_message_ptr == nullptr) {
    // We must have crashed _very_ early.
    return;
  }

  if (*__abort_message_ptr != nullptr) {
    // We already have an abort message.
    // Assume that the first crash is the one most worth reporting.
    return;
  }

  size_t size = sizeof(abort_msg_t) + strlen(msg) + 1;
  void* map = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (map == MAP_FAILED) {
    return;
  }

  abort_msg_t* new_abort_message = reinterpret_cast<abort_msg_t*>(map);
  new_abort_message->size = size;
  strcpy(new_abort_message->msg, msg);
  *__abort_message_ptr = new_abort_message;
}
