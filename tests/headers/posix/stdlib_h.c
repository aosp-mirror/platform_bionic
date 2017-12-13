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

#include <stdlib.h>

#include "header_checks.h"

static void stdlib_h() {
  MACRO(EXIT_FAILURE);
  MACRO_VALUE(EXIT_SUCCESS, 0);

  MACRO(RAND_MAX);

  MACRO(MB_CUR_MAX);

  MACRO(NULL);

  TYPE(div_t);
  TYPE(ldiv_t);
  TYPE(lldiv_t);
  TYPE(size_t);
  TYPE(wchar_t);

#if !defined(WEXITSTATUS)
#error WEXITSTATUS
#endif
#if !defined(WIFEXITED)
#error WIFEXITED
#endif
#if !defined(WIFSIGNALED)
#error WIFSIGNALED
#endif
#if !defined(WIFSTOPPED)
#error WIFSTOPPED
#endif
  MACRO(WNOHANG);
#if !defined(WSTOPSIG)
#error WSTOPSIG
#endif
#if !defined(WTERMSIG)
#error WTERMSIG
#endif
  MACRO(WUNTRACED);

  FUNCTION(_Exit, void (*f)(int));
#if !defined(__BIONIC__)
  FUNCTION(a64l, long (*f)(const char*));
#endif
  FUNCTION(abort, void (*f)(void));
  FUNCTION(abs, int (*f)(int));
  FUNCTION(atexit, int (*f)(void (*)(void)));
  FUNCTION(atof, double (*f)(const char*));
  FUNCTION(atoi, int (*f)(const char*));
  FUNCTION(atol, long (*f)(const char*));
  FUNCTION(atoll, long long (*f)(const char*));
  FUNCTION(bsearch, void* (*f)(const void*, const void*, size_t, size_t, int (*)(const void*, const void*)));
  FUNCTION(calloc, void* (*f)(size_t, size_t));
  FUNCTION(div, div_t (*f)(int, int));
  FUNCTION(drand48, double (*f)(void));
  FUNCTION(erand48, double (*f)(unsigned short[3]));
  FUNCTION(exit, void (*f)(int));
  FUNCTION(free, void (*f)(void*));
  FUNCTION(getenv, char* (*f)(const char*));
  FUNCTION(getsubopt, int (*f)(char**, char* const*, char**));
  FUNCTION(grantpt, int (*f)(int));
  FUNCTION(initstate, char* (*f)(unsigned, char*, size_t));
  FUNCTION(jrand48, long (*f)(unsigned short[3]));
#if !defined(__BIONIC__)
  FUNCTION(l64a, char* (*f)(long));
#endif
  FUNCTION(labs, long (*f)(long));
  FUNCTION(lcong48, void (*f)(unsigned short[7]));
  FUNCTION(ldiv, ldiv_t (*f)(long, long));
  FUNCTION(llabs, long long (*f)(long long));
  FUNCTION(lldiv, lldiv_t (*f)(long long, long long));
  FUNCTION(lrand48, long (*f)(void));
  FUNCTION(malloc, void* (*f)(size_t));
  FUNCTION(mblen, int (*f)(const char*, size_t));
  FUNCTION(mbstowcs, size_t (*f)(wchar_t*, const char*, size_t));
  FUNCTION(mbtowc, int (*f)(wchar_t*, const char*, size_t));
  FUNCTION(mkdtemp, char* (*f)(char*));
  FUNCTION(mkstemp, int (*f)(char*));
  FUNCTION(mrand48, long (*f)(void));
  FUNCTION(nrand48, long (*f)(unsigned short[3]));
  FUNCTION(posix_memalign, int (*f)(void**, size_t, size_t));
  FUNCTION(posix_openpt, int (*f)(int));
  FUNCTION(ptsname, char* (*f)(int));
  FUNCTION(putenv, int (*f)(char*));
  FUNCTION(qsort, void (*f)(void*, size_t, size_t, int (*)(const void*, const void*)));
  FUNCTION(rand, int (*f)(void));
  FUNCTION(rand_r, int (*f)(unsigned*));
  FUNCTION(random, long (*f)(void));
  FUNCTION(realloc, void* (*f)(void*, size_t));
  FUNCTION(realpath, char* (*f)(const char*, char*));
  FUNCTION(seed48, unsigned short* (*f)(unsigned short[3]));
  FUNCTION(setenv, int (*f)(const char*, const char*, int));
#if !defined(__BIONIC__)
  FUNCTION(setkey, void (*f)(const char*));
#endif
  FUNCTION(setstate, char* (*f)(char*));
  FUNCTION(srand, void (*f)(unsigned));
  FUNCTION(srand48, void (*f)(long));
  FUNCTION(srandom, void (*f)(unsigned));
  FUNCTION(strtod, double (*f)(const char*, char**));
  FUNCTION(strtof, float (*f)(const char*, char**));
  FUNCTION(strtol, long (*f)(const char*, char**, int));
  FUNCTION(strtold, long double (*f)(const char*, char**));
  FUNCTION(strtoll, long long (*f)(const char*, char**, int));
  FUNCTION(strtoul, unsigned long (*f)(const char*, char**, int));
  FUNCTION(strtoull, unsigned long long (*f)(const char*, char**, int));
  FUNCTION(system, int (*f)(const char*));
  FUNCTION(unlockpt, int (*f)(int));
  FUNCTION(unsetenv, int (*f)(const char*));
  FUNCTION(wcstombs, size_t (*f)(char*, const wchar_t*, size_t));
  FUNCTION(wctomb, int (*f)(char*, wchar_t));
}
