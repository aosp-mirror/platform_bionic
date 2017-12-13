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

#include <regex.h>

#include "header_checks.h"

static void regex_h() {
  TYPE(regex_t);
  STRUCT_MEMBER(regex_t, size_t, re_nsub);

  TYPE(size_t);

  TYPE(regmatch_t);
  STRUCT_MEMBER(regmatch_t, regoff_t, rm_so);
  STRUCT_MEMBER(regmatch_t, regoff_t, rm_eo);

  MACRO(REG_EXTENDED);
  MACRO(REG_ICASE);
  MACRO(REG_NOSUB);
  MACRO(REG_NEWLINE);

  MACRO(REG_NOTBOL);
  MACRO(REG_NOTEOL);

  MACRO(REG_NOMATCH);
  MACRO(REG_BADPAT);
  MACRO(REG_ECOLLATE);
  MACRO(REG_ECTYPE);
  MACRO(REG_EESCAPE);
  MACRO(REG_ESUBREG);
  MACRO(REG_EBRACK);
  MACRO(REG_EPAREN);
  MACRO(REG_EBRACE);
  MACRO(REG_BADBR);
  MACRO(REG_ERANGE);
  MACRO(REG_ESPACE);
  MACRO(REG_BADRPT);

  FUNCTION(regcomp, int (*f)(regex_t*, const char*, int));
  FUNCTION(regerror, size_t (*f)(int, const regex_t*, char*, size_t));
  FUNCTION(regexec, int (*f)(const regex_t*, const char*, size_t, regmatch_t*, int));
}
