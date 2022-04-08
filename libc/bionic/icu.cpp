/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "private/icu.h"

#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include <async_safe/log.h>

static void* g_libicuuc_handle = nullptr;

static bool __find_icu() {
  g_libicuuc_handle = dlopen("libandroidicu.so", RTLD_LOCAL);
  if (g_libicuuc_handle == nullptr) {
    async_safe_format_log(ANDROID_LOG_ERROR, "bionic-icu", "couldn't open libandroidicu.so: %s",
                          dlerror());
    return false;
  }

  return true;
}

void* __find_icu_symbol(const char* symbol_name) {
  static bool found_icu = __find_icu();
  if (!found_icu) return nullptr;

  char versioned_symbol_name[strlen(symbol_name) + strlen("_android") + 1];
  snprintf(versioned_symbol_name, sizeof(versioned_symbol_name), "%s_android",
           symbol_name);

  void* symbol = dlsym(g_libicuuc_handle, versioned_symbol_name);
  if (symbol == nullptr) {
    async_safe_format_log(ANDROID_LOG_ERROR, "bionic-icu", "couldn't find %s",
                          versioned_symbol_name);
  }
  return symbol;
}
