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

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pthread_t (*pthread_self_fp)() = pthread_self;

static void BM_pthread_self(int iters) {
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_self_fp();
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

static void BM_pthread_setspecific(int iters) {
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
BENCHMARK(BM_pthread_setspecific);

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
  pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
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
  pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_pthread_mutex_lock_RECURSIVE);

static void BM_pthread_rw_lock_read(int iters) {
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
BENCHMARK(BM_pthread_rw_lock_read);

static void BM_pthread_rw_lock_write(int iters) {
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
BENCHMARK(BM_pthread_rw_lock_write);

static void* IdleThread(void*) {
  return NULL;
}

static void BM_pthread_create(int iters) {
  StopBenchmarkTiming();
  pthread_t thread;

  for (int i = 0; i < iters; ++i) {
    StartBenchmarkTiming();
    pthread_create(&thread, NULL, IdleThread, NULL);
    StopBenchmarkTiming();
    pthread_join(thread, NULL);
  }
}
BENCHMARK(BM_pthread_create);

static void* RunThread(void*) {
  StopBenchmarkTiming();
  return NULL;
}

static void BM_pthread_create_and_run(int iters) {
  StopBenchmarkTiming();
  pthread_t thread;

  for (int i = 0; i < iters; ++i) {
    StartBenchmarkTiming();
    pthread_create(&thread, NULL, RunThread, NULL);
    pthread_join(thread, NULL);
  }
}
BENCHMARK(BM_pthread_create_and_run);

static void* ExitThread(void*) {
  StartBenchmarkTiming();
  pthread_exit(NULL);
}

static void BM_pthread_exit_and_join(int iters) {
  StopBenchmarkTiming();
  pthread_t thread;

  for (int i = 0; i < iters; ++i) {
    pthread_create(&thread, NULL, ExitThread, NULL);
    pthread_join(thread, NULL);
    StopBenchmarkTiming();
  }
}
BENCHMARK(BM_pthread_exit_and_join);

static void BM_pthread_key_create(int iters) {
  StopBenchmarkTiming();
  pthread_key_t key;

  for (int i = 0; i < iters; ++i) {
    StartBenchmarkTiming();
    pthread_key_create(&key, NULL);
    StopBenchmarkTiming();
    pthread_key_delete(key);
  }
}
BENCHMARK(BM_pthread_key_create);

static void BM_pthread_key_delete(int iters) {
  StopBenchmarkTiming();
  pthread_key_t key;

  for (int i = 0; i < iters; ++i) {
    pthread_key_create(&key, NULL);
    StartBenchmarkTiming();
    pthread_key_delete(key);
    StopBenchmarkTiming();
  }
}
BENCHMARK(BM_pthread_key_delete);
