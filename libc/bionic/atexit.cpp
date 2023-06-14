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

#include "atexit.h"

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/prctl.h>

#include <async_safe/CHECK.h>
#include <async_safe/log.h>

#include "platform/bionic/page.h"

extern "C" void __libc_stdio_cleanup();
extern "C" void __unregister_atfork(void* dso);

namespace {

struct AtexitEntry {
  void (*fn)(void*);  // the __cxa_atexit callback
  void* arg;          // argument for `fn` callback
  void* dso;          // shared module handle
};

class AtexitArray {
 public:
  size_t size() const { return size_; }
  uint64_t total_appends() const { return total_appends_; }
  const AtexitEntry& operator[](size_t idx) const { return array_[idx]; }

  bool append_entry(const AtexitEntry& entry);
  AtexitEntry extract_entry(size_t idx);
  void recompact();

 private:
  AtexitEntry* array_;
  size_t size_;
  size_t extracted_count_;
  size_t capacity_;

  // An entry can be appended by a __cxa_finalize callback. Track the number of appends so we
  // restart concurrent __cxa_finalize passes.
  uint64_t total_appends_;

  static size_t page_start_of_index(size_t idx) { return page_start(idx * sizeof(AtexitEntry)); }
  static size_t page_end_of_index(size_t idx) { return page_end(idx * sizeof(AtexitEntry)); }

  // Recompact the array if it will save at least one page of memory at the end.
  bool needs_recompaction() const {
    return page_end_of_index(size_ - extracted_count_) < page_end_of_index(size_);
  }

  void set_writable(bool writable, size_t start_idx, size_t num_entries);
  static bool next_capacity(size_t capacity, size_t* result);
  bool expand_capacity();
};

}  // anonymous namespace

bool AtexitArray::append_entry(const AtexitEntry& entry) {
  if (size_ >= capacity_ && !expand_capacity()) return false;

  size_t idx = size_++;

  set_writable(true, idx, 1);
  array_[idx] = entry;
  ++total_appends_;
  set_writable(false, idx, 1);

  return true;
}

// Extract an entry and return it.
AtexitEntry AtexitArray::extract_entry(size_t idx) {
  AtexitEntry result = array_[idx];

  set_writable(true, idx, 1);
  array_[idx] = {};
  ++extracted_count_;
  set_writable(false, idx, 1);

  return result;
}

void AtexitArray::recompact() {
  if (!needs_recompaction()) return;

  set_writable(true, 0, size_);

  // Optimization: quickly skip over the initial non-null entries.
  size_t src = 0, dst = 0;
  while (src < size_ && array_[src].fn != nullptr) {
    ++src;
    ++dst;
  }

  // Shift the non-null entries forward, and zero out the removed entries at the end of the array.
  for (; src < size_; ++src) {
    const AtexitEntry entry = array_[src];
    array_[src] = {};
    if (entry.fn != nullptr) {
      array_[dst++] = entry;
    }
  }

  // If the table uses fewer pages, clean the pages at the end.
  size_t old_bytes = page_end_of_index(size_);
  size_t new_bytes = page_end_of_index(dst);
  if (new_bytes < old_bytes) {
    madvise(reinterpret_cast<char*>(array_) + new_bytes, old_bytes - new_bytes, MADV_DONTNEED);
  }

  set_writable(false, 0, size_);

  size_ = dst;
  extracted_count_ = 0;
}

// Use mprotect to make the array writable or read-only. Returns true on success. Making the array
// read-only could protect against either unintentional or malicious corruption of the array.
void AtexitArray::set_writable(bool writable, size_t start_idx, size_t num_entries) {
  if (array_ == nullptr) return;

  const size_t start_byte = page_start_of_index(start_idx);
  const size_t stop_byte = page_end_of_index(start_idx + num_entries);
  const size_t byte_len = stop_byte - start_byte;

  const int prot = PROT_READ | (writable ? PROT_WRITE : 0);
  if (mprotect(reinterpret_cast<char*>(array_) + start_byte, byte_len, prot) != 0) {
    async_safe_fatal("mprotect failed on atexit array: %s", strerror(errno));
  }
}

// Approximately double the capacity. Returns true if successful (no overflow). AtexitEntry is
// smaller than a page, but this function should still be correct even if AtexitEntry were larger
// than one.
bool AtexitArray::next_capacity(size_t capacity, size_t* result) {
  if (capacity == 0) {
    *result = page_end(sizeof(AtexitEntry)) / sizeof(AtexitEntry);
    return true;
  }
  size_t num_bytes;
  if (__builtin_mul_overflow(page_end_of_index(capacity), 2, &num_bytes)) {
    async_safe_format_log(ANDROID_LOG_WARN, "libc", "__cxa_atexit: capacity calculation overflow");
    return false;
  }
  *result = num_bytes / sizeof(AtexitEntry);
  return true;
}

bool AtexitArray::expand_capacity() {
  size_t new_capacity;
  if (!next_capacity(capacity_, &new_capacity)) return false;
  const size_t new_capacity_bytes = page_end_of_index(new_capacity);

  set_writable(true, 0, capacity_);

  bool result = false;
  void* new_pages;
  if (array_ == nullptr) {
    new_pages = mmap(nullptr, new_capacity_bytes, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  } else {
    // mremap fails if the source buffer crosses a boundary between two VMAs. When a single array
    // element is modified, the kernel should split then rejoin the buffer's VMA.
    new_pages = mremap(array_, page_end_of_index(capacity_), new_capacity_bytes, MREMAP_MAYMOVE);
  }
  if (new_pages == MAP_FAILED) {
    async_safe_format_log(ANDROID_LOG_WARN, "libc",
                          "__cxa_atexit: mmap/mremap failed to allocate %zu bytes: %s",
                          new_capacity_bytes, strerror(errno));
  } else {
    result = true;
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, new_pages, new_capacity_bytes, "atexit handlers");
    array_ = static_cast<AtexitEntry*>(new_pages);
    capacity_ = new_capacity;
  }
  set_writable(false, 0, capacity_);
  return result;
}

static AtexitArray g_array;
static pthread_mutex_t g_atexit_lock = PTHREAD_MUTEX_INITIALIZER;

static inline void atexit_lock() {
  pthread_mutex_lock(&g_atexit_lock);
}

static inline void atexit_unlock() {
  pthread_mutex_unlock(&g_atexit_lock);
}

// Register a function to be called either when a library is unloaded (dso != nullptr), or when the
// program exits (dso == nullptr). The `dso` argument is typically the address of a hidden
// __dso_handle variable. This function is also used as the backend for the atexit function.
//
// See https://itanium-cxx-abi.github.io/cxx-abi/abi.html#dso-dtor.
//
int __cxa_atexit(void (*func)(void*), void* arg, void* dso) {
  int result = -1;

  if (func != nullptr) {
    atexit_lock();
    if (g_array.append_entry({.fn = func, .arg = arg, .dso = dso})) {
      result = 0;
    }
    atexit_unlock();
  }

  return result;
}

void __cxa_finalize(void* dso) {
  atexit_lock();

  static uint32_t call_depth = 0;
  ++call_depth;

restart:
  const uint64_t total_appends = g_array.total_appends();

  for (ssize_t i = g_array.size() - 1; i >= 0; --i) {
    if (g_array[i].fn == nullptr || (dso != nullptr && g_array[i].dso != dso)) continue;

    // Clear the entry in the array because its DSO handle will become invalid, and to avoid calling
    // an entry again if __cxa_finalize is called recursively.
    const AtexitEntry entry = g_array.extract_entry(i);

    atexit_unlock();
    entry.fn(entry.arg);
    atexit_lock();

    if (g_array.total_appends() != total_appends) goto restart;
  }

  // Avoid recompaction on recursive calls because it's unnecessary and would require earlier,
  // concurrent __cxa_finalize calls to restart. Skip recompaction on program exit too
  // (dso == nullptr), because the memory will be reclaimed soon anyway.
  --call_depth;
  if (call_depth == 0 && dso != nullptr) {
    g_array.recompact();
  }

  atexit_unlock();

  if (dso != nullptr) {
    __unregister_atfork(dso);
  } else {
    // If called via exit(), flush output of all open files.
    __libc_stdio_cleanup();
  }
}
