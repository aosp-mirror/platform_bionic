/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>

static const char* g_local_string = "This string is local to root library";
extern "C" const char* g_private_extern_string;
extern "C" const char* g_public_extern_string;

// This is resolved only if public library is in the same namespace as
// the root one. It should remain unresolved if looking up for public library
// crosses namespace boundary.
//
// Defined in libnstest_public_internal.so on which libnstest_public.so
// depends on
extern "C" const char* __attribute__((weak)) internal_extern_string();

bool g_dlopened = false;

extern "C" const char* ns_get_local_string() {
  return g_local_string;
}

extern "C" const char* ns_get_private_extern_string() {
  return g_private_extern_string;
}

extern "C" const char* ns_get_public_extern_string() {
  return g_public_extern_string;
}

extern "C" const char* ns_get_internal_extern_string() {
  if (internal_extern_string != nullptr) {
    return internal_extern_string();
  } else {
    return nullptr;
  }
}

extern "C" const char* ns_get_dlopened_string() {
  void* handle = dlopen("libnstest_dlopened.so", RTLD_NOW | RTLD_GLOBAL);
  if (handle == nullptr) {
    return nullptr;
  }

  const char** result = static_cast<const char**>(dlsym(handle, "g_private_dlopened_string"));
  if (result == nullptr) {
    return nullptr;
  } else {
    g_dlopened = true;
  }

  return *result;
}
