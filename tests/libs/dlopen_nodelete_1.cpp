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

#include <stdint.h>
#include <stdlib.h>

uint32_t dlopen_nodelete_1_taxicab_number = 1729;
static bool* unload_flag_ptr = nullptr;

extern "C" void dlopen_nodelete_1_set_unload_flag_ptr(bool* ptr) {
  unload_flag_ptr = ptr;
}

static void __attribute__((destructor)) unload_guard() {
  if (unload_flag_ptr != nullptr) {
    *unload_flag_ptr = true;
  }
}
