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

#include <ctype.h>

#include "header_checks.h"

static void ctype_h() {
  FUNCTION(isalnum, int (*f)(int));
  FUNCTION(isalnum_l, int (*f)(int, locale_t));
  FUNCTION(isalpha, int (*f)(int));
  FUNCTION(isalpha_l, int (*f)(int, locale_t));
  FUNCTION(isascii, int (*f)(int));
  FUNCTION(isblank, int (*f)(int));
  FUNCTION(isblank_l, int (*f)(int, locale_t));
  FUNCTION(iscntrl, int (*f)(int));
  FUNCTION(iscntrl_l, int (*f)(int, locale_t));
  FUNCTION(isdigit, int (*f)(int));
  FUNCTION(isdigit_l, int (*f)(int, locale_t));
  FUNCTION(isgraph, int (*f)(int));
  FUNCTION(isgraph_l, int (*f)(int, locale_t));
  FUNCTION(islower, int (*f)(int));
  FUNCTION(islower_l, int (*f)(int, locale_t));
  FUNCTION(isprint, int (*f)(int));
  FUNCTION(isprint_l, int (*f)(int, locale_t));
  FUNCTION(ispunct, int (*f)(int));
  FUNCTION(ispunct_l, int (*f)(int, locale_t));
  FUNCTION(isspace, int (*f)(int));
  FUNCTION(isspace_l, int (*f)(int, locale_t));
  FUNCTION(isupper, int (*f)(int));
  FUNCTION(isupper_l, int (*f)(int, locale_t));
  FUNCTION(isxdigit, int (*f)(int));
  FUNCTION(isxdigit_l, int (*f)(int, locale_t));

  FUNCTION(toascii, int (*f)(int));
  FUNCTION(tolower, int (*f)(int));
  FUNCTION(tolower_l, int (*f)(int, locale_t));
  FUNCTION(toupper, int (*f)(int));
  FUNCTION(toupper_l, int (*f)(int, locale_t));

#if !defined(__BIONIC__) // These are marked obsolescent.
  #if !defined(_toupper)
    #error _toupper
  #endif
  #if !defined(_tolower)
    #error _tolower
  #endif
#endif
}
