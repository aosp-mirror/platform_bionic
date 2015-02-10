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
#include <semaphore.h>
#include <stdatomic.h>
#include <stdio.h>

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

/*
 *    This test reports the overhead of the underlying futex wake syscall on
 * the producer. It does not report the overhead from issuing the wake to the
 * point where the posted consumer thread wakes up. It suffers from
 * clock_gettime syscall overhead. Lock the CPU speed for consistent results
 * as we may not reach >50% cpu utilization.
 *
 *    We will run a background thread that catches the sem_post wakeup and
 * loops immediately returning back to sleep in sem_wait for the next one. This
 * thread is run with policy SCHED_OTHER (normal policy), a middle policy.
 *
 *    The primary thread will run at SCHED_IDLE (lowest priority policy) when
 * monitoring the background thread to detect when it hits sem_wait sleep. It
 * will do so with no clock running. Once we are ready, we will switch to
 * SCHED_FIFO (highest priority policy) to time the act of running sem_post
 * with the benchmark clock running. This ensures nothing else in the system
 * can preempt our timed activity, including the background thread. We are
 * also protected with the scheduling policy of letting a process hit a
 * resource limit rather than get hit with a context switch.
 *
 *    The background thread will start executing either on another CPU, or
 * after we back down from SCHED_FIFO, but certainly not in the context of
 * the timing of the sem_post.
 */
static atomic_int BM_semaphore_sem_post_running;

static void *BM_semaphore_sem_post_start_thread(void *obj) {
    sem_t *semaphore = reinterpret_cast<sem_t *>(obj);

    while ((BM_semaphore_sem_post_running > 0) && !sem_wait(semaphore)) {
        ;
    }
    BM_semaphore_sem_post_running = -1;
    return NULL;
}

static void BM_semaphore_sem_post(int iters) {
  StopBenchmarkTiming();

  sem_t semaphore;
  sem_init(&semaphore, 0, 0);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  BM_semaphore_sem_post_running = 1;
  struct sched_param param = { 0, };
  pthread_attr_setschedparam(&attr, &param);
  pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#ifdef PTHREAD_SET_INHERIT_SCHED
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
#endif
  pthread_t pthread;
  pthread_create(&pthread, &attr, BM_semaphore_sem_post_start_thread, &semaphore);
  pthread_attr_destroy(&attr);

  sched_setscheduler((pid_t)0, SCHED_IDLE, &param);
  for (int i = 0; i < iters; ++i) {
    int trys = 3, dummy = 0;
    do {
      if (BM_semaphore_sem_post_running < 0) {
        sched_setscheduler((pid_t)0, SCHED_OTHER, &param);
        fprintf(stderr, "BM_semaphore_sem_post: start_thread died unexpectedly\n");
        return;
      }
      sched_yield();
      sem_getvalue(&semaphore, &dummy);
      if (dummy < 0) {  // POSIX.1-2001 possibility 1
        break;
      }
      if (dummy == 0) { // POSIX.1-2001 possibility 2
        --trys;
      }
    } while (trys);
    param.sched_priority = 1;
    sched_setscheduler((pid_t)0, SCHED_FIFO, &param);
    StartBenchmarkTiming();
    sem_post(&semaphore);
    StopBenchmarkTiming(); // Remember to subtract clock syscall overhead
    param.sched_priority = 0;
    sched_setscheduler((pid_t)0, SCHED_IDLE, &param);
  }
  sched_setscheduler((pid_t)0, SCHED_OTHER, &param);

  if (BM_semaphore_sem_post_running > 0) {
    BM_semaphore_sem_post_running = 0;
  }
  do {
    sem_post(&semaphore);
    sched_yield();
  } while (!BM_semaphore_sem_post_running);
}
BENCHMARK(BM_semaphore_sem_post);

/*
 *    This test reports the overhead of sem_post to sem_wake. A circle of
 * num_semaphore - 1 threads are run on a set of semaphores to measure the
 * activity. One can calculate the sem_wake overhead alone by:
 *
 * BM_semaphore_sem_post_sem_wait - BM_semaphore_sem_post - BM_time_clock_gettime
 *
 * Differences will result if there are more threads than active processors,
 * there will be delay induced when scheduling the processes. This cost is
 * measured by trying different values of num_semaphore. The governor selected
 * will have a major impact on the results for a large number of threads.
 *
 *     To reduce the chances for threads racing ahead and not triggering the
 * futex, for example the background threads finish their job before the
 * sem_wait is hit in the main thread, the background threads will run at
 * batch priority and the main thread at fifo priority. This should generally
 * guarantee the main thread completes its task of priming itself with the
 * sem_wait before the other threads can start. In practice without the
 * sched mechanics here, this works on Android configured kernels, this is
 * insurance for wacky(tm) sched configurations.
 */
static void *BM_semaphore_sem_post_sem_wait_start_thread(void *obj) {
  sem_t *semaphore = reinterpret_cast<sem_t *>(obj);

  while ((BM_semaphore_sem_post_running > 0) && !sem_wait(semaphore)) {
    sem_post(semaphore + 1);
  }
  --BM_semaphore_sem_post_running;
  return NULL;
}

static void BM_semaphore_sem_post_sem_wait_num(int iters, int num_semaphore) {
  StopBenchmarkTiming();

  sem_t semaphore[num_semaphore];

  for (int i = 0; i < num_semaphore; ++i) {
    sem_init(semaphore + i, 0, 0);
  }

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  BM_semaphore_sem_post_running = 1;
  struct sched_param param = { 0, };
  pthread_attr_setschedparam(&attr, &param);
  pthread_attr_setschedpolicy(&attr, SCHED_BATCH);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#ifdef PTHREAD_SET_INHERIT_SCHED
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
#endif
  for (int i = 0; i < (num_semaphore - 1); ++i) {
    pthread_t pthread;
    pthread_create(&pthread, &attr, BM_semaphore_sem_post_sem_wait_start_thread, semaphore + i);
  }
  pthread_attr_destroy(&attr);
  sched_yield();

  param.sched_priority = 1;
  sched_setscheduler((pid_t)0, SCHED_FIFO, &param);

  StartBenchmarkTiming();

  for (int i = 0; i < iters; i += num_semaphore) {
    sem_post(semaphore);
    sem_wait(semaphore + num_semaphore - 1);
  }

  StopBenchmarkTiming();

  param.sched_priority = 0;
  sched_setscheduler((pid_t)0, SCHED_OTHER, &param);

  if (BM_semaphore_sem_post_running > 0) {
    BM_semaphore_sem_post_running = 0;
  }
  for (int i = 0;
       (i < (10 * num_semaphore)) && (BM_semaphore_sem_post_running > (1 - num_semaphore));
       ++i) {
    for (int j = 0; j < (num_semaphore - 1); ++j) {
      sem_post(semaphore + j);
    }
    sched_yield();
  }
}

static void BM_semaphore_sem_post_sem_wait_low(int iters) {
    BM_semaphore_sem_post_sem_wait_num(iters, 2);
}
BENCHMARK(BM_semaphore_sem_post_sem_wait_low);

static void BM_semaphore_sem_post_sem_wait_high(int iters) {
    BM_semaphore_sem_post_sem_wait_num(iters, 100);
}
BENCHMARK(BM_semaphore_sem_post_sem_wait_high);
