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

#include <stdint.h>

#include "header_checks.h"

static void stdint_h() {
  TYPE(int8_t);
  TYPE(int16_t);
  TYPE(int32_t);
  TYPE(uint8_t);
  TYPE(uint16_t);
  TYPE(uint32_t);

  TYPE(int64_t);
  TYPE(uint64_t);

  TYPE(int_least8_t);
  TYPE(int_least16_t);
  TYPE(int_least32_t);
  TYPE(int_least64_t);
  TYPE(uint_least8_t);
  TYPE(uint_least16_t);
  TYPE(uint_least32_t);
  TYPE(uint_least64_t);

  TYPE(int_fast8_t);
  TYPE(int_fast16_t);
  TYPE(int_fast32_t);
  TYPE(int_fast64_t);
  TYPE(uint_fast8_t);
  TYPE(uint_fast16_t);
  TYPE(uint_fast32_t);
  TYPE(uint_fast64_t);

  TYPE(intptr_t);
  TYPE(uintptr_t);

  TYPE(intmax_t);
  TYPE(uintmax_t);

  MACRO(INT8_MIN);
  MACRO(INT16_MIN);
  MACRO(INT32_MIN);
  MACRO(INT64_MIN);
  MACRO(INT8_MAX);
  MACRO(INT16_MAX);
  MACRO(INT32_MAX);
  MACRO(INT64_MAX);
  MACRO(UINT8_MAX);
  MACRO(UINT16_MAX);
  MACRO(UINT32_MAX);
  MACRO(UINT64_MAX);

  MACRO(INT_LEAST8_MIN);
  MACRO(INT_LEAST16_MIN);
  MACRO(INT_LEAST32_MIN);
  MACRO(INT_LEAST64_MIN);
  MACRO(INT_LEAST8_MAX);
  MACRO(INT_LEAST16_MAX);
  MACRO(INT_LEAST32_MAX);
  MACRO(INT_LEAST64_MAX);
  MACRO(UINT_LEAST8_MAX);
  MACRO(UINT_LEAST16_MAX);
  MACRO(UINT_LEAST32_MAX);
  MACRO(UINT_LEAST64_MAX);

  MACRO(INT_FAST8_MIN);
  MACRO(INT_FAST16_MIN);
  MACRO(INT_FAST32_MIN);
  MACRO(INT_FAST64_MIN);
  MACRO(INT_FAST8_MAX);
  MACRO(INT_FAST16_MAX);
  MACRO(INT_FAST32_MAX);
  MACRO(INT_FAST64_MAX);
  MACRO(UINT_FAST8_MAX);
  MACRO(UINT_FAST16_MAX);
  MACRO(UINT_FAST32_MAX);
  MACRO(UINT_FAST64_MAX);

  MACRO(INTPTR_MIN);
  MACRO(INTPTR_MAX);
  MACRO(UINTPTR_MAX);

  MACRO(INTMAX_MIN);
  MACRO(INTMAX_MAX);
  MACRO(UINTMAX_MAX);

  MACRO(PTRDIFF_MIN);
  MACRO(PTRDIFF_MAX);

  MACRO(SIG_ATOMIC_MIN);
  MACRO(SIG_ATOMIC_MAX);

  MACRO(SIZE_MAX);

  MACRO(WCHAR_MIN);
  MACRO(WCHAR_MAX);

  MACRO(WINT_MIN);
  MACRO(WINT_MAX);

#if !defined(INT8_C)
#error INT8_C
#endif
#if !defined(INT16_C)
#error INT16_C
#endif
#if !defined(INT32_C)
#error INT32_C
#endif
#if !defined(INT32_C)
#error INT32_C
#endif

#if !defined(UINT8_C)
#error UINT8_C
#endif
#if !defined(UINT16_C)
#error UINT16_C
#endif
#if !defined(UINT32_C)
#error UINT32_C
#endif
#if !defined(UINT32_C)
#error UINT32_C
#endif

#if !defined(INTMAX_C)
#error INTMAX_C
#endif
#if !defined(UINTMAX_C)
#error UINTMAX_C
#endif
}
