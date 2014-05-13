/*	$OpenBSD: citrus_utf8.c,v 1.6 2012/12/05 23:19:59 deraadt Exp $ */

/*-
 * Copyright (c) 2002-2004 Tim J. Robbins
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <string.h>
#include <sys/param.h>
#include <wchar.h>

//
// This file is basically OpenBSD's citrus_utf8.c but rewritten to not require a
// 12-byte mbstate_t so we're backwards-compatible with our LP32 ABI where
// mbstate_t was only 4 bytes.
//
// The state is the UTF-8 sequence. We only support <= 4-bytes sequences so LP32
// mbstate_t already has enough space (out of the 4 available bytes we only
// need 3 since we should never need to store the entire sequence in the
// intermediary state).
//
// The C standard leaves the conversion state undefined after a bad conversion.
// To avoid unexpected failures due to the possible use of the internal private
// state we always reset the conversion state when encountering illegal
// sequences.
//
// We also implement the POSIX interface directly rather than being accessed via
// function pointers.
//

#define ERR_ILLEGAL_SEQUENCE static_cast<size_t>(-1)
#define ERR_INCOMPLETE_SEQUENCE static_cast<size_t>(-2)

static size_t mbstate_bytes_so_far(const mbstate_t* ps) {
  return
    (ps->__seq[2] != 0) ? 3 :
    (ps->__seq[1] != 0) ? 2 :
    (ps->__seq[0] != 0) ? 1 : 0;
}

static void mbstate_set_byte(mbstate_t* ps, int i, char byte) {
  ps->__seq[i] = static_cast<uint8_t>(byte);
}

static uint8_t mbstate_get_byte(const mbstate_t* ps, int n) {
  return ps->__seq[n];
}

static size_t reset_and_return_illegal(int _errno, mbstate_t* ps) {
  errno = _errno;
  *(reinterpret_cast<uint32_t*>(ps->__seq)) = 0;
  return ERR_ILLEGAL_SEQUENCE;
}

static size_t reset_and_return(int _return, mbstate_t* ps) {
  *(reinterpret_cast<uint32_t*>(ps->__seq)) = 0;
  return _return;
}


int mbsinit(const mbstate_t* ps) {
  return (ps == NULL || (*(reinterpret_cast<const uint32_t*>(ps->__seq)) == 0));
}

size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;

  // We should never get to a state which has all 4 bytes of the sequence set.
  // Full state verification is done when decoding the sequence (after we have
  // all the bytes).
  if (mbstate_get_byte(state, 3) != 0) {
    return reset_and_return_illegal(EINVAL, state);
  }

  if (s == NULL) {
    s = "";
    n = 1;
    pwc = NULL;
  }

  if (n == 0) {
    return 0;
  }

  uint8_t ch;
  if (mbsinit(state) && (((ch = static_cast<uint8_t>(*s)) & ~0x7f) == 0)) {
    // Fast path for plain ASCII characters.
    if (pwc != NULL) {
      *pwc = ch;
    }
    return (ch != '\0' ? 1 : 0);
  }

  // Determine the number of octets that make up this character
  // from the first octet, and a mask that extracts the
  // interesting bits of the first octet. We already know
  // the character is at least two bytes long.
  size_t length;
  int mask;

  // We also specify a lower bound for the character code to
  // detect redundant, non-"shortest form" encodings. For
  // example, the sequence C0 80 is _not_ a legal representation
  // of the null character. This enforces a 1-to-1 mapping
  // between character codes and their multibyte representations.
  wchar_t lower_bound;

  // The first byte in the state (if any) tells the length.
  size_t bytes_so_far = mbstate_bytes_so_far(state);
  ch = bytes_so_far > 0 ? mbstate_get_byte(state, 0) : static_cast<uint8_t>(*s);
  if ((ch & 0x80) == 0) {
    mask = 0x7f;
    length = 1;
    lower_bound = 0;
  } else if ((ch & 0xe0) == 0xc0) {
    mask = 0x1f;
    length = 2;
    lower_bound = 0x80;
  } else if ((ch & 0xf0) == 0xe0) {
    mask = 0x0f;
    length = 3;
    lower_bound = 0x800;
  } else if ((ch & 0xf8) == 0xf0) {
    mask = 0x07;
    length = 4;
    lower_bound = 0x10000;
  } else {
    // Malformed input; input is not UTF-8. See RFC 3629.
    return reset_and_return_illegal(EILSEQ, state);
  }

  // Fill in the state.
  size_t bytes_wanted = length - bytes_so_far;
  size_t i;
  for (i = 0; i < MIN(bytes_wanted, n); i++) {
    if (!mbsinit(state) && ((*s & 0xc0) != 0x80)) {
      // Malformed input; bad characters in the middle of a character.
      return reset_and_return_illegal(EILSEQ, state);
    }
    mbstate_set_byte(state, bytes_so_far + i, *s++);
  }
  if (i < bytes_wanted) {
    return ERR_INCOMPLETE_SEQUENCE;
  }

  // Decode the octet sequence representing the character in chunks
  // of 6 bits, most significant first.
  wchar_t wch = mbstate_get_byte(state, 0) & mask;
  for (i = 1; i < length; i++) {
    wch <<= 6;
    wch |= mbstate_get_byte(state, i) & 0x3f;
  }

  if (wch < lower_bound) {
    // Malformed input; redundant encoding.
    return reset_and_return_illegal(EILSEQ, state);
  }
  if ((wch >= 0xd800 && wch <= 0xdfff) || wch == 0xfffe || wch == 0xffff) {
    // Malformed input; invalid code points.
    return reset_and_return_illegal(EILSEQ, state);
  }
  if (pwc != NULL) {
    *pwc = wch;
  }
  return reset_and_return(wch == L'\0' ? 0 : bytes_wanted, state);
}

size_t mbsnrtowcs(wchar_t* dst, const char** src, size_t nmc, size_t len, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;
  size_t i, o, r;

  if (dst == NULL) {
    /*
     * The fast path in the loop below is not safe if an ASCII
     * character appears as anything but the first byte of a
     * multibyte sequence. Check now to avoid doing it in the loop.
     */
    if ((nmc > 0) && (mbstate_bytes_so_far(state) > 0)
        && (static_cast<uint8_t>((*src)[0]) < 0x80)) {
      return reset_and_return_illegal(EILSEQ, state);
    }
    for (i = o = 0; i < nmc; i += r, o++) {
      if (static_cast<uint8_t>((*src)[i]) < 0x80) {
        // Fast path for plain ASCII characters.
        if ((*src)[i] == '\0') {
          return reset_and_return(o, state);
        }
        r = 1;
      } else {
        r = mbrtowc(NULL, *src + i, nmc - i, state);
        if (r == ERR_ILLEGAL_SEQUENCE) {
          return reset_and_return_illegal(EILSEQ, state);
        }
        if (r == ERR_INCOMPLETE_SEQUENCE) {
          return reset_and_return_illegal(EILSEQ, state);
        }
        if (r == 0) {
          return reset_and_return(o, state);
        }
      }
    }
    return reset_and_return(o, state);
  }

  /*
   * The fast path in the loop below is not safe if an ASCII
   * character appears as anything but the first byte of a
   * multibyte sequence. Check now to avoid doing it in the loop.
   */
  if ((nmc > 0) && (mbstate_bytes_so_far(state) > 0)
      && (static_cast<uint8_t>((*src)[0]) < 0x80)) {
    return reset_and_return_illegal(EILSEQ, state);
  }
  for (i = o = 0; i < nmc && o < len; i += r, o++) {
    if (static_cast<uint8_t>((*src)[i]) < 0x80) {
      // Fast path for plain ASCII characters.
      dst[o] = (*src)[i];
      if ((*src)[i] == '\0') {
        *src = NULL;
        return reset_and_return_illegal(EILSEQ, state);
      }
      r = 1;
    } else {
      r = mbrtowc(dst + o, *src + i, nmc - i, state);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return reset_and_return_illegal(EILSEQ, state);
      }
      if (r == ERR_INCOMPLETE_SEQUENCE) {
        *src += nmc;
        return reset_and_return(EILSEQ, state);
      }
      if (r == 0) {
        *src = NULL;
        return reset_and_return(o, state);
      }
    }
  }
  *src += i;
  return reset_and_return(o, state);
}

size_t mbsrtowcs(wchar_t* dst, const char** src, size_t len, mbstate_t* ps) {
  return mbsnrtowcs(dst, src, SIZE_MAX, len, ps);
}

size_t wcrtomb(char* s, wchar_t wc, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;

  if (s == NULL) {
    // Equivalent to wcrtomb(buf, L'\0', ps).
    return reset_and_return(1, state);
  }

  // POSIX states that if wc is a null wide character, a null byte shall be
  // stored, preceded by any shift sequence needed to restore the initial shift
  // state. Since shift states are not supported, only the null byte is stored.
  if (wc == L'\0') {
    *s = '\0';
    reset_and_return(1, state);
  }

  if (!mbsinit(state)) {
    return reset_and_return_illegal(EILSEQ, state);
  }

  if ((wc & ~0x7f) == 0) {
    // Fast path for plain ASCII characters.
    *s = wc;
    return 1;
  }

  // Determine the number of octets needed to represent this character.
  // We always output the shortest sequence possible. Also specify the
  // first few bits of the first octet, which contains the information
  // about the sequence length.
  uint8_t lead;
  size_t length;
  if ((wc & ~0x7f) == 0) {
    lead = 0;
    length = 1;
  } else if ((wc & ~0x7ff) == 0) {
    lead = 0xc0;
    length = 2;
  } else if ((wc & ~0xffff) == 0) {
    lead = 0xe0;
    length = 3;
  } else if ((wc & ~0x1fffff) == 0) {
    lead = 0xf0;
    length = 4;
  } else {
    errno = EILSEQ;
    return ERR_ILLEGAL_SEQUENCE;
  }

  // Output the octets representing the character in chunks
  // of 6 bits, least significant last. The first octet is
  // a special case because it contains the sequence length
  // information.
  for (size_t i = length - 1; i > 0; i--) {
    s[i] = (wc & 0x3f) | 0x80;
    wc >>= 6;
  }
  *s = (wc & 0xff) | lead;

  return length;
}

size_t wcsnrtombs(char* dst, const wchar_t** src, size_t nwc, size_t len, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;

  if (!mbsinit(state)) {
    return reset_and_return_illegal(EILSEQ, state);
  }

  char buf[MB_LEN_MAX];
  size_t i, o, r;
  if (dst == NULL) {
    for (i = o = 0; i < nwc; i++, o += r) {
      wchar_t wc = (*src)[i];
      if (static_cast<uint32_t>(wc) < 0x80) {
        // Fast path for plain ASCII characters.
        if (wc == 0) {
          return o;
        }
        r = 1;
      } else {
        r = wcrtomb(buf, wc, state);
        if (r == ERR_ILLEGAL_SEQUENCE) {
          return r;
        }
      }
    }
    return o;
  }

  for (i = o = 0; i < nwc && o < len; i++, o += r) {
    wchar_t wc = (*src)[i];
    if (static_cast<uint32_t>(wc) < 0x80) {
      // Fast path for plain ASCII characters.
      dst[o] = wc;
      if (wc == 0) {
        *src = NULL;
        return o;
      }
      r = 1;
    } else if (len - o >= sizeof(buf)) {
      // Enough space to translate in-place.
      r = wcrtomb(dst + o, wc, state);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
    } else {
      // May not be enough space; use temp buffer.
      r = wcrtomb(buf, wc, state);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
      if (r > len - o) {
        break;
      }
      memcpy(dst + o, buf, r);
    }
  }
  *src += i;
  return o;
}

size_t wcsrtombs(char* dst, const wchar_t** src, size_t len, mbstate_t* ps) {
  return wcsnrtombs(dst, src, SIZE_MAX, len, ps);
}
