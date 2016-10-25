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
#include <sys/param.h>
#include <string.h>
#include <wchar.h>
#include <uchar.h>

#include "private/bionic_mbstate.h"

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

int mbsinit(const mbstate_t* ps) {
  return (ps == NULL || (*(reinterpret_cast<const uint32_t*>(ps->__seq)) == 0));
}

size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;

  // Our wchar_t is UTF-32.
  return mbrtoc32(reinterpret_cast<char32_t*>(pwc), s, n, state);
}

size_t mbsnrtowcs(wchar_t* dst, const char** src, size_t nmc, size_t len, mbstate_t* ps) {
  static mbstate_t __private_state;
  mbstate_t* state = (ps == NULL) ? &__private_state : ps;
  size_t i, o, r;

  // The fast paths in the loops below are not safe if an ASCII
  // character appears as anything but the first byte of a
  // multibyte sequence. Check now to avoid doing it in the loops.
  if (nmc > 0 && mbstate_bytes_so_far(state) > 0 && static_cast<uint8_t>((*src)[0]) < 0x80) {
    return reset_and_return_illegal(EILSEQ, state);
  }

  // Measure only?
  if (dst == NULL) {
    for (i = o = 0; i < nmc; i += r, o++) {
      if (static_cast<uint8_t>((*src)[i]) < 0x80) {
        // Fast path for plain ASCII characters.
        if ((*src)[i] == '\0') {
          return reset_and_return(o, state);
        }
        r = 1;
      } else {
        r = mbrtowc(NULL, *src + i, nmc - i, state);
        if (r == __MB_ERR_ILLEGAL_SEQUENCE) {
          return reset_and_return_illegal(EILSEQ, state);
        }
        if (r == __MB_ERR_INCOMPLETE_SEQUENCE) {
          return reset_and_return_illegal(EILSEQ, state);
        }
        if (r == 0) {
          return reset_and_return(o, state);
        }
      }
    }
    return reset_and_return(o, state);
  }

  // Actually convert, updating `dst` and `src`.
  for (i = o = 0; i < nmc && o < len; i += r, o++) {
    if (static_cast<uint8_t>((*src)[i]) < 0x80) {
      // Fast path for plain ASCII characters.
      dst[o] = (*src)[i];
      r = 1;
      if ((*src)[i] == '\0') {
        *src = nullptr;
        return reset_and_return(o, state);
      }
    } else {
      r = mbrtowc(dst + o, *src + i, nmc - i, state);
      if (r == __MB_ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return reset_and_return_illegal(EILSEQ, state);
      }
      if (r == __MB_ERR_INCOMPLETE_SEQUENCE) {
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

  // Our wchar_t is UTF-32.
  return c32rtomb(s, static_cast<char32_t>(wc), state);
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
        if (r == __MB_ERR_ILLEGAL_SEQUENCE) {
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
      if (r == __MB_ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
    } else {
      // May not be enough space; use temp buffer.
      r = wcrtomb(buf, wc, state);
      if (r == __MB_ERR_ILLEGAL_SEQUENCE) {
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

int wcscoll_l(const wchar_t *ws1, const wchar_t *ws2, locale_t) {
  return wcscoll(ws1, ws2);
}

size_t wcsxfrm_l(wchar_t *dest, const wchar_t *src, size_t n, locale_t) {
  return wcsxfrm(dest, src, n);
}

long long wcstoll_l(const wchar_t *nptr, wchar_t **endptr, int base,
                    locale_t) {
  return wcstoll(nptr, endptr, base);
}

unsigned long long wcstoull_l(const wchar_t *nptr, wchar_t **endptr,
                              int base, locale_t) {
  return wcstoull(nptr, endptr, base);
}

long double wcstold_l(const wchar_t *nptr, wchar_t **endptr, locale_t) {
  return wcstold(nptr, endptr);
}
