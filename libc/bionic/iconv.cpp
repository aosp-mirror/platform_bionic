/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <iconv.h>

#include <ctype.h>
#include <endian.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

#include "private/bionic_mbstate.h"

#define INVALID_ICONV_T reinterpret_cast<iconv_t>(-1)

// Ideally we'd use icu4c but the API mismatch seems too great. So we just offer something
// equivalent to (but slightly easier to use for runs of text than) <uchar.h>. If you're
// here to add more encodings, consider working on finishing the icu4c NDK wrappers instead.
enum Encoding {
  US_ASCII,
  UTF_8,
  UTF_16_LE,
  UTF_16_BE,
  UTF_32_LE,
  UTF_32_BE,
  WCHAR_T,
};

enum Mode {
  ERROR,
  IGNORE,
  TRANSLIT,
};

// This matching is strange but true.
// See http://www.unicode.org/reports/tr22/#Charset_Alias_Matching.
static bool __match_encoding(const char* lhs, const char* rhs) {
  while (*lhs && *rhs) {
    // Skip non-alnum in lhs; "UTF-8", "UTF_8", "UTF8", "UTF 8" are all equivalent.
    // Also implement the "delete each 0 that is not preceded by a digit" rule.
    for (; *lhs; ++lhs) {
      if (isalnum(*lhs) && (*lhs != '0' || !isdigit(*(lhs + 1)))) break;
    }
    // Case doesn't matter either.
    if (tolower(*lhs) != tolower(*rhs)) break;
    ++lhs;
    ++rhs;
  }
  // As a special case we treat the GNU "//" extensions as end of string.
  if ((*lhs == '\0' || strstr(lhs, "//") == lhs) && *rhs == '\0') return true;
  return false;
}

static bool __parse_encoding(const char* s, Encoding* encoding, Mode* mode) {
  const char* suffix = strstr(s, "//");
  if (suffix) {
    if (!mode) return false;
    if (strcmp(suffix, "//IGNORE") == 0) {
      *mode = IGNORE;
    } else if (strcmp(suffix, "//TRANSLIT") == 0) {
      *mode = TRANSLIT;
    } else {
      return false;
    }
  }
  if (__match_encoding(s, "utf8")) {
    *encoding = UTF_8;
  } else if (__match_encoding(s, "ascii") || __match_encoding(s, "usascii")) {
    *encoding = US_ASCII;
  } else if (__match_encoding(s, "utf16le")) {
    *encoding = UTF_16_LE;
  } else if (__match_encoding(s, "utf16be")) {
    *encoding = UTF_16_BE;
  } else if (__match_encoding(s, "utf32le")) {
    *encoding = UTF_32_LE;
  } else if (__match_encoding(s, "utf32be")) {
    *encoding = UTF_32_BE;
  } else if (__match_encoding(s, "wchart")) {
    *encoding = WCHAR_T;
  } else {
    return false;
  }
  return true;
}

struct __iconv_t {
  Encoding src_encoding;
  Encoding dst_encoding;
  Mode mode;

  __iconv_t() : mode(ERROR) {
  }

  int Convert(char** src_buf0, size_t* src_bytes_left0, char** dst_buf0, size_t* dst_bytes_left0) {
    // Reset state.
    wc = 0;
    memset(&ps, 0, sizeof(ps));
    replacement_count = 0;
    ignored = false;
    src_buf = src_buf0;
    src_bytes_left = src_bytes_left0;
    dst_buf = dst_buf0;
    dst_bytes_left = dst_bytes_left0;

    while (*src_bytes_left > 0) {
      if (!GetNext() || !Convert()) return -1;
    }
    return Done();
  }

 private:
  char32_t wc;
  char buf[16];
  size_t src_bytes_used;
  size_t dst_bytes_used;
  mbstate_t ps;

  size_t replacement_count;
  bool ignored;

  char** src_buf;
  size_t* src_bytes_left;
  char** dst_buf;
  size_t* dst_bytes_left;

  bool GetNext() {
    errno = 0;
    switch (src_encoding) {
      case US_ASCII:
        wc = **src_buf;
        src_bytes_used = 1;
        if (wc > 0x7f) errno = EILSEQ;
        break;

      case UTF_8:
        src_bytes_used = mbrtoc32(&wc, *src_buf, *src_bytes_left, &ps);
        if (src_bytes_used == __MB_ERR_ILLEGAL_SEQUENCE) {
          break;  // EILSEQ already set.
        } else if (src_bytes_used == __MB_ERR_INCOMPLETE_SEQUENCE) {
          errno = EINVAL;
          return false;
        }
        break;

      case UTF_16_BE:
      case UTF_16_LE: {
        if (*src_bytes_left < 2) {
          errno = EINVAL;
          return false;
        }
        bool swap = (src_encoding == UTF_16_BE);
        wc = In16(*src_buf, swap);
        // 0xd800-0xdbff: high surrogates
        // 0xdc00-0xdfff: low surrogates
        if (wc >= 0xd800 && wc <= 0xdfff) {
          if (wc >= 0xdc00) {  // Low surrogate before high surrogate.
            errno = EILSEQ;
            return false;
          }
          if (*src_bytes_left < 4) {
            errno = EINVAL;
            return false;
          }
          uint16_t hi = wc;
          uint16_t lo = In16(*src_buf + 2, swap);
          wc = 0x10000 + ((hi - 0xd800) << 10) + (lo - 0xdc00);
          src_bytes_used = 4;
        }
        break;
      }

      case UTF_32_BE:
      case UTF_32_LE:
      case WCHAR_T:
        if (*src_bytes_left < 4) {
          errno = EINVAL;
          return false;
        }
        wc = In32(*src_buf, (src_encoding == UTF_32_BE));
        break;
    }

    if (errno == EILSEQ) {
      switch (mode) {
        case ERROR:
          return false;
        case IGNORE:
          *src_buf += src_bytes_used;
          *src_bytes_left -= src_bytes_used;
          ignored = true;
          return GetNext();
        case TRANSLIT:
          wc = '?';
          ++replacement_count;
          return true;
      }
    }
    return true;
  }

  bool Convert() {
    errno = 0;
    switch (dst_encoding) {
      case US_ASCII:
        buf[0] = wc;
        dst_bytes_used = 1;
        if (wc > 0x7f) errno = EILSEQ;
        break;

      case UTF_8:
        dst_bytes_used = c32rtomb(buf, wc, &ps);
        if (dst_bytes_used == __MB_ERR_ILLEGAL_SEQUENCE) {
          break;  // EILSEQ already set.
        } else if (dst_bytes_used == __MB_ERR_INCOMPLETE_SEQUENCE) {
          errno = EINVAL;
          return false;
        }
        break;

      case UTF_16_BE:
      case UTF_16_LE: {
        bool swap = (dst_encoding == UTF_16_BE);
        if (wc < 0x10000) {  // BMP.
          Out16(buf, wc, swap);
        } else {  // Supplementary plane; output surrogate pair.
          wc -= 0x10000;
          char16_t hi = 0xd800 | (wc >> 10);
          char16_t lo = 0xdc00 | (wc & 0x3ff);
          Out16(buf + 0, hi, swap);
          Out16(buf + 2, lo, swap);
          dst_bytes_used = 4;
        }
      } break;

      case UTF_32_BE:
      case UTF_32_LE:
      case WCHAR_T:
        Out32(wc, (dst_encoding == UTF_32_BE));
        break;
    }

    if (errno == EILSEQ) {
      if (mode == IGNORE) {
        *src_buf += src_bytes_used;
        *src_bytes_left -= src_bytes_used;
        ignored = true;
        return true;
      } else if (mode == TRANSLIT) {
        wc = '?';
        ++replacement_count;
        return Convert();
      }
      return false;
    }

    return Emit();
  }

  uint16_t In16(const char* buf, bool swap) {
    const uint8_t* src = reinterpret_cast<const uint8_t*>(buf);
    uint16_t wc = (src[0]) | (src[1] << 8);
    if (swap) wc = __swap16(wc);
    src_bytes_used = 2;
    return wc;
  }

  uint32_t In32(const char* buf, bool swap) {
    const uint8_t* src = reinterpret_cast<const uint8_t*>(buf);
    uint32_t wc = (src[0]) | (src[1] << 8) | (src[2] << 16) | (src[3] << 24);
    if (swap) wc = __swap32(wc);
    src_bytes_used = 4;
    return wc;
  }

  void Out16(char* dst, char16_t ch, bool swap) {
    if (swap) ch = __swap16(ch);
    dst[0] = ch;
    dst[1] = ch >> 8;
    dst_bytes_used = 2;
  }

  void Out32(char32_t ch, bool swap) {
    if (swap) ch = __swap32(ch);
    buf[0] = ch;
    buf[1] = ch >> 8;
    buf[2] = ch >> 16;
    buf[3] = ch >> 24;
    dst_bytes_used = 4;
  }

  bool Emit() {
    if (dst_bytes_used > *dst_bytes_left) {
      errno = E2BIG;
      return false;
    }

    memcpy(*dst_buf, buf, dst_bytes_used);
    *src_buf += src_bytes_used;
    *src_bytes_left -= src_bytes_used;
    *dst_buf += dst_bytes_used;
    *dst_bytes_left -= dst_bytes_used;
    return true;
  }

  int Done() {
    if (mode == TRANSLIT) return replacement_count;
    if (ignored) {
      errno = EILSEQ;
      return -1;
    }
    return 0;
  }
};

iconv_t iconv_open(const char* __dst_encoding, const char* __src_encoding) {
  iconv_t result = new __iconv_t;
  if (!__parse_encoding(__src_encoding, &result->src_encoding, nullptr) ||
      !__parse_encoding(__dst_encoding, &result->dst_encoding, &result->mode)) {
    delete result;
    errno = EINVAL;
    return INVALID_ICONV_T;
  }
  return result;
}

size_t iconv(iconv_t __converter,
             char** __src_buf, size_t* __src_bytes_left,
             char** __dst_buf, size_t* __dst_bytes_left) {
  if (__converter == INVALID_ICONV_T) {
    errno = EBADF;
    return -1;
  }
  return __converter->Convert(__src_buf, __src_bytes_left, __dst_buf, __dst_bytes_left);
}

int iconv_close(iconv_t __converter) {
  if (__converter == INVALID_ICONV_T) {
    errno = EBADF;
    return -1;
  }
  delete __converter;
  return 0;
}
