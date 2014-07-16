/*
 * Copyright (C) 2014 The Android Open Source Project
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

int isalnum_l(int c, locale_t) {
  return isalnum(c);
}

int isalpha_l(int c, locale_t) {
  return isalpha(c);
}

int isblank_l(int c, locale_t) {
  return isblank(c);
}

int iscntrl_l(int c, locale_t) {
  return iscntrl(c);
}

int isdigit_l(int c, locale_t) {
  return isdigit(c);
}

int isgraph_l(int c, locale_t) {
  return isgraph(c);
}

int islower_l(int c, locale_t) {
  return islower(c);
}

int isprint_l(int c, locale_t) {
  return isprint(c);
}

int ispunct_l(int c, locale_t) {
  return ispunct(c);
}

int isspace_l(int c, locale_t) {
  return isspace(c);
}

int isupper_l(int c, locale_t) {
  return isupper(c);
}

int isxdigit_l(int c, locale_t) {
  return isxdigit(c);
}

int toupper_l(int c, locale_t) {
  return toupper(c);
}

int tolower_l(int c, locale_t) {
  return tolower(c);
}
