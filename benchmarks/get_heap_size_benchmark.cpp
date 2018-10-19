/*
 * Copyright (C) 2018 The Android Open Source Project
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

// This measures the time required to compute two different native heap size metrics that
// we might use to guide GC triggering.

#include <atomic>
#include <err.h>
#include <fcntl.h>
#include <malloc.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <benchmark/benchmark.h>
#include "util.h"

static volatile size_t sink;

static constexpr int NTHREADS = 5;

static std::atomic<int> thread_count;

static void* thread_func(void* arg) {
  *reinterpret_cast<void**>(arg) = malloc(17);
  thread_count++;
  return nullptr;
}

static void BM_mallinfo(benchmark::State& state) {
  pthread_t t[NTHREADS];
  void* object[NTHREADS];
  // Create threads to populate thread-local arenas, if any.
  thread_count = 0;
  for (int i = 0; i < 5; i++) {
    int res = pthread_create(t + i, NULL, thread_func, object + i);
    if (res != 0) {
      errx(1, "ERROR: pthread_create failed");
    }
  }
  while (thread_count != NTHREADS) {
    sched_yield();
  }
  for (auto _ : state) {
    sink = mallinfo().uordblks;
  }
  for (int i = 0; i < 5; i++) {
    int res = pthread_join(t[i], NULL);
    if (res != 0) {
      errx(1, "ERROR: pthread_join failed");
    }
    free(object[i]);
  }
}

BIONIC_BENCHMARK(BM_mallinfo);

static constexpr int BUF_SIZE = 100;

static void BM_read_statm(benchmark::State& state) {
  char buf[BUF_SIZE];
  int statm_fd = open("/proc/self/statm", O_RDONLY);
  if (statm_fd == -1) {
    errx(1, "ERROR: Open failed");
  }
  for (auto _ : state) {
    off_t result = lseek(statm_fd, 0 /* offset */, SEEK_SET);
    if (result == (off_t) -1) {
      errx(1, "ERROR: lseek to beginning failed");
    }
    ssize_t nread = read(statm_fd, buf, BUF_SIZE);
    if (nread < 13 || nread > 50) {
      errx(1, "ERROR: Implausible read result; result = %zd", nread);
    }
    // We should really add the parsing overhead for the result; but that should be < 100 nsecs
    // if we tweak it enough.
  }
  if (close(statm_fd) == -1) {
    errx(1, "ERROR: Close failed");
  }
}

BIONIC_BENCHMARK(BM_read_statm);
