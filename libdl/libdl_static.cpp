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

void* dlopen(const char* /*filename*/, int /*flag*/) {
  return nullptr;
}

char* dlerror() {
  return nullptr;
}

void* dlsym(void* /*handle*/, const char* /*symbol*/) {
  return nullptr;
}

void* dlvsym(void* /*handle*/, const char* /*symbol*/, const char* /*version*/) {
  return nullptr;
}

int dladdr(const void* /*addr*/, Dl_info* /*info*/) {
  return 0;
}

int dlclose(void* /*handle*/) {
  return -1;
}

#if defined(__arm__)
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr /*pc*/, int* /*pcount*/) {
  return 0;
}
#endif
