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

#include <ctype.h>

static inline int __in_range(int c, char lo, char hi) {
  return c >= lo && c <= hi;
}

int isalnum(int c) {
  // `isalnum(c)` is `isalpha(c) || isdigit(c)`, but there's no obvious way
  // to simplify that, and the table lookup is just slightly faster...
  // Note that this is unsafe for inputs less than -1 (EOF) or greater than
  // 0xff. This is true of other C libraries too.
  return (_ctype_[c + 1] & (_CTYPE_U|_CTYPE_L|_CTYPE_N));
}

int isalnum_l(int c, locale_t) {
  return isalnum(c);
}

int isalpha(int c) {
  return __in_range(c, 'A', 'Z') || __in_range(c, 'a', 'z');
}

int isalpha_l(int c, locale_t) {
  return isalpha(c);
}

int isascii(int c) {
  return static_cast<unsigned>(c) < 0x80;
}

int isblank(int c) {
  return c == ' ' || c == '\t';
}

int isblank_l(int c, locale_t) {
  return isblank(c);
}

int iscntrl(int c) {
  return (static_cast<unsigned>(c) < ' ') || c == 0x7f;
}

int iscntrl_l(int c, locale_t) {
  return iscntrl(c);
}

int isdigit(int c) {
  return __in_range(c, '0', '9');
}

int isdigit_l(int c, locale_t) {
  return isdigit(c);
}

int isgraph(int c) {
  return __in_range(c, '!', '~');
}

int isgraph_l(int c, locale_t) {
  return isgraph(c);
}

int islower(int c) {
  return __in_range(c, 'a', 'z');
}

int islower_l(int c, locale_t) {
  return islower(c);
}

int isprint(int c) {
  return __in_range(c, ' ', '~');
}

int isprint_l(int c, locale_t) {
  return isprint(c);
}

int ispunct(int c) {
  // `ispunct(c)` is `isgraph(c) && !isalnum(c)`, but there's no obvious way
  // to simplify that, and the table lookup is just slightly faster...
  // Note that this is unsafe for inputs less than -1 (EOF) or greater than
  // 0xff. This is true of other C libraries too.
  return (_ctype_[c + 1] & _CTYPE_P);
}

int ispunct_l(int c, locale_t) {
  return ispunct(c);
}

int isspace(int c) {
  return c == ' ' || __in_range(c, '\t', '\r');
}

int isspace_l(int c, locale_t) {
  return isspace(c);
}

int isupper(int c) {
  return __in_range(c, 'A', 'Z');
}

int isupper_l(int c, locale_t) {
  return isupper(c);
}

int isxdigit(int c) {
  return __in_range(c, '0', '9') || __in_range(c, 'a', 'f') || __in_range(c, 'A', 'F');
}

int isxdigit_l(int c, locale_t) {
  return isxdigit(c);
}

int toascii(int c) {
  return c & 0x7f;
}

int _toupper(int c) {
  // Using EOR rather than AND makes no difference on arm, but saves an
  // instruction on arm64.
  return c ^ 0x20;
}

int toupper(int c) {
  if (c >= 'a' && c <= 'z') return _toupper(c);
  return c;
}

int toupper_l(int c, locale_t) {
  return toupper(c);
}

int _tolower(int c) {
  return c | 0x20;
}

int tolower(int c) {
  if (c >= 'A' && c <= 'Z') return _tolower(c);
  return c;
}

int tolower_l(int c, locale_t) {
  return tolower(c);
}
