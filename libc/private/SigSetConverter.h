/*
 * Copyright (C) 2018 The Android Open Source Project
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

// Android's 32-bit ABI shipped with a sigset_t too small to include any
// of the realtime signals, so we have both sigset_t and sigset64_t. Many
// new system calls only accept a sigset64_t, so this helps paper over
// the difference at zero cost to LP64 in most cases after the optimizer
// removes the unnecessary temporary `ptr`.
struct SigSetConverter {
 public:
  SigSetConverter(const sigset_t* s) : SigSetConverter(const_cast<sigset_t*>(s)) {}

  SigSetConverter(sigset_t* s) {
#if defined(__LP64__)
    // sigset_t == sigset64_t on LP64.
    ptr = s;
#else
    sigset64 = {};
    if (s != nullptr) {
      original_ptr = s;
      sigset = *s;
      ptr = &sigset64;
    } else {
      ptr = nullptr;
    }
#endif
  }

  void copy_out() {
#if defined(__LP64__)
    // We used the original pointer directly, so no copy needed.
#else
    *original_ptr = sigset;
#endif
  }

  sigset64_t* ptr;

 private:
  [[maybe_unused]] sigset_t* original_ptr;
  union {
    sigset_t sigset;
    sigset64_t sigset64;
  };
};
