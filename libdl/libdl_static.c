/*
 * Copyright (C) 2007 The Android Open Source Project
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
#include <link.h>
#include <stdlib.h>
#include <stdbool.h>

// Proxy calls to bionic loader
void* dlopen(const char* filename __unused, int flag __unused) {
  return NULL;
}

char* dlerror() {
  return NULL;
}

void* dlsym(void* handle __unused, const char* symbol __unused) {
  return NULL;
}

void* dlvsym(void* handle __unused,
             const char* symbol __unused,
             const char* version __unused) {
  return NULL;
}

int dladdr(const void* addr __unused, Dl_info* info __unused) {
  return 0;
}

int dlclose(void* handle __unused) {
  return -1;
}

#if defined(__arm__)
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc __unused, int* pcount __unused) {
  return 0;
}
#endif

void android_set_application_target_sdk_version(uint32_t target __unused) {
}

