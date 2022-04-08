/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include "linker.h"
#include "linker_translate_path.h"
#include "linker_utils.h"

#if defined(__LP64__)
static const char* const kSystemLibDir        = "/system/lib64";
static const char* const kI18nApexLibDir      = "/apex/com.android.i18n/lib64";
#else
static const char* const kSystemLibDir        = "/system/lib";
static const char* const kI18nApexLibDir      = "/apex/com.android.i18n/lib";
#endif

// Workaround for dlopen(/system/lib(64)/<soname>) when .so is in /apex. http://b/121248172
/**
 * Translate /system path to /apex path if needed
 * The workaround should work only when targetSdkVersion < Q.
 *
 * param out_name_to_apex pointing to /apex path
 * return true if translation is needed
 */
bool translateSystemPathToApexPath(const char* name, std::string* out_name_to_apex) {
  static const char* const kSystemToArtApexLibs[] = {
      "libicuuc.so",
      "libicui18n.so",
  };
  // New mapping for new apex should be added below

  // Nothing to do if target sdk version is Q or above
  if (get_application_target_sdk_version() >= 29) {
    return false;
  }

  // If the path isn't /system/lib, there's nothing to do.
  if (name == nullptr || dirname(name) != kSystemLibDir) {
    return false;
  }

  const char* base_name = basename(name);

  for (const char* soname : kSystemToArtApexLibs) {
    if (strcmp(base_name, soname) == 0) {
      *out_name_to_apex = std::string(kI18nApexLibDir) + "/" + base_name;
      return true;
    }
  }

  return false;
}
// End Workaround for dlopen(/system/lib/<soname>) when .so is in /apex.
