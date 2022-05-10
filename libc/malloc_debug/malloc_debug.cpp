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
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <mutex>
#include <vector>

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/stringprintf.h>
#include <bionic/malloc_tagged_pointers.h>
#include <platform/bionic/reserved_signals.h>
#include <private/MallocXmlElem.h>
#include <private/bionic_malloc_dispatch.h>
#include <unwindstack/Unwinder.h>

#include "Config.h"
#include "DebugData.h"
#include "backtrace.h"
#include "debug_disable.h"
#include "debug_log.h"
#include "malloc_debug.h"
#include "UnwindBacktrace.h"

// ------------------------------------------------------------------------
// Global Data
// ------------------------------------------------------------------------
DebugData* g_debug;

bool* g_zygote_child;

const MallocDispatch* g_dispatch;
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Use C style prototypes for all exported functions. This makes it easy
// to do dlsym lookups during libc initialization when malloc debug
// is enabled.
// ------------------------------------------------------------------------
__BEGIN_DECLS

bool debug_initialize(const MallocDispatch* malloc_dispatch, bool* malloc_zygote_child,
                      const char* options);
void debug_finalize();
void debug_dump_heap(const char* file_name);
void debug_get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size,
                                size_t* total_memory, size_t* backtrace_size);
bool debug_write_malloc_leak_info(FILE* fp);
ssize_t debug_malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count);
void debug_free_malloc_leak_info(uint8_t* info);
size_t debug_malloc_usable_size(void* pointer);
void* debug_malloc(size_t size);
void debug_free(void* pointer);
void* debug_aligned_alloc(size_t alignment, size_t size);
void* debug_memalign(size_t alignment, size_t bytes);
void* debug_realloc(void* pointer, size_t bytes);
void* debug_calloc(size_t nmemb, size_t bytes);
struct mallinfo debug_mallinfo();
int debug_mallopt(int param, int value);
int debug_malloc_info(int options, FILE* fp);
int debug_posix_memalign(void** memptr, size_t alignment, size_t size);
int debug_malloc_iterate(uintptr_t base, size_t size,
                         void (*callback)(uintptr_t base, size_t size, void* arg), void* arg);
void debug_malloc_disable();
void debug_malloc_enable();

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* debug_pvalloc(size_t bytes);
void* debug_valloc(size_t size);
#endif

__END_DECLS
// ------------------------------------------------------------------------

class ScopedConcurrentLock {
 public:
  ScopedConcurrentLock() {
    pthread_rwlock_rdlock(&lock_);
  }
  ~ScopedConcurrentLock() {
    pthread_rwlock_unlock(&lock_);
  }

  static void Init() {
    pthread_rwlockattr_t attr;
    // Set the attribute so that when a write lock is pending, read locks are no
    // longer granted.
    pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&lock_, &attr);
  }

  static void BlockAllOperations() {
    pthread_rwlock_wrlock(&lock_);
  }

 private:
  static pthread_rwlock_t lock_;
};
pthread_rwlock_t ScopedConcurrentLock::lock_;

// Use this because the sigprocmask* functions filter out the reserved bionic
// signals including the signal this code blocks.
static inline int __rt_sigprocmask(int how, const sigset64_t* new_set, sigset64_t* old_set,
                                   size_t sigset_size) {
  return syscall(SYS_rt_sigprocmask, how, new_set, old_set, sigset_size);
}

// Need to block the backtrace signal while in malloc debug routines
// otherwise there is a chance of a deadlock and timeout when unwinding.
// This can occur if a thread is paused while owning a malloc debug
// internal lock.
class ScopedBacktraceSignalBlocker {
 public:
  ScopedBacktraceSignalBlocker() {
    sigemptyset64(&backtrace_set_);
    sigaddset64(&backtrace_set_, BIONIC_SIGNAL_BACKTRACE);
    sigset64_t old_set;
    __rt_sigprocmask(SIG_BLOCK, &backtrace_set_, &old_set, sizeof(backtrace_set_));
    if (sigismember64(&old_set, BIONIC_SIGNAL_BACKTRACE)) {
      unblock_ = false;
    }
  }

  ~ScopedBacktraceSignalBlocker() {
    if (unblock_) {
      __rt_sigprocmask(SIG_UNBLOCK, &backtrace_set_, nullptr, sizeof(backtrace_set_));
    }
  }

 private:
  bool unblock_ = true;
  sigset64_t backtrace_set_;
};

static void InitAtfork() {
  static pthread_once_t atfork_init = PTHREAD_ONCE_INIT;
  pthread_once(&atfork_init, []() {
    pthread_atfork(
        []() {
          if (g_debug != nullptr) {
            g_debug->PrepareFork();
          }
        },
        []() {
          if (g_debug != nullptr) {
            g_debug->PostForkParent();
          }
        },
        []() {
          if (g_debug != nullptr) {
            g_debug->PostForkChild();
          }
        });
  });
}

void BacktraceAndLog() {
  if (g_debug->config().options() & BACKTRACE_FULL) {
    std::vector<uintptr_t> frames;
    std::vector<unwindstack::FrameData> frames_info;
    if (!Unwind(&frames, &frames_info, 256)) {
      error_log("  Backtrace failed to get any frames.");
    } else {
      UnwindLog(frames_info);
    }
  } else {
    std::vector<uintptr_t> frames(256);
    size_t num_frames = backtrace_get(frames.data(), frames.size());
    if (num_frames == 0) {
      error_log("  Backtrace failed to get any frames.");
    } else {
      backtrace_log(frames.data(), num_frames);
    }
  }
}

static void LogError(const void* pointer, const char* error_str) {
  error_log(LOG_DIVIDER);
  error_log("+++ ALLOCATION %p %s", pointer, error_str);

  // If we are tracking already freed pointers, check to see if this is
  // one so we can print extra information.
  if (g_debug->config().options() & FREE_TRACK) {
    PointerData::LogFreeBacktrace(pointer);
  }

  error_log("Backtrace at time of failure:");
  BacktraceAndLog();
  error_log(LOG_DIVIDER);
  if (g_debug->config().options() & ABORT_ON_ERROR) {
    abort();
  }
}

static bool VerifyPointer(const void* pointer, const char* function_name) {
  if (g_debug->HeaderEnabled()) {
    Header* header = g_debug->GetHeader(pointer);
    if (header->tag != DEBUG_TAG) {
      std::string error_str;
      if (header->tag == DEBUG_FREE_TAG) {
        error_str = std::string("USED AFTER FREE (") + function_name + ")";
      } else {
        error_str = android::base::StringPrintf("HAS INVALID TAG %" PRIx32 " (%s)", header->tag,
                                                function_name);
      }
      LogError(pointer, error_str.c_str());
      return false;
    }
  }

  if (g_debug->TrackPointers()) {
    if (!PointerData::Exists(pointer)) {
      std::string error_str(std::string("UNKNOWN POINTER (") + function_name + ")");
      LogError(pointer, error_str.c_str());
      return false;
    }
  }
  return true;
}

static size_t InternalMallocUsableSize(void* pointer) {
  if (g_debug->HeaderEnabled()) {
    return g_debug->GetHeader(pointer)->usable_size;
  } else {
    return g_dispatch->malloc_usable_size(pointer);
  }
}

static void* InitHeader(Header* header, void* orig_pointer, size_t size) {
  header->tag = DEBUG_TAG;
  header->orig_pointer = orig_pointer;
  header->size = size;
  header->usable_size = g_dispatch->malloc_usable_size(orig_pointer);
  if (header->usable_size == 0) {
    g_dispatch->free(orig_pointer);
    return nullptr;
  }
  header->usable_size -= g_debug->pointer_offset() + reinterpret_cast<uintptr_t>(header) -
                         reinterpret_cast<uintptr_t>(orig_pointer);

  if (g_debug->config().options() & FRONT_GUARD) {
    uint8_t* guard = g_debug->GetFrontGuard(header);
    memset(guard, g_debug->config().front_guard_value(), g_debug->config().front_guard_bytes());
  }

  if (g_debug->config().options() & REAR_GUARD) {
    uint8_t* guard = g_debug->GetRearGuard(header);
    memset(guard, g_debug->config().rear_guard_value(), g_debug->config().rear_guard_bytes());
    // If the rear guard is enabled, set the usable size to the exact size
    // of the allocation.
    header->usable_size = header->size;
  }

  return g_debug->GetPointer(header);
}

extern "C" void __asan_init() __attribute__((weak));

bool debug_initialize(const MallocDispatch* malloc_dispatch, bool* zygote_child,
                      const char* options) {
  if (zygote_child == nullptr || options == nullptr) {
    return false;
  }

  if (__asan_init != 0) {
    error_log("malloc debug cannot be enabled alongside ASAN");
    return false;
  }

  InitAtfork();

  g_zygote_child = zygote_child;

  g_dispatch = malloc_dispatch;

  if (!DebugDisableInitialize()) {
    return false;
  }

  DebugData* debug = new DebugData();
  if (!debug->Initialize(options)) {
    delete debug;
    DebugDisableFinalize();
    return false;
  }
  g_debug = debug;

  // Always enable the backtrace code since we will use it in a number
  // of different error cases.
  backtrace_startup();

  if (g_debug->config().options() & VERBOSE) {
    info_log("%s: malloc debug enabled", getprogname());
  }

  ScopedConcurrentLock::Init();

  return true;
}

void debug_finalize() {
  if (g_debug == nullptr) {
    return;
  }

  // Make sure that there are no other threads doing debug allocations
  // before we kill everything.
  ScopedConcurrentLock::BlockAllOperations();

  // Turn off capturing allocations calls.
  DebugDisableSet(true);

  if (g_debug->config().options() & FREE_TRACK) {
    PointerData::VerifyAllFreed();
  }

  if (g_debug->config().options() & LEAK_TRACK) {
    PointerData::LogLeaks();
  }

  if ((g_debug->config().options() & BACKTRACE) && g_debug->config().backtrace_dump_on_exit()) {
    debug_dump_heap(android::base::StringPrintf("%s.%d.exit.txt",
                                                g_debug->config().backtrace_dump_prefix().c_str(),
                                                getpid()).c_str());
  }

  backtrace_shutdown();

  // In order to prevent any issues of threads freeing previous pointers
  // after the main thread calls this code, simply leak the g_debug pointer
  // and do not destroy the debug disable pthread key.
}

void debug_get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size,
                                size_t* total_memory, size_t* backtrace_size) {
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  // Verify the arguments.
  if (info == nullptr || overall_size == nullptr || info_size == nullptr || total_memory == nullptr ||
      backtrace_size == nullptr) {
    error_log("get_malloc_leak_info: At least one invalid parameter.");
    return;
  }

  *info = nullptr;
  *overall_size = 0;
  *info_size = 0;
  *total_memory = 0;
  *backtrace_size = 0;

  if (!(g_debug->config().options() & BACKTRACE)) {
    error_log(
        "get_malloc_leak_info: Allocations not being tracked, to enable "
        "set the option 'backtrace'.");
    return;
  }

  PointerData::GetInfo(info, overall_size, info_size, total_memory, backtrace_size);
}

void debug_free_malloc_leak_info(uint8_t* info) {
  g_dispatch->free(info);
}

size_t debug_malloc_usable_size(void* pointer) {
  if (DebugCallsDisabled() || pointer == nullptr) {
    return g_dispatch->malloc_usable_size(pointer);
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  if (!VerifyPointer(pointer, "malloc_usable_size")) {
    return 0;
  }

  return InternalMallocUsableSize(pointer);
}

static void* InternalMalloc(size_t size) {
  if ((g_debug->config().options() & BACKTRACE) && g_debug->pointer->ShouldDumpAndReset()) {
    debug_dump_heap(android::base::StringPrintf(
                        "%s.%d.txt", g_debug->config().backtrace_dump_prefix().c_str(), getpid())
                        .c_str());
  }

  if (size == 0) {
    size = 1;
  }

  size_t real_size = size + g_debug->extra_bytes();
  if (real_size < size) {
    // Overflow.
    errno = ENOMEM;
    return nullptr;
  }

  if (size > PointerInfoType::MaxSize()) {
    errno = ENOMEM;
    return nullptr;
  }

  void* pointer;
  if (g_debug->HeaderEnabled()) {
    Header* header =
        reinterpret_cast<Header*>(g_dispatch->memalign(MINIMUM_ALIGNMENT_BYTES, real_size));
    if (header == nullptr) {
      return nullptr;
    }
    pointer = InitHeader(header, header, size);
  } else {
    pointer = g_dispatch->malloc(real_size);
  }

  if (pointer != nullptr) {
    if (g_debug->TrackPointers()) {
      PointerData::Add(pointer, size);
    }

    if (g_debug->config().options() & FILL_ON_ALLOC) {
      size_t bytes = InternalMallocUsableSize(pointer);
      size_t fill_bytes = g_debug->config().fill_on_alloc_bytes();
      bytes = (bytes < fill_bytes) ? bytes : fill_bytes;
      memset(pointer, g_debug->config().fill_alloc_value(), bytes);
    }
  }
  return pointer;
}

void* debug_malloc(size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->malloc(size);
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  void* pointer = InternalMalloc(size);

  if (g_debug->config().options() & RECORD_ALLOCS) {
    g_debug->record->AddEntry(new MallocEntry(pointer, size));
  }

  return pointer;
}

static void InternalFree(void* pointer) {
  if ((g_debug->config().options() & BACKTRACE) && g_debug->pointer->ShouldDumpAndReset()) {
    debug_dump_heap(android::base::StringPrintf(
                        "%s.%d.txt", g_debug->config().backtrace_dump_prefix().c_str(), getpid())
                        .c_str());
  }

  void* free_pointer = pointer;
  size_t bytes;
  Header* header;
  if (g_debug->HeaderEnabled()) {
    header = g_debug->GetHeader(pointer);
    free_pointer = header->orig_pointer;

    if (g_debug->config().options() & FRONT_GUARD) {
      if (!g_debug->front_guard->Valid(header)) {
        g_debug->front_guard->LogFailure(header);
      }
    }
    if (g_debug->config().options() & REAR_GUARD) {
      if (!g_debug->rear_guard->Valid(header)) {
        g_debug->rear_guard->LogFailure(header);
      }
    }

    header->tag = DEBUG_FREE_TAG;

    bytes = header->usable_size;
  } else {
    bytes = g_dispatch->malloc_usable_size(pointer);
  }

  if (g_debug->config().options() & FILL_ON_FREE) {
    size_t fill_bytes = g_debug->config().fill_on_free_bytes();
    bytes = (bytes < fill_bytes) ? bytes : fill_bytes;
    memset(pointer, g_debug->config().fill_free_value(), bytes);
  }

  if (g_debug->TrackPointers()) {
    PointerData::Remove(pointer);
  }

  if (g_debug->config().options() & FREE_TRACK) {
    // Do not add the allocation until we are done modifying the pointer
    // itself. This avoids a race if a lot of threads are all doing
    // frees at the same time and we wind up trying to really free this
    // pointer from another thread, while still trying to free it in
    // this function.
    pointer = PointerData::AddFreed(pointer);
    if (pointer != nullptr) {
      if (g_debug->HeaderEnabled()) {
        pointer = g_debug->GetHeader(pointer)->orig_pointer;
      }
      g_dispatch->free(pointer);
    }
  } else {
    g_dispatch->free(free_pointer);
  }
}

void debug_free(void* pointer) {
  if (DebugCallsDisabled() || pointer == nullptr) {
    return g_dispatch->free(pointer);
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  if (g_debug->config().options() & RECORD_ALLOCS) {
    g_debug->record->AddEntry(new FreeEntry(pointer));
  }

  if (!VerifyPointer(pointer, "free")) {
    return;
  }

  InternalFree(pointer);
}

void* debug_memalign(size_t alignment, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->memalign(alignment, bytes);
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  if (bytes == 0) {
    bytes = 1;
  }

  if (bytes > PointerInfoType::MaxSize()) {
    errno = ENOMEM;
    return nullptr;
  }

  void* pointer;
  if (g_debug->HeaderEnabled()) {
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

  if (pointer != nullptr) {
    if (g_debug->TrackPointers()) {
      PointerData::Add(pointer, bytes);
    }

    if (g_debug->config().options() & FILL_ON_ALLOC) {
      size_t bytes = InternalMallocUsableSize(pointer);
      size_t fill_bytes = g_debug->config().fill_on_alloc_bytes();
      bytes = (bytes < fill_bytes) ? bytes : fill_bytes;
      memset(pointer, g_debug->config().fill_alloc_value(), bytes);
    }

    if (g_debug->config().options() & RECORD_ALLOCS) {
      g_debug->record->AddEntry(new MemalignEntry(pointer, bytes, alignment));
    }
  }

  return pointer;
}

void* debug_realloc(void* pointer, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->realloc(pointer, bytes);
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  if (pointer == nullptr) {
    pointer = InternalMalloc(bytes);
    if (g_debug->config().options() & RECORD_ALLOCS) {
      g_debug->record->AddEntry(new ReallocEntry(pointer, bytes, nullptr));
    }
    return pointer;
  }

  if (!VerifyPointer(pointer, "realloc")) {
    return nullptr;
  }

  if (bytes == 0) {
    if (g_debug->config().options() & RECORD_ALLOCS) {
      g_debug->record->AddEntry(new ReallocEntry(nullptr, bytes, pointer));
    }

    InternalFree(pointer);
    return nullptr;
  }

  size_t real_size = bytes;
  if (g_debug->config().options() & EXPAND_ALLOC) {
    real_size += g_debug->config().expand_alloc_bytes();
    if (real_size < bytes) {
      // Overflow.
      errno = ENOMEM;
      return nullptr;
    }
  }

  if (bytes > PointerInfoType::MaxSize()) {
    errno = ENOMEM;
    return nullptr;
  }

  void* new_pointer;
  size_t prev_size;
  if (g_debug->HeaderEnabled()) {
    // Same size, do nothing.
    Header* header = g_debug->GetHeader(pointer);
    if (real_size == header->size) {
      if (g_debug->TrackPointers()) {
        // Remove and re-add so that the backtrace is updated.
        PointerData::Remove(pointer);
        PointerData::Add(pointer, real_size);
      }
      return pointer;
    }

    // Allocation is shrinking.
    if (real_size < header->usable_size) {
      header->size = real_size;
      if (g_debug->config().options() & REAR_GUARD) {
        // Don't bother allocating a smaller pointer in this case, simply
        // change the header usable_size and reset the rear guard.
        header->usable_size = header->size;
        memset(g_debug->GetRearGuard(header), g_debug->config().rear_guard_value(),
               g_debug->config().rear_guard_bytes());
      }
      if (g_debug->TrackPointers()) {
        // Remove and re-add so that the backtrace is updated.
        PointerData::Remove(pointer);
        PointerData::Add(pointer, real_size);
      }
      return pointer;
    }

    // Allocate the new size.
    new_pointer = InternalMalloc(bytes);
    if (new_pointer == nullptr) {
      errno = ENOMEM;
      return nullptr;
    }

    prev_size = header->usable_size;
    memcpy(new_pointer, pointer, prev_size);
    InternalFree(pointer);
  } else {
    if (g_debug->TrackPointers()) {
      PointerData::Remove(pointer);
    }

    prev_size = g_dispatch->malloc_usable_size(pointer);
    new_pointer = g_dispatch->realloc(pointer, real_size);
    if (new_pointer == nullptr) {
      return nullptr;
    }

    if (g_debug->TrackPointers()) {
      PointerData::Add(new_pointer, real_size);
    }
  }

  if (g_debug->config().options() & FILL_ON_ALLOC) {
    size_t bytes = InternalMallocUsableSize(new_pointer);
    if (bytes > g_debug->config().fill_on_alloc_bytes()) {
      bytes = g_debug->config().fill_on_alloc_bytes();
    }
    if (bytes > prev_size) {
      memset(reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(new_pointer) + prev_size),
             g_debug->config().fill_alloc_value(), bytes - prev_size);
    }
  }

  if (g_debug->config().options() & RECORD_ALLOCS) {
    g_debug->record->AddEntry(new ReallocEntry(new_pointer, bytes, pointer));
  }

  return new_pointer;
}

void* debug_calloc(size_t nmemb, size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->calloc(nmemb, bytes);
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  size_t size;
  if (__builtin_mul_overflow(nmemb, bytes, &size)) {
    // Overflow
    errno = ENOMEM;
    return nullptr;
  }

  if (size == 0) {
    size = 1;
  }

  size_t real_size;
  if (__builtin_add_overflow(size, g_debug->extra_bytes(), &real_size)) {
    // Overflow.
    errno = ENOMEM;
    return nullptr;
  }

  if (real_size > PointerInfoType::MaxSize()) {
    errno = ENOMEM;
    return nullptr;
  }

  void* pointer;
  if (g_debug->HeaderEnabled()) {
    // Need to guarantee the alignment of the header.
    Header* header =
        reinterpret_cast<Header*>(g_dispatch->memalign(MINIMUM_ALIGNMENT_BYTES, real_size));
    if (header == nullptr) {
      return nullptr;
    }
    memset(header, 0, g_dispatch->malloc_usable_size(header));
    pointer = InitHeader(header, header, size);
  } else {
    pointer = g_dispatch->calloc(1, real_size);
  }

  if (g_debug->config().options() & RECORD_ALLOCS) {
    g_debug->record->AddEntry(new CallocEntry(pointer, bytes, nmemb));
  }

  if (pointer != nullptr && g_debug->TrackPointers()) {
    PointerData::Add(pointer, size);
  }
  return pointer;
}

struct mallinfo debug_mallinfo() {
  return g_dispatch->mallinfo();
}

int debug_mallopt(int param, int value) {
  return g_dispatch->mallopt(param, value);
}

int debug_malloc_info(int options, FILE* fp) {
  if (DebugCallsDisabled() || !g_debug->TrackPointers()) {
    return g_dispatch->malloc_info(options, fp);
  }

  // Make sure any pending output is written to the file.
  fflush(fp);

  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  // Avoid any issues where allocations are made that will be freed
  // in the fclose.
  int fd = fileno(fp);
  MallocXmlElem root(fd, "malloc", "version=\"debug-malloc-1\"");
  std::vector<ListInfoType> list;
  PointerData::GetAllocList(&list);

  size_t alloc_num = 0;
  for (size_t i = 0; i < list.size(); i++) {
    MallocXmlElem alloc(fd, "allocation", "nr=\"%zu\"", alloc_num);

    size_t total = 1;
    size_t size = list[i].size;
    while (i < list.size() - 1 && list[i + 1].size == size) {
      i++;
      total++;
    }
    MallocXmlElem(fd, "size").Contents("%zu", list[i].size);
    MallocXmlElem(fd, "total").Contents("%zu", total);
    alloc_num++;
  }
  return 0;
}

void* debug_aligned_alloc(size_t alignment, size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->aligned_alloc(alignment, size);
  }
  if (!powerof2(alignment) || (size % alignment) != 0) {
    errno = EINVAL;
    return nullptr;
  }
  return debug_memalign(alignment, size);
}

int debug_posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (DebugCallsDisabled()) {
    return g_dispatch->posix_memalign(memptr, alignment, size);
  }

  if (alignment < sizeof(void*) || !powerof2(alignment)) {
    return EINVAL;
  }
  int saved_errno = errno;
  *memptr = debug_memalign(alignment, size);
  errno = saved_errno;
  return (*memptr != nullptr) ? 0 : ENOMEM;
}

int debug_malloc_iterate(uintptr_t base, size_t size, void (*callback)(uintptr_t, size_t, void*),
                  void* arg) {
  ScopedConcurrentLock lock;
  if (g_debug->TrackPointers()) {
    PointerData::IteratePointers([&callback, &arg](uintptr_t pointer) {
      callback(pointer, InternalMallocUsableSize(reinterpret_cast<void*>(pointer)), arg);
    });
    return 0;
  }

  // An option that adds a header will add pointer tracking, so no need to
  // check if headers are enabled.
  return g_dispatch->malloc_iterate(base, size, callback, arg);
}

void debug_malloc_disable() {
  ScopedConcurrentLock lock;
  g_dispatch->malloc_disable();
  if (g_debug->pointer) {
    g_debug->pointer->PrepareFork();
  }
}

void debug_malloc_enable() {
  ScopedConcurrentLock lock;
  if (g_debug->pointer) {
    g_debug->pointer->PostForkParent();
  }
  g_dispatch->malloc_enable();
}

ssize_t debug_malloc_backtrace(void* pointer, uintptr_t* frames, size_t max_frames) {
  if (DebugCallsDisabled() || pointer == nullptr) {
    return 0;
  }
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  if (!(g_debug->config().options() & BACKTRACE)) {
    return 0;
  }
  pointer = UntagPointer(pointer);
  return PointerData::GetFrames(pointer, frames, max_frames);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* debug_pvalloc(size_t bytes) {
  if (DebugCallsDisabled()) {
    return g_dispatch->pvalloc(bytes);
  }

  size_t pagesize = getpagesize();
  size_t size = __BIONIC_ALIGN(bytes, pagesize);
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

static std::mutex g_dump_lock;

static void write_dump(int fd) {
  dprintf(fd, "Android Native Heap Dump v1.2\n\n");

  std::string fingerprint = android::base::GetProperty("ro.build.fingerprint", "unknown");
  dprintf(fd, "Build fingerprint: '%s'\n\n", fingerprint.c_str());

  PointerData::DumpLiveToFile(fd);

  dprintf(fd, "MAPS\n");
  std::string content;
  if (!android::base::ReadFileToString("/proc/self/maps", &content)) {
    dprintf(fd, "Could not open /proc/self/maps\n");
  } else {
    dprintf(fd, "%s", content.c_str());
  }
  dprintf(fd, "END\n");
}

bool debug_write_malloc_leak_info(FILE* fp) {
  // Make sure any pending output is written to the file.
  fflush(fp);

  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  std::lock_guard<std::mutex> guard(g_dump_lock);

  if (!(g_debug->config().options() & BACKTRACE)) {
    return false;
  }

  write_dump(fileno(fp));

  return true;
}

void debug_dump_heap(const char* file_name) {
  ScopedConcurrentLock lock;
  ScopedDisableDebugCalls disable;
  ScopedBacktraceSignalBlocker blocked;

  std::lock_guard<std::mutex> guard(g_dump_lock);

  int fd = open(file_name, O_RDWR | O_CREAT | O_NOFOLLOW | O_TRUNC | O_CLOEXEC, 0644);
  if (fd == -1) {
    error_log("Unable to create file: %s", file_name);
    return;
  }

  error_log("Dumping to file: %s\n", file_name);
  write_dump(fd);
  close(fd);
}
