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

#include <async_safe/log.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#include "private/ScopedRWLock.h"
#include "private/bionic_globals.h"
#include "private/bionic_macros.h"
#include "private/bionic_tls.h"

// Search for a TLS segment in the given phdr table. Returns true if it has a
// TLS segment and false otherwise.
bool __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                              ElfW(Addr) load_bias, TlsSegment* out) {
  for (size_t i = 0; i < phdr_count; ++i) {
    const ElfW(Phdr)& phdr = phdr_table[i];
    if (phdr.p_type == PT_TLS) {
      *out = TlsSegment {
        phdr.p_memsz,
        phdr.p_align,
        reinterpret_cast<void*>(load_bias + phdr.p_vaddr),
        phdr.p_filesz,
      };
      return true;
    }
  }
  return false;
}

// Return true if the alignment of a TLS segment is a valid power-of-two. Also
// cap the alignment if it's too high.
bool __bionic_check_tls_alignment(size_t* alignment) {
  // N.B. The size does not need to be a multiple of the alignment. With
  // ld.bfd (or after using binutils' strip), the TLS segment's size isn't
  // rounded up.
  if (*alignment == 0 || !powerof2(*alignment)) {
    return false;
  }
  // Bionic only respects TLS alignment up to one page.
  *alignment = MIN(*alignment, PAGE_SIZE);
  return true;
}

size_t StaticTlsLayout::offset_thread_pointer() const {
  return offset_bionic_tcb_ + (-MIN_TLS_SLOT * sizeof(void*));
}

// Reserves space for the Bionic TCB and the executable's TLS segment. Returns
// the offset of the executable's TLS segment.
size_t StaticTlsLayout::reserve_exe_segment_and_tcb(const TlsSegment* exe_segment,
                                                    const char* progname __attribute__((unused))) {
  // Special case: if the executable has no TLS segment, then just allocate a
  // TCB and skip the minimum alignment check on ARM.
  if (exe_segment == nullptr) {
    offset_bionic_tcb_ = reserve_type<bionic_tcb>();
    return 0;
  }

#if defined(__arm__) || defined(__aarch64__)

  // First reserve enough space for the TCB before the executable segment.
  reserve(sizeof(bionic_tcb), 1);

  // Then reserve the segment itself.
  const size_t result = reserve(exe_segment->size, exe_segment->alignment);

  // The variant 1 ABI that ARM linkers follow specifies a 2-word TCB between
  // the thread pointer and the start of the executable's TLS segment, but both
  // the thread pointer and the TLS segment are aligned appropriately for the
  // TLS segment. Calculate the distance between the thread pointer and the
  // EXE's segment.
  const size_t exe_tpoff = __BIONIC_ALIGN(sizeof(void*) * 2, exe_segment->alignment);

  const size_t min_bionic_alignment = BIONIC_ROUND_UP_POWER_OF_2(MAX_TLS_SLOT) * sizeof(void*);
  if (exe_tpoff < min_bionic_alignment) {
    async_safe_fatal("error: \"%s\": executable's TLS segment is underaligned: "
                     "alignment is %zu, needs to be at least %zu for %s Bionic",
                     progname, exe_segment->alignment, min_bionic_alignment,
                     (sizeof(void*) == 4 ? "ARM" : "ARM64"));
  }

  offset_bionic_tcb_ = result - exe_tpoff - (-MIN_TLS_SLOT * sizeof(void*));
  return result;

#elif defined(__i386__) || defined(__x86_64__)

  // x86 uses variant 2 TLS layout. The executable's segment is located just
  // before the TCB.
  static_assert(MIN_TLS_SLOT == 0, "First slot of bionic_tcb must be slot #0 on x86");
  const size_t exe_size = round_up_with_overflow_check(exe_segment->size, exe_segment->alignment);
  reserve(exe_size, 1);
  const size_t max_align = MAX(alignof(bionic_tcb), exe_segment->alignment);
  offset_bionic_tcb_ = reserve(sizeof(bionic_tcb), max_align);
  return offset_bionic_tcb_ - exe_size;

#else
#error "Unrecognized architecture"
#endif
}

void StaticTlsLayout::reserve_bionic_tls() {
  offset_bionic_tls_ = reserve_type<bionic_tls>();
}

void StaticTlsLayout::finish_layout() {
  // Round the offset up to the alignment.
  offset_ = round_up_with_overflow_check(offset_, alignment_);

  if (overflowed_) {
    async_safe_fatal("error: TLS segments in static TLS overflowed");
  }
}

// The size is not required to be a multiple of the alignment. The alignment
// must be a positive power-of-two.
size_t StaticTlsLayout::reserve(size_t size, size_t alignment) {
  offset_ = round_up_with_overflow_check(offset_, alignment);
  const size_t result = offset_;
  if (__builtin_add_overflow(offset_, size, &offset_)) overflowed_ = true;
  alignment_ = MAX(alignment_, alignment);
  return result;
}

size_t StaticTlsLayout::round_up_with_overflow_check(size_t value, size_t alignment) {
  const size_t old_value = value;
  value = __BIONIC_ALIGN(value, alignment);
  if (value < old_value) overflowed_ = true;
  return value;
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
