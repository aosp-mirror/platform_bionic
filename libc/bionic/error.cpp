/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <error.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int error_message_count = 0;
void (*error_print_progname)(void) = nullptr;
int error_one_per_line = 0;

static void __error_head() {
  ++error_message_count;

  if (error_print_progname != nullptr) {
    error_print_progname();
  } else {
    fflush(stdout);
    fprintf(stderr, "%s:", getprogname());
  }
}

static void __error_tail(int status, int error) {
  if (error != 0) {
    fprintf(stderr, ": %s", strerror(error));
  }

  putc('\n', stderr);
  fflush(stderr);

  if (status != 0) {
    exit(status);
  }
}

void error(int status, int error, const char* fmt, ...) {
  __error_head();
  putc(' ', stderr);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  __error_tail(status, error);
}

void error_at_line(int status, int error, const char* file, unsigned int line, const char* fmt, ...) {
  if (error_one_per_line) {
    static const char* last_file;
    static unsigned int last_line;
    if (last_line == line && last_file != nullptr && strcmp(last_file, file) == 0) {
      return;
    }
    last_file = file;
    last_line = line;
  }

  __error_head();
  fprintf(stderr, "%s:%d: ", file, line);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  __error_tail(status, error);
}
