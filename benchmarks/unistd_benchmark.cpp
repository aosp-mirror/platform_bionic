/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <sys/syscall.h>
#include <unistd.h>

#include <benchmark/benchmark.h>
#include "util.h"

BIONIC_TRIVIAL_BENCHMARK(BM_unistd_getpid, getpid());
BIONIC_TRIVIAL_BENCHMARK(BM_unistd_getpid_syscall, syscall(__NR_getpid));

// TODO: glibc 2.30 added gettid() too.
#if defined(__BIONIC__)
BIONIC_TRIVIAL_BENCHMARK(BM_unistd_gettid, gettid());
#endif
BIONIC_TRIVIAL_BENCHMARK(BM_unistd_gettid_syscall, syscall(__NR_gettid));
