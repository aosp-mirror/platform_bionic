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
#include <stdint.h>
#include <sys/cdefs.h>

struct TlsSegment {
  size_t size = 0;
  size_t alignment = 1;
  const void* init_ptr = "";    // Field is non-null even when init_size is 0.
  size_t init_size = 0;
};

__LIBC_HIDDEN__ bool __bionic_get_tls_segment(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                              ElfW(Addr) load_bias, const char* mod_name,
                                              TlsSegment* out);

struct StaticTlsLayout {
  constexpr StaticTlsLayout() {}

private:
  size_t offset_ = 0;
  size_t alignment_ = 1;
  bool overflowed_ = false;

  // Offsets to various Bionic TLS structs from the beginning of static TLS.
  size_t offset_bionic_tcb_ = SIZE_MAX;
  size_t offset_bionic_tls_ = SIZE_MAX;

public:
  size_t offset_bionic_tcb() const { return offset_bionic_tcb_; }
  size_t offset_bionic_tls() const { return offset_bionic_tls_; }

  size_t size() const { return offset_; }
  size_t alignment() const { return alignment_; }
  bool overflowed() const { return overflowed_; }

  size_t reserve_exe_segment_and_tcb(const TlsSegment* exe_segment, const char* progname);
  void reserve_bionic_tls();
  size_t reserve_solib_segment(const TlsSegment& segment) {
    return reserve(segment.size, segment.alignment);
  }
  void finish_layout();

private:
  size_t reserve(size_t size, size_t alignment);

  template <typename T> size_t reserve_type() {
    return reserve(sizeof(T), alignof(T));
  }

  size_t round_up_with_overflow_check(size_t value, size_t alignment);
};
