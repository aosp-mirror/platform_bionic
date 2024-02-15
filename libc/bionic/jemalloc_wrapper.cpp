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

#include <errno.h>
#include <malloc.h>
#include <sys/param.h>
#include <unistd.h>

#include <async_safe/log.h>
#include <private/MallocXmlElem.h>

#include "jemalloc.h"

__BEGIN_DECLS

size_t je_mallinfo_narenas();
size_t je_mallinfo_nbins();
struct mallinfo je_mallinfo_arena_info(size_t);
struct mallinfo je_mallinfo_bin_info(size_t, size_t);

__END_DECLS

void* je_pvalloc(size_t bytes) {
  size_t pagesize = getpagesize();
  size_t size = __BIONIC_ALIGN(bytes, pagesize);
  if (size < bytes) {
    return nullptr;
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

#ifdef je_aligned_alloc
#undef je_aligned_alloc
#endif

// The aligned_alloc function requires that size is a multiple of alignment.
// jemalloc doesn't enforce this, so add enforcement here.
void* je_aligned_alloc_wrapper(size_t alignment, size_t size) {
  if ((size % alignment) != 0) {
    errno = EINVAL;
    return nullptr;
  }
  return je_aligned_alloc(alignment, size);
}

int je_mallopt(int param, int value) {
  // The only parameter we currently understand is M_DECAY_TIME.
  if (param == M_DECAY_TIME) {
    // Only support setting the value to 1 or 0.
    ssize_t decay_time_ms;
    if (value) {
      decay_time_ms = 1000;
    } else {
      decay_time_ms = 0;
    }
    // First get the total number of arenas.
    unsigned narenas;
    size_t sz = sizeof(unsigned);
    if (je_mallctl("arenas.narenas", &narenas, &sz, nullptr, 0) != 0) {
      return 0;
    }

    // Set the decay time for any arenas that will be created in the future.
    if (je_mallctl("arenas.dirty_decay_ms", nullptr, nullptr, &decay_time_ms, sizeof(decay_time_ms)) != 0) {
      return 0;
    }
    if (je_mallctl("arenas.muzzy_decay_ms", nullptr, nullptr, &decay_time_ms, sizeof(decay_time_ms)) != 0) {
      return 0;
    }

    // Change the decay on the already existing arenas.
    char buffer[100];
    for (unsigned i = 0; i < narenas; i++) {
      snprintf(buffer, sizeof(buffer), "arena.%d.dirty_decay_ms", i);
      if (je_mallctl(buffer, nullptr, nullptr, &decay_time_ms, sizeof(decay_time_ms)) != 0) {
        break;
      }
      snprintf(buffer, sizeof(buffer), "arena.%d.muzzy_decay_ms", i);
      if (je_mallctl(buffer, nullptr, nullptr, &decay_time_ms, sizeof(decay_time_ms)) != 0) {
        break;
      }
    }
    return 1;
  } else if (param == M_PURGE || param == M_PURGE_ALL) {
    // Only clear the current thread cache since there is no easy way to
    // clear the caches of other threads.
    // This must be done first so that cleared allocations get purged
    // in the next calls.
    // Ignore the return call since this will fail if the tcache is disabled.
    je_mallctl("thread.tcache.flush", nullptr, nullptr, nullptr, 0);

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
  } else if (param == M_LOG_STATS) {
    for (size_t i = 0; i < je_mallinfo_narenas(); i++) {
      struct mallinfo mi = je_mallinfo_arena_info(i);
      if (mi.hblkhd != 0) {
        async_safe_format_log(ANDROID_LOG_INFO, "jemalloc",
                              "Arena %zu: large bytes %zu huge bytes %zu bin bytes %zu", i,
                              mi.ordblks, mi.uordblks, mi.fsmblks);

        for (size_t j = 0; j < je_mallinfo_nbins(); j++) {
          struct mallinfo mi = je_mallinfo_bin_info(i, j);
          if (mi.ordblks != 0) {
            size_t total_allocs = 1;
            if (mi.uordblks > mi.fordblks) {
              total_allocs = mi.uordblks - mi.fordblks;
            }
            size_t bin_size = mi.ordblks / total_allocs;
            async_safe_format_log(
                ANDROID_LOG_INFO, "jemalloc",
                "  Bin %zu (%zu bytes): allocated bytes %zu nmalloc %zu ndalloc %zu", j, bin_size,
                mi.ordblks, mi.uordblks, mi.fordblks);
          }
        }
      }
    }
    return 1;
  }

  return 0;
}

int je_malloc_info(int options, FILE* fp) {
  if (options != 0) {
    errno = EINVAL;
    return -1;
  }

  fflush(fp);
  int fd = fileno(fp);
  MallocXmlElem root(fd, "malloc", "version=\"jemalloc-1\"");

  // Dump all of the large allocations in the arenas.
  for (size_t i = 0; i < je_mallinfo_narenas(); i++) {
    struct mallinfo mi = je_mallinfo_arena_info(i);
    if (mi.hblkhd != 0) {
      MallocXmlElem arena_elem(fd, "heap", "nr=\"%d\"", i);
      {
        MallocXmlElem(fd, "allocated-large").Contents("%zu", mi.ordblks);
        MallocXmlElem(fd, "allocated-huge").Contents("%zu", mi.uordblks);
        MallocXmlElem(fd, "allocated-bins").Contents("%zu", mi.fsmblks);

        size_t total = 0;
        for (size_t j = 0; j < je_mallinfo_nbins(); j++) {
          struct mallinfo mi = je_mallinfo_bin_info(i, j);
          if (mi.ordblks != 0) {
            MallocXmlElem bin_elem(fd, "bin", "nr=\"%d\"", j);
            MallocXmlElem(fd, "allocated").Contents("%zu", mi.ordblks);
            MallocXmlElem(fd, "nmalloc").Contents("%zu", mi.uordblks);
            MallocXmlElem(fd, "ndalloc").Contents("%zu", mi.fordblks);
            total += mi.ordblks;
          }
        }
        MallocXmlElem(fd, "bins-total").Contents("%zu", total);
      }
    }
  }

  return 0;
}
