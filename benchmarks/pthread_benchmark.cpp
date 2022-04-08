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

#include <benchmark/benchmark.h>
#include "util.h"

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pthread_t (*pthread_self_fp)() = pthread_self;

static void BM_pthread_self(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_self_fp();
  }
}
BIONIC_BENCHMARK(BM_pthread_self);

static void BM_pthread_getspecific(benchmark::State& state) {
  pthread_key_t key;
  pthread_key_create(&key, nullptr);

  while (state.KeepRunning()) {
    pthread_getspecific(key);
  }

  pthread_key_delete(key);
}
BIONIC_BENCHMARK(BM_pthread_getspecific);

static void BM_pthread_setspecific(benchmark::State& state) {
  pthread_key_t key;
  pthread_key_create(&key, nullptr);

  while (state.KeepRunning()) {
    pthread_setspecific(key, nullptr);
  }

  pthread_key_delete(key);
}
BIONIC_BENCHMARK(BM_pthread_setspecific);

static void DummyPthreadOnceInitFunction() {
}

static void BM_pthread_once(benchmark::State& state) {
  static pthread_once_t once = PTHREAD_ONCE_INIT;
  pthread_once(&once, DummyPthreadOnceInitFunction);

  while (state.KeepRunning()) {
    pthread_once(&once, DummyPthreadOnceInitFunction);
  }
}
BIONIC_BENCHMARK(BM_pthread_once);

static void BM_pthread_mutex_lock(benchmark::State& state) {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  while (state.KeepRunning()) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }
}
BIONIC_BENCHMARK(BM_pthread_mutex_lock);

static void BM_pthread_mutex_lock_ERRORCHECK(benchmark::State& state) {
  pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

  while (state.KeepRunning()) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }
}
BIONIC_BENCHMARK(BM_pthread_mutex_lock_ERRORCHECK);

static void BM_pthread_mutex_lock_RECURSIVE(benchmark::State& state) {
  pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

  while (state.KeepRunning()) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }
}
BIONIC_BENCHMARK(BM_pthread_mutex_lock_RECURSIVE);

namespace {
struct PIMutex {
  pthread_mutex_t mutex;

  explicit PIMutex(int type) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, type);
    pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
    pthread_mutex_init(&mutex, &attr);
    pthread_mutexattr_destroy(&attr);
  }

  ~PIMutex() {
    pthread_mutex_destroy(&mutex);
  }
};
}

static void BM_pthread_mutex_lock_PI(benchmark::State& state) {
  PIMutex m(PTHREAD_MUTEX_NORMAL);

  while (state.KeepRunning()) {
    pthread_mutex_lock(&m.mutex);
    pthread_mutex_unlock(&m.mutex);
  }
}
BIONIC_BENCHMARK(BM_pthread_mutex_lock_PI);

static void BM_pthread_mutex_lock_ERRORCHECK_PI(benchmark::State& state) {
  PIMutex m(PTHREAD_MUTEX_ERRORCHECK);

  while (state.KeepRunning()) {
    pthread_mutex_lock(&m.mutex);
    pthread_mutex_unlock(&m.mutex);
  }
}
BIONIC_BENCHMARK(BM_pthread_mutex_lock_ERRORCHECK_PI);

static void BM_pthread_mutex_lock_RECURSIVE_PI(benchmark::State& state) {
  PIMutex m(PTHREAD_MUTEX_RECURSIVE);

  while (state.KeepRunning()) {
    pthread_mutex_lock(&m.mutex);
    pthread_mutex_unlock(&m.mutex);
  }
}
BIONIC_BENCHMARK(BM_pthread_mutex_lock_RECURSIVE_PI);

static void BM_pthread_rwlock_read(benchmark::State& state) {
  pthread_rwlock_t lock;
  pthread_rwlock_init(&lock, nullptr);

  while (state.KeepRunning()) {
    pthread_rwlock_rdlock(&lock);
    pthread_rwlock_unlock(&lock);
  }

  pthread_rwlock_destroy(&lock);
}
BIONIC_BENCHMARK(BM_pthread_rwlock_read);

static void BM_pthread_rwlock_write(benchmark::State& state) {
  pthread_rwlock_t lock;
  pthread_rwlock_init(&lock, nullptr);

  while (state.KeepRunning()) {
    pthread_rwlock_wrlock(&lock);
    pthread_rwlock_unlock(&lock);
  }

  pthread_rwlock_destroy(&lock);
}
BIONIC_BENCHMARK(BM_pthread_rwlock_write);

static void* IdleThread(void*) {
  return nullptr;
}

static void BM_pthread_create(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_t thread;
    pthread_create(&thread, nullptr, IdleThread, nullptr);
    state.PauseTiming();
    pthread_join(thread, nullptr);
    state.ResumeTiming();
  }
}
BIONIC_BENCHMARK(BM_pthread_create);

static void* RunThread(void*) {
  return nullptr;
}

static void BM_pthread_create_and_run(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_t thread;
    pthread_create(&thread, nullptr, RunThread, &state);
    pthread_join(thread, nullptr);
  }
}
BIONIC_BENCHMARK(BM_pthread_create_and_run);

static void* ExitThread(void*) {
  pthread_exit(nullptr);
}

static void BM_pthread_exit_and_join(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_t thread;
    pthread_create(&thread, nullptr, ExitThread, nullptr);
    pthread_join(thread, nullptr);
  }
}
BIONIC_BENCHMARK(BM_pthread_exit_and_join);

static void BM_pthread_key_create(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_key_t key;
    pthread_key_create(&key, nullptr);

    state.PauseTiming();
    pthread_key_delete(key);
    state.ResumeTiming();
  }
}
BIONIC_BENCHMARK(BM_pthread_key_create);

static void BM_pthread_key_delete(benchmark::State& state) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    pthread_key_t key;
    pthread_key_create(&key, nullptr);
    state.ResumeTiming();

    pthread_key_delete(key);
  }
}
BIONIC_BENCHMARK(BM_pthread_key_delete);
