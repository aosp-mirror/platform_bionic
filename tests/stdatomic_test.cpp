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

#if !defined(__GLIBC__) /* TODO: fix our prebuilt toolchains! */

#include <stdatomic.h>

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
  atomic_intmax_t big;
  ASSERT_TRUE(atomic_is_lock_free(&small));
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
  ASSERT_TRUE(atomic_compare_exchange_strong_explicit(&i, &expected, 456, memory_order_relaxed, memory_order_relaxed));
  ASSERT_FALSE(atomic_compare_exchange_strong_explicit(&i, &expected, 456, memory_order_relaxed, memory_order_relaxed));
  ASSERT_EQ(456, expected);

  atomic_store(&i, 123);
  expected = 123;
  ASSERT_TRUE(atomic_compare_exchange_weak(&i, &expected, 456));
  ASSERT_FALSE(atomic_compare_exchange_weak(&i, &expected, 456));
  ASSERT_EQ(456, expected);

  atomic_store(&i, 123);
  expected = 123;
  ASSERT_TRUE(atomic_compare_exchange_weak_explicit(&i, &expected, 456, memory_order_relaxed, memory_order_relaxed));
  ASSERT_FALSE(atomic_compare_exchange_weak_explicit(&i, &expected, 456, memory_order_relaxed, memory_order_relaxed));
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

#endif
