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

#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include <benchmark/benchmark.h>
#include "util.h"

static void BM_semaphore_sem_getvalue(benchmark::State& state) {
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);

  while (state.KeepRunning()) {
    int unused;
    sem_getvalue(&semaphore, &unused);
  }
}
BIONIC_BENCHMARK(BM_semaphore_sem_getvalue);

static void BM_semaphore_sem_wait_sem_post(benchmark::State& state) {
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);

  while (state.KeepRunning()) {
    sem_wait(&semaphore);
    sem_post(&semaphore);
  }
}
BIONIC_BENCHMARK(BM_semaphore_sem_wait_sem_post);
