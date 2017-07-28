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

#include "private/icu.h"

int8_t __icu_charType(wint_t wc) {
  typedef int8_t (*u_charType_t)(UChar32);
  static auto u_charType = reinterpret_cast<u_charType_t>(__find_icu_symbol("u_charType"));
  return u_charType ? u_charType(wc) : -1;
}

int32_t __icu_getIntPropertyValue(wint_t wc, UProperty property) {
  typedef int32_t (*u_getIntPropertyValue_t)(UChar32, UProperty);
  static auto u_getIntPropertyValue =
      reinterpret_cast<u_getIntPropertyValue_t>(__find_icu_symbol("u_getIntPropertyValue"));
  return u_getIntPropertyValue ? u_getIntPropertyValue(wc, property) : 0;
}

bool __icu_hasBinaryProperty(wint_t wc, UProperty property, int (*fallback)(int)) {
  typedef UBool (*u_hasBinaryProperty_t)(UChar32, UProperty);
  static auto u_hasBinaryProperty =
      reinterpret_cast<u_hasBinaryProperty_t>(__find_icu_symbol("u_hasBinaryProperty"));
  return u_hasBinaryProperty ? u_hasBinaryProperty(wc, property) : fallback(wc);
}
