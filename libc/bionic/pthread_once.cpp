/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include "private/bionic_futex.h"

#define ONCE_INITIALIZATION_NOT_YET_STARTED   0
#define ONCE_INITIALIZATION_UNDERWAY          1
#define ONCE_INITIALIZATION_COMPLETE          2

/* NOTE: this implementation doesn't support a init function that throws a C++ exception
 *       or calls fork()
 */
int pthread_once(pthread_once_t* once_control, void (*init_routine)(void)) {
  static_assert(sizeof(atomic_int) == sizeof(pthread_once_t),
                "pthread_once_t should actually be atomic_int in implementation.");

  // We prefer casting to atomic_int instead of declaring pthread_once_t to be atomic_int directly.
  // Because using the second method pollutes pthread.h, and causes an error when compiling libcxx.
  atomic_int* once_control_ptr = reinterpret_cast<atomic_int*>(once_control);

  // First check if the once is already initialized. This will be the common
  // case and we want to make this as fast as possible. Note that this still
  // requires a load_acquire operation here to ensure that all the
  // stores performed by the initialization function are observable on
  // this CPU after we exit.
  int old_value = atomic_load_explicit(once_control_ptr, memory_order_acquire);

  while (true) {
    if (__predict_true(old_value == ONCE_INITIALIZATION_COMPLETE)) {
      return 0;
    }

    // Try to atomically set the initialization underway flag. This requires a compare exchange
    // in a loop, and we may need to exit prematurely if the initialization is complete.
    if (!atomic_compare_exchange_weak_explicit(once_control_ptr, &old_value,
                                               ONCE_INITIALIZATION_UNDERWAY,
                                               memory_order_acquire, memory_order_acquire)) {
      continue;
    }

    if (old_value == ONCE_INITIALIZATION_NOT_YET_STARTED) {
      // We got here first, we can handle the initialization.
      (*init_routine)();

      // Do a store_release indicating that initialization is complete.
      atomic_store_explicit(once_control_ptr, ONCE_INITIALIZATION_COMPLETE, memory_order_release);

      // Wake up any waiters, if any.
      __futex_wake_ex(once_control_ptr, 0, INT_MAX);
      return 0;
    }

    // The initialization is underway, wait for its finish.
    __futex_wait_ex(once_control_ptr, 0, old_value, false, nullptr);
    old_value = atomic_load_explicit(once_control_ptr, memory_order_acquire);
  }
}
