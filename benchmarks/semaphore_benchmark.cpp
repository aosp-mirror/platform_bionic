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

#include "benchmark.h"

#include <semaphore.h>

static void BM_semaphore_sem_getvalue(int iters) {
  StopBenchmarkTiming();
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    int dummy;
    sem_getvalue(&semaphore, &dummy);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_semaphore_sem_getvalue);

static void BM_semaphore_sem_wait_sem_post(int iters) {
  StopBenchmarkTiming();
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    sem_wait(&semaphore);
    sem_post(&semaphore);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_semaphore_sem_wait_sem_post);
