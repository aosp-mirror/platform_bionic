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

extern "C" {
  int versioned_function_v1(); // __attribute__((visibility("hidden")));
  int versioned_function_v2(); // __attribute__((visibility("hidden")));
  int versioned_function_v3(); // __attribute__((visibility("hidden")));
  int version_zero_function();
}

int versioned_function_v1() {
  return 1;
}

int versioned_function_v2() {
  return 2;
}

int versioned_function_v3() {
  return 3;
}

int version_zero_function() {
  return 1000;
}

__asm__(".symver versioned_function_v1,versioned_function@TESTLIB_V1");
__asm__(".symver versioned_function_v2,versioned_function@TESTLIB_V2");
__asm__(".symver versioned_function_v3,versioned_function@@TESTLIB_V3");
