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

#include <errno.h>
#include <uchar.h>
#include <wchar.h>

#include "private/bionic_mbstate.h"

static inline constexpr bool is_high_surrogate(char16_t c16) {
  return c16 >= 0xd800 && c16 < 0xdc00;
}

static inline constexpr bool is_low_surrogate(char16_t c16) {
  return c16 >= 0xdc00 && c16 < 0xe000;
}

size_t c16rtomb(char* s, char16_t c16, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == nullptr) ? &__private_state : ps;
  if (mbsinit(state)) {
    if (is_high_surrogate(c16)) {
      char32_t c32 = (c16 & ~0xd800) << 10;
      mbstate_set_byte(state, 3, (c32 & 0xff0000) >> 16);
      mbstate_set_byte(state, 2, (c32 & 0x00ff00) >> 8);
      return 0;
    } else if (is_low_surrogate(c16)) {
      return mbstate_reset_and_return_illegal(EINVAL, state);
    } else {
      return c32rtomb(s, static_cast<char32_t>(c16), state);
    }
  } else {
    if (!is_low_surrogate(c16)) {
      return mbstate_reset_and_return_illegal(EINVAL, state);
    }

    char32_t c32 = ((mbstate_get_byte(state, 3) << 16) |
                    (mbstate_get_byte(state, 2) << 8) |
                    (c16 & ~0xdc00)) + 0x10000;
    return mbstate_reset_and_return(c32rtomb(s, c32, nullptr), state);
  }
}
