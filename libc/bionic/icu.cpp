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

#include "private/libc_logging.h"

// Allowed icu4c version numbers are in the range [44, 999].
// Gingerbread's icu4c 4.4 is the minimum supported ICU version.
static constexpr auto ICUDATA_VERSION_MIN_LENGTH = 2;
static constexpr auto ICUDATA_VERSION_MAX_LENGTH = 3;
static constexpr auto ICUDATA_VERSION_MIN = 44;

static char g_icudata_version[ICUDATA_VERSION_MAX_LENGTH + 1];

static void* g_libicuuc_handle = nullptr;

static int __icu_dat_file_filter(const dirent* dirp) {
  const char* name = dirp->d_name;

  // Is the name the right length to match 'icudt(\d\d\d)l.dat'?
  const size_t len = strlen(name);
  if (len < 10 + ICUDATA_VERSION_MIN_LENGTH || len > 10 + ICUDATA_VERSION_MAX_LENGTH) return 0;

  return !strncmp(name, "icudt", 5) && !strncmp(&name[len - 5], "l.dat", 5);
}

static bool __find_icu() {
  dirent** namelist = nullptr;
  int n = scandir("/system/usr/icu", &namelist, &__icu_dat_file_filter, alphasort);
  int max_version = -1;
  while (n--) {
    // We prefer the latest version available.
    int version = atoi(&namelist[n]->d_name[strlen("icudt")]);
    if (version != 0 && version > max_version) max_version = version;
    free(namelist[n]);
  }
  free(namelist);

  if (max_version == -1 || max_version < ICUDATA_VERSION_MIN) {
    __libc_write_log(ANDROID_LOG_ERROR, "bionic-icu", "couldn't find an ICU .dat file");
    return false;
  }

  snprintf(g_icudata_version, sizeof(g_icudata_version), "_%d", max_version);

  g_libicuuc_handle = dlopen("libicuuc.so", RTLD_LOCAL);
  if (g_libicuuc_handle == nullptr) {
    __libc_format_log(ANDROID_LOG_ERROR, "bionic-icu", "couldn't open libicuuc.so: %s", dlerror());
    return false;
  }

  return true;
}

void* __find_icu_symbol(const char* symbol_name) {
  static bool found_icu = __find_icu();
  if (!found_icu) return nullptr;

  char versioned_symbol_name[strlen(symbol_name) + sizeof(g_icudata_version)];
  snprintf(versioned_symbol_name, sizeof(versioned_symbol_name), "%s%s",
           symbol_name, g_icudata_version);

  void* symbol = dlsym(g_libicuuc_handle, versioned_symbol_name);
  if (symbol == nullptr) {
    __libc_format_log(ANDROID_LOG_ERROR, "bionic-icu", "couldn't find %s", versioned_symbol_name);
  }
  return symbol;
}
