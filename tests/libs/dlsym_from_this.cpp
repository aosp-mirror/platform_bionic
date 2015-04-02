/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <stdio.h>

int test_dlsym_symbol = 42;

extern "C" int* lookup_dlsym_symbol_using_RTLD_DEFAULT() {
  dlerror();
  int* result = static_cast<int*>(dlsym(RTLD_DEFAULT, "test_dlsym_symbol"));
  // TODO: remove this once b/20049306 is fixed
  if (result == nullptr) {
    printf("Cannot find the answer\n");
  }
  return result;
}

