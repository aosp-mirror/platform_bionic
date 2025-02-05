// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#include "header_checks.h"

static void stdio_h() {
  TYPE(FILE*);
  TYPE(fpos_t);
  TYPE(off_t);
  TYPE(size_t);
  TYPE(ssize_t);
  TYPE(va_list);

  MACRO(BUFSIZ);
  MACRO(L_ctermid);
  MACRO(L_tmpnam);

  MACRO(_IOFBF);
  MACRO(_IOLBF);
  MACRO(_IONBF);

  MACRO(SEEK_CUR);
  MACRO(SEEK_END);
  MACRO(SEEK_SET);

  MACRO(FILENAME_MAX);
  MACRO(FOPEN_MAX);
  MACRO(TMP_MAX);

  MACRO(EOF);

  MACRO(NULL);

  const char* s;
  s = P_tmpdir;

#if !defined(stderr)
#error stderr
#endif
#if !defined(stdin)
#error stdin
#endif
#if !defined(stdout)
#error stdout
#endif
  FILE* fp;
  fp = stderr;
  fp = stdin;
  fp = stdout;

  FUNCTION(clearerr, void (*f)(FILE*));
  FUNCTION(ctermid, char* (*f)(char*));
  FUNCTION(dprintf, int (*f)(int, const char*, ...));
  FUNCTION(fclose, int (*f)(FILE*));
  FUNCTION(fdopen, FILE* (*f)(int, const char*));
  FUNCTION(feof, int (*f)(FILE*));
  FUNCTION(ferror, int (*f)(FILE*));
  FUNCTION(fflush, int (*f)(FILE*));
  FUNCTION(fgetc, int (*f)(FILE*));
  FUNCTION(fgetpos, int (*f)(FILE*, fpos_t*));
  FUNCTION(fgets, char* (*f)(char*, int, FILE*));
  FUNCTION(fileno, int (*f)(FILE*));
  FUNCTION(flockfile, void (*f)(FILE*));
  FUNCTION(fmemopen, FILE* (*f)(void*, size_t, const char*));
  FUNCTION(fopen, FILE* (*f)(const char*, const char*));
  FUNCTION(fprintf, int (*f)(FILE*, const char*, ...));
  FUNCTION(fputc, int (*f)(int, FILE*));
  FUNCTION(fputs, int (*f)(const char*, FILE*));
  FUNCTION(fread, size_t (*f)(void*, size_t, size_t, FILE*));
  FUNCTION(freopen, FILE* (*f)(const char*, const char*, FILE*));
  FUNCTION(fscanf, int (*f)(FILE*, const char*, ...));
  FUNCTION(fseek, int (*f)(FILE*, long, int));
  FUNCTION(fseeko, int (*f)(FILE*, off_t, int));
  FUNCTION(fsetpos, int (*f)(FILE*, const fpos_t*));
  FUNCTION(ftell, long (*f)(FILE*));
  FUNCTION(ftello, off_t (*f)(FILE*));
  FUNCTION(ftrylockfile, int (*f)(FILE*));
  FUNCTION(funlockfile, void (*f)(FILE*));
  FUNCTION(fwrite, size_t (*f)(const void*, size_t, size_t, FILE*));
  FUNCTION(getc, int (*f)(FILE*));
  FUNCTION(getchar, int (*f)(void));
  FUNCTION(getc_unlocked, int (*f)(FILE*));
  FUNCTION(getchar_unlocked, int (*f)(void));
  FUNCTION(getdelim, ssize_t (*f)(char**, size_t*, int, FILE*));
  FUNCTION(getline, ssize_t (*f)(char**, size_t*, FILE*));
  // gets() was removed in C11.
  // FUNCTION(gets, char* (*f)(char*));
  FUNCTION(open_memstream, FILE* (*f)(char**, size_t*));
  FUNCTION(pclose, int (*f)(FILE*));
  FUNCTION(perror, void (*f)(const char*));
  FUNCTION(popen, FILE* (*f)(const char*, const char*));
  FUNCTION(printf, int (*f)(const char*, ...));
  FUNCTION(putc, int (*f)(int, FILE*));
  FUNCTION(putchar, int (*f)(int));
  FUNCTION(putc_unlocked, int (*f)(int, FILE*));
  FUNCTION(putchar_unlocked, int (*f)(int));
  FUNCTION(puts, int (*f)(const char*));
  FUNCTION(remove, int (*f)(const char*));
  FUNCTION(rename, int (*f)(const char*, const char*));
  FUNCTION(renameat, int (*f)(int, const char*, int, const char*));
  FUNCTION(rewind, void (*f)(FILE*));
  FUNCTION(scanf, int (*f)(const char*, ...));
  FUNCTION(setbuf, void (*f)(FILE*, char*));
  FUNCTION(setvbuf, int (*f)(FILE*, char*, int, size_t));
  FUNCTION(snprintf, int (*f)(char*, size_t, const char*, ...));
  FUNCTION(sprintf, int (*f)(char*, const char*, ...));
  FUNCTION(sscanf, int (*f)(const char*, const char*, ...));
  FUNCTION(tempnam, char* (*f)(const char*, const char*));
  FUNCTION(tmpfile, FILE* (*f)(void));
  FUNCTION(tmpnam, char* (*f)(char*));
  FUNCTION(ungetc, int (*f)(int, FILE*));
  FUNCTION(vdprintf, int (*f)(int, const char*, va_list));
  FUNCTION(vfprintf, int (*f)(FILE*, const char*, va_list));
  FUNCTION(vfscanf, int (*f)(FILE*, const char*, va_list));
  FUNCTION(vprintf, int (*f)(const char*, va_list));
  FUNCTION(vscanf, int (*f)(const char*, va_list));
  FUNCTION(vsnprintf, int (*f)(char*, size_t, const char*, va_list));
  FUNCTION(vsprintf, int (*f)(char*, const char*, va_list));
  FUNCTION(vsscanf, int (*f)(const char*, const char*, va_list));
}
