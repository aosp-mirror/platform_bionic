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

#include <alloca.h>
#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "gwp_asan/guarded_pool_allocator.h"
#include "gwp_asan/options.h"
#include "gwp_asan_wrappers.h"
#include "malloc_common.h"
#include "platform/bionic/android_unsafe_frame_pointer_chase.h"
#include "platform/bionic/macros.h"
#include "platform/bionic/malloc.h"
#include "private/bionic_arc4random.h"
#include "private/bionic_globals.h"
#include "private/bionic_malloc_dispatch.h"
#include "sys/system_properties.h"
#include "sysprop_helpers.h"

#ifndef LIBC_STATIC
#include "bionic/malloc_common_dynamic.h"
#endif  // LIBC_STATIC

static gwp_asan::GuardedPoolAllocator GuardedAlloc;
static const MallocDispatch* prev_dispatch;

using Action = android_mallopt_gwp_asan_options_t::Action;
using Options = gwp_asan::options::Options;

// basename() is a mess, see the manpage. Let's be explicit what handling we
// want (don't touch my string!).
extern "C" const char* __gnu_basename(const char* path);

namespace {

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
  // GPA::pointerIsMine(p) always returns false where `p == nullptr` (and thus
  // malloc(bytes) is requested). We always fall back to the backing allocator,
  // technically missing some coverage, but reducing an extra conditional
  // branch.
  if (__predict_false(GuardedAlloc.pointerIsMine(old_mem))) {
    if (__predict_false(bytes == 0)) {
      GuardedAlloc.deallocate(old_mem);
      return nullptr;
    }
    void* new_ptr = gwp_asan_malloc(bytes);
    // If malloc() fails, then don't destroy the old memory.
    if (__predict_false(new_ptr == nullptr)) return nullptr;

    size_t old_size = GuardedAlloc.getSize(old_mem);
    memcpy(new_ptr, old_mem, (bytes < old_size) ? bytes : old_size);
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

const MallocDispatch gwp_asan_dispatch __attribute__((unused)) = {
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

bool isPowerOfTwo(uint64_t x) {
  assert(x != 0);
  return (x & (x - 1)) == 0;
}

bool ShouldGwpAsanSampleProcess(unsigned sample_rate) {
  if (!isPowerOfTwo(sample_rate)) {
    warning_log(
        "GWP-ASan process sampling rate of %u is not a power-of-two, and so modulo bias occurs.",
        sample_rate);
  }

  uint8_t random_number;
  __libc_safe_arc4random_buf(&random_number, sizeof(random_number));
  return random_number % sample_rate == 0;
}

bool GwpAsanInitialized = false;

// The probability (1 / SampleRate) that an allocation gets chosen to be put
// into the special GWP-ASan pool.
using SampleRate_t = typeof(gwp_asan::options::Options::SampleRate);
constexpr SampleRate_t kDefaultSampleRate = 2500;
static const char* kSampleRateSystemSysprop = "libc.debug.gwp_asan.sample_rate.system_default";
static const char* kSampleRateAppSysprop = "libc.debug.gwp_asan.sample_rate.app_default";
static const char* kSampleRateTargetedSyspropPrefix = "libc.debug.gwp_asan.sample_rate.";
static const char* kSampleRateEnvVar = "GWP_ASAN_SAMPLE_RATE";

// The probability (1 / ProcessSampling) that a process will be randomly
// selected for sampling, for system apps and system processes. The process
// sampling rate should always be a power of two to avoid modulo bias.
constexpr unsigned kDefaultProcessSampling = 128;
static const char* kProcessSamplingSystemSysprop =
    "libc.debug.gwp_asan.process_sampling.system_default";
static const char* kProcessSamplingAppSysprop = "libc.debug.gwp_asan.process_sampling.app_default";
static const char* kProcessSamplingTargetedSyspropPrefix = "libc.debug.gwp_asan.process_sampling.";
static const char* kProcessSamplingEnvVar = "GWP_ASAN_PROCESS_SAMPLING";

// The upper limit of simultaneous allocations supported by GWP-ASan. Any
// allocations in excess of this limit will be passed to the backing allocator
// and can't be sampled. This value, if unspecified, will be automatically
// calculated to keep the same ratio as the default (2500 sampling : 32 allocs).
// So, if you specify GWP_ASAN_SAMPLE_RATE=1250 (i.e. twice as frequent), we'll
// automatically calculate that we need double the slots (64).
using SimultaneousAllocations_t = typeof(gwp_asan::options::Options::MaxSimultaneousAllocations);
constexpr SimultaneousAllocations_t kDefaultMaxAllocs = 32;
static const char* kMaxAllocsSystemSysprop = "libc.debug.gwp_asan.max_allocs.system_default";
static const char* kMaxAllocsAppSysprop = "libc.debug.gwp_asan.max_allocs.app_default";
static const char* kMaxAllocsTargetedSyspropPrefix = "libc.debug.gwp_asan.max_allocs.";
static const char* kMaxAllocsEnvVar = "GWP_ASAN_MAX_ALLOCS";

static const char kPersistPrefix[] = "persist.";

void SetDefaultGwpAsanOptions(Options* options, unsigned* process_sample_rate,
                              const android_mallopt_gwp_asan_options_t& mallopt_options) {
  options->Enabled = true;
  options->InstallSignalHandlers = false;
  options->InstallForkHandlers = true;
  options->Backtrace = android_unsafe_frame_pointer_chase;
  options->SampleRate = kDefaultSampleRate;
  options->MaxSimultaneousAllocations = kDefaultMaxAllocs;

  *process_sample_rate = 1;
  if (mallopt_options.desire == Action::TURN_ON_WITH_SAMPLING) {
    *process_sample_rate = kDefaultProcessSampling;
  }
}

bool GetGwpAsanOption(unsigned long long* result,
                      const android_mallopt_gwp_asan_options_t& mallopt_options,
                      const char* system_sysprop, const char* app_sysprop,
                      const char* targeted_sysprop_prefix, const char* env_var,
                      const char* descriptive_name) {
  const char* basename = "";
  if (mallopt_options.program_name) basename = __gnu_basename(mallopt_options.program_name);

  constexpr size_t kSyspropMaxLen = 512;
  char program_specific_sysprop[kSyspropMaxLen] = {};
  char persist_program_specific_sysprop[kSyspropMaxLen] = {};
  char persist_default_sysprop[kSyspropMaxLen] = {};
  const char* sysprop_names[4] = {};
  // Tests use a blank program name to specify that system properties should not
  // be used. Tests still continue to use the environment variable though.
  if (*basename != '\0') {
    const char* default_sysprop = system_sysprop;
    if (mallopt_options.desire == Action::TURN_ON_FOR_APP) {
      default_sysprop = app_sysprop;
    }
    async_safe_format_buffer(&program_specific_sysprop[0], kSyspropMaxLen, "%s%s",
                             targeted_sysprop_prefix, basename);
    async_safe_format_buffer(&persist_program_specific_sysprop[0], kSyspropMaxLen, "%s%s",
                             kPersistPrefix, program_specific_sysprop);
    async_safe_format_buffer(&persist_default_sysprop[0], kSyspropMaxLen, "%s%s", kPersistPrefix,
                             default_sysprop);

    // In order of precedence, always take the program-specific sysprop (e.g.
    // '[persist.]libc.debug.gwp_asan.sample_rate.cameraserver') over the
    // generic sysprop (e.g.
    // '[persist.]libc.debug.gwp_asan.(system_default|app_default)'). In
    // addition, always take the non-persistent option over the persistent
    // option.
    sysprop_names[0] = program_specific_sysprop;
    sysprop_names[1] = persist_program_specific_sysprop;
    sysprop_names[2] = default_sysprop;
    sysprop_names[3] = persist_default_sysprop;
  }

  char settings_buf[PROP_VALUE_MAX];
  if (!get_config_from_env_or_sysprops(env_var, sysprop_names, arraysize(sysprop_names),
                                       settings_buf, PROP_VALUE_MAX)) {
    return false;
  }

  char* end;
  unsigned long long value = strtoull(settings_buf, &end, 10);
  if (value == ULLONG_MAX || *end != '\0') {
    warning_log("Invalid GWP-ASan %s: \"%s\". Using default value instead.", descriptive_name,
                settings_buf);
    return false;
  }

  *result = value;
  return true;
}

// Initialize the GWP-ASan options structure in *options, taking into account whether someone has
// asked for specific GWP-ASan settings. The order of priority is:
//  1. Environment variables.
//  2. Process-specific system properties.
//  3. Global system properties.
// If any of these overrides are found, we return true. Otherwise, use the default values, and
// return false.
bool GetGwpAsanOptions(Options* options, unsigned* process_sample_rate,
                       const android_mallopt_gwp_asan_options_t& mallopt_options) {
  SetDefaultGwpAsanOptions(options, process_sample_rate, mallopt_options);

  bool had_overrides = false;

  unsigned long long buf;
  if (GetGwpAsanOption(&buf, mallopt_options, kSampleRateSystemSysprop, kSampleRateAppSysprop,
                       kSampleRateTargetedSyspropPrefix, kSampleRateEnvVar, "sample rate")) {
    options->SampleRate = buf;
    had_overrides = true;
  }

  if (GetGwpAsanOption(&buf, mallopt_options, kProcessSamplingSystemSysprop,
                       kProcessSamplingAppSysprop, kProcessSamplingTargetedSyspropPrefix,
                       kProcessSamplingEnvVar, "process sampling rate")) {
    *process_sample_rate = buf;
    had_overrides = true;
  }

  if (GetGwpAsanOption(&buf, mallopt_options, kMaxAllocsSystemSysprop, kMaxAllocsAppSysprop,
                       kMaxAllocsTargetedSyspropPrefix, kMaxAllocsEnvVar,
                       "maximum simultaneous allocations")) {
    options->MaxSimultaneousAllocations = buf;
    had_overrides = true;
  } else if (had_overrides) {
    // Multiply the number of slots available, such that the ratio between
    // sampling rate and slots is kept the same as the default. For example, a
    // sampling rate of 1000 is 2.5x more frequent than default, and so
    // requires 80 slots (32 * 2.5).
    float frequency_multiplier = static_cast<float>(options->SampleRate) / kDefaultSampleRate;
    options->MaxSimultaneousAllocations =
        /* default */ kDefaultMaxAllocs / frequency_multiplier;
  }
  return had_overrides;
}

bool MaybeInitGwpAsan(libc_globals* globals,
                      const android_mallopt_gwp_asan_options_t& mallopt_options) {
  if (GwpAsanInitialized) {
    error_log("GWP-ASan was already initialized for this process.");
    return false;
  }

  Options options;
  unsigned process_sample_rate = kDefaultProcessSampling;
  if (!GetGwpAsanOptions(&options, &process_sample_rate, mallopt_options) &&
      mallopt_options.desire == Action::DONT_TURN_ON_UNLESS_OVERRIDDEN) {
    return false;
  }

  if (options.SampleRate == 0 || process_sample_rate == 0 ||
      options.MaxSimultaneousAllocations == 0) {
    return false;
  }

  if (!ShouldGwpAsanSampleProcess(process_sample_rate)) {
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

  GwpAsanInitialized = true;

  prev_dispatch = NativeAllocatorDispatch();

  GuardedAlloc.init(options);

  __libc_shared_globals()->gwp_asan_state = GuardedAlloc.getAllocatorState();
  __libc_shared_globals()->gwp_asan_metadata = GuardedAlloc.getMetadataRegion();

  return true;
}
};  // anonymous namespace

bool MaybeInitGwpAsanFromLibc(libc_globals* globals) {
  // Never initialize the Zygote here. A Zygote chosen for sampling would also
  // have all of its children sampled. Instead, the Zygote child will choose
  // whether it samples or not just after the Zygote forks. Note that the Zygote
  // changes its name after it's started, at this point it's still called
  // "app_process" or "app_process64".
  static const char kAppProcessNamePrefix[] = "app_process";
  const char* progname = getprogname();
  if (strncmp(progname, kAppProcessNamePrefix, sizeof(kAppProcessNamePrefix) - 1) == 0)
    return false;

  android_mallopt_gwp_asan_options_t mallopt_options;
  mallopt_options.program_name = progname;
  mallopt_options.desire = Action::TURN_ON_WITH_SAMPLING;

  return MaybeInitGwpAsan(globals, mallopt_options);
}

bool DispatchIsGwpAsan(const MallocDispatch* dispatch) {
  return dispatch == &gwp_asan_dispatch;
}

bool EnableGwpAsan(const android_mallopt_gwp_asan_options_t& options) {
  if (GwpAsanInitialized) {
    return true;
  }

  bool ret_value;
  __libc_globals.mutate(
      [&](libc_globals* globals) { ret_value = MaybeInitGwpAsan(globals, options); });
  return ret_value;
}
