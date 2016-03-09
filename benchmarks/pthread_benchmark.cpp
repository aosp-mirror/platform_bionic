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

// Stop GCC optimizing out our pure function.
/* Must not be static! */ pthread_t (*pthread_self_fp)() = pthread_self;

static void BM_pthread_self(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_self_fp();
  }
}
BENCHMARK(BM_pthread_self);

static void BM_pthread_getspecific(benchmark::State& state) {
  pthread_key_t key;
  pthread_key_create(&key, NULL);

  while (state.KeepRunning()) {
    pthread_getspecific(key);
  }

  pthread_key_delete(key);
}
BENCHMARK(BM_pthread_getspecific);

static void BM_pthread_setspecific(benchmark::State& state) {
  pthread_key_t key;
  pthread_key_create(&key, NULL);

  while (state.KeepRunning()) {
    pthread_setspecific(key, NULL);
  }

  pthread_key_delete(key);
}
BENCHMARK(BM_pthread_setspecific);

static void DummyPthreadOnceInitFunction() {
}

static void BM_pthread_once(benchmark::State& state) {
  pthread_once_t once = PTHREAD_ONCE_INIT;
  pthread_once(&once, DummyPthreadOnceInitFunction);

  while (state.KeepRunning()) {
    pthread_once(&once, DummyPthreadOnceInitFunction);
  }
}
BENCHMARK(BM_pthread_once);

static void BM_pthread_mutex_lock(benchmark::State& state) {
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  while (state.KeepRunning()) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }
}
BENCHMARK(BM_pthread_mutex_lock);

static void BM_pthread_mutex_lock_ERRORCHECK(benchmark::State& state) {
  pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

  while (state.KeepRunning()) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }
}
BENCHMARK(BM_pthread_mutex_lock_ERRORCHECK);

static void BM_pthread_mutex_lock_RECURSIVE(benchmark::State& state) {
  pthread_mutex_t mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

  while (state.KeepRunning()) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_unlock(&mutex);
  }
}
BENCHMARK(BM_pthread_mutex_lock_RECURSIVE);

static void BM_pthread_rwlock_read(benchmark::State& state) {
  pthread_rwlock_t lock;
  pthread_rwlock_init(&lock, NULL);

  while (state.KeepRunning()) {
    pthread_rwlock_rdlock(&lock);
    pthread_rwlock_unlock(&lock);
  }

  pthread_rwlock_destroy(&lock);
}
BENCHMARK(BM_pthread_rwlock_read);

static void BM_pthread_rwlock_write(benchmark::State& state) {
  pthread_rwlock_t lock;
  pthread_rwlock_init(&lock, NULL);

  while (state.KeepRunning()) {
    pthread_rwlock_wrlock(&lock);
    pthread_rwlock_unlock(&lock);
  }

  pthread_rwlock_destroy(&lock);
}
BENCHMARK(BM_pthread_rwlock_write);

static void* IdleThread(void*) {
  return NULL;
}

static void BM_pthread_create(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_t thread;
    pthread_create(&thread, NULL, IdleThread, NULL);
    state.PauseTiming();
    pthread_join(thread, NULL);
    state.ResumeTiming();
  }
}
BENCHMARK(BM_pthread_create);

static void* RunThread(void* arg) {
  benchmark::State& state = *reinterpret_cast<benchmark::State*>(arg);
  state.PauseTiming();
  return NULL;
}

static void BM_pthread_create_and_run(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_t thread;
    pthread_create(&thread, NULL, RunThread, &state);
    pthread_join(thread, NULL);
    state.ResumeTiming();
  }
}
BENCHMARK(BM_pthread_create_and_run);

static void* ExitThread(void* arg) {
  benchmark::State& state = *reinterpret_cast<benchmark::State*>(arg);
  state.ResumeTiming();
  pthread_exit(NULL);
}

static void BM_pthread_exit_and_join(benchmark::State& state) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    pthread_t thread;
    pthread_create(&thread, NULL, ExitThread, &state);
    pthread_join(thread, NULL);
  }
}
BENCHMARK(BM_pthread_exit_and_join);

static void BM_pthread_key_create(benchmark::State& state) {
  while (state.KeepRunning()) {
    pthread_key_t key;
    pthread_key_create(&key, NULL);

    state.PauseTiming();
    pthread_key_delete(key);
    state.ResumeTiming();
  }
}
BENCHMARK(BM_pthread_key_create);

static void BM_pthread_key_delete(benchmark::State& state) {
  while (state.KeepRunning()) {
    state.PauseTiming();
    pthread_key_t key;
    pthread_key_create(&key, NULL);
    state.ResumeTiming();

    pthread_key_delete(key);
  }
}
BENCHMARK(BM_pthread_key_delete);
