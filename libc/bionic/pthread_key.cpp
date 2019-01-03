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

#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>

#include "private/bionic_defs.h"
#include "private/bionic_tls.h"
#include "pthread_internal.h"

typedef void (*key_destructor_t)(void*);

#define SEQ_KEY_IN_USE_BIT     0

#define SEQ_INCREMENT_STEP  (1 << SEQ_KEY_IN_USE_BIT)

// pthread_key_internal_t records the use of each pthread key slot:
//   seq records the state of the slot.
//      bit 0 is 1 when the key is in use, 0 when it is unused. Each time we create or delete the
//      pthread key in the slot, we increse the seq by 1 (which inverts bit 0). The reason to use
//      a sequence number instead of a boolean value here is that when the key slot is deleted and
//      reused for a new key, pthread_getspecific will not return stale data.
//   key_destructor records the destructor called at thread exit.
struct pthread_key_internal_t {
  atomic_uintptr_t seq;
  atomic_uintptr_t key_destructor;
};

static pthread_key_internal_t key_map[BIONIC_PTHREAD_KEY_COUNT];

static inline bool SeqOfKeyInUse(uintptr_t seq) {
  return seq & (1 << SEQ_KEY_IN_USE_BIT);
}

#define KEY_VALID_FLAG (1 << 31)

static_assert(sizeof(pthread_key_t) == sizeof(int) && static_cast<pthread_key_t>(-1) < 0,
              "pthread_key_t should be typedef to int");

static inline bool KeyInValidRange(pthread_key_t key) {
  // key < 0 means bit 31 is set.
  // Then key < (2^31 | BIONIC_PTHREAD_KEY_COUNT) means the index part of key < BIONIC_PTHREAD_KEY_COUNT.
  return (key < (KEY_VALID_FLAG | BIONIC_PTHREAD_KEY_COUNT));
}

static inline pthread_key_data_t* get_thread_key_data() {
  return __get_bionic_tls().key_data;
}

// Called from pthread_exit() to remove all pthread keys. This must call the destructor of
// all keys that have a non-NULL data value and a non-NULL destructor.
__LIBC_HIDDEN__ void pthread_key_clean_all() {
  // Because destructors can do funky things like deleting/creating other keys,
  // we need to implement this in a loop.
  pthread_key_data_t* key_data = get_thread_key_data();
  for (size_t rounds = PTHREAD_DESTRUCTOR_ITERATIONS; rounds > 0; --rounds) {
    size_t called_destructor_count = 0;
    for (size_t i = 0; i < BIONIC_PTHREAD_KEY_COUNT; ++i) {
      uintptr_t seq = atomic_load_explicit(&key_map[i].seq, memory_order_relaxed);
      if (SeqOfKeyInUse(seq) && seq == key_data[i].seq && key_data[i].data != nullptr) {
        // Other threads may be calling pthread_key_delete/pthread_key_create while current thread
        // is exiting. So we need to ensure we read the right key_destructor.
        // We can rely on a user-established happens-before relationship between the creation and
        // use of pthread key to ensure that we're not getting an earlier key_destructor.
        // To avoid using the key_destructor of the newly created key in the same slot, we need to
        // recheck the sequence number after reading key_destructor. As a result, we either see the
        // right key_destructor, or the sequence number must have changed when we reread it below.
        key_destructor_t key_destructor = reinterpret_cast<key_destructor_t>(
          atomic_load_explicit(&key_map[i].key_destructor, memory_order_relaxed));
        if (key_destructor == nullptr) {
          continue;
        }
        atomic_thread_fence(memory_order_acquire);
        if (atomic_load_explicit(&key_map[i].seq, memory_order_relaxed) != seq) {
           continue;
        }

        // We need to clear the key data now, this will prevent the destructor (or a later one)
        // from seeing the old value if it calls pthread_getspecific().
        // We don't do this if 'key_destructor == NULL' just in case another destructor
        // function is responsible for manually releasing the corresponding data.
        void* data = key_data[i].data;
        key_data[i].data = nullptr;

        (*key_destructor)(data);
        ++called_destructor_count;
      }
    }

    // If we didn't call any destructors, there is no need to check the pthread keys again.
    if (called_destructor_count == 0) {
      break;
    }
  }
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_key_create(pthread_key_t* key, void (*key_destructor)(void*)) {
  for (size_t i = 0; i < BIONIC_PTHREAD_KEY_COUNT; ++i) {
    uintptr_t seq = atomic_load_explicit(&key_map[i].seq, memory_order_relaxed);
    while (!SeqOfKeyInUse(seq)) {
      if (atomic_compare_exchange_weak(&key_map[i].seq, &seq, seq + SEQ_INCREMENT_STEP)) {
        atomic_store(&key_map[i].key_destructor, reinterpret_cast<uintptr_t>(key_destructor));
        *key = i | KEY_VALID_FLAG;
        return 0;
      }
    }
  }
  return EAGAIN;
}

// Deletes a pthread_key_t. note that the standard mandates that this does
// not call the destructors for non-NULL key values. Instead, it is the
// responsibility of the caller to properly dispose of the corresponding data
// and resources, using any means it finds suitable.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_key_delete(pthread_key_t key) {
  if (__predict_false(!KeyInValidRange(key))) {
    return EINVAL;
  }
  key &= ~KEY_VALID_FLAG;
  // Increase seq to invalidate values in all threads.
  uintptr_t seq = atomic_load_explicit(&key_map[key].seq, memory_order_relaxed);
  if (SeqOfKeyInUse(seq)) {
    if (atomic_compare_exchange_strong(&key_map[key].seq, &seq, seq + SEQ_INCREMENT_STEP)) {
      return 0;
    }
  }
  return EINVAL;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void* pthread_getspecific(pthread_key_t key) {
  if (__predict_false(!KeyInValidRange(key))) {
    return nullptr;
  }
  key &= ~KEY_VALID_FLAG;
  uintptr_t seq = atomic_load_explicit(&key_map[key].seq, memory_order_relaxed);
  pthread_key_data_t* data = &get_thread_key_data()[key];
  // It is user's responsibility to synchornize between the creation and use of pthread keys,
  // so we use memory_order_relaxed when checking the sequence number.
  if (__predict_true(SeqOfKeyInUse(seq) && data->seq == seq)) {
    return data->data;
  }
  // We arrive here when current thread holds the seq of an deleted pthread key. So the
  // data is for the deleted pthread key, and should be cleared.
  data->data = nullptr;
  return nullptr;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_setspecific(pthread_key_t key, const void* ptr) {
  if (__predict_false(!KeyInValidRange(key))) {
    return EINVAL;
  }
  key &= ~KEY_VALID_FLAG;
  uintptr_t seq = atomic_load_explicit(&key_map[key].seq, memory_order_relaxed);
  if (__predict_true(SeqOfKeyInUse(seq))) {
    pthread_key_data_t* data = &get_thread_key_data()[key];
    data->seq = seq;
    data->data = const_cast<void*>(ptr);
    return 0;
  }
  return EINVAL;
}
