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

#include <string.h>

#include "header_checks.h"

static void string_h() {
  MACRO(NULL);
  TYPE(size_t);
  TYPE(locale_t);

  FUNCTION(memccpy, void* (*f)(void*, const void*, int, size_t));
  FUNCTION(memchr, void* (*f)(const void*, int, size_t));
  FUNCTION(memcmp, int (*f)(const void*, const void*, size_t));
  FUNCTION(memcpy, void* (*f)(void*, const void*, size_t));
  FUNCTION(memmove, void* (*f)(void*, const void*, size_t));
  FUNCTION(memset, void* (*f)(void*, int, size_t));
  FUNCTION(stpcpy, char* (*f)(char*, const char*));
  FUNCTION(stpncpy, char* (*f)(char*, const char*, size_t));
  FUNCTION(strcat, char* (*f)(char*, const char*));
  FUNCTION(strchr, char* (*f)(const char*, int));
  FUNCTION(strcmp, int (*f)(const char*, const char*));
  FUNCTION(strcoll, int (*f)(const char*, const char*));
  FUNCTION(strcoll_l, int (*f)(const char*, const char*, locale_t));
  FUNCTION(strcpy, char* (*f)(char*, const char*));
  FUNCTION(strcspn, size_t (*f)(const char*, const char*));
  FUNCTION(strdup, char* (*f)(const char*));
  FUNCTION(strerror, char* (*f)(int));
  FUNCTION(strerror_l, char* (*f)(int, locale_t));
  FUNCTION(strerror_r, int (*f)(int, char*, size_t));
  FUNCTION(strlen, size_t (*f)(const char*));
  FUNCTION(strncat, char* (*f)(char*, const char*, size_t));
  FUNCTION(strncmp, int (*f)(const char*, const char*, size_t));
  FUNCTION(strncpy, char* (*f)(char*, const char*, size_t));
  FUNCTION(strndup, char* (*f)(const char*, size_t));
  FUNCTION(strnlen, size_t (*f)(const char*, size_t));
  FUNCTION(strpbrk, char* (*f)(const char*, const char*));
  FUNCTION(strrchr, char* (*f)(const char*, int));
  FUNCTION(strsignal, char* (*f)(int));
  FUNCTION(strspn, size_t (*f)(const char*, const char*));
  FUNCTION(strstr, char* (*f)(const char*, const char*));
  FUNCTION(strtok, char* (*f)(char*, const char*));
  FUNCTION(strtok_r, char* (*f)(char*, const char*, char**));
  FUNCTION(strxfrm, size_t (*f)(char*, const char*, size_t));
  FUNCTION(strxfrm_l, size_t (*f)(char*, const char*, size_t, locale_t));
}
