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
  FUNCTION(gets, char* (*f)(char*));
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
