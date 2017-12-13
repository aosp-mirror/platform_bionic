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

#include <glob.h>

#include "header_checks.h"

static void glob_h() {
  TYPE(glob_t);
  STRUCT_MEMBER(glob_t, size_t, gl_pathc);
  STRUCT_MEMBER(glob_t, char**, gl_pathv);
  STRUCT_MEMBER(glob_t, size_t, gl_offs);
  TYPE(size_t);

  MACRO(GLOB_APPEND);
  MACRO(GLOB_DOOFFS);
  MACRO(GLOB_ERR);
  MACRO(GLOB_MARK);
  MACRO(GLOB_NOCHECK);
  MACRO(GLOB_NOESCAPE);
  MACRO(GLOB_NOSORT);

  MACRO(GLOB_ABORTED);
  MACRO(GLOB_NOMATCH);
  MACRO(GLOB_NOSPACE);

  FUNCTION(glob, int (*f)(const char*, int, int (*)(const char*, int), glob_t*));
  FUNCTION(globfree, void (*f)(glob_t*));
}
