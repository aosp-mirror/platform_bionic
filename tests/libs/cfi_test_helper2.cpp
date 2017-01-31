/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <assert.h>
#include <dlfcn.h>

#include "libs_utils.h"

int main(void) {
  void* handle;
  // libcfi-test.so does some basic testing in a global constructor. Check that it is linked.
  handle = dlopen("libcfi-test.so", RTLD_NOW | RTLD_NOLOAD);
  CHECK(handle != nullptr);
  dlclose(handle);
  return 0;
}
