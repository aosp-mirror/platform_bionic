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

// This library is built for all targets, including host tests, so __cfi_slowpath may not be
// present. But it is only used in the bionic loader tests.
extern "C" __attribute__((weak)) void __cfi_slowpath(uint64_t, void*);

static size_t g_count;
static uint64_t g_last_type_id;
static void* g_last_address;
static void* g_last_diag;

extern "C" {

// Make sure the library crosses at least one kLibraryAlignment(=256KB) boundary.
char bss[1024 * 1024];

// Mock a CFI-enabled library without relying on the compiler.
__attribute__((aligned(4096))) void __cfi_check(uint64_t CallSiteTypeId, void* TargetAddr,
                                                void* Diag) {
  ++g_count;
  g_last_type_id = CallSiteTypeId;
  g_last_address = TargetAddr;
  g_last_diag = Diag;
}

size_t get_count() {
  return g_count;
}

uint64_t get_last_type_id() {
  return g_last_type_id;
}

void* get_last_address() {
  return g_last_address;
}

void* get_last_diag() {
  return g_last_diag;
}

void* get_global_address() {
  return &g_count;
}
}

// Check that CFI is set up in module constructors and destructors.
struct A {
  void check_cfi_self() {
    g_last_type_id = 0;
    assert(&__cfi_slowpath);
    // CFI check for an address inside this DSO. This goes to the current module's __cfi_check,
    // which updates g_last_type_id.
    __cfi_slowpath(13, static_cast<void*>(&g_last_type_id));
    assert(g_last_type_id == 13);
    // CFI check for a libc function. This never goes into this module's __cfi_check, and must pass.
    __cfi_slowpath(14, reinterpret_cast<void*>(&exit));
    assert(g_last_type_id == 13);
  }
  A() {
    check_cfi_self();
  }
  ~A() {
    check_cfi_self();
  }
} a;
