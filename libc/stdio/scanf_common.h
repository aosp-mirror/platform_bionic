/*	$OpenBSD: vfscanf.c,v 1.31 2014/03/19 05:17:01 guenther Exp $ */
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
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
#include <inttypes.h>
#include <limits.h>
#include <locale.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <wctype.h>
#include "local.h"

#include <platform/bionic/macros.h>
#include <private/bionic_fortify.h>
#include <private/bionic_mbstate.h>

#define BUF 513 /* Maximum length of numeric string. */

// Flags used during conversion.
// Size/type:
#define LONG 0x00001        // l: long or double
#define LONGDBL 0x00002     // L: long double
#define SHORT 0x00004       // h: short
#define SHORTSHORT 0x00008  // hh: 8 bit integer
#define LLONG 0x00010       // ll: long long (+ deprecated q: quad)
#define POINTER 0x00020     // p: void* (as hex)
#define SIZEINT 0x00040     // z: (signed) size_t
#define MAXINT 0x00080      // j: intmax_t
#define PTRINT 0x00100      // t: ptrdiff_t
#define NOSKIP 0x00200      // [ or c: do not skip blanks
// Modifiers:
#define SUPPRESS 0x00400  // *: suppress assignment
#define UNSIGNED 0x00800  // %[oupxX] conversions
#define ALLOCATE 0x01000  // m: allocate a char*
// Internal use during integer parsing:
#define SIGNOK 0x02000    // +/- is (still) legal
#define HAVESIGN 0x04000  // Sign detected
#define NDIGITS 0x08000   // No digits detected
#define PFXOK 0x10000     // "0x" prefix is (still) legal
#define PFBOK 0x20000     // "0b" prefix is (still) legal
#define NZDIGITS 0x40000  // No zero digits detected

// Conversion types.
#define CT_CHAR 0    // %c conversion
#define CT_CCL 1     // %[...] conversion
#define CT_STRING 2  // %s conversion
#define CT_INT 3     // Integer: strtoimax/strtoumax
#define CT_FLOAT 4   // Float: strtod

#define to_digit(c) static_cast<int>((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)

// Append a digit to a value and check for overflow.
#define APPEND_DIGIT(val, dig)               \
  do {                                       \
    if ((val) > INT_MAX / 10)                \
      errno = ENOMEM;                        \
    else {                                   \
      (val) *= 10;                           \
      if ((val) > INT_MAX - to_digit((dig))) \
        errno = ENOMEM;                      \
      else                                   \
        (val) += to_digit((dig));            \
    }                                        \
  } while (0)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
// Trasnlate a fixed size integer argument for the %w/%wf format to a
// flag representation. Supported sizes are 8, 16, 32, and 64 so far.
// See details in bionic/libc/include/stdint.h
static int w_to_flag(int size, bool fast) {
  static constexpr int fast_size = sizeof(void*) == 8 ? LLONG : 0;
  if (size == 8) return SHORTSHORT;
  if (size == 16) return fast ? fast_size : SHORT;
  if (size == 32) return fast ? fast_size : 0;
  if (size == 64) return LLONG;
  __fortify_fatal("%%w%s%d is unsupported", fast ? "f" : "", size);
}

#pragma clang diagnostic pop