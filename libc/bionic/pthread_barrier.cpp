/*
 * Copyright (C) 2015 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include "private/bionic_futex.h"

int pthread_barrierattr_init(pthread_barrierattr_t* attr) {
  *attr = 0;
  return 0;
}

int pthread_barrierattr_destroy(pthread_barrierattr_t* attr) {
  *attr = 0;
  return 0;
}

int pthread_barrierattr_getpshared(pthread_barrierattr_t* attr, int* pshared) {
  *pshared = (*attr & 1) ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
  return 0;
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t* attr, int pshared) {
  if (pshared == PTHREAD_PROCESS_SHARED) {
    *attr |= 1;
  } else {
    *attr &= ~1;
  }
  return 0;
}

enum BarrierState {
  WAIT,
  RELEASE,
};

struct pthread_barrier_internal_t {
  // One barrier can be used for unlimited number of cycles. In each cycle, [init_count]
  // threads must call pthread_barrier_wait() before any of them successfully return from
  // the call. It is undefined behavior if there are more than [init_count] threads call
  // pthread_barrier_wait() in one cycle.
  uint32_t init_count;
  // Barrier state. It is WAIT if waiting for more threads to enter the barrier in this cycle,
  // otherwise threads are leaving the barrier.
  _Atomic(BarrierState) state;
  // Number of threads having entered but not left the barrier in this cycle.
  atomic_uint wait_count;
  // Whether the barrier is shared across processes.
  bool pshared;
  uint32_t __reserved[4];
};

static_assert(sizeof(pthread_barrier_t) == sizeof(pthread_barrier_internal_t),
              "pthread_barrier_t should actually be pthread_barrier_internal_t in implementation."
              );

static_assert(alignof(pthread_barrier_t) >= 4,
              "pthread_barrier_t should fulfill the alignment of pthread_barrier_internal_t.");

static inline pthread_barrier_internal_t* __get_internal_barrier(pthread_barrier_t* barrier) {
  return reinterpret_cast<pthread_barrier_internal_t*>(barrier);
}

int pthread_barrier_init(pthread_barrier_t* barrier_interface, const pthread_barrierattr_t* attr,
                         unsigned count) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
  if (count == 0) {
    return EINVAL;
  }
  barrier->init_count = count;
  atomic_init(&barrier->state, WAIT);
  atomic_init(&barrier->wait_count, 0);
  barrier->pshared = false;
  if (attr != nullptr && (*attr & 1)) {
    barrier->pshared = true;
  }
  return 0;
}

// According to POSIX standard, pthread_barrier_wait() synchronizes memory between participating
// threads. It means all memory operations made by participating threads before calling
// pthread_barrier_wait() can be seen by all participating threads after the function call.
// We establish this by making a happens-before relation between all threads entering the barrier
// with the last thread entering the barrier, and a happens-before relation between the last
// thread entering the barrier with all threads leaving the barrier.
int pthread_barrier_wait(pthread_barrier_t* barrier_interface) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);

  // Wait until all threads for the previous cycle have left the barrier. This is needed
  // as a participating thread can call pthread_barrier_wait() again before other
  // threads have left the barrier. Use acquire operation here to synchronize with
  // the last thread leaving the previous cycle, so we can read correct wait_count below.
  while(atomic_load_explicit(&barrier->state, memory_order_acquire) == RELEASE) {
    __futex_wait_ex(&barrier->state, barrier->pshared, RELEASE, false, nullptr);
  }

  uint32_t prev_wait_count = atomic_load_explicit(&barrier->wait_count, memory_order_relaxed);
  while (true) {
    // It happens when there are more than [init_count] threads trying to enter the barrier
    // at one cycle. We read the POSIX standard as disallowing this, since additional arriving
    // threads are not synchronized with respect to the barrier reset. We also don't know of
    // any reasonable cases in which this would be intentional.
    if (prev_wait_count >= barrier->init_count) {
      return EINVAL;
    }
    // Use memory_order_acq_rel operation here to synchronize between all threads entering
    // the barrier with the last thread entering the barrier.
    if (atomic_compare_exchange_weak_explicit(&barrier->wait_count, &prev_wait_count,
                                              prev_wait_count + 1u, memory_order_acq_rel,
                                              memory_order_relaxed)) {
      break;
    }
  }

  int result = 0;
  if (prev_wait_count + 1 == barrier->init_count) {
    result = PTHREAD_BARRIER_SERIAL_THREAD;
    if (prev_wait_count != 0) {
      // Use release operation here to synchronize between the last thread entering the
      // barrier with all threads leaving the barrier.
      atomic_store_explicit(&barrier->state, RELEASE, memory_order_release);
      __futex_wake_ex(&barrier->state, barrier->pshared, prev_wait_count);
    }
  } else {
    // Use acquire operation here to synchronize between the last thread entering the
    // barrier with all threads leaving the barrier.
    while (atomic_load_explicit(&barrier->state, memory_order_acquire) == WAIT) {
      __futex_wait_ex(&barrier->state, barrier->pshared, WAIT, false, nullptr);
    }
  }
  // Use release operation here to make it not reordered with previous operations.
  if (atomic_fetch_sub_explicit(&barrier->wait_count, 1, memory_order_release) == 1) {
    // Use release operation here to synchronize with threads entering the barrier for
    // the next cycle, or the thread calling pthread_barrier_destroy().
    atomic_store_explicit(&barrier->state, WAIT, memory_order_release);
    __futex_wake_ex(&barrier->state, barrier->pshared, barrier->init_count);
  }
  return result;
}

int pthread_barrier_destroy(pthread_barrier_t* barrier_interface) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
  if (barrier->init_count == 0) {
    return EINVAL;
  }
  // Use acquire operation here to synchronize with the last thread leaving the barrier.
  // So we can read correct wait_count below.
  while (atomic_load_explicit(&barrier->state, memory_order_acquire) == RELEASE) {
    __futex_wait_ex(&barrier->state, barrier->pshared, RELEASE, false, nullptr);
  }
  if (atomic_load_explicit(&barrier->wait_count, memory_order_relaxed) != 0) {
    return EBUSY;
  }
  barrier->init_count = 0;
  return 0;
}
