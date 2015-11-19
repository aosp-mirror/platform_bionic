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

#include "private/bionic_lock.h"

// User-level spinlocks can be hazardous to battery life on Android.
// We implement a simple compromise that behaves mostly like a spinlock,
// but prevents excessively long spinning.

struct pthread_spinlock_internal_t {
  Lock lock;
};

static_assert(sizeof(pthread_spinlock_t) == sizeof(pthread_spinlock_internal_t),
              "pthread_spinlock_t should actually be pthread_spinlock_internal_t.");

static_assert(alignof(pthread_spinlock_t) >= 4,
              "pthread_spinlock_t should fulfill the alignment of pthread_spinlock_internal_t.");

static inline pthread_spinlock_internal_t* __get_internal_spinlock(pthread_spinlock_t* lock) {
  return reinterpret_cast<pthread_spinlock_internal_t*>(lock);
}

int pthread_spin_init(pthread_spinlock_t* lock_interface, int pshared) {
  pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
  lock->lock.init(pshared);
  return 0;
}

int pthread_spin_destroy(pthread_spinlock_t* lock_interface) {
  pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
  return lock->lock.trylock() ? 0 : EBUSY;
}

int pthread_spin_trylock(pthread_spinlock_t* lock_interface) {
  pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
  return lock->lock.trylock() ? 0 : EBUSY;
}

int pthread_spin_lock(pthread_spinlock_t* lock_interface) {
  pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
  for (int i = 0; i < 10000; ++i) {
    if (lock->lock.trylock()) {
      return 0;
    }
  }
  lock->lock.lock();
  return 0;
}

int pthread_spin_unlock(pthread_spinlock_t* lock_interface) {
  pthread_spinlock_internal_t* lock = __get_internal_spinlock(lock_interface);
  lock->lock.unlock();
  return 0;
}
