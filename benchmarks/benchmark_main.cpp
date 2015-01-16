/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <benchmark.h>

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <vector>

#include <inttypes.h>

static int64_t g_bytes_processed;
static int64_t g_benchmark_total_time_ns;
static int64_t g_benchmark_start_time_ns;
static int g_name_column_width = 20;

typedef std::vector<::testing::Benchmark*> BenchmarkList;

static BenchmarkList& Benchmarks() {
  static BenchmarkList benchmarks;
  return benchmarks;
}

// Similar to the code in art, but supporting both binary and decimal prefixes.
static std::string PrettyInt(uint64_t count, size_t base) {
  if (base != 2 && base != 10) abort();

  // The byte thresholds at which we display amounts. A count is displayed
  // in unit U when kUnitThresholds[U] <= bytes < kUnitThresholds[U+1].
  static const uint64_t kUnitThresholds2[] = {
    1024*1024*1024 /* Gi */, 2*1024*1024 /* Mi */, 3*1024 /* Ki */, 0,
  };
  static const uint64_t kUnitThresholds10[] = {
    1000*1000*1000 /* G */, 2*1000*1000 /* M */, 3*1000 /* k */, 0,
  };
  static const uint64_t kAmountPerUnit2[] = { 1024*1024*1024, 1024*1024, 1024, 1 };
  static const uint64_t kAmountPerUnit10[] = { 1000*1000*1000, 1000*1000, 1000, 1 };
  static const char* const kUnitStrings2[] = { "Gi", "Mi", "Ki", "" };
  static const char* const kUnitStrings10[] = { "G", "M", "k", "" };

  // Which set are we using?
  const uint64_t* kUnitThresholds = ((base == 2) ? kUnitThresholds2 : kUnitThresholds10);
  const uint64_t* kAmountPerUnit = ((base == 2) ? kAmountPerUnit2 : kAmountPerUnit10);
  const char* const* kUnitStrings = ((base == 2) ? kUnitStrings2 : kUnitStrings10);

  size_t i = 0;
  for (; kUnitThresholds[i] != 0; ++i) {
    if (count >= kUnitThresholds[i]) {
      break;
    }
  }
  char* s = NULL;
  asprintf(&s, "%" PRId64 "%s", count / kAmountPerUnit[i], kUnitStrings[i]);
  std::string result(s);
  free(s);
  return result;
}

static int Round(int n) {
  int base = 1;
  while (base*10 < n) {
    base *= 10;
  }
  if (n < 2*base) {
    return 2*base;
  }
  if (n < 5*base) {
    return 5*base;
  }
  return 10*base;
}

static int64_t NanoTime() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<int64_t>(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

namespace testing {

Benchmark* Benchmark::Arg(int arg) {
  args_.push_back(arg);
  return this;
}

const char* Benchmark::Name() {
  return name_;
}

bool Benchmark::ShouldRun(int argc, char* argv[]) {
  if (argc == 1) {
    return true;  // With no arguments, we run all benchmarks.
  }
  // Otherwise, we interpret each argument as a regular expression and
  // see if any of our benchmarks match.
  for (int i = 1; i < argc; i++) {
    regex_t re;
    if (regcomp(&re, argv[i], 0) != 0) {
      fprintf(stderr, "couldn't compile \"%s\" as a regular expression!\n", argv[i]);
      exit(EXIT_FAILURE);
    }
    int match = regexec(&re, name_, 0, NULL, 0);
    regfree(&re);
    if (match != REG_NOMATCH) {
      return true;
    }
  }
  return false;
}

void Benchmark::Register(const char* name, void (*fn)(int), void (*fn_range)(int, int)) {
  name_ = name;
  fn_ = fn;
  fn_range_ = fn_range;

  if (fn_ == NULL && fn_range_ == NULL) {
    fprintf(stderr, "%s: missing function\n", name_);
    exit(EXIT_FAILURE);
  }

  Benchmarks().push_back(this);
}

void Benchmark::Run() {
  if (fn_ != NULL) {
    RunWithArg(0);
  } else {
    if (args_.empty()) {
      fprintf(stderr, "%s: no args!\n", name_);
      exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < args_.size(); ++i) {
      RunWithArg(args_[i]);
    }
  }
}

void Benchmark::RunRepeatedlyWithArg(int iterations, int arg) {
  g_bytes_processed = 0;
  g_benchmark_total_time_ns = 0;
  g_benchmark_start_time_ns = NanoTime();
  if (fn_ != NULL) {
    fn_(iterations);
  } else {
    fn_range_(iterations, arg);
  }
  if (g_benchmark_start_time_ns != 0) {
    g_benchmark_total_time_ns += NanoTime() - g_benchmark_start_time_ns;
  }
}

void Benchmark::RunWithArg(int arg) {
  // Run once in case it's expensive.
  int iterations = 1;
  int64_t realStartTime = NanoTime();
  RunRepeatedlyWithArg(iterations, arg);
  int64_t realTotalTime = NanoTime() - realStartTime;
  while (realTotalTime < 1e9 && iterations < 1e8) {
    int last = iterations;
    if (realTotalTime/iterations == 0) {
      iterations = 1e9;
    } else {
      iterations = 1e9 / (realTotalTime/iterations);
    }
    iterations = std::max(last + 1, std::min(iterations + iterations/2, 100*last));
    iterations = Round(iterations);
    realStartTime = NanoTime();
    RunRepeatedlyWithArg(iterations, arg);
    realTotalTime = NanoTime() - realStartTime;
  }

  char throughput[100];
  throughput[0] = '\0';

  if (g_benchmark_total_time_ns > 0 && g_bytes_processed > 0) {
    double gib_processed = static_cast<double>(g_bytes_processed)/1e9;
    double seconds = static_cast<double>(g_benchmark_total_time_ns)/1e9;
    snprintf(throughput, sizeof(throughput), " %8.3f GiB/s", gib_processed/seconds);
  }

  char full_name[100];
  if (fn_range_ != NULL) {
    snprintf(full_name, sizeof(full_name), "%s/%s", name_, PrettyInt(arg, 2).c_str());
  } else {
    snprintf(full_name, sizeof(full_name), "%s", name_);
  }

  printf("%-*s %10s %10" PRId64 "%s\n",
         g_name_column_width, full_name,
         PrettyInt(iterations, 10).c_str(),
         g_benchmark_total_time_ns/iterations,
         throughput);
  fflush(stdout);
}

}  // namespace testing

void SetBenchmarkBytesProcessed(int64_t x) {
  g_bytes_processed = x;
}

void StopBenchmarkTiming() {
  if (g_benchmark_start_time_ns != 0) {
    g_benchmark_total_time_ns += NanoTime() - g_benchmark_start_time_ns;
  }
  g_benchmark_start_time_ns = 0;
}

void StartBenchmarkTiming() {
  if (g_benchmark_start_time_ns == 0) {
    g_benchmark_start_time_ns = NanoTime();
  }
}

int main(int argc, char* argv[]) {
  if (Benchmarks().empty()) {
    fprintf(stderr, "No benchmarks registered!\n");
    exit(EXIT_FAILURE);
  }

  for (auto& b : Benchmarks()) {
    int name_width = static_cast<int>(strlen(b->Name()));
    g_name_column_width = std::max(g_name_column_width, name_width);
  }

  bool need_header = true;
  for (auto& b : Benchmarks()) {
    if (b->ShouldRun(argc, argv)) {
      if (need_header) {
        printf("%-*s %10s %10s\n", g_name_column_width, "", "iterations", "ns/op");
        fflush(stdout);
        need_header = false;
      }
      b->Run();
    }
  }

  if (need_header) {
    fprintf(stderr, "No matching benchmarks!\n");
    fprintf(stderr, "Available benchmarks:\n");
    for (auto& b : Benchmarks()) {
      fprintf(stderr, "  %s\n", b->Name());
    }
    exit(EXIT_FAILURE);
  }

  return 0;
}
