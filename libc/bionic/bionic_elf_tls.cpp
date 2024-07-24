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

#include "private/bionic_elf_tls.h"

#include <async_safe/CHECK.h>
#include <async_safe/log.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "platform/bionic/macros.h"
#include "platform/bionic/page.h"
#include "private/ScopedRWLock.h"
#include "private/ScopedSignalBlocker.h"
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "pthread_internal.h"

// Every call to __tls_get_addr needs to check the generation counter, so
// accesses to the counter need to be as fast as possible. Keep a copy of it in
// a hidden variable, which can be accessed without using the GOT. The linker
// will update this variable when it updates its counter.
//
// To allow the linker to update this variable, libc.so's constructor passes its
// address to the linker. To accommodate a possible __tls_get_addr call before
// libc.so's constructor, this local copy is initialized to SIZE_MAX, forcing
// __tls_get_addr to initially use the slow path.
__LIBC_HIDDEN__ _Atomic(size_t) __libc_tls_generation_copy = SIZE_MAX;

// Search for a TLS segment in the given phdr table. Returns true if it has a
// TLS segment and false otherwise.
bool __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                              ElfW(Addr) load_bias, TlsSegment* out) {
  for (size_t i = 0; i < phdr_count; ++i) {
    const ElfW(Phdr)& phdr = phdr_table[i];
    if (phdr.p_type == PT_TLS) {
      *out = TlsSegment{
          .aligned_size =
              TlsAlignedSize{
                  .size = phdr.p_memsz,
                  .align =
                      TlsAlign{
                          .value = phdr.p_align ?: 1,  // 0 means "no alignment requirement"
                          .skew = phdr.p_vaddr % MAX(1, phdr.p_align),
                      },
              },
          .init_ptr = reinterpret_cast<void*>(load_bias + phdr.p_vaddr),
          .init_size = phdr.p_filesz,
      };
      return true;
    }
  }
  return false;
}

// Return true if the alignment of a TLS segment is a valid power-of-two.
bool __bionic_check_tls_align(size_t align) {
  // Note: The size does not need to be a multiple of the alignment. With ld.bfd
  // (or after using binutils' strip), the TLS segment's size isn't rounded up.
  return powerof2(align);
}

static void static_tls_layout_overflow() {
  async_safe_fatal("error: TLS segments in static TLS overflowed");
}

static size_t align_checked(size_t value, TlsAlign tls_align) {
  const size_t align = tls_align.value;
  const size_t skew = tls_align.skew;
  CHECK(align != 0 && powerof2(align + 0) && skew < align);
  const size_t result = ((value - skew + align - 1) & ~(align - 1)) + skew;
  if (result < value) static_tls_layout_overflow();
  return result;
}

size_t StaticTlsLayout::offset_thread_pointer() const {
  return offset_bionic_tcb_ + (-MIN_TLS_SLOT * sizeof(void*));
}

// Allocates the Bionic TCB and the executable's TLS segment in the static TLS
// layout, satisfying alignment requirements for both.
//
// For an executable's TLS accesses (using the LocalExec model), the static
// linker bakes TLS offsets directly into the .text section, so the loader must
// place the executable segment at the same offset relative to the TP.
// Similarly, the Bionic TLS slots (bionic_tcb) must also be allocated at the
// correct offset relative to the TP.
//
// Returns the offset of the executable's TLS segment.
//
// Note: This function has unit tests, but they are in bionic-unit-tests-static,
// not bionic-unit-tests.
size_t StaticTlsLayout::reserve_exe_segment_and_tcb(const TlsSegment* seg,
                                                    const char* progname __attribute__((unused))) {
  // Special case: if the executable has no TLS segment, then just allocate a
  // TCB and skip the minimum alignment check on ARM.
  if (seg == nullptr) {
    offset_bionic_tcb_ = reserve_type<bionic_tcb>();
    return 0;
  }

#if defined(__arm__) || defined(__aarch64__)
  // ARM uses a "variant 1" TLS layout. The ABI specifies that the TP points at
  // a 2-word TCB, followed by the executable's segment. In practice, libc
  // implementations actually allocate a larger TCB at negative offsets from the
  // TP.
  //
  // Historically, Bionic allocated an 8-word TCB starting at TP+0, so to keep
  // the executable's TLS segment from overlapping the last 6 slots, Bionic
  // requires that executables have an 8-word PT_TLS alignment to ensure that
  // the TCB fits in the alignment padding, which it accomplishes using
  // crtbegin.c. Bionic uses negative offsets for new TLS slots to avoid this
  // problem.

  static_assert(MIN_TLS_SLOT <= 0 && MAX_TLS_SLOT >= 1);
  static_assert(sizeof(bionic_tcb) == (MAX_TLS_SLOT - MIN_TLS_SLOT + 1) * sizeof(void*));
  static_assert(alignof(bionic_tcb) == sizeof(void*));
  const size_t max_align = MAX(alignof(bionic_tcb), seg->aligned_size.align.value);

  // Allocate the TCB first. Split it into negative and non-negative slots and
  // ensure that TP (i.e. the first non-negative slot) is aligned to max_align.
  const size_t tcb_size_pre = -MIN_TLS_SLOT * sizeof(void*);
  const size_t tcb_size_post = (MAX_TLS_SLOT + 1) * sizeof(void*);
  const auto pair =
      reserve_tp_pair(TlsAlignedSize{.size = tcb_size_pre},
                      TlsAlignedSize{.size = tcb_size_post, .align = TlsAlign{.value = max_align}});
  offset_bionic_tcb_ = pair.before;
  const size_t offset_tp = pair.tp;

  // Allocate the segment.
  offset_exe_ = reserve(seg->aligned_size);

  // Verify that the ABI and Bionic tpoff values are equal, which is equivalent
  // to checking whether the segment is sufficiently aligned.
  const size_t abi_tpoff = align_checked(2 * sizeof(void*), seg->aligned_size.align);
  const size_t actual_tpoff = align_checked(tcb_size_post, seg->aligned_size.align);
  CHECK(actual_tpoff == offset_exe_ - offset_tp);

  if (abi_tpoff != actual_tpoff) {
    async_safe_fatal(
        "error: \"%s\": executable's TLS segment is underaligned: "
        "alignment is %zu (skew %zu), needs to be at least %zu for %s Bionic",
        progname, seg->aligned_size.align.value, seg->aligned_size.align.skew, tcb_size_post,
        (sizeof(void*) == 4 ? "ARM" : "ARM64"));
  }

#elif defined(__i386__) || defined(__x86_64__)

  auto pair = reserve_tp_pair(seg->aligned_size, TlsAlignedSize::of_type<bionic_tcb>());
  offset_exe_ = pair.before;
  offset_bionic_tcb_ = pair.after;

#elif defined(__riscv)
  static_assert(MAX_TLS_SLOT == -1, "Last slot of bionic_tcb must be slot #(-1) on riscv");

  auto pair = reserve_tp_pair(TlsAlignedSize::of_type<bionic_tcb>(), seg->aligned_size);
  offset_bionic_tcb_ = pair.before;
  offset_exe_ = pair.after;

#else
#error "Unrecognized architecture"
#endif

  return offset_exe_;
}

size_t StaticTlsLayout::reserve_bionic_tls() {
  offset_bionic_tls_ = reserve_type<bionic_tls>();
  return offset_bionic_tls_;
}

void StaticTlsLayout::finish_layout() {
  // Round the offset up to the alignment.
  cursor_ = align_checked(cursor_, TlsAlign{.value = align_});
}

size_t StaticTlsLayout::align_cursor(TlsAlign align) {
  cursor_ = align_checked(cursor_, align);
  align_ = MAX(align_, align.value);
  return cursor_;
}

size_t StaticTlsLayout::align_cursor_unskewed(size_t align) {
  return align_cursor(TlsAlign{.value = align});
}

// Reserve the requested number of bytes at the requested alignment. The
// requested size is not required to be a multiple of the alignment, nor is the
// cursor aligned after the allocation.
size_t StaticTlsLayout::reserve(TlsAlignedSize aligned_size) {
  align_cursor(aligned_size.align);
  const size_t result = cursor_;
  if (__builtin_add_overflow(cursor_, aligned_size.size, &cursor_)) static_tls_layout_overflow();
  return result;
}

// Calculate the TP offset and allocate something before it and something after
// it. The TP will be aligned to:
//
//     MAX(before.align.value, after.align.value)
//
// The `before` and `after` allocations are each allocated as closely as
// possible to the TP.
StaticTlsLayout::TpAllocations StaticTlsLayout::reserve_tp_pair(TlsAlignedSize before,
                                                                TlsAlignedSize after) {
  // Tentative `before` allocation.
  const size_t tentative_before = reserve(before);
  const size_t tentative_before_end = align_cursor_unskewed(before.align.value);

  const size_t offset_tp = align_cursor_unskewed(MAX(before.align.value, after.align.value));

  const size_t offset_after = reserve(after);

  // If the `after` allocation has higher alignment than `before`, then there
  // may be alignment padding to remove between `before` and the TP. Shift
  // `before` forward to remove this padding.
  CHECK(((offset_tp - tentative_before_end) & (before.align.value - 1)) == 0);
  const size_t offset_before = tentative_before + (offset_tp - tentative_before_end);

  return TpAllocations{offset_before, offset_tp, offset_after};
}

// Copy each TLS module's initialization image into a newly-allocated block of
// static TLS memory. To reduce dirty pages, this function only writes to pages
// within the static TLS that need initialization. The memory should already be
// zero-initialized on entry.
void __init_static_tls(void* static_tls) {
  // The part of the table we care about (i.e. static TLS modules) never changes
  // after startup, but we still need the mutex because the table could grow,
  // moving the initial part. If this locking is too slow, we can duplicate the
  // static part of the table.
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  ScopedSignalBlocker ssb;
  ScopedReadLock locker(&modules.rwlock);

  for (size_t i = 0; i < modules.module_count; ++i) {
    TlsModule& module = modules.module_table[i];
    if (module.static_offset == SIZE_MAX) {
      // All of the static modules come before all of the dynamic modules, so
      // once we see the first dynamic module, we're done.
      break;
    }
    if (module.segment.init_size == 0) {
      // Skip the memcpy call for TLS segments with no initializer, which is
      // common.
      continue;
    }
    memcpy(static_cast<char*>(static_tls) + module.static_offset,
           module.segment.init_ptr,
           module.segment.init_size);
  }
}

static inline size_t dtv_size_in_bytes(size_t module_count) {
  return sizeof(TlsDtv) + module_count * sizeof(void*);
}

// Calculates the number of module slots to allocate in a new DTV. For small
// objects (up to 1KiB), the TLS allocator allocates memory in power-of-2 sizes,
// so for better space usage, ensure that the DTV size (header + slots) is a
// power of 2.
//
// The lock on TlsModules must be held.
static size_t calculate_new_dtv_count() {
  size_t loaded_cnt = __libc_shared_globals()->tls_modules.module_count;
  size_t bytes = dtv_size_in_bytes(MAX(1, loaded_cnt));
  if (!powerof2(bytes)) {
    bytes = BIONIC_ROUND_UP_POWER_OF_2(bytes);
  }
  return (bytes - sizeof(TlsDtv)) / sizeof(void*);
}

// This function must be called with signals blocked and a write lock on
// TlsModules held.
static void update_tls_dtv(bionic_tcb* tcb) {
  const TlsModules& modules = __libc_shared_globals()->tls_modules;
  BionicAllocator& allocator = __libc_shared_globals()->tls_allocator;

  // Use the generation counter from the shared globals instead of the local
  // copy, which won't be initialized yet if __tls_get_addr is called before
  // libc.so's constructor.
  if (__get_tcb_dtv(tcb)->generation == atomic_load(&modules.generation)) {
    return;
  }

  const size_t old_cnt = __get_tcb_dtv(tcb)->count;

  // If the DTV isn't large enough, allocate a larger one. Because a signal
  // handler could interrupt the fast path of __tls_get_addr, we don't free the
  // old DTV. Instead, we add the old DTV to a list, then free all of a thread's
  // DTVs at thread-exit. Each time the DTV is reallocated, its size at least
  // doubles.
  if (modules.module_count > old_cnt) {
    size_t new_cnt = calculate_new_dtv_count();
    TlsDtv* const old_dtv = __get_tcb_dtv(tcb);
    TlsDtv* const new_dtv = static_cast<TlsDtv*>(allocator.alloc(dtv_size_in_bytes(new_cnt)));
    memcpy(new_dtv, old_dtv, dtv_size_in_bytes(old_cnt));
    new_dtv->count = new_cnt;
    new_dtv->next = old_dtv;
    __set_tcb_dtv(tcb, new_dtv);
  }

  TlsDtv* const dtv = __get_tcb_dtv(tcb);

  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  char* static_tls = reinterpret_cast<char*>(tcb) - layout.offset_bionic_tcb();

  // Initialize static TLS modules and free unloaded modules.
  for (size_t i = 0; i < dtv->count; ++i) {
    if (i < modules.module_count) {
      const TlsModule& mod = modules.module_table[i];
      if (mod.static_offset != SIZE_MAX) {
        dtv->modules[i] = static_tls + mod.static_offset;
        continue;
      }
      if (mod.first_generation != kTlsGenerationNone &&
          mod.first_generation <= dtv->generation) {
        continue;
      }
    }
    if (modules.on_destruction_cb != nullptr) {
      void* dtls_begin = dtv->modules[i];
      void* dtls_end =
          static_cast<void*>(static_cast<char*>(dtls_begin) + allocator.get_chunk_size(dtls_begin));
      modules.on_destruction_cb(dtls_begin, dtls_end);
    }
    allocator.free(dtv->modules[i]);
    dtv->modules[i] = nullptr;
  }

  dtv->generation = atomic_load(&modules.generation);
}

__attribute__((noinline)) static void* tls_get_addr_slow_path(const TlsIndex* ti) {
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  bionic_tcb* tcb = __get_bionic_tcb();

  // Block signals and lock TlsModules. We may need the allocator, so take
  // a write lock.
  ScopedSignalBlocker ssb;
  ScopedWriteLock locker(&modules.rwlock);

  update_tls_dtv(tcb);

  TlsDtv* dtv = __get_tcb_dtv(tcb);
  const size_t module_idx = __tls_module_id_to_idx(ti->module_id);
  void* mod_ptr = dtv->modules[module_idx];
  if (mod_ptr == nullptr) {
    const TlsSegment& segment = modules.module_table[module_idx].segment;
    // TODO: Currently the aligned_size.align.skew property is ignored.
    // That is, for a dynamic TLS block at addr A, (A % p_align) will be 0, not
    // (p_vaddr % p_align).
    mod_ptr = __libc_shared_globals()->tls_allocator.memalign(segment.aligned_size.align.value,
                                                              segment.aligned_size.size);
    if (segment.init_size > 0) {
      memcpy(mod_ptr, segment.init_ptr, segment.init_size);
    }
    dtv->modules[module_idx] = mod_ptr;

    // Reports the allocation to the listener, if any.
    if (modules.on_creation_cb != nullptr) {
      modules.on_creation_cb(
          mod_ptr, static_cast<void*>(static_cast<char*>(mod_ptr) + segment.aligned_size.size));
    }
  }

  return static_cast<char*>(mod_ptr) + ti->offset + TLS_DTV_OFFSET;
}

// Returns the address of a thread's TLS memory given a module ID and an offset
// into that module's TLS segment. This function is called on every access to a
// dynamic TLS variable on targets that don't use TLSDESC. arm64 uses TLSDESC,
// so it only calls this function on a thread's first access to a module's TLS
// segment.
//
// On most targets, this accessor function is __tls_get_addr and
// TLS_GET_ADDR_CALLING_CONVENTION is unset, but 32-bit x86 uses
// ___tls_get_addr (with three underscores) instead, and a regparm
// calling convention.
extern "C" void* TLS_GET_ADDR(const TlsIndex* ti) TLS_GET_ADDR_CALLING_CONVENTION {
  TlsDtv* dtv = __get_tcb_dtv(__get_bionic_tcb());

  // TODO: See if we can use a relaxed memory ordering here instead.
  size_t generation = atomic_load(&__libc_tls_generation_copy);
  if (__predict_true(generation == dtv->generation)) {
    void* mod_ptr = dtv->modules[__tls_module_id_to_idx(ti->module_id)];
    if (__predict_true(mod_ptr != nullptr)) {
      return static_cast<char*>(mod_ptr) + ti->offset + TLS_DTV_OFFSET;
    }
  }

  return tls_get_addr_slow_path(ti);
}

// This function frees:
//  - TLS modules referenced by the current DTV.
//  - The list of DTV objects associated with the current thread.
//
// The caller must have already blocked signals.
void __free_dynamic_tls(bionic_tcb* tcb) {
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  BionicAllocator& allocator = __libc_shared_globals()->tls_allocator;

  // If we didn't allocate any dynamic memory, skip out early without taking
  // the lock.
  TlsDtv* dtv = __get_tcb_dtv(tcb);
  if (dtv->generation == kTlsGenerationNone) {
    return;
  }

  // We need the write lock to use the allocator.
  ScopedWriteLock locker(&modules.rwlock);

  // First free everything in the current DTV.
  for (size_t i = 0; i < dtv->count; ++i) {
    if (i < modules.module_count && modules.module_table[i].static_offset != SIZE_MAX) {
      // This module's TLS memory is allocated statically, so don't free it here.
      continue;
    }

    if (modules.on_destruction_cb != nullptr) {
      void* dtls_begin = dtv->modules[i];
      void* dtls_end =
          static_cast<void*>(static_cast<char*>(dtls_begin) + allocator.get_chunk_size(dtls_begin));
      modules.on_destruction_cb(dtls_begin, dtls_end);
    }

    allocator.free(dtv->modules[i]);
  }

  // Now free the thread's list of DTVs.
  while (dtv->generation != kTlsGenerationNone) {
    TlsDtv* next = dtv->next;
    allocator.free(dtv);
    dtv = next;
  }

  // Clear the DTV slot. The DTV must not be used again with this thread.
  tcb->tls_slot(TLS_SLOT_DTV) = nullptr;
}

// Invokes all the registered thread_exit callbacks, if any.
void __notify_thread_exit_callbacks() {
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  if (modules.first_thread_exit_callback == nullptr) {
    // If there is no first_thread_exit_callback, there shouldn't be a tail.
    CHECK(modules.thread_exit_callback_tail_node == nullptr);
    return;
  }

  // Callbacks are supposed to be invoked in the reverse order
  // in which they were registered.
  CallbackHolder* node = modules.thread_exit_callback_tail_node;
  while (node != nullptr) {
    node->cb();
    node = node->prev;
  }
  modules.first_thread_exit_callback();
}
