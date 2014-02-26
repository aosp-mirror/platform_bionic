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

#include "private/bionic_atomic_inline.h"
#include "private/bionic_futex.h"

#define ONCE_INITIALIZING           (1 << 0)
#define ONCE_COMPLETED              (1 << 1)

/* NOTE: this implementation doesn't support a init function that throws a C++ exception
 *       or calls fork()
 */
int pthread_once(pthread_once_t* once_control, void (*init_routine)(void)) {
  volatile pthread_once_t* once_control_ptr = once_control;

  // PTHREAD_ONCE_INIT is 0, we use the following bit flags
  //   bit 0 set  -> initialization is under way
  //   bit 1 set  -> initialization is complete

  // First check if the once is already initialized. This will be the common
  // case and we want to make this as fast as possible. Note that this still
  // requires a load_acquire operation here to ensure that all the
  // stores performed by the initialization function are observable on
  // this CPU after we exit.
  if (__predict_true((*once_control_ptr & ONCE_COMPLETED) != 0)) {
    ANDROID_MEMBAR_FULL();
    return 0;
  }

  while (true) {
    // Try to atomically set the INITIALIZING flag.
    // This requires a cmpxchg loop, and we may need
    // to exit prematurely if we detect that
    // COMPLETED is now set.
    int32_t  old_value, new_value;

    do {
      old_value = *once_control_ptr;
      if ((old_value & ONCE_COMPLETED) != 0) {
        break;
      }

      new_value = old_value | ONCE_INITIALIZING;
    } while (__bionic_cmpxchg(old_value, new_value, once_control_ptr) != 0);

    if ((old_value & ONCE_COMPLETED) != 0) {
      // We detected that COMPLETED was set while in our loop.
      ANDROID_MEMBAR_FULL();
      return 0;
    }

    if ((old_value & ONCE_INITIALIZING) == 0) {
      // We got there first, we can jump out of the loop to handle the initialization.
      break;
    }

    // Another thread is running the initialization and hasn't completed
    // yet, so wait for it, then try again.
    __futex_wait_ex(once_control_ptr, 0, old_value, NULL);
  }

  // Call the initialization function.
  (*init_routine)();

  // Do a store_release indicating that initialization is complete.
  ANDROID_MEMBAR_FULL();
  *once_control_ptr = ONCE_COMPLETED;

  // Wake up any waiters, if any.
  __futex_wake_ex(once_control_ptr, 0, INT_MAX);

  return 0;
}
