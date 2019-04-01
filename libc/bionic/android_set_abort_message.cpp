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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include "private/bionic_defs.h"
#include "private/bionic_globals.h"
#include "private/ScopedPthreadMutexLocker.h"

struct abort_msg_t {
  size_t size;
  char msg[0];
};
static_assert(
    offsetof(abort_msg_t, msg) == sizeof(size_t),
    "The in-memory layout of abort_msg_t is not consistent with what libdebuggerd expects.");

struct magic_abort_msg_t {
  uint64_t magic1;
  uint64_t magic2;
  abort_msg_t msg;
};
static_assert(offsetof(magic_abort_msg_t, msg) == 2 * sizeof(uint64_t),
              "The in-memory layout of magic_abort_msg_t is not consistent with what automated "
              "tools expect.");

[[clang::optnone]]
static void fill_abort_message_magic(magic_abort_msg_t* new_magic_abort_message) {
  // 128-bit magic for the abort message. Chosen by fair dice roll.
  // This function is intentionally deoptimized to avoid the magic to be present
  // in the final binary. This causes clang to only use instructions where parts
  // of the magic are encoded into immediate arguments for the instructions in
  // all supported architectures.
  new_magic_abort_message->magic1 = 0xb18e40886ac388f0ULL;
  new_magic_abort_message->magic2 = 0xc6dfba755a1de0b5ULL;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void android_set_abort_message(const char* msg) {
  ScopedPthreadMutexLocker locker(&__libc_shared_globals()->abort_msg_lock);

  if (__libc_shared_globals()->abort_msg != nullptr) {
    // We already have an abort message.
    // Assume that the first crash is the one most worth reporting.
    return;
  }

  size_t size = sizeof(magic_abort_msg_t) + strlen(msg) + 1;
  void* map = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (map == MAP_FAILED) {
    return;
  }

  // Name the abort message mapping to make it easier for tools to find the
  // mapping.
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, map, size, "abort message");

  magic_abort_msg_t* new_magic_abort_message = reinterpret_cast<magic_abort_msg_t*>(map);
  fill_abort_message_magic(new_magic_abort_message);
  new_magic_abort_message->msg.size = size;
  strcpy(new_magic_abort_message->msg.msg, msg);
  __libc_shared_globals()->abort_msg = &new_magic_abort_message->msg;
}
