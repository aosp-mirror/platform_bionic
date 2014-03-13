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

#include <pthread.h>

static void BM_pthread_self(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_self();
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_pthread_self);

static void BM_pthread_getspecific(int iters) {
  StopBenchmarkTiming();
  pthread_key_t key;
  pthread_key_create(&key, NULL);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_getspecific(key);
  }

  StopBenchmarkTiming();
  pthread_key_delete(key);
}
BENCHMARK(BM_pthread_getspecific);

static void DummyPthreadOnceInitFunction() {
}

static void BM_pthread_once(int iters) {
  StopBenchmarkTiming();
  pthread_once_t once = PTHREAD_ONCE_INIT;
  pthread_once(&once, DummyPthreadOnceInitFunction);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_once(&once, DummyPthreadOnceInitFunction);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_pthread_once);

static void BM_pthread_mutex_lock(int iters) {
  StopBenchmarkTiming();
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_pthread_mutex_lock);

static void BM_pthread_mutex_lock_ERRORCHECK(int iters) {
  StopBenchmarkTiming();
  pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_pthread_mutex_lock_ERRORCHECK);

static void BM_pthread_mutex_lock_RECURSIVE(int iters) {
  StopBenchmarkTiming();
  pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_pthread_mutex_lock_RECURSIVE);
