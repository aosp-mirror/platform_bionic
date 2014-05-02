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
// This file is basically OpenBSD's citrus_utf8.c but rewritten to not require a 12-byte mbstate_t
// so we're backwards-compatible with our LP32 ABI where mbstate_t was only 4 bytes. An additional
// advantage of this is that callers who don't supply their own mbstate_t won't be accessing shared
// state.
//
// We also implement the POSIX interface directly rather than being accessed via function pointers.
//

#define ERR_ILLEGAL_SEQUENCE static_cast<size_t>(-1)
#define ERR_INCOMPLETE_SEQUENCE static_cast<size_t>(-2)

int mbsinit(const mbstate_t*) {
  // We have no state, so we're always in the initial state.
  return 1;
}

size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t*) {
  if (s == NULL) {
    s = "";
    n = 1;
    pwc = NULL;
  }

  if (n == 0) {
    return 0;
  }

  int ch;
  if (((ch = static_cast<uint8_t>(*s)) & ~0x7f) == 0) {
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

  ch = static_cast<uint8_t>(*s);
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
    errno = EILSEQ;
    return ERR_ILLEGAL_SEQUENCE;
  }

  // Decode the octet sequence representing the character in chunks
  // of 6 bits, most significant first.
  wchar_t wch = static_cast<uint8_t>(*s++) & mask;
  size_t i;
  for (i = 1; i < MIN(length, n); i++) {
    if ((*s & 0xc0) != 0x80) {
      // Malformed input; bad characters in the middle of a character.
      errno = EILSEQ;
      return ERR_ILLEGAL_SEQUENCE;
    }
    wch <<= 6;
    wch |= *s++ & 0x3f;
  }
  if (i < length) {
    return ERR_INCOMPLETE_SEQUENCE;
  }
  if (wch < lower_bound) {
    // Malformed input; redundant encoding.
    errno = EILSEQ;
    return ERR_ILLEGAL_SEQUENCE;
  }
  if ((wch >= 0xd800 && wch <= 0xdfff) || wch == 0xfffe || wch == 0xffff) {
    // Malformed input; invalid code points.
    errno = EILSEQ;
    return ERR_ILLEGAL_SEQUENCE;
  }
  if (pwc != NULL) {
    *pwc = wch;
  }
  return (wch == L'\0' ? 0 : length);
}

size_t mbsnrtowcs(wchar_t* dst, const char** src, size_t nmc, size_t len, mbstate_t* ps) {
  size_t i, o, r;

  if (dst == NULL) {
    for (i = o = 0; i < nmc; i += r, o++) {
      if (static_cast<uint8_t>((*src)[i]) < 0x80) {
        // Fast path for plain ASCII characters.
        if ((*src)[i] == '\0') {
          return o;
        }
        r = 1;
      } else {
        r = mbrtowc(NULL, *src + i, nmc - i, ps);
        if (r == ERR_ILLEGAL_SEQUENCE) {
          return r;
        }
        if (r == ERR_INCOMPLETE_SEQUENCE) {
          return o;
        }
        if (r == 0) {
          return o;
        }
      }
    }
    return o;
  }

  for (i = o = 0; i < nmc && o < len; i += r, o++) {
    if (static_cast<uint8_t>((*src)[i]) < 0x80) {
      // Fast path for plain ASCII characters.
      dst[o] = (*src)[i];
      if ((*src)[i] == '\0') {
        *src = NULL;
        return o;
      }
      r = 1;
    } else {
      r = mbrtowc(dst + o, *src + i, nmc - i, ps);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
      if (r == ERR_INCOMPLETE_SEQUENCE) {
        *src += nmc;
        return o;
      }
      if (r == 0) {
        *src = NULL;
        return o;
      }
    }
  }
  *src += i;
  return o;
}

size_t mbsrtowcs(wchar_t* dst, const char** src, size_t len, mbstate_t* ps) {
  return mbsnrtowcs(dst, src, SIZE_MAX, len, ps);
}

size_t wcrtomb(char* s, wchar_t wc, mbstate_t*) {
  if (s == NULL) {
    // Reset to initial shift state (no-op).
    return 1;
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
        r = wcrtomb(buf, wc, ps);
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
      r = wcrtomb(dst + o, wc, ps);
      if (r == ERR_ILLEGAL_SEQUENCE) {
        *src += i;
        return r;
      }
    } else {
      // May not be enough space; use temp buffer.
      r = wcrtomb(buf, wc, ps);
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
