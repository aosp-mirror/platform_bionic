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

#include <gtest/gtest.h>

// The real <stdatomic.h> checks for the availability of C++'s atomics and uses them if present. Since
// we want to test the libc versions, we instead include <bits/stdatomic.h> where they're actually defined.
#include <bits/stdatomic.h>

#include <pthread.h>
#include <stdint.h>

TEST(stdatomic, LOCK_FREE) {
  ASSERT_TRUE(ATOMIC_BOOL_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_CHAR16_T_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_CHAR32_T_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_CHAR_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_INT_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_LLONG_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_LONG_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_POINTER_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_SHORT_LOCK_FREE);
  ASSERT_TRUE(ATOMIC_WCHAR_T_LOCK_FREE);
}

TEST(stdatomic, init) {
  atomic_int v = ATOMIC_VAR_INIT(123);
  ASSERT_EQ(123, atomic_load(&v));

  atomic_init(&v, 456);
  ASSERT_EQ(456, atomic_load(&v));

  atomic_flag f = ATOMIC_FLAG_INIT;
  ASSERT_FALSE(atomic_flag_test_and_set(&f));
}

TEST(stdatomic, atomic_thread_fence) {
  atomic_thread_fence(memory_order_relaxed);
  atomic_thread_fence(memory_order_consume);
  atomic_thread_fence(memory_order_acquire);
  atomic_thread_fence(memory_order_release);
  atomic_thread_fence(memory_order_acq_rel);
  atomic_thread_fence(memory_order_seq_cst);
}

TEST(stdatomic, atomic_signal_fence) {
  atomic_signal_fence(memory_order_relaxed);
  atomic_signal_fence(memory_order_consume);
  atomic_signal_fence(memory_order_acquire);
  atomic_signal_fence(memory_order_release);
  atomic_signal_fence(memory_order_acq_rel);
  atomic_signal_fence(memory_order_seq_cst);
}

TEST(stdatomic, atomic_is_lock_free) {
  atomic_char small;
  ASSERT_TRUE(atomic_is_lock_free(&small));
  atomic_intmax_t big;
  ASSERT_TRUE(atomic_is_lock_free(&big));
}

TEST(stdatomic, atomic_flag) {
  atomic_flag f = ATOMIC_FLAG_INIT;
  ASSERT_FALSE(atomic_flag_test_and_set(&f));
  ASSERT_TRUE(atomic_flag_test_and_set(&f));

  atomic_flag_clear(&f);

  ASSERT_FALSE(atomic_flag_test_and_set_explicit(&f, memory_order_relaxed));
  ASSERT_TRUE(atomic_flag_test_and_set_explicit(&f, memory_order_relaxed));

  atomic_flag_clear_explicit(&f, memory_order_relaxed);
  ASSERT_FALSE(atomic_flag_test_and_set_explicit(&f, memory_order_relaxed));
}

TEST(stdatomic, atomic_store) {
  atomic_int i;
  atomic_store(&i, 123);
  ASSERT_EQ(123, atomic_load(&i));
  atomic_store_explicit(&i, 123, memory_order_relaxed);
  ASSERT_EQ(123, atomic_load_explicit(&i, memory_order_relaxed));
}

TEST(stdatomic, atomic_exchange) {
  atomic_int i;
  atomic_store(&i, 123);
  ASSERT_EQ(123, atomic_exchange(&i, 456));
  ASSERT_EQ(456, atomic_exchange_explicit(&i, 123, memory_order_relaxed));
}

TEST(stdatomic, atomic_compare_exchange) {
  atomic_int i;
  int expected;

  atomic_store(&i, 123);
  expected = 123;
  ASSERT_TRUE(atomic_compare_exchange_strong(&i, &expected, 456));
  ASSERT_FALSE(atomic_compare_exchange_strong(&i, &expected, 456));
  ASSERT_EQ(456, expected);

  atomic_store(&i, 123);
  expected = 123;
  ASSERT_TRUE(atomic_compare_exchange_strong_explicit(&i, &expected, 456, memory_order_relaxed,
          memory_order_relaxed));
  ASSERT_FALSE(atomic_compare_exchange_strong_explicit(&i, &expected, 456, memory_order_relaxed,
          memory_order_relaxed));
  ASSERT_EQ(456, expected);

  atomic_store(&i, 123);
  expected = 123;
  int iter_count = 0;
  do {
    ++iter_count;
    ASSERT_LT(iter_count, 100);  // Arbitrary limit on spurious compare_exchange failures.
    ASSERT_EQ(expected, 123);
  } while(!atomic_compare_exchange_weak(&i, &expected, 456));
  ASSERT_FALSE(atomic_compare_exchange_weak(&i, &expected, 456));
  ASSERT_EQ(456, expected);

  atomic_store(&i, 123);
  expected = 123;
  iter_count = 0;
  do {
    ++iter_count;
    ASSERT_LT(iter_count, 100);
    ASSERT_EQ(expected, 123);
  } while(!atomic_compare_exchange_weak_explicit(&i, &expected, 456, memory_order_relaxed,
          memory_order_relaxed));
  ASSERT_FALSE(atomic_compare_exchange_weak_explicit(&i, &expected, 456, memory_order_relaxed,
          memory_order_relaxed));
  ASSERT_EQ(456, expected);
}

TEST(stdatomic, atomic_fetch_add) {
  atomic_int i = ATOMIC_VAR_INIT(123);
  ASSERT_EQ(123, atomic_fetch_add(&i, 1));
  ASSERT_EQ(124, atomic_fetch_add_explicit(&i, 1, memory_order_relaxed));
  ASSERT_EQ(125, atomic_load(&i));
}

TEST(stdatomic, atomic_fetch_sub) {
  atomic_int i = ATOMIC_VAR_INIT(123);
  ASSERT_EQ(123, atomic_fetch_sub(&i, 1));
  ASSERT_EQ(122, atomic_fetch_sub_explicit(&i, 1, memory_order_relaxed));
  ASSERT_EQ(121, atomic_load(&i));
}

TEST(stdatomic, atomic_fetch_or) {
  atomic_int i = ATOMIC_VAR_INIT(0x100);
  ASSERT_EQ(0x100, atomic_fetch_or(&i, 0x020));
  ASSERT_EQ(0x120, atomic_fetch_or_explicit(&i, 0x003, memory_order_relaxed));
  ASSERT_EQ(0x123, atomic_load(&i));
}

TEST(stdatomic, atomic_fetch_xor) {
  atomic_int i = ATOMIC_VAR_INIT(0x100);
  ASSERT_EQ(0x100, atomic_fetch_xor(&i, 0x120));
  ASSERT_EQ(0x020, atomic_fetch_xor_explicit(&i, 0x103, memory_order_relaxed));
  ASSERT_EQ(0x123, atomic_load(&i));
}

TEST(stdatomic, atomic_fetch_and) {
  atomic_int i = ATOMIC_VAR_INIT(0x123);
  ASSERT_EQ(0x123, atomic_fetch_and(&i, 0x00f));
  ASSERT_EQ(0x003, atomic_fetch_and_explicit(&i, 0x2, memory_order_relaxed));
  ASSERT_EQ(0x002, atomic_load(&i));
}

// And a rudimentary test of acquire-release memory ordering:

static constexpr uint_least32_t BIG = 30'000'000ul;
static_assert((BIG % 2) == 0);  // Assumed below.

struct three_atomics {
  atomic_uint_least32_t x;
  char a[123];  // Everything in different cache lines,
                // increase chance of compiler getting alignment wrong.
  atomic_uint_least32_t y;
  char b[4013];
  atomic_uint_least32_t z;
};

atomic_bool read_enough(false);

// Very simple acquire/release memory ordering smoke test.
static void* writer(void* arg) {
  three_atomics* a = reinterpret_cast<three_atomics*>(arg);
  for (uint_least32_t i = 0; i <= BIG; i+=2) {
    atomic_store_explicit(&a->x, i, memory_order_relaxed);
    atomic_store_explicit(&a->z, i, memory_order_relaxed);
    atomic_store_explicit(&a->y, i, memory_order_release);

    // Force stores to be visible in spite of being overwritten below.
    asm volatile("" ::: "memory");

    atomic_store_explicit(&a->x, i+1, memory_order_relaxed);
    atomic_store_explicit(&a->z, i+1, memory_order_relaxed);
    atomic_store_explicit(&a->y, i+1, memory_order_release);
    if (i >= BIG - 1000 && !atomic_load(&read_enough)) {
      // Give reader a chance to catch up, at the expense of making the test
      // less effective.
      usleep(1000);
    }
  }
  return nullptr;
}

static void* reader(void* arg) {
  three_atomics* a = reinterpret_cast<three_atomics*>(arg);
  uint_least32_t xval = 0, yval = 0, zval = 0;
  size_t repeat = 0;
  size_t repeat_limit = 1000;
  while (yval != BIG + 1) {
    yval = atomic_load_explicit(&a->y, memory_order_acquire);
    zval = atomic_load_explicit(&a->z, memory_order_relaxed);
    xval = atomic_load_explicit(&a->x, memory_order_relaxed);
    // If we see a given value of y, the immediately preceding
    // stores to z and x, or later ones, should also be visible.
    if (zval < yval) {
      // Cant just ASSERT, since we are in a non-void function.
      ADD_FAILURE() << "acquire-release ordering violation: "
                    << zval << " < " << yval << ", " << xval << "\n";
      return nullptr; // Only report once.
    }
    if (xval < yval) {
      // Cant just ASSERT, since we are in a non-void function.
      ADD_FAILURE() << "acquire-release ordering violation: "
                    << xval << " < " << yval << ", " << zval <<  "\n";
      return nullptr; // Only report once.
    }
    if (repeat < repeat_limit) {
      ++repeat;
    } else if (!atomic_load_explicit(&read_enough, memory_order_relaxed)) {
      atomic_store_explicit(&read_enough, true, memory_order_relaxed);
    }
  }
  // The following assertion is not technically guaranteed to hold.
  // But if it fails to hold, this test was useless, and we have a
  // serious scheduling issue that we should probably know about.
  EXPECT_EQ(repeat, repeat_limit);
  return nullptr;
}

TEST(stdatomic, ordering) {
  // Run a memory ordering smoke test.
  void* result;
  three_atomics a;
  atomic_init(&a.x, 0ul);
  atomic_init(&a.y, 0ul);
  atomic_init(&a.z, 0ul);
  pthread_t t1,t2;
  ASSERT_EQ(0, pthread_create(&t1, nullptr, reader, &a));
  ASSERT_EQ(0, pthread_create(&t2, nullptr, writer, &a));
  ASSERT_EQ(0, pthread_join(t1, &result));
  EXPECT_EQ(nullptr, result);
  ASSERT_EQ(0, pthread_join(t2, &result));
  EXPECT_EQ(nullptr, result);
  EXPECT_EQ(atomic_load_explicit(&a.x, memory_order_consume), BIG + 1);
  EXPECT_EQ(atomic_load_explicit(&a.y, memory_order_seq_cst), BIG + 1);
  EXPECT_EQ(atomic_load(&a.z), BIG + 1);
}
