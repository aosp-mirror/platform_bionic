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
#include <xlocale.h>

double strtod_l(const char* s, char** end_ptr, locale_t) {
  return strtod(s, end_ptr);
}

float strtof_l(const char* s, char** end_ptr, locale_t) {
  return strtof(s, end_ptr);
}

long strtol_l(const char* s, char** end_ptr, int base, locale_t) {
  return strtol(s, end_ptr, base);
}

long double strtold_l(const char* s, char** end_ptr, locale_t) {
  return strtold(s, end_ptr);
}

long long strtoll_l(const char* s, char** end_ptr, int base, locale_t) {
  return strtoll(s, end_ptr, base);
}

unsigned long strtoul_l(const char* s, char** end_ptr, int base, locale_t) {
  return strtoul(s, end_ptr, base);
}

unsigned long long strtoull_l(const char* s, char** end_ptr, int base, locale_t) {
  return strtoull(s, end_ptr, base);
}

