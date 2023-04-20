/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <stdint.h>

#if __has_feature(hwaddress_sanitizer)
extern "C" uint32_t dlopen_testlib_compiled_with_hwasan = true;
#else
extern "C" uint32_t dlopen_testlib_compiled_with_hwasan = false;
#endif

extern "C" bool dlopen_testlib_simple_hwasan_func() {
  return true;
}
