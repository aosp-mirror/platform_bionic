/*
 * Copyright (C) 2016 The Android Open Source Project
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
#include <stdlib.h>

static void* g_libc_close_ptr;

static void __attribute__((constructor)) __libc_close_lookup() {
  g_libc_close_ptr = dlsym(RTLD_NEXT, "close");
}

// A libc function used for RTLD_NEXT
// This function in not supposed to be called
extern "C" int __attribute__((weak)) close(int) {
  abort();
}

extern "C" void* get_libc_close_ptr() {
  return g_libc_close_ptr;
}


