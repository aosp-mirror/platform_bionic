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

#include <stdlib.h>
#include <stdint.h>
#include <sys/reboot.h>
#include <unistd.h>

#include <async_safe/log.h>
#include <private/bionic_globals.h>

// We choose a static pointer tag here for performance reasons. Dynamic tagging
// doesn't improve our detection, and simply hurts performance. This tag is
// deliberately chosen to always point to inaccessible memory on a standard
// 64-bit userspace process, and be easily identifiable by developers. This tag
// is also deliberately different from the standard pattern-init tag (0xAA), as
// to be distinguishable from an uninitialized-pointer access. The first and
// second nibbles are also deliberately designed to be the bitset-mirror of each
// other (0b1011, 0b0100) in order to reduce incidental matches. We also ensure
// that the top bit is set, as this catches incorrect code that assumes that a
// "negative" pointer indicates error. Users must not rely on the
// implementation-defined value of this pointer tag, as it may change.
static constexpr uintptr_t POINTER_TAG = 0xB4;
static constexpr unsigned UNTAG_SHIFT = 40;
static constexpr unsigned CHECK_SHIFT = 48;
static constexpr unsigned TAG_SHIFT = 56;
#if defined(__aarch64__)
static constexpr uintptr_t ADDRESS_MASK = (static_cast<uintptr_t>(1) << TAG_SHIFT) - 1;
static constexpr uintptr_t TAG_MASK = static_cast<uintptr_t>(0xFF) << TAG_SHIFT;

static inline uintptr_t FixedPointerTag() {
  return __libc_globals->heap_pointer_tag & TAG_MASK;
}

static inline uintptr_t PointerCheckMask() {
  return (__libc_globals->heap_pointer_tag << (TAG_SHIFT - CHECK_SHIFT)) & TAG_MASK;
}

static inline uintptr_t PointerUntagMask() {
  return ~(__libc_globals->heap_pointer_tag << (TAG_SHIFT - UNTAG_SHIFT));
}
#endif // defined(__aarch64__)

// Return a forcibly-tagged pointer.
static inline void* TagPointer(void* ptr) {
#if defined(__aarch64__)
  return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) | FixedPointerTag());
#else
  async_safe_fatal("Attempting to tag a pointer (%p) on non-aarch64.", ptr);
#endif
}

#if defined(__aarch64__)
// Return a forcibly-untagged pointer. The pointer tag is not checked for
// validity.
static inline void* UntagPointer(const volatile void* ptr) {
  return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) & ADDRESS_MASK);
}

// Untag the pointer, and check the pointer tag iff the kernel supports tagged pointers and the
// pointer tag isn't being used by HWASAN or MTE. If the tag is incorrect, trap.
static inline void* MaybeUntagAndCheckPointer(const volatile void* ptr) {
  if (__predict_false(ptr == nullptr)) {
    return nullptr;
  }

  uintptr_t ptr_int = reinterpret_cast<uintptr_t>(ptr);

  // Applications may disable pointer tagging, which will be propagated to
  // libc in the zygote. This means that there may already be tagged heap
  // allocations that will fail when checked against the zero-ed heap tag. The
  // check below allows us to turn *off* pointer tagging (by setting PointerCheckMask() and
  // FixedPointerTag() to zero) and still allow tagged heap allocations to be freed.
  if ((ptr_int & PointerCheckMask()) != FixedPointerTag()) {
    // TODO(b/145604058) - Upstream tagged pointers documentation and provide
    // a link to it in the abort message here.
    async_safe_fatal("Pointer tag for %p was truncated.", ptr);
  }
  return reinterpret_cast<void*>(ptr_int & PointerUntagMask());
}

// Return a tagged pointer iff the kernel supports tagged pointers, and `ptr` is
// non-null.
static inline void* MaybeTagPointer(void* ptr) {
  if (__predict_true(ptr != nullptr)) {
    return TagPointer(ptr);
  }
  return ptr;
}

#else  // defined(__aarch64__)
static inline void* UntagPointer(const volatile void* ptr) {
  return const_cast<void*>(ptr);
}

static inline void* MaybeTagPointer(void* ptr) {
  return ptr;
}

static inline void* MaybeUntagAndCheckPointer(const volatile void* ptr) {
  return const_cast<void *>(ptr);
}

#endif  // defined(__aarch64__)
