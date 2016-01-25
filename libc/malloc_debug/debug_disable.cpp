/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <pthread.h>

#include "DebugData.h"
#include "debug_disable.h"
#include "debug_log.h"

extern DebugData* g_debug;
pthread_key_t g_disable_key;

bool DebugCallsDisabled() {
  if (g_debug == nullptr || pthread_getspecific(g_disable_key) != nullptr) {
    return true;
  }
  return false;
}

bool DebugDisableInitialize() {
  int error = pthread_key_create(&g_disable_key, nullptr);
  if (error != 0) {
    error_log("pthread_key_create failed: %s", strerror(error));
    return false;
  }
  pthread_setspecific(g_disable_key, nullptr);

  return true;
}

void DebugDisableFinalize() {
  pthread_key_delete(g_disable_key);
}

void DebugDisableSet(bool disable) {
  if (disable) {
    pthread_setspecific(g_disable_key, reinterpret_cast<void*>(1));
  } else {
    pthread_setspecific(g_disable_key, nullptr);
  }
}
