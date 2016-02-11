/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <errno.h>
#include <inttypes.h>
#include <malloc.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/param.h>
#include <unistd.h>

#include <vector>

#include <private/bionic_malloc_dispatch.h>

#include "backtrace.h"
#include "Config.h"
#include "DebugData.h"
#include "debug_disable.h"
#include "debug_log.h"
#include "malloc_debug.h"

// ------------------------------------------------------------------------
// Global Data
// ------------------------------------------------------------------------
DebugData* g_debug;

int* g_malloc_zygote_child;

const MallocDispatch* g_dispatch;
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Use C style prototypes for all exported functions. This makes it easy
// to do dlsym lookups during libc initialization when malloc debug
// is enabled.
// ------------------------------------------------------------------------
__BEGIN_DECLS

bool debug_initialize(const MallocDispatch* malloc_dispatch, int* malloc_zygote_child);
void debug_finalize();
void debug_get_malloc_leak_info(
    uint8_t** info, size_t* overall_size, size_t* info_size, size_t* total_memory,
    size_t* backtrace_size);
void debug_free_malloc_leak_info(uint8_t* info);
size_t debug_malloc_usable_size(void* pointer);
void* debug_malloc(size_t size);
void debug_free(void* pointer);
void* debug_memalign(size_t alignment, size_t bytes);
void* debug_realloc(void* pointer, size_t bytes);
void* debug_calloc(size_t nmemb, size_t bytes);
struct mallinfo debug_mallinfo();
int debug_posix_memalign(void** memptr, size_t alignment, size_t size);

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* debug_pvalloc(size_t bytes);
void* debug_valloc(size_t size);
#endif

__END_DECLS
// ------------------------------------------------------------------------

static void LogTagError(const Header* header, const void* pointer, const char* name) {
  ScopedDisableDebugCalls disable;

  error_log(LOG_DIVIDER);
  if (header->tag == DEBUG_FREE_TAG) {
    error_log("+++ ALLOCATION %p USED AFTER FREE (%s)", pointer, name);
    if (g_debug->config().options & FREE_TRACK) {
      g_debug->free_track->LogBacktrace(header);
    }
  } else {
    error_log("+++ ALLOCATION %p HAS INVALID TAG %" PRIx32 " (%s)", pointer, header->tag, name);
  }
  error_log("Backtrace at time of failure:");
  std::vector<uintptr_t> frames(64);
  size_t frame_num = backtrace_get(frames.data(), frames.size());
  frames.resize(frame_num);
  backtrace_log(frames.data(), frames.size());
  error_log(LOG_DIVIDER);
}

static void* InitHeader(Header* header, void* orig_pointer, size_t size) {
  header->tag = DEBUG_TAG;
  header->orig_pointer = orig_pointer;
  header->size = size;
  if (*g_malloc_zygote_child) {
    header->set_zygote();
  }
  header->usable_size = g_dispatch->malloc_usable_size(orig_pointer);
  if (header->usable_size == 0) {
    g_dispatch->free(orig_pointer);
    return nullptr;
  }
  header->usable_size -= g_debug->pointer_offset() +
      reinterpret_cast<uintptr_t>(orig_pointer) - reinterpret_cast<uintptr_t>(header);

  if (g_debug->config().options & FRONT_GUARD) {
    uint8_t* guard = g_debug->GetFrontGuard(header);
    memset(guard, g_debug->config().front_guard_value, g_debug->config().front_guard_bytes);
  }

  if (g_debug->config().options & REAR_GUARD) {
    uint8_t* guard = g_debug->GetRearGuard(header);
    memset(guard, g_debug->config().rear_guard_value, g_debug->config().rear_guard_bytes);
    // If the rear guard is enabled, set the usable size to the exact size
    // of the allocation.
    header->usable_size = header->real_size();
  }

  bool backtrace_found = false;
  if (g_debug->config().options & BACKTRACE) {
    BacktraceHeader* back_header = g_debug->GetAllocBacktrace(header);
    if (g_debug->backtrace->enabled()) {
      back_header->num_frames = backtrace_get(
          &back_header->frames[0], g_debug->config().backtrace_frames);
      backtrace_found = back_header->num_frames > 0;
    } else {
      back_header->num_frames = 0;
    }
  }

  if (g_debug->config().options & TRACK_ALLOCS) {
    g_debug->track->Add(header, backtrace_found);
  }

  return g_debug->GetPointer(header);
}

bool debug_initialize(const MallocDispatch* malloc_dispatch, int* malloc_zygote_child) {
  if (malloc_zygote_child == nullptr) {
    return false;
  }
  g_malloc_zygote_child = malloc_zygote_child;

  g_dispatch = malloc_dispatch;

  if (!DebugDisableInitialize()) {
    return false;
  }

  DebugData* debug = new DebugData();
  if (!debug->Initialize()) {
    delete debug;
    DebugDisableFinalize();
    return false;
  }
  g_debug = debug;

  // Always enable the backtrace code since we will use it in a number
  // of different error cases.
  backtrace_startup();

  return true;
}

void debug_finalize() {
  if (g_debug == nullptr) {
    return;
  }

  if (g_debug->config().options & FREE_TRACK) {
    g_debug->free_track->VerifyAll(*g_debug);
  }

  if (g_debug->config().options & LEAK_TRACK) {
    g_debug->track->DisplayLeaks(*g_debug);
  }

  backtrace_shutdown();

  DebugDisableSet(true);

  delete g_debug;
  g_debug = nullptr;

  DebugDisableFinalize();
}

void debug_get_malloc_leak_info(uint8_t** info, size_t* overall_size,
    size_t* info_size, size_t* total_memory, size_t* backtrace_size) {
  ScopedDisableDebugCalls disable;

  // Verify the arguments.
  if (info == nullptr || overall_size == nullptr || info_size == NULL ||
      total_memory == nullptr || backtrace_size == nullptr) {
    error_log("get_malloc_leak_info: At least one invalid parameter.");
    return;
  }

  *info = nullptr;
  *overall_size = 0;
  *info_size = 0;
  *total_memory = 0;
  *backtrace_size = 0;

  if (!(g_debug->config().options & BACKTRACE)) {
    error_log("get_malloc_leak_info: Allocations not being tracked, to enable "
              "set the option 'backtrace'.");
    return;
  }

  g_debug->track->GetInfo(*g_debug, info, overall_size, info_size, total_memory, backtrace_size);
}

void debug_free_malloc_leak_info(uint8_t* info) {
  g_dispatch->free(info);
}

size_t debug_malloc_usable_size(void* pointer) {
  if (DebugCallsDisabled() || !g_debug->need_header() || pointer == nullptr) {
    return g_dispatch->malloc_usable_size(pointer);
  }

  Header* header = g_debug->GetHeader(pointer);
  if (header->tag != DEBUG_TAG) {
    LogTagError(header, pointer, "malloc_usable_size");
    return 0;
  }

  return header->usable_size;
}

void* debug_malloc(size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->malloc(size);
  }

  size_t real_size = size + g_debug->extra_bytes();
  if (real_size < size) {
    // Overflow.
    errno = ENOMEM;
    return nullptr;
  }

  void* pointer;
  if (g_debug->need_header()) {
    if (size > Header::max_size()) {
      errno = ENOMEM;
      return nullptr;
    }

    Header* header = reinterpret_cast<Header*>(
        g_dispatch->memalign(MINIMUM_ALIGNMENT_BYTES, real_size));
    if (header == nullptr) {
      return nullptr;
    }
    pointer = InitHeader(header, header, size);
  } else {
    pointer = g_dispatch->malloc(real_size);
  }

  if (pointer != nullptr && g_debug->config().options & FILL_ON_ALLOC) {
    size_t bytes = debug_malloc_usable_size(pointer);
    size_t fill_bytes = g_debug->config().fill_on_alloc_bytes;
    bytes = (bytes < fill_bytes) ? bytes : fill_bytes;
    memset(pointer, g_debug->config().fill_alloc_value, bytes);
  }
  return pointer;
}

void debug_free(void* pointer) {
  if (DebugCallsDisabled() || pointer == nullptr) {
    return g_dispatch->free(pointer);
  }

  void* free_pointer = pointer;
  size_t bytes;
  if (g_debug->need_header()) {
    Header* header = g_debug->GetHeader(pointer);
    if (header->tag != DEBUG_TAG) {
      LogTagError(header, pointer, "free");
      return;
    }
    free_pointer = header->orig_pointer;

    if (g_debug->config().options & FRONT_GUARD) {
      if (!g_debug->front_guard->Valid(*g_debug, header)) {
        g_debug->front_guard->LogFailure(*g_debug, header);
      }
    }
    if (g_debug->config().options & REAR_GUARD) {
      if (!g_debug->rear_guard->Valid(*g_debug, header)) {
        g_debug->rear_guard->LogFailure(*g_debug, header);
      }
    }

    if (g_debug->config().options & TRACK_ALLOCS) {
      bool backtrace_found = false;
      if (g_debug->config().options & BACKTRACE) {
        BacktraceHeader* back_header = g_debug->GetAllocBacktrace(header);
        backtrace_found = back_header->num_frames > 0;
      }
      g_debug->track->Remove(header, backtrace_found);
    }

    if (g_debug->config().options & FREE_TRACK) {
      g_debug->free_track->Add(*g_debug, header);

      // Do not free this pointer just yet.
      free_pointer = nullptr;
    }
    header->tag = DEBUG_FREE_TAG;

    bytes = header->usable_size;
  } else {
    bytes = g_dispatch->malloc_usable_size(pointer);
  }

  if (g_debug->config().options & FILL_ON_FREE) {
    size_t fill_bytes = g_debug->config().fill_on_free_bytes;
    bytes = (bytes < fill_bytes) ? bytes : fill_bytes;
    memset(pointer, g_debug->config().fill_free_value, bytes);
  }

  g_dispatch->free(free_pointer);
}

void* debug_memalign(size_t alignment, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->memalign(alignment, bytes);
  }

  void* pointer;
  if (g_debug->need_header()) {
    if (bytes > Header::max_size()) {
      errno = ENOMEM;
      return nullptr;
    }

    // Make the alignment a power of two.
    if (!powerof2(alignment)) {
      alignment = BIONIC_ROUND_UP_POWER_OF_2(alignment);
    }
    // Force the alignment to at least MINIMUM_ALIGNMENT_BYTES to guarantee
    // that the header is aligned properly.
    if (alignment < MINIMUM_ALIGNMENT_BYTES) {
      alignment = MINIMUM_ALIGNMENT_BYTES;
    }

    // We don't have any idea what the natural alignment of
    // the underlying native allocator is, so we always need to
    // over allocate.
    size_t real_size = alignment + bytes + g_debug->extra_bytes();
    if (real_size < bytes) {
      // Overflow.
      errno = ENOMEM;
      return nullptr;
    }

    pointer = g_dispatch->malloc(real_size);
    if (pointer == nullptr) {
      return nullptr;
    }

    uintptr_t value = reinterpret_cast<uintptr_t>(pointer) + g_debug->pointer_offset();
    // Now align the pointer.
    value += (-value % alignment);

    Header* header = g_debug->GetHeader(reinterpret_cast<void*>(value));
    pointer = InitHeader(header, pointer, bytes);
  } else {
    size_t real_size = bytes + g_debug->extra_bytes();
    if (real_size < bytes) {
      // Overflow.
      errno = ENOMEM;
      return nullptr;
    }
    pointer = g_dispatch->memalign(alignment, real_size);
  }

  if (pointer != nullptr && g_debug->config().options & FILL_ON_ALLOC) {
    size_t bytes = debug_malloc_usable_size(pointer);
    size_t fill_bytes = g_debug->config().fill_on_alloc_bytes;
    bytes = (bytes < fill_bytes) ? bytes : fill_bytes;
    memset(pointer, g_debug->config().fill_alloc_value, bytes);
  }
  return pointer;
}

void* debug_realloc(void* pointer, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->realloc(pointer, bytes);
  }

  if (pointer == nullptr) {
    return debug_malloc(bytes);
  }

  if (bytes == 0) {
    debug_free(pointer);
    return nullptr;
  }

  size_t real_size = bytes;
  if (g_debug->config().options & EXPAND_ALLOC) {
    real_size += g_debug->config().expand_alloc_bytes;
    if (real_size < bytes) {
      // Overflow.
      errno = ENOMEM;
      return nullptr;
    }
  }

  void* new_pointer;
  size_t prev_size;
  if (g_debug->need_header()) {
    if (bytes > Header::max_size()) {
      errno = ENOMEM;
      return nullptr;
    }

    Header* header = g_debug->GetHeader(pointer);
    if (header->tag != DEBUG_TAG) {
      LogTagError(header, pointer, "realloc");
      return nullptr;
    }

    // Same size, do nothing.
    if (real_size == header->real_size()) {
      return pointer;
    }

    // Allocation is shrinking.
    if (real_size < header->usable_size) {
      header->size = real_size;
      if (*g_malloc_zygote_child) {
        header->set_zygote();
      }
      if (g_debug->config().options & REAR_GUARD) {
        // Don't bother allocating a smaller pointer in this case, simply
        // change the header usable_size and reset the rear guard.
        header->usable_size = header->real_size();
        memset(g_debug->GetRearGuard(header), g_debug->config().rear_guard_value,
               g_debug->config().rear_guard_bytes);
      }
      return pointer;
    }

    // Allocate the new size.
    new_pointer = debug_malloc(bytes);
    if (new_pointer == nullptr) {
      errno = ENOMEM;
      return nullptr;
    }

    prev_size = header->usable_size;
    memcpy(new_pointer, pointer, prev_size);
    debug_free(pointer);
  } else {
    prev_size = g_dispatch->malloc_usable_size(pointer);
    new_pointer = g_dispatch->realloc(pointer, real_size);
    if (new_pointer == nullptr) {
      return nullptr;
    }
  }

  if (g_debug->config().options & FILL_ON_ALLOC) {
    size_t bytes = debug_malloc_usable_size(new_pointer);
    if (bytes > g_debug->config().fill_on_alloc_bytes) {
      bytes = g_debug->config().fill_on_alloc_bytes;
    }
    if (bytes > prev_size) {
      memset(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(new_pointer) + prev_size),
             g_debug->config().fill_alloc_value, bytes - prev_size);
    }
  }

  return new_pointer;
}

void* debug_calloc(size_t nmemb, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->calloc(nmemb, bytes);
  }

  size_t real_size = nmemb * bytes + g_debug->extra_bytes();
  if (real_size < bytes || real_size < nmemb) {
    // Overflow.
    errno = ENOMEM;
    return nullptr;
  }

  if (g_debug->need_header()) {
    // The above check will guarantee the multiply will not overflow.
    if (bytes * nmemb > Header::max_size()) {
      errno = ENOMEM;
      return nullptr;
    }

    // Need to guarantee the alignment of the header.
    Header* header = reinterpret_cast<Header*>(
        g_dispatch->memalign(MINIMUM_ALIGNMENT_BYTES, real_size));
    if (header == nullptr) {
      return nullptr;
    }
    memset(header, 0, g_dispatch->malloc_usable_size(header));
    return InitHeader(header, header, nmemb * bytes);
  } else {
    return g_dispatch->calloc(1, real_size);
  }
}

struct mallinfo debug_mallinfo() {
  return g_dispatch->mallinfo();
}

int debug_posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->posix_memalign(memptr, alignment, size);
  }

  if (!powerof2(alignment)) {
    return EINVAL;
  }
  int saved_errno = errno;
  *memptr = debug_memalign(alignment, size);
  errno = saved_errno;
  return (*memptr != nullptr) ? 0 : ENOMEM;
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* debug_pvalloc(size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->pvalloc(bytes);
  }

  size_t pagesize = getpagesize();
  size_t size = BIONIC_ALIGN(bytes, pagesize);
  if (size < bytes) {
    // Overflow
    errno = ENOMEM;
    return nullptr;
  }
  return debug_memalign(pagesize, size);
}

void* debug_valloc(size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->valloc(size);
  }
  return debug_memalign(getpagesize(), size);
}
#endif
