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

#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>

extern const uint32_t private_taxicab_number;

extern "C" {
uint32_t dlsym_local_symbol_get_taxicab_number();
uint32_t dlsym_local_symbol_get_taxicab_number_using_dlsym();
}

uint32_t dlsym_local_symbol_get_taxicab_number() {
  return private_taxicab_number;
}

// Let's make sure that dlsym works correctly for local symbol
uint32_t dlsym_local_symbol_get_taxicab_number_using_dlsym() {
  dlerror();
  uint32_t* ptr = reinterpret_cast<uint32_t*>(dlsym(RTLD_DEFAULT, "private_taxicab_number"));
  if (ptr == nullptr) {
    const char* dlerr = dlerror();
    if (dlerr != nullptr) {
      fprintf(stderr, "dlsym error: %s\n", dlerr);
    } else {
      fprintf(stderr, "dlsym returned NULL with no dlerror.\n");
    }
    return 0;
  }

  return *ptr;
}
