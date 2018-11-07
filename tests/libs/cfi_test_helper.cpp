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
#include <stdint.h>
#include <stdlib.h>

#include "libs_utils.h"

// This library is built for all targets, including host tests, so __cfi_slowpath may not be
// present. But it is only used in the bionic loader tests.
extern "C" __attribute__((weak)) void __cfi_slowpath(uint64_t, void*);

static int g_count;

// Mock a CFI-enabled library without relying on the compiler.
extern "C" __attribute__((no_sanitize("hwaddress")))  __attribute__((aligned(4096)))
void __cfi_check(uint64_t /*CallSiteTypeId*/, void* /*TargetAddr*/, void* /*Diag*/) {
  ++g_count;
}

// This code runs before hwasan is initialized.
__attribute__((no_sanitize("hwaddress")))
void preinit_ctor() {
  CHECK(g_count == 0);
  __cfi_slowpath(42, reinterpret_cast<void*>(&preinit_ctor));
  CHECK(g_count == 1);
}

__attribute__((section(".preinit_array"), used)) void (*preinit_ctor_p)(void) = preinit_ctor;

__attribute__((constructor, used)) void ctor() {
  CHECK(g_count == 1);
  __cfi_slowpath(42, reinterpret_cast<void*>(&ctor));
  CHECK(g_count == 2);
}

int main(void) {
  CHECK(g_count == 2);
  __cfi_slowpath(42, reinterpret_cast<void*>(&main));
  CHECK(g_count == 3);
  return 0;
}
