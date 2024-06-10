/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <link.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <sys/cdefs.h>

#include "bionic_elf_dtv_offset.h"

__LIBC_HIDDEN__ extern _Atomic(size_t) __libc_tls_generation_copy;

struct TlsAlign {
  size_t value = 1;
  size_t skew = 0;  // p_vaddr % p_align

  template <typename T>
  static constexpr TlsAlign of_type() {
    return TlsAlign{.value = alignof(T)};
  }
};

struct TlsAlignedSize {
  size_t size = 0;
  TlsAlign align;

  template <typename T>
  static constexpr TlsAlignedSize of_type() {
    return TlsAlignedSize{.size = sizeof(T), .align = TlsAlign::of_type<T>()};
  }
};

struct TlsSegment {
  TlsAlignedSize aligned_size;
  const void* init_ptr = "";    // Field is non-null even when init_size is 0.
  size_t init_size = 0;
};

__LIBC_HIDDEN__ bool __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                              ElfW(Addr) load_bias, TlsSegment* out);

__LIBC_HIDDEN__ bool __bionic_check_tls_align(size_t align);

struct StaticTlsLayout {
  constexpr StaticTlsLayout() {}

public:
  size_t offset_bionic_tcb() const { return offset_bionic_tcb_; }
  size_t offset_bionic_tls() const { return offset_bionic_tls_; }
  size_t offset_thread_pointer() const;
  size_t offset_exe() const { return offset_exe_; }

  size_t size() const { return cursor_; }

  size_t reserve_exe_segment_and_tcb(const TlsSegment* exe_segment, const char* progname);
  size_t reserve_bionic_tls();
  size_t reserve_solib_segment(const TlsSegment& segment) { return reserve(segment.aligned_size); }
  void finish_layout();

#if !defined(STATIC_TLS_LAYOUT_TEST)
 private:
#endif
  size_t cursor_ = 0;
  size_t align_ = 1;

  // Offsets to various Bionic TLS structs from the beginning of static TLS.
  size_t offset_bionic_tcb_ = SIZE_MAX;
  size_t offset_bionic_tls_ = SIZE_MAX;

  size_t offset_exe_ = SIZE_MAX;

  struct TpAllocations {
    size_t before;
    size_t tp;
    size_t after;
  };

  size_t align_cursor(TlsAlign align);
  size_t align_cursor_unskewed(size_t align);
  size_t reserve(TlsAlignedSize aligned_size);
  TpAllocations reserve_tp_pair(TlsAlignedSize before, TlsAlignedSize after);

  template <typename T> size_t reserve_type() {
    return reserve(TlsAlignedSize::of_type<T>());
  }
};

static constexpr size_t kTlsGenerationNone = 0;
static constexpr size_t kTlsGenerationFirst = 1;

// The first ELF TLS module has ID 1. Zero is reserved for the first word of
// the DTV, a generation count. Unresolved weak symbols also use module ID 0.
static constexpr size_t kTlsUninitializedModuleId = 0;

static inline size_t __tls_module_id_to_idx(size_t id) { return id - 1; }
static inline size_t __tls_module_idx_to_id(size_t idx) { return idx + 1; }

// A descriptor for a single ELF TLS module.
struct TlsModule {
  TlsSegment segment;

  // Offset into the static TLS block or SIZE_MAX for a dynamic module.
  size_t static_offset = SIZE_MAX;

  // The generation in which this module was loaded. Dynamic TLS lookups use
  // this field to detect when a module has been unloaded.
  size_t first_generation = kTlsGenerationNone;

  // Used by the dynamic linker to track the associated soinfo* object.
  void* soinfo_ptr = nullptr;
};

// Signature of the callbacks that will be called after DTLS creation and
// before DTLS destruction.
typedef void (*dtls_listener_t)(void* dynamic_tls_begin, void* dynamic_tls_end);

// Signature of the thread-exit callbacks.
typedef void (*thread_exit_cb_t)(void);

struct CallbackHolder {
  thread_exit_cb_t cb;
  CallbackHolder* prev;
};

// Table of the ELF TLS modules. Either the dynamic linker or the static
// initialization code prepares this table, and it's then used during thread
// creation and for dynamic TLS lookups.
struct TlsModules {
  constexpr TlsModules() {}

  // A pointer to the TLS generation counter in libc.so. The counter is
  // incremented each time an solib is loaded or unloaded.
  _Atomic(size_t) generation = kTlsGenerationFirst;
  _Atomic(size_t) *generation_libc_so = nullptr;

  // Access to the TlsModule[] table requires taking this lock.
  pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

  // Pointer to a block of TlsModule objects. The first module has ID 1 and
  // is stored at index 0 in this table.
  size_t module_count = 0;
  size_t static_module_count = 0;
  TlsModule* module_table = nullptr;

  // Callback to be invoked after a dynamic TLS allocation.
  dtls_listener_t on_creation_cb = nullptr;

  // Callback to be invoked before a dynamic TLS deallocation.
  dtls_listener_t on_destruction_cb = nullptr;

  // The first thread-exit callback; inlined to avoid allocation.
  thread_exit_cb_t first_thread_exit_callback = nullptr;

  // The additional callbacks, if any.
  CallbackHolder* thread_exit_callback_tail_node = nullptr;
};

void __init_static_tls(void* static_tls);

// Dynamic Thread Vector. Each thread has a different DTV. For each module
// (executable or solib), the DTV has a pointer to that module's TLS memory. The
// DTV is initially empty and is allocated on-demand. It grows as more modules
// are dlopen'ed. See https://www.akkadia.org/drepper/tls.pdf.
//
// The layout of the DTV is specified in various documents, but it is not part
// of Bionic's public ABI. A compiler can't generate code to access it directly,
// because it can't access libc's global generation counter.
struct TlsDtv {
  // Number of elements in this object's modules field.
  size_t count;

  // A pointer to an older TlsDtv object that should be freed when the thread
  // exits. The objects aren't immediately freed because a DTV could be
  // reallocated by a signal handler that interrupted __tls_get_addr's fast
  // path.
  TlsDtv* next;

  // The DTV slot points at this field, which allows omitting an add instruction
  // on the fast path for a TLS lookup. The arm64 tlsdesc_resolver.S depends on
  // the layout of fields past this point.
  size_t generation;
  void* modules[];
};

struct TlsIndex {
  size_t module_id;
  size_t offset;
};

#if defined(__i386__)
#define TLS_GET_ADDR_CALLING_CONVENTION __attribute__((regparm(1)))
#define TLS_GET_ADDR ___tls_get_addr
#else
#define TLS_GET_ADDR_CALLING_CONVENTION
#define TLS_GET_ADDR __tls_get_addr
#endif

extern "C" void* TLS_GET_ADDR(const TlsIndex* ti) TLS_GET_ADDR_CALLING_CONVENTION;

struct bionic_tcb;
void __free_dynamic_tls(bionic_tcb* tcb);
void __notify_thread_exit_callbacks();

