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
#include <string.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "memtag_globals.h"

// Adapted from the LLD test suite: lld/test/ELF/Inputs/aarch64-memtag-globals.s

/// Global variables defined here, of various semantics.
char global[30] = {};
__attribute__((no_sanitize("memtag"))) int global_untagged = 0;
const int const_global = 0;
static const int hidden_const_global = 0;
static char hidden_global[12] = {};
__attribute__((visibility("hidden"))) int hidden_attr_global = 0;
__attribute__((visibility("hidden"))) const int hidden_attr_const_global = 0;

/// Should be untagged.
__thread int tls_global;
__thread static int hidden_tls_global;

/// Tagged, from the other file.
extern int global_extern;
/// Untagged, from the other file.
extern __attribute__((no_sanitize("memtag"))) int global_extern_untagged;
/// Tagged here, but untagged in the definition found in the sister objfile
/// (explicitly).
extern int global_extern_untagged_definition_but_tagged_import;

/// ABS64 relocations. Also, forces symtab entries for local and external
/// globals.
char* pointer_to_global = &global[0];
char* pointer_inside_global = &global[17];
char* pointer_to_global_end = &global[30];
char* pointer_past_global_end = &global[48];
int* pointer_to_global_untagged = &global_untagged;
const int* pointer_to_const_global = &const_global;
/// RELATIVE relocations.
const int* pointer_to_hidden_const_global = &hidden_const_global;
char* pointer_to_hidden_global = &hidden_global[0];
int* pointer_to_hidden_attr_global = &hidden_attr_global;
const int* pointer_to_hidden_attr_const_global = &hidden_attr_const_global;
/// RELATIVE relocations with special AArch64 MemtagABI semantics, with the
/// offset ('12' or '16') encoded in the place.
char* pointer_to_hidden_global_end = &hidden_global[12];
char* pointer_past_hidden_global_end = &hidden_global[16];
/// ABS64 relocations.
int* pointer_to_global_extern = &global_extern;
int* pointer_to_global_extern_untagged = &global_extern_untagged;
int* pointer_to_global_extern_untagged_definition_but_tagged_import =
    &global_extern_untagged_definition_but_tagged_import;

// Force materialization of these globals into the symtab.
int* get_address_to_tls_global() {
  return &tls_global;
}
int* get_address_to_hidden_tls_global() {
  return &hidden_tls_global;
}

static const std::vector<std::pair<const char*, const void*>>& get_expected_tagged_vars() {
  static std::vector<std::pair<const char*, const void*>> expected_tagged_vars = {
      {"global", &global},
      {"pointer_inside_global", pointer_inside_global},
      {"pointer_to_global_end", pointer_to_global_end},
      {"pointer_past_global_end", pointer_past_global_end},
      {"hidden_global", &hidden_global},
      {"hidden_attr_global", &hidden_attr_global},
      {"global_extern", &global_extern},
  };
  return expected_tagged_vars;
}

static const std::vector<std::pair<const char*, const void*>>& get_expected_untagged_vars() {
  static std::vector<std::pair<const char*, const void*>> expected_untagged_vars = {
      {"global_extern_untagged", &global_extern_untagged},
      {"global_extern_untagged_definition_but_tagged_import",
       &global_extern_untagged_definition_but_tagged_import},
      {"global_untagged", &global_untagged},
      {"const_global", &const_global},
      {"hidden_const_global", &hidden_const_global},
      {"hidden_attr_const_global", &hidden_attr_const_global},
      {"tls_global", &tls_global},
      {"hidden_tls_global", &hidden_tls_global},
  };
  return expected_untagged_vars;
}

void exe_print_variables() {
  print_variables("  Variables accessible from the binary:\n", get_expected_tagged_vars(),
                  get_expected_untagged_vars());
}

// Dump the addresses of the global variables to stderr
void dso_print();
void dso_print_others();

void exe_check_assertions(bool check_pointers_are_tagged) {
  // Check that non-const variables are writeable.
  *pointer_to_global = 0;
  *pointer_inside_global = 0;
  *(pointer_to_global_end - 1) = 0;
  *pointer_to_global_untagged = 0;
  *pointer_to_hidden_global = 0;
  *pointer_to_hidden_attr_global = 0;
  *(pointer_to_hidden_global_end - 1) = 0;
  *pointer_to_global_extern = 0;
  *pointer_to_global_extern_untagged = 0;
  *pointer_to_global_extern_untagged_definition_but_tagged_import = 0;

  if (check_pointers_are_tagged) {
    for (const auto& [_, pointer] : get_expected_tagged_vars()) {
      check_tagged(pointer);
    }
  }

  for (const auto& [_, pointer] : get_expected_untagged_vars()) {
    check_untagged(pointer);
  }

  check_matching_tags(pointer_to_global, pointer_inside_global);
  check_matching_tags(pointer_to_global, pointer_to_global_end);
  check_matching_tags(pointer_to_global, pointer_past_global_end);
  check_eq(pointer_inside_global, pointer_to_global + 17);
  check_eq(pointer_to_global_end, pointer_to_global + 30);
  check_eq(pointer_past_global_end, pointer_to_global + 48);

  check_matching_tags(pointer_to_hidden_global, pointer_to_hidden_global_end);
  check_matching_tags(pointer_to_hidden_global, pointer_past_hidden_global_end);
  check_eq(pointer_to_hidden_global_end, pointer_to_hidden_global + 12);
  check_eq(pointer_past_hidden_global_end, pointer_to_hidden_global + 16);
}

void crash() {
  *pointer_past_global_end = 0;
}

int main(int argc, char** argv) {
  bool check_pointers_are_tagged = false;
  // For an MTE-capable device, provide argv[1] == '1' to enable the assertions
  // that pointers should be tagged.
  if (argc >= 2 && argv[1][0] == '1') {
    check_pointers_are_tagged = true;
  }

  char* heap_ptr = static_cast<char*>(malloc(1));
  print_variable_address("heap address", heap_ptr);
  *heap_ptr = 0;
  if (check_pointers_are_tagged) check_tagged(heap_ptr);
  free(heap_ptr);

  exe_print_variables();
  dso_print_variables();

  exe_check_assertions(check_pointers_are_tagged);
  dso_check_assertions(check_pointers_are_tagged);

  printf("Assertions were passed. Now doing a global-buffer-overflow.\n");
  fflush(stdout);
  crash();
  printf("global-buffer-overflow went uncaught.\n");
  return 0;
}
