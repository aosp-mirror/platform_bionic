/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>

#include <benchmark/benchmark.h>
#include "util.h"

// Avoid optimization.
volatile int A = 'A';
volatile int a = 'a';
volatile int X = 'X';
volatile int x = 'x';
volatile int backspace = '\b';
volatile int del = '\x7f';
volatile int space = ' ';
volatile int tab = '\t';
volatile int zero = '0';
volatile int underscore = '_';
volatile int top_bit_set = 0x88;

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalnum_y1, isalnum(A));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalnum_y2, isalnum(a));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalnum_y3, isalnum(zero));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalnum_n, isalnum(underscore));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalpha_y1, isalpha(A));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalpha_y2, isalpha(a));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isalpha_n, isalpha(underscore));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isascii_y, isascii(x));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isascii_n, isascii(top_bit_set));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isblank_y1, isblank(space));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isblank_y2, isblank(tab));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isblank_n, isblank(underscore));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_iscntrl_y1, iscntrl(backspace));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_iscntrl_y2, iscntrl(del));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_iscntrl_n, iscntrl(underscore));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isdigit_y, iscntrl(zero));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isdigit_n, iscntrl(underscore));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isgraph_y1, isgraph(A));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isgraph_y2, isgraph(a));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isgraph_y3, isgraph(zero));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isgraph_y4, isgraph(underscore));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isgraph_n, isgraph(space));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_islower_y, islower(x));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_islower_n, islower(X));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isprint_y1, isprint(A));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isprint_y2, isprint(a));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isprint_y3, isprint(zero));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isprint_y4, isprint(underscore));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isprint_y5, isprint(space));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isprint_n, isprint(backspace));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_ispunct_y, ispunct(underscore));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_ispunct_n, ispunct(A));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isspace_y1, isspace(space));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isspace_y2, isspace(tab));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isspace_n, isspace(A));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isupper_y, isupper(X));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isupper_n, isupper(x));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isxdigit_y1, isxdigit(zero));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isxdigit_y2, isxdigit(a));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isxdigit_y3, isxdigit(A));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_isxdigit_n, isxdigit(underscore));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_toascii_y, isascii(x));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_toascii_n, isascii(top_bit_set));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_tolower_y, tolower(X));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_tolower_n, tolower(x));

BIONIC_TRIVIAL_BENCHMARK(BM_ctype_toupper_y, toupper(x));
BIONIC_TRIVIAL_BENCHMARK(BM_ctype_toupper_n, toupper(X));
