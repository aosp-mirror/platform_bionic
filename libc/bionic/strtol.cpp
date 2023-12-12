/*
 * Copyright (c) 1992 The Regents of the University of California.
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
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <wchar.h>

template <typename T, T Min, T Max, typename CharT>
__attribute__((always_inline)) T StrToI(const CharT* s, CharT** end_ptr, int base) {
  // Ensure that base is between 2 and 36 inclusive, or the special value of 0.
  if (base < 0 || base == 1 || base > 36) {
    if (end_ptr != nullptr) *end_ptr = const_cast<CharT*>(s);
    errno = EINVAL;
    return 0;
  }

  // Skip white space and pick up leading +/- sign if any.
  const CharT* p = s;
  int c;
  while (isspace(c = *p++)) {
  }
  bool neg = false;
  if (c == '-') {
    neg = true;
    c = *p++;
  } else if (c == '+') {
    c = *p++;
  }

  // If base is 0 or 16, allow "0x" prefix for hex.
  if ((base == 0 || base == 16) && c == '0' && (*p == 'x' || *p == 'X') && isxdigit(p[1])) {
    c = p[1];
    p += 2;
    base = 16;
  }
  // If base is 0 or 2, allow "0b" prefix for binary.
  if ((base == 0 || base == 2) && c == '0' && (*p == 'b' || *p == 'B') && isdigit(p[1])) {
    c = p[1];
    p += 2;
    base = 2;
  }
  // If base is 0, allow "0" prefix for octal, otherwise base is 10.
  if (base == 0) base = (c == '0') ? 8 : 10;

  constexpr bool is_signed = (Min != 0);
  T acc = 0;
  // Non-zero if any digits consumed; negative to indicate overflow/underflow.
  int any = 0;
  for (;; c = *p++) {
    if (isdigit(c)) {
      c -= '0';
    } else if (isalpha(c)) {
      c = 10 + (_tolower(c) - 'a');
    } else {
      break;
    }
    if (c >= base) break;
    if (any < 0) continue;
    if (is_signed) {
      // We work in the negative space because the most negative value has a
      // larger magnitude than the most positive value.
      if (__builtin_mul_overflow(acc, base, &acc) || __builtin_sub_overflow(acc, c, &acc)) {
        any = -1;
        continue;
      }
    } else {
      if (__builtin_mul_overflow(acc, base, &acc) || __builtin_add_overflow(acc, c, &acc)) {
        any = -1;
        continue;
      }
    }
    any = 1;
  }

  if (end_ptr != nullptr) *end_ptr = const_cast<CharT*>(any ? p - 1 : s);

  // Detected overflow/underflow in the loop?
  if (any == -1) {
    errno = ERANGE;
    return (is_signed && neg) ? Min : Max;
  }

  // Will we overflow by trying to negate the most negative value?
  if (any > 0 && is_signed && !neg && acc == Min) {
    errno = ERANGE;
    return Max;
  }

  if (is_signed) return neg ? acc : -acc;
  return neg ? -acc : acc;
}

int atoi(const char* s) {
  return strtol(s, nullptr, 10);
}

long atol(const char* s) {
  return strtol(s, nullptr, 10);
}

long long atoll(const char* s) {
  return strtoll(s, nullptr, 10);
}

intmax_t strtoimax(const char* s, char** end, int base) {
  return StrToI<intmax_t, INTMAX_MIN, INTMAX_MAX, char>(s, end, base);
}

intmax_t wcstoimax(const wchar_t* s, wchar_t** end, int base) {
  return StrToI<intmax_t, INTMAX_MIN, INTMAX_MAX, wchar_t>(s, end, base);
}

long strtol(const char* s, char** end, int base) {
  return StrToI<long, LONG_MIN, LONG_MAX, char>(s, end, base);
}

long wcstol(const wchar_t* s, wchar_t** end, int base) {
  return StrToI<long, LONG_MIN, LONG_MAX, wchar_t>(s, end, base);
}

long long strtoll(const char* s, char** end, int base) {
  return StrToI<long long, LLONG_MIN, LLONG_MAX, char>(s, end, base);
}

long long wcstoll(const wchar_t* s, wchar_t** end, int base) {
  return StrToI<long long, LLONG_MIN, LLONG_MAX, wchar_t>(s, end, base);
}

unsigned long strtoul(const char* s, char** end, int base) {
  return StrToI<unsigned long, 0, ULONG_MAX, char>(s, end, base);
}

unsigned long wcstoul(const wchar_t* s, wchar_t** end, int base) {
  return StrToI<unsigned long, 0, ULONG_MAX, wchar_t>(s, end, base);
}

unsigned long long strtoull(const char* s, char** end, int base) {
  return StrToI<unsigned long long, 0, ULLONG_MAX, char>(s, end, base);
}

unsigned long long wcstoull(const wchar_t* s, wchar_t** end, int base) {
  return StrToI<unsigned long long, 0, ULLONG_MAX, wchar_t>(s, end, base);
}

uintmax_t strtoumax(const char* s, char** end, int base) {
  return StrToI<uintmax_t, 0, UINTMAX_MAX, char>(s, end, base);
}

uintmax_t wcstoumax(const wchar_t* s, wchar_t** end, int base) {
  return StrToI<uintmax_t, 0, UINTMAX_MAX, wchar_t>(s, end, base);
}
