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

// This one should be preempted by the function
// defined in libdl_preempt_test_1.so
extern "C" int __attribute__((weak)) lib_global_default_serial() {
  return 2716057;
}

// Even though this one is defined by
// libdl_preempt_test_1.so it should not be
// preempted because of protected visibility
extern "C" int __attribute__((weak,visibility("protected"))) lib_global_protected_serial() {
  return 3370318;
}

extern "C" int lib_global_default_get_serial() {
  return lib_global_default_serial();
}

extern "C" int lib_global_protected_get_serial() {
  return lib_global_protected_serial();
}

