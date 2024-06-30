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

#pragma once

#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#include "page.h"

// Note: Most PR_MTE_* constants come from the upstream kernel. This tag mask
// allows for the hardware to provision any nonzero tag. Zero tags are reserved
// for scudo to use for the chunk headers in order to prevent linear heap
// overflow/underflow.
#define PR_MTE_TAG_SET_NONZERO (0xfffeUL << PR_MTE_TAG_SHIFT)

inline bool mte_supported() {
#if defined(__aarch64__)
  static bool supported = getauxval(AT_HWCAP2) & HWCAP2_MTE;
#else
  static bool supported = false;
#endif
  return supported;
}

#ifdef __aarch64__
class ScopedDisableMTE {
  size_t prev_tco_;

 public:
  ScopedDisableMTE() {
    if (mte_supported()) {
      __asm__ __volatile__(".arch_extension mte; mrs %0, tco; msr tco, #1" : "=r"(prev_tco_));
    }
  }

  ~ScopedDisableMTE() {
    if (mte_supported()) {
      __asm__ __volatile__(".arch_extension mte; msr tco, %0" : : "r"(prev_tco_));
    }
  }
};

// N.B. that this is NOT the pagesize, but 4096. This is hardcoded in the codegen.
// See
// https://github.com/search?q=repo%3Allvm/llvm-project%20AArch64StackTagging%3A%3AinsertBaseTaggedPointer&type=code
constexpr size_t kStackMteRingbufferSizeMultiplier = 4096;

inline size_t stack_mte_ringbuffer_size(uintptr_t size_cls) {
  return kStackMteRingbufferSizeMultiplier * (1 << size_cls);
}

inline size_t stack_mte_ringbuffer_size_from_pointer(uintptr_t ptr) {
  // The size in the top byte is not the size_cls, but the number of "pages" (not OS pages, but
  // kStackMteRingbufferSizeMultiplier).
  return kStackMteRingbufferSizeMultiplier * (ptr >> 56ULL);
}

inline uintptr_t stack_mte_ringbuffer_size_add_to_pointer(uintptr_t ptr, uintptr_t size_cls) {
  return ptr | ((1ULL << size_cls) << 56ULL);
}

inline void* stack_mte_ringbuffer_allocate(size_t n, const char* name) {
  if (n > 7) return nullptr;
  // Allocation needs to be aligned to 2*size to make the fancy code-gen work.
  // So we allocate 3*size - pagesz bytes, which will always contain size bytes
  // aligned to 2*size, and unmap the unneeded part.
  // See
  // https://github.com/search?q=repo%3Allvm/llvm-project%20AArch64StackTagging%3A%3AinsertBaseTaggedPointer&type=code
  //
  // In the worst case, we get an allocation that is one page past the properly
  // aligned address, in which case we have to unmap the previous
  // 2*size - pagesz bytes. In that case, we still have size properly aligned
  // bytes left.
  size_t size = stack_mte_ringbuffer_size(n);
  size_t pgsize = page_size();

  size_t alloc_size = __BIONIC_ALIGN(3 * size - pgsize, pgsize);
  void* allocation_ptr =
      mmap(nullptr, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (allocation_ptr == MAP_FAILED)
    return nullptr;
  uintptr_t allocation = reinterpret_cast<uintptr_t>(allocation_ptr);

  size_t alignment = 2 * size;
  uintptr_t aligned_allocation = __BIONIC_ALIGN(allocation, alignment);
  if (allocation != aligned_allocation) {
    munmap(reinterpret_cast<void*>(allocation), aligned_allocation - allocation);
  }
  if (aligned_allocation + size != allocation + alloc_size) {
    munmap(reinterpret_cast<void*>(aligned_allocation + size),
           (allocation + alloc_size) - (aligned_allocation + size));
  }

  if (name) {
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, reinterpret_cast<void*>(aligned_allocation), size, name);
  }

  // We store the size in the top byte of the pointer (which is ignored)
  return reinterpret_cast<void*>(stack_mte_ringbuffer_size_add_to_pointer(aligned_allocation, n));
}
#else
struct ScopedDisableMTE {
  // Silence unused variable warnings in non-aarch64 builds.
  ScopedDisableMTE() {}
};
#endif
