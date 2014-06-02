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

size_t c32rtomb(char* s, char32_t c32, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;

  if (s == NULL) {
    // Equivalent to c32rtomb(buf, U'\0', ps).
    return reset_and_return(1, state);
  }

  // POSIX states that if char32_t is a null wide character, a null byte shall
  // be stored, preceded by any shift sequence needed to restore the initial
  // shift state. Since shift states are not supported, only the null byte is
  // stored.
  if (c32 == U'\0') {
    *s = '\0';
    reset_and_return(1, state);
  }

  if (!mbsinit(state)) {
    return reset_and_return_illegal(EILSEQ, state);
  }

  if ((c32 & ~0x7f) == 0) {
    // Fast path for plain ASCII characters.
    *s = c32;
    return 1;
  }

  // Determine the number of octets needed to represent this character.
  // We always output the shortest sequence possible. Also specify the
  // first few bits of the first octet, which contains the information
  // about the sequence length.
  uint8_t lead;
  size_t length;
  if ((c32 & ~0x7f) == 0) {
    lead = 0;
    length = 1;
  } else if ((c32 & ~0x7ff) == 0) {
    lead = 0xc0;
    length = 2;
  } else if ((c32 & ~0xffff) == 0) {
    lead = 0xe0;
    length = 3;
  } else if ((c32 & ~0x1fffff) == 0) {
    lead = 0xf0;
    length = 4;
  } else {
    errno = EILSEQ;
    return __MB_ERR_ILLEGAL_SEQUENCE;
  }

  // Output the octets representing the character in chunks
  // of 6 bits, least significant last. The first octet is
  // a special case because it contains the sequence length
  // information.
  for (size_t i = length - 1; i > 0; i--) {
    s[i] = (c32 & 0x3f) | 0x80;
    c32 >>= 6;
  }
  *s = (c32 & 0xff) | lead;

  return length;
}
