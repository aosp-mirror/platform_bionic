/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <assert.h>
#include <errno.h>
#include <uchar.h>
#include <wchar.h>

#include "private/bionic_mbstate.h"

static inline bool mbspartialc16(const mbstate_t* state) {
  return mbstate_get_byte(state, 3) != 0;
}

static size_t begin_surrogate(char32_t c32, char16_t* pc16,
                              size_t nconv, mbstate_t* state) {
  c32 -= 0x10000;
  char16_t trail = (c32 & 0x3ff) | 0xdc00;

  mbstate_set_byte(state, 0, trail & 0x00ff);
  mbstate_set_byte(state, 1, (trail & 0xff00) >> 8);
  mbstate_set_byte(state, 3, nconv & 0xff);

  *pc16 = ((c32 & 0xffc00) >> 10) | 0xd800;
  // https://issuetracker.google.com/289419882
  //
  // We misread the spec when implementing this. The first call should return
  // the length of the decoded character, and the second call should return -3
  // to indicate that the output is a continuation of the character decoded by
  // the first call.
  //
  // C23 7.30.1.3.4:
  //
  //     between 1 and n inclusive if the next n or fewer bytes complete a valid
  //     multibyte character (which is the value stored); the value returned is
  //     the number of bytes that complete the multibyte character.
  //
  //     (size_t)(-3) if the next character resulting from a previous call has
  //     been stored (no bytes from the input have been consumed by this call).
  //
  // The first call returns the number of bytes consumed, and the second call
  // returns -3.
  //
  // All UTF-8 sequences that encode a surrogate pair are 4 bytes, but we may
  // not have seen the full sequence yet.
  return nconv;
}

static size_t finish_surrogate(char16_t* pc16, mbstate_t* state) {
  char16_t trail = mbstate_get_byte(state, 1) << 8 |
                   mbstate_get_byte(state, 0);
  *pc16 = trail;
  mbstate_reset(state);
  return static_cast<size_t>(-3);
}

size_t mbrtoc16(char16_t* pc16, const char* s, size_t n, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == nullptr) ? &__private_state : ps;

  char16_t __private_pc16;
  if (pc16 == nullptr) {
    pc16 = &__private_pc16;
  }

  if (mbspartialc16(state)) {
    return finish_surrogate(pc16, state);
  }

  char32_t c32;
  size_t nconv = mbrtoc32(&c32, s, n, state);
  if (__MB_IS_ERR(nconv)) {
    return nconv;
  } else if (nconv == 0) {
    return mbstate_reset_and_return(nconv, state);
  } else if (c32 < 0x10000) {
    *pc16 = static_cast<char16_t>(c32);
    return mbstate_reset_and_return(nconv, state);
  } else if (c32 > 0x10ffff) {
    // This case is currently handled by mbrtoc32() returning an error, but
    // if that function is extended to cover 5-byte sequences (which are
    // illegal at the moment), we'd need to explicitly handle the case of
    // codepoints that can't be represented as a surrogate pair here.
    return mbstate_reset_and_return_illegal(EILSEQ, state);
  } else {
    return begin_surrogate(c32, pc16, nconv, state);
  }
}
