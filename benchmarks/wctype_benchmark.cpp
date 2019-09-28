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

#include <wctype.h>

#include <benchmark/benchmark.h>
#include "util.h"

BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towlower_ascii_y, towlower('X'));
BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towlower_ascii_n, towlower('x'));

BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towlower_unicode_y, towlower(0x0391));
BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towlower_unicode_n, towlower(0x03b1));

BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towupper_ascii_y, towupper('x'));
BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towupper_ascii_n, towupper('X'));

BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towupper_unicode_y, towupper(0x03b1));
BIONIC_TRIVIAL_BENCHMARK(BM_wctype_towupper_unicode_n, towupper(0x0391));
