/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <platform/bionic/android_unsafe_frame_pointer_chase.h>
#include <platform/bionic/malloc.h>
#include <private/bionic_arc4random.h>
#include <private/bionic_globals.h>
#include <private/bionic_malloc_dispatch.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "bionic/gwp_asan_wrappers.h"
#include "gwp_asan/guarded_pool_allocator.h"
#include "gwp_asan/options.h"
#include "gwp_asan/random.h"
#include "malloc_common.h"

#ifndef LIBC_STATIC
#include "bionic/malloc_common_dynamic.h"
#endif  // LIBC_STATIC

static gwp_asan::GuardedPoolAllocator GuardedAlloc;
static const MallocDispatch* prev_dispatch;

using Options = gwp_asan::options::Options;

// ============================================================================
// Implementation of gFunctions.
// ============================================================================

// This function handles initialisation as asked for by MallocInitImpl. This
// should always be called in a single-threaded context.
bool gwp_asan_initialize(const MallocDispatch* dispatch, bool*, const char*) {
  prev_dispatch = dispatch;

  Options Opts;
  Opts.Enabled = true;
  Opts.MaxSimultaneousAllocations = 32;
  Opts.SampleRate = 2500;
  Opts.InstallSignalHandlers = false;
  Opts.InstallForkHandlers = true;
  Opts.Backtrace = android_unsafe_frame_pointer_chase;

  GuardedAlloc.init(Opts);
  // TODO(b/149790891): The log line below causes ART tests to fail as they're
  // not expecting any output. Disable the output for now.
  // info_log("GWP-ASan has been enabled.");

  __libc_shared_globals()->gwp_asan_state = GuardedAlloc.getAllocatorState();
  __libc_shared_globals()->gwp_asan_metadata = GuardedAlloc.getMetadataRegion();
  return true;
}

void gwp_asan_finalize() {
}

void gwp_asan_get_malloc_leak_info(uint8_t**, size_t*, size_t*, size_t*, size_t*) {
}

void gwp_asan_free_malloc_leak_info(uint8_t*) {
}

ssize_t gwp_asan_malloc_backtrace(void*, uintptr_t*, size_t) {
  // TODO(mitchp): GWP-ASan might be able to return the backtrace for the
  // provided address.
  return -1;
}

bool gwp_asan_write_malloc_leak_info(FILE*) {
  return false;
}

void* gwp_asan_gfunctions[] = {
  (void*)gwp_asan_initialize,           (void*)gwp_asan_finalize,
  (void*)gwp_asan_get_malloc_leak_info, (void*)gwp_asan_free_malloc_leak_info,
  (void*)gwp_asan_malloc_backtrace,     (void*)gwp_asan_write_malloc_leak_info,
};

// ============================================================================
// Implementation of GWP-ASan malloc wrappers.
// ============================================================================

void* gwp_asan_calloc(size_t n_elements, size_t elem_size) {
  if (__predict_false(GuardedAlloc.shouldSample())) {
    size_t bytes;
    if (!__builtin_mul_overflow(n_elements, elem_size, &bytes)) {
      if (void* result = GuardedAlloc.allocate(bytes)) {
        return result;
      }
    }
  }
  return prev_dispatch->calloc(n_elements, elem_size);
}

void gwp_asan_free(void* mem) {
  if (__predict_false(GuardedAlloc.pointerIsMine(mem))) {
    GuardedAlloc.deallocate(mem);
    return;
  }
  prev_dispatch->free(mem);
}

void* gwp_asan_malloc(size_t bytes) {
  if (__predict_false(GuardedAlloc.shouldSample())) {
    if (void* result = GuardedAlloc.allocate(bytes)) {
      return result;
    }
  }
  return prev_dispatch->malloc(bytes);
}

size_t gwp_asan_malloc_usable_size(const void* mem) {
  if (__predict_false(GuardedAlloc.pointerIsMine(mem))) {
    return GuardedAlloc.getSize(mem);
  }
  return prev_dispatch->malloc_usable_size(mem);
}

void* gwp_asan_realloc(void* old_mem, size_t bytes) {
  if (__predict_false(GuardedAlloc.pointerIsMine(old_mem))) {
    size_t old_size = GuardedAlloc.getSize(old_mem);
    void* new_ptr = gwp_asan_malloc(bytes);
    if (new_ptr) memcpy(new_ptr, old_mem, (bytes < old_size) ? bytes : old_size);
    GuardedAlloc.deallocate(old_mem);
    return new_ptr;
  }
  return prev_dispatch->realloc(old_mem, bytes);
}

int gwp_asan_malloc_iterate(uintptr_t base, size_t size,
                            void (*callback)(uintptr_t base, size_t size, void* arg), void* arg) {
  if (__predict_false(GuardedAlloc.pointerIsMine(reinterpret_cast<void*>(base)))) {
    // TODO(mitchp): GPA::iterate() returns void, but should return int.
    // TODO(mitchp): GPA::iterate() should take uintptr_t, not void*.
    GuardedAlloc.iterate(reinterpret_cast<void*>(base), size, callback, arg);
    return 0;
  }
  return prev_dispatch->malloc_iterate(base, size, callback, arg);
}

void gwp_asan_malloc_disable() {
  GuardedAlloc.disable();
  prev_dispatch->malloc_disable();
}

void gwp_asan_malloc_enable() {
  GuardedAlloc.enable();
  prev_dispatch->malloc_enable();
}

static const MallocDispatch gwp_asan_dispatch __attribute__((unused)) = {
  gwp_asan_calloc,
  gwp_asan_free,
  Malloc(mallinfo),
  gwp_asan_malloc,
  gwp_asan_malloc_usable_size,
  Malloc(memalign),
  Malloc(posix_memalign),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  Malloc(pvalloc),
#endif
  gwp_asan_realloc,
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  Malloc(valloc),
#endif
  gwp_asan_malloc_iterate,
  gwp_asan_malloc_disable,
  gwp_asan_malloc_enable,
  Malloc(mallopt),
  Malloc(aligned_alloc),
  Malloc(malloc_info),
};

// The probability (1 / kProcessSampleRate) that a process will be ranodmly
// selected for sampling. kProcessSampleRate should always be a power of two to
// avoid modulo bias.
static constexpr uint8_t kProcessSampleRate = 128;

bool ShouldGwpAsanSampleProcess() {
  uint8_t random_number;
  __libc_safe_arc4random_buf(&random_number, sizeof(random_number));
  return random_number % kProcessSampleRate == 0;
}

bool MaybeInitGwpAsanFromLibc(libc_globals* globals) {
  // Never initialize the Zygote here. A Zygote chosen for sampling would also
  // have all of its children sampled. Instead, the Zygote child will choose
  // whether it samples or not just after the Zygote forks. For
  // libc_scudo-preloaded executables (like mediaswcodec), the program name
  // might not be available yet. The zygote never uses dynamic libc_scudo.
  const char* progname = getprogname();
  if (progname && strncmp(progname, "app_process", 11) == 0) {
    return false;
  }
  return MaybeInitGwpAsan(globals);
}

static bool GwpAsanInitialized = false;

// Maybe initializes GWP-ASan. Called by android_mallopt() and libc's
// initialisation. This should always be called in a single-threaded context.
bool MaybeInitGwpAsan(libc_globals* globals, bool force_init) {
  if (GwpAsanInitialized) {
    error_log("GWP-ASan was already initialized for this process.");
    return false;
  }

  // If the caller hasn't forced GWP-ASan on, check whether we should sample
  // this process.
  if (!force_init && !ShouldGwpAsanSampleProcess()) {
    return false;
  }

  // GWP-ASan is compatible with heapprofd/malloc_debug/malloc_hooks iff
  // GWP-ASan was installed first. If one of these other libraries was already
  // installed, we don't enable GWP-ASan. These libraries are normally enabled
  // in libc_init after GWP-ASan, but if the new process is a zygote child and
  // trying to initialize GWP-ASan through mallopt(), one of these libraries may
  // be installed. It may be possible to change this in future by modifying the
  // internal dispatch pointers of these libraries at this point in time, but
  // given that they're all debug-only, we don't really mind for now.
  if (GetDefaultDispatchTable() != nullptr) {
    // Something else is installed.
    return false;
  }

  // GWP-ASan's initialization is always called in a single-threaded context, so
  // we can initialize lock-free.
  // Set GWP-ASan as the malloc dispatch table.
  globals->malloc_dispatch_table = gwp_asan_dispatch;
  atomic_store(&globals->default_dispatch_table, &gwp_asan_dispatch);

  // If malloc_limit isn't installed, we can skip the default_dispatch_table
  // lookup.
  if (GetDispatchTable() == nullptr) {
    atomic_store(&globals->current_dispatch_table, &gwp_asan_dispatch);
  }

#ifndef LIBC_STATIC
  SetGlobalFunctions(gwp_asan_gfunctions);
#endif  // LIBC_STATIC

  GwpAsanInitialized = true;

  gwp_asan_initialize(NativeAllocatorDispatch(), nullptr, nullptr);

  return true;
}

bool DispatchIsGwpAsan(const MallocDispatch* dispatch) {
  return dispatch == &gwp_asan_dispatch;
}
