/*
 * Copyright (C) 2024 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "memtag_globals.h"

// Adapted from the LLD test suite: lld/test/ELF/Inputs/aarch64-memtag-globals.s

int global_extern;
static int global_extern_hidden;
__attribute__((no_sanitize("memtag"))) int global_extern_untagged;
__attribute__((no_sanitize("memtag"))) int global_extern_untagged_definition_but_tagged_import;

void assertion_failure() {
  exit(1);
}

void check_tagged(const void* a) {
  uintptr_t a_uptr = reinterpret_cast<uintptr_t>(a);
#if defined(__aarch64__)
  if ((a_uptr >> 56) == 0) {
    fprintf(stderr, "**********************************\n");
    fprintf(stderr, "Failed assertion:\n");
    fprintf(stderr, "  tag(0x%zx) != 0\n", a_uptr);
    fprintf(stderr, "**********************************\n");

    assertion_failure();
  }
#endif  // defined(__aarch64__)
}

void check_untagged(const void* a) {
  uintptr_t a_uptr = reinterpret_cast<uintptr_t>(a);
#if defined(__aarch64__)
  if ((a_uptr >> 56) != 0) {
    fprintf(stderr, "**********************************\n");
    fprintf(stderr, "Failed assertion:\n");
    fprintf(stderr, "  tag(0x%zx) == 0\n", a_uptr);
    fprintf(stderr, "**********************************\n");

    assertion_failure();
  }
#endif  // defined(__aarch64__)
}

void check_matching_tags(const void* a, const void* b) {
  uintptr_t a_uptr = reinterpret_cast<uintptr_t>(a);
  uintptr_t b_uptr = reinterpret_cast<uintptr_t>(b);
#if defined(__aarch64__)
  if (a_uptr >> 56 != b_uptr >> 56) {
    fprintf(stderr, "**********************************\n");
    fprintf(stderr, "Failed assertion:\n");
    fprintf(stderr, "  tag(0x%zx) != tag(0x%zx)\n", a_uptr, b_uptr);
    fprintf(stderr, "**********************************\n");

    assertion_failure();
  }
#endif  // defined(__aarch64__)
}

void check_eq(const void* a, const void* b) {
  if (a != b) {
    fprintf(stderr, "**********************************\n");
    fprintf(stderr, "Failed assertion:\n");
    fprintf(stderr, "  %p != %p\n", a, b);
    fprintf(stderr, "**********************************\n");

    assertion_failure();
  }
}

#define LONGEST_VARIABLE_NAME "51"
void print_variable_address(const char* name, const void* ptr) {
  printf("%" LONGEST_VARIABLE_NAME "s: %16p\n", name, ptr);
}

static const std::vector<std::pair<const char*, const void*>>& get_expected_tagged_vars() {
  static std::vector<std::pair<const char*, const void*>> expected_tagged_vars = {
      {"global_extern", &global_extern},
      {"global_extern_hidden", &global_extern_hidden},
  };
  return expected_tagged_vars;
}

static const std::vector<std::pair<const char*, const void*>>& get_expected_untagged_vars() {
  static std::vector<std::pair<const char*, const void*>> expected_untagged_vars = {
      {"global_extern_untagged", &global_extern_untagged},
      {"global_extern_untagged_definition_but_tagged_import",
       &global_extern_untagged_definition_but_tagged_import},
  };
  return expected_untagged_vars;
}

void dso_print_variables() {
  print_variables("  Variables declared in the DSO:\n", get_expected_tagged_vars(),
                  get_expected_untagged_vars());
}

void print_variables(const char* header,
                     const std::vector<std::pair<const char*, const void*>>& tagged_variables,
                     const std::vector<std::pair<const char*, const void*>>& untagged_variables) {
  printf("==========================================================\n");
  printf("%s", header);
  printf("==========================================================\n");
  printf(" Variables expected to be tagged:\n");
  printf("----------------------------------------------------------\n");
  for (const auto& [name, pointer] : tagged_variables) {
    print_variable_address(name, pointer);
  }

  printf("\n----------------------------------------------------------\n");
  printf(" Variables expected to be untagged:\n");
  printf("----------------------------------------------------------\n");
  for (const auto& [name, pointer] : untagged_variables) {
    print_variable_address(name, pointer);
  }
  printf("\n");
}

void dso_check_assertions(bool check_pointers_are_tagged) {
  // Check that non-const variables are writeable.
  global_extern = 0;
  global_extern_hidden = 0;
  global_extern_untagged = 0;
  global_extern_untagged_definition_but_tagged_import = 0;

  if (check_pointers_are_tagged) {
    for (const auto& [_, pointer] : get_expected_tagged_vars()) {
      check_tagged(pointer);
    }
  }

  for (const auto& [_, pointer] : get_expected_untagged_vars()) {
    check_untagged(pointer);
  }
}
