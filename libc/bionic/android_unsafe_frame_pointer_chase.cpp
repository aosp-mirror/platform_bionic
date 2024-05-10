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

#include "platform/bionic/android_unsafe_frame_pointer_chase.h"

#include "platform/bionic/mte.h"
#include "platform/bionic/pac.h"
#include "private/bionic_defs.h"
#include "pthread_internal.h"

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" __LIBC_HIDDEN__ uintptr_t __get_thread_stack_top() {
  return __get_thread()->stack_top;
}

/*
 * Implement fast stack unwinding for stack frames with frame pointers. Stores at most num_entries
 * return addresses to buffer buf. Returns the number of available return addresses, which may be
 * greater than num_entries.
 *
 * This function makes no guarantees about its behavior on encountering a frame built without frame
 * pointers, except that it should not crash or enter an infinite loop, and that any frames prior to
 * the frame built without frame pointers should be correct.
 *
 * This function is only meant to be used with memory safety tools such as sanitizers which need to
 * take stack traces efficiently. Normal applications should use APIs such as libunwindstack or
 * _Unwind_Backtrace.
 */
__attribute__((no_sanitize("address", "hwaddress"))) size_t android_unsafe_frame_pointer_chase(
    uintptr_t* buf, size_t num_entries) {
  // Disable MTE checks for the duration of this function, since we can't be sure that following
  // next_frame pointers won't cause us to read from tagged memory. ASAN/HWASAN are disabled here
  // for the same reason.
  ScopedDisableMTE x;

  struct frame_record {
    uintptr_t next_frame, return_addr;
  };

  auto begin = reinterpret_cast<uintptr_t>(__builtin_frame_address(0));
  auto end = __get_thread_stack_top();

  stack_t ss;
  if (sigaltstack(nullptr, &ss) == 0 && (ss.ss_flags & SS_ONSTACK)) {
    end = reinterpret_cast<uintptr_t>(ss.ss_sp) + ss.ss_size;
  }

  size_t num_frames = 0;
  while (1) {
#if defined(__riscv)
    // Frame addresses seem to have been implemented incorrectly for RISC-V.
    // See https://reviews.llvm.org/D87579. We did at least manage to get this
    // documented in the RISC-V psABI though:
    // https://github.com/riscv-non-isa/riscv-elf-psabi-doc/blob/master/riscv-cc.adoc#frame-pointer-convention
    auto* frame = reinterpret_cast<frame_record*>(begin - 16);
#else
    auto* frame = reinterpret_cast<frame_record*>(begin);
#endif
    if (num_frames < num_entries) {
      uintptr_t addr = __bionic_clear_pac_bits(frame->return_addr);
      if (addr == 0) {
        break;
      }
      buf[num_frames] = addr;
    }
    ++num_frames;
    if (frame->next_frame < begin + sizeof(frame_record) || frame->next_frame >= end ||
        frame->next_frame % sizeof(void*) != 0) {
      break;
    }
    begin = frame->next_frame;
  }

  return num_frames;
}
