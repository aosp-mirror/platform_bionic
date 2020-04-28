/*
 * Copyright (C) 2017 The Android Open Source Project
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

#pragma once

#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

typedef void (*benchmark_func_t) (void);

extern std::mutex g_map_lock;

extern std::map<std::string, std::pair<benchmark_func_t, std::string>> g_str_to_func;

static int  __attribute__((unused)) EmplaceBenchmark(const std::string& fn_name, benchmark_func_t fn_ptr, const std::string& arg = "") {
  g_map_lock.lock();
  g_str_to_func.emplace(std::string(fn_name), std::make_pair(fn_ptr, arg));
  g_map_lock.unlock();
  return 0;
}

#define BIONIC_BENCHMARK(n) \
  int _bionic_benchmark_##n __attribute__((unused)) = EmplaceBenchmark(std::string(#n), reinterpret_cast<benchmark_func_t>(n))

#define BIONIC_BENCHMARK_WITH_ARG(n, arg) \
  int _bionic_benchmark_##n __attribute__((unused)) = EmplaceBenchmark(std::string(#n), reinterpret_cast<benchmark_func_t>(n), arg)


constexpr auto KB = 1024;

typedef struct {
  int cpu_to_lock = -1;
  long num_iterations = 0;
  std::string xmlpath;
  std::vector<std::string> extra_benchmarks;
} bench_opts_t;

// This function returns a pointer less than 2 * alignment + or_mask bytes into the array.
char* GetAlignedMemory(char* orig_ptr, size_t alignment, size_t or_mask);

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes);

wchar_t* GetAlignedPtr(std::vector<wchar_t>* buf, size_t alignment, size_t nbytes);

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fill_byte);

bool LockToCPU(int cpu_to_lock);
