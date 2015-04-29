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

#include <benchmark/Benchmark.h>

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pthread_t (*pthread_self_fp)() = pthread_self;

BENCHMARK_NO_ARG(BM_pthread_self);
void BM_pthread_self::Run(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_self_fp();
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_pthread_getspecific);
void BM_pthread_getspecific::Run(int iters) {
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

BENCHMARK_NO_ARG(BM_pthread_setspecific);
void BM_pthread_setspecific::Run(int iters) {
  StopBenchmarkTiming();
  pthread_key_t key;
  pthread_key_create(&key, NULL);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_setspecific(key, NULL);
  }

  StopBenchmarkTiming();
  pthread_key_delete(key);
}

static void DummyPthreadOnceInitFunction() {
}

BENCHMARK_NO_ARG(BM_pthread_once);
void BM_pthread_once::Run(int iters) {
  StopBenchmarkTiming();
  pthread_once_t once = PTHREAD_ONCE_INIT;
  pthread_once(&once, DummyPthreadOnceInitFunction);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_once(&once, DummyPthreadOnceInitFunction);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_pthread_mutex_lock);
void BM_pthread_mutex_lock::Run(int iters) {
  StopBenchmarkTiming();
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_pthread_mutex_lock_ERRORCHECK);
void BM_pthread_mutex_lock_ERRORCHECK::Run(int iters) {
  StopBenchmarkTiming();
  pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_pthread_mutex_lock_RECURSIVE);
void BM_pthread_mutex_lock_RECURSIVE::Run(int iters) {
  StopBenchmarkTiming();
  pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}

BENCHMARK_NO_ARG(BM_pthread_rwlock_read);
void BM_pthread_rwlock_read::Run(int iters) {
  StopBenchmarkTiming();
  pthread_rwlock_t lock;
  pthread_rwlock_init(&lock, NULL);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_rwlock_rdlock(&lock);
    pthread_rwlock_unlock(&lock);
  }

  StopBenchmarkTiming();
  pthread_rwlock_destroy(&lock);
}

BENCHMARK_NO_ARG(BM_pthread_rwlock_write);
void BM_pthread_rwlock_write::Run(int iters) {
  StopBenchmarkTiming();
  pthread_rwlock_t lock;
  pthread_rwlock_init(&lock, NULL);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_rwlock_wrlock(&lock);
    pthread_rwlock_unlock(&lock);
  }

  StopBenchmarkTiming();
  pthread_rwlock_destroy(&lock);
}

static void* IdleThread(void*) {
  return NULL;
}

BENCHMARK_NO_ARG(BM_pthread_create);
void BM_pthread_create::Run(int iters) {
  StopBenchmarkTiming();
  pthread_t thread;

  for (int i = 0; i < iters; ++i) {
    StartBenchmarkTiming();
    pthread_create(&thread, NULL, IdleThread, NULL);
    StopBenchmarkTiming();
    pthread_join(thread, NULL);
  }
}

static void* RunThread(void* arg) {
  ::testing::Benchmark* benchmark = reinterpret_cast<::testing::Benchmark*>(arg);
  benchmark->StopBenchmarkTiming();
  return NULL;
}

BENCHMARK_NO_ARG(BM_pthread_create_and_run);
void BM_pthread_create_and_run::Run(int iters) {
  StopBenchmarkTiming();
  pthread_t thread;

  for (int i = 0; i < iters; ++i) {
    StartBenchmarkTiming();
    pthread_create(&thread, NULL, RunThread, this);
    pthread_join(thread, NULL);
  }
}

static void* ExitThread(void* arg) {
  ::testing::Benchmark* benchmark = reinterpret_cast<::testing::Benchmark*>(arg);
  benchmark->StartBenchmarkTiming();
  pthread_exit(NULL);
}

BENCHMARK_NO_ARG(BM_pthread_exit_and_join);
void BM_pthread_exit_and_join::Run(int iters) {
  StopBenchmarkTiming();
  pthread_t thread;

  for (int i = 0; i < iters; ++i) {
    pthread_create(&thread, NULL, ExitThread, this);
    pthread_join(thread, NULL);
    StopBenchmarkTiming();
  }
}

BENCHMARK_NO_ARG(BM_pthread_key_create);
void BM_pthread_key_create::Run(int iters) {
  StopBenchmarkTiming();
  pthread_key_t key;

  for (int i = 0; i < iters; ++i) {
    StartBenchmarkTiming();
    pthread_key_create(&key, NULL);
    StopBenchmarkTiming();
    pthread_key_delete(key);
  }
}

BENCHMARK_NO_ARG(BM_pthread_key_delete);
void BM_pthread_key_delete::Run(int iters) {
  StopBenchmarkTiming();
  pthread_key_t key;

  for (int i = 0; i < iters; ++i) {
    pthread_key_create(&key, NULL);
    StartBenchmarkTiming();
    pthread_key_delete(key);
    StopBenchmarkTiming();
  }
}
