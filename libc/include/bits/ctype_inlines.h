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

#pragma once

#include <sys/cdefs.h>
#include <xlocale.h>

#if !defined(__BIONIC_CTYPE_INLINE)
#define __BIONIC_CTYPE_INLINE static __inline
#endif

/** Internal implementation detail. Do not use. */
#define _CTYPE_U 0x01
/** Internal implementation detail. Do not use. */
#define _CTYPE_L 0x02
/** Internal implementation detail. Do not use. */
#define _CTYPE_D 0x04
/** Internal implementation detail. Do not use. */
#define _CTYPE_S 0x08
/** Internal implementation detail. Do not use. */
#define _CTYPE_P 0x10
/** Internal implementation detail. Do not use. */
#define _CTYPE_C 0x20
/** Internal implementation detail. Do not use. */
#define _CTYPE_X 0x40
/** Internal implementation detail. Do not use. */
#define _CTYPE_B 0x80
/** Internal implementation detail. Do not use. */
#define _CTYPE_R (_CTYPE_P|_CTYPE_U|_CTYPE_L|_CTYPE_D|_CTYPE_B)
/** Internal implementation detail. Do not use. */
#define _CTYPE_A (_CTYPE_L|_CTYPE_U)
/** Internal implementation detail. Do not use. */
#define _CTYPE_N _CTYPE_D

__BEGIN_DECLS

/** Internal implementation detail. Do not use. */
extern const char* _ctype_;

/** Returns true if `ch` is in `[A-Za-z0-9]`. */
__BIONIC_CTYPE_INLINE int isalnum(int __ch) {
  // `isalnum(c)` is `isalpha(c) || isdigit(c)`, but there's no obvious way
  // to simplify that, and the table lookup is just slightly faster...
  // Note that this is unsafe for inputs less than -1 (EOF) or greater than
  // 0xff. This is true of other C libraries too.
  return (_ctype_[__ch + 1] & (_CTYPE_U|_CTYPE_L|_CTYPE_N));
}

/** Returns true if `ch` is in `[A-Za-z]`. */
__BIONIC_CTYPE_INLINE int isalpha(int __ch) {
  return (__ch >= 'A' && __ch <= 'Z') || (__ch >= 'a' && __ch <= 'z');
}

/** Returns true if `ch` is a space or tab. */
__BIONIC_CTYPE_INLINE int isblank(int __ch) {
  return __ch == ' ' || __ch == '\t';
}

/** Returns true if `ch` is a control character (any character before space, plus DEL). */
__BIONIC_CTYPE_INLINE int iscntrl(int __ch) {
  return (__BIONIC_CAST(static_cast, unsigned, __ch) < ' ') || __ch == 0x7f;
}

/** Returns true if `ch` is in `[0-9]`. */
__BIONIC_CTYPE_INLINE int isdigit(int __ch) {
  return (__ch >= '0' && __ch <= '9');
}

/** Returns true if `ch` is `[A-Za-z0-9]` or punctuation. */
__BIONIC_CTYPE_INLINE int isgraph(int __ch) {
  return (__ch >= '!' && __ch <= '~');
}

/** Returns true if `ch` is in `[a-z]`. */
__BIONIC_CTYPE_INLINE int islower(int __ch) {
  return (__ch >= 'a' && __ch <= 'z');
}

/** Returns true if `ch` is `[A-Za-z0-9]` or punctuation or space. */
__BIONIC_CTYPE_INLINE int isprint(int __ch) {
  return (__ch >= ' ' && __ch <= '~');
}

/** Returns true if `ch` is punctuation. */
__BIONIC_CTYPE_INLINE int ispunct(int __ch) {
  // `ispunct(c)` is `isgraph(c) && !isalnum(c)`, but there's no obvious way
  // to simplify that, and the table lookup is just slightly faster...
  // Note that this is unsafe for inputs less than -1 (EOF) or greater than
  // 0xff. This is true of other C libraries too.
  return (_ctype_[__ch + 1] & _CTYPE_P);
}

/** Returns true if `ch` is in `[ \f\n\r\t\v]`. */
__BIONIC_CTYPE_INLINE int isspace(int __ch) {
  return __ch == ' ' || (__ch >= '\t' && __ch <= '\r');
}

/** Returns true if `ch` is in `[A-Z]`. */
__BIONIC_CTYPE_INLINE int isupper(int __ch) {
  return (__ch >= 'A' && __ch <= 'Z');
}

/** Returns true if `ch` is in `[0-9A-Fa-f]`. */
__BIONIC_CTYPE_INLINE int isxdigit(int __ch) {
  return (__ch >= '0' && __ch <= '9') || (__ch >= 'a' && __ch <= 'f') || (__ch >= 'A' && __ch <= 'F');
}

/**
 * Returns the corresponding lower-case character if `ch` is upper-case, or undefined otherwise.
 *
 * Prefer tolower() instead.
 */
__BIONIC_CTYPE_INLINE int _tolower(int __ch) {
  return __ch | 0x20;
}

/** Returns the corresponding lower-case character if `ch` is upper-case, or `ch` otherwise. */
__BIONIC_CTYPE_INLINE int tolower(int __ch) {
  if (__ch >= 'A' && __ch <= 'Z') return _tolower(__ch);
  return __ch;
}

/**
 * Returns the corresponding upper-case character if `ch` is lower-case, or undefined otherwise.
 *
 * Prefer toupper() instead.
 */
__BIONIC_CTYPE_INLINE int _toupper(int __ch) {
  // Using EOR rather than AND makes no difference on arm, but saves an
  // instruction on arm64.
  return __ch ^ 0x20;
}

/** Returns the corresponding upper-case character if `ch` is lower-case, or `ch` otherwise. */
__BIONIC_CTYPE_INLINE int toupper(int __ch) {
  if (__ch >= 'a' && __ch <= 'z') return _toupper(__ch);
  return __ch;
}

/** Returns true if `ch` is less than 0x80. */
__BIONIC_CTYPE_INLINE int isascii(int __ch) {
  return __BIONIC_CAST(static_cast, unsigned, __ch) < 0x80;
}

/** Returns `ch & 0x7f`. */
__BIONIC_CTYPE_INLINE int toascii(int __ch) {
  return __ch & 0x7f;
}

#if __ANDROID_API__ >= 21
/** Like isalnum but with an ignored `locale_t`. */
int isalnum_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isalpha but with an ignored `locale_t`. */
int isalpha_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isblank but with an ignored `locale_t`. */
int isblank_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like iscntrl but with an ignored `locale_t`. */
int iscntrl_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isdigit but with an ignored `locale_t`. */
int isdigit_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isgraph but with an ignored `locale_t`. */
int isgraph_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like islower but with an ignored `locale_t`. */
int islower_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isprint but with an ignored `locale_t`. */
int isprint_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like ispunct but with an ignored `locale_t`. */
int ispunct_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isspace but with an ignored `locale_t`. */
int isspace_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isupper but with an ignored `locale_t`. */
int isupper_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like isxdigit but with an ignored `locale_t`. */
int isxdigit_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like tolower but with an ignored `locale_t`. */
int tolower_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
/** Like toupper but with an ignored `locale_t`. */
int toupper_l(int __ch, locale_t __l) __INTRODUCED_IN(21);
#else
// Implemented as static inlines in libc++ before 21.
#endif

__END_DECLS
