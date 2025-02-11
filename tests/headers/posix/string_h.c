// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
#if !defined(__GLIBC__) // Our glibc is too old.
  FUNCTION(memmem, void* (*f)(const void*, size_t, const void*, size_t));
#endif
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
#if !defined(__GLIBC__) // Our glibc is too old.
  FUNCTION(strlcat, size_t (*f)(char*, const char*, size_t));
  FUNCTION(strlcpy, size_t (*f)(char*, const char*, size_t));
#endif
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
