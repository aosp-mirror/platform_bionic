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

template <typename T, T Min, T Max> T StrToI(const char* nptr, char** endptr, int base) {
  // Ensure that base is between 2 and 36 inclusive, or the special value of 0.
  if (base < 0 || base == 1 || base > 36) {
    if (endptr != nullptr) *endptr = const_cast<char*>(nptr);
    errno = EINVAL;
    return 0;
  }

  // Skip white space and pick up leading +/- sign if any.
  // If base is 0, allow 0x for hex and 0 for octal, else
  // assume decimal; if base is already 16, allow 0x.
  const char* s = nptr;
  int c;
  do {
    c = static_cast<unsigned char>(*s++);
  } while (isspace(c));
  int neg;
  if (c == '-') {
    neg = 1;
    c = *s++;
  } else {
    neg = 0;
    if (c == '+') c = *s++;
  }
  if ((base == 0 || base == 16) && c == '0' &&
      (*s == 'x' || *s == 'X') && isxdigit(static_cast<unsigned char>(s[1]))) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0) base = (c == '0') ? 8 : 10;

  // Compute the cutoff value between legal numbers and illegal
  // numbers.  That is the largest legal value, divided by the
  // base.  An input number that is greater than this value, if
  // followed by a legal input character, is too big.  One that
  // is equal to this value may be valid or not; the limit
  // between valid and invalid numbers is then based on the last
  // digit.  For instance, if the range for intmax_t is
  // [-9223372036854775808..9223372036854775807] and the input base
  // is 10, cutoff will be set to 922337203685477580 and cutlim to
  // either 7 (neg==0) or 8 (neg==1), meaning that if we have
  // accumulated a value > 922337203685477580, or equal but the
  // next digit is > 7 (or 8), the number is too big, and we will
  // return a range error.
  T cutoff = neg ? Min : Max;
  int cutlim = cutoff % base;
  cutoff /= base;
  if (neg) {
    if (cutlim > 0) {
      cutlim -= base;
      cutoff += 1;
    }
    cutlim = -cutlim;
  }

  // Set `any` if any digits consumed; make it negative to indicate overflow.
  int any = 0;
  T acc = 0;
  for (; ; c = static_cast<unsigned char>(*s++)) {
    if (isdigit(c)) {
      c -= '0';
    } else if (isalpha(c)) {
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    } else {
      break;
    }
    if (c >= base) break;
    if (any < 0) continue;
    if (neg) {
      if (acc < cutoff || (acc == cutoff && c > cutlim)) {
        any = -1;
        acc = Min;
        errno = ERANGE;
      } else {
        any = 1;
        acc *= base;
        acc -= c;
      }
    } else {
      if (acc > cutoff || (acc == cutoff && c > cutlim)) {
        any = -1;
        acc = Max;
        errno = ERANGE;
      } else {
        any = 1;
        acc *= base;
        acc += c;
      }
    }
  }
  if (endptr != nullptr) *endptr = const_cast<char*>(any ? s - 1 : nptr);
  return acc;
}

template <typename T, T Max> T StrToU(const char* nptr, char** endptr, int base) {
  if (base < 0 || base == 1 || base > 36) {
    if (endptr != nullptr) *endptr = const_cast<char*>(nptr);
    errno = EINVAL;
    return 0;
  }

  const char* s = nptr;
  int c;
  do {
    c = static_cast<unsigned char>(*s++);
  } while (isspace(c));
  int neg;
  if (c == '-') {
    neg = 1;
    c = *s++;
  } else {
    neg = 0;
    if (c == '+') c = *s++;
  }
  if ((base == 0 || base == 16) && c == '0' &&
      (*s == 'x' || *s == 'X') && isxdigit(static_cast<unsigned char>(s[1]))) {
    c = s[1];
    s += 2;
    base = 16;
  }
  if (base == 0) base = (c == '0') ? 8 : 10;

  T cutoff = Max / static_cast<T>(base);
  int cutlim = Max % static_cast<T>(base);
  T acc = 0;
  int any = 0;
  for (; ; c = static_cast<unsigned char>(*s++)) {
    if (isdigit(c)) {
      c -= '0';
    } else if (isalpha(c)) {
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    } else {
      break;
    }
    if (c >= base) break;
    if (any < 0) continue;
    if (acc > cutoff || (acc == cutoff && c > cutlim)) {
      any = -1;
      acc = Max;
      errno = ERANGE;
    } else {
      any = 1;
      acc *= static_cast<T>(base);
      acc += c;
    }
  }
  if (neg && any > 0) acc = -acc;
  if (endptr != nullptr) *endptr = const_cast<char*>(any ? s - 1 : nptr);
  return acc;
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
  return StrToI<intmax_t, INTMAX_MIN, INTMAX_MAX>(s, end, base);
}

long strtol(const char* s, char** end, int base) {
  return StrToI<long, LONG_MIN, LONG_MAX>(s, end, base);
}

long long strtoll(const char* s, char** end, int base) {
  return StrToI<long long, LLONG_MIN, LLONG_MAX>(s, end, base);
}

// Public API since L, but not in any header.
extern "C" long long strtoq(const char* s, char** end, int base) {
  return strtoll(s, end, base);
}

unsigned long strtoul(const char* s, char** end, int base) {
  return StrToU<unsigned long, ULONG_MAX>(s, end, base);
}

unsigned long long strtoull(const char* s, char** end, int base) {
  return StrToU<unsigned long long, ULLONG_MAX>(s, end, base);
}

uintmax_t strtoumax(const char* s, char** end, int base) {
  return StrToU<uintmax_t, UINTMAX_MAX>(s, end, base);
}

// Public API since L, but not in any header.
extern "C" unsigned long long strtouq(const char* s, char** end, int base) {
  return strtoull(s, end, base);
}
