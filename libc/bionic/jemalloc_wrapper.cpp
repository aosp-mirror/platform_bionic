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

#include <malloc.h>
#include <sys/param.h>
#include <unistd.h>

#include "jemalloc.h"
#include "private/bionic_macros.h"

void* je_pvalloc(size_t bytes) {
  size_t pagesize = getpagesize();
  size_t size = __BIONIC_ALIGN(bytes, pagesize);
  if (size < bytes) {
    return NULL;
  }
  return je_memalign(pagesize, size);
}

#ifdef je_memalign
#undef je_memalign
#endif

// The man page for memalign says it fails if boundary is not a power of 2,
// but this is not true. Both glibc and dlmalloc round up to the next power
// of 2, so we'll do the same.
void* je_memalign_round_up_boundary(size_t boundary, size_t size) {
  if (boundary != 0) {
    if (!powerof2(boundary)) {
      boundary = BIONIC_ROUND_UP_POWER_OF_2(boundary);
    }
  } else {
    boundary = 1;
  }
  return je_memalign(boundary, size);
}

int je_mallopt(int param, int value) {
  // The only parameter we currently understand is M_DECAY_TIME.
  if (param == M_DECAY_TIME) {
    // Only support setting the value to 1 or 0.
    ssize_t decay_time;
    if (value) {
      decay_time = 1;
    } else {
      decay_time = 0;
    }
    // First get the total number of arenas.
    unsigned narenas;
    size_t sz = sizeof(unsigned);
    if (je_mallctl("arenas.narenas", &narenas, &sz, nullptr, 0) != 0) {
      return 0;
    }

    // Set the decay time for any arenas that will be created in the future.
    if (je_mallctl("arenas.decay_time", nullptr, nullptr, &decay_time, sizeof(decay_time)) != 0) {
      return 0;
    }

    // Change the decay on the already existing arenas.
    char buffer[100];
    for (unsigned i = 0; i < narenas; i++) {
      snprintf(buffer, sizeof(buffer), "arena.%d.decay_time", i);
      if (je_mallctl(buffer, nullptr, nullptr, &decay_time, sizeof(decay_time)) != 0) {
        break;
      }
    }
    return 1;
  } else if (param == M_PURGE) {
    unsigned narenas;
    size_t sz = sizeof(unsigned);
    if (je_mallctl("arenas.narenas", &narenas, &sz, nullptr, 0) != 0) {
      return 0;
    }
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "arena.%u.purge", narenas);
    if (je_mallctl(buffer, nullptr, nullptr, nullptr, 0) != 0) {
      return 0;
    }
    return 1;
  }
  return 0;
}
