/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <errno.h>
#include <pthread.h>
#include <unistd.h>

TEST(pthread, pthread_key_create) {
  pthread_key_t key;
  ASSERT_EQ(0, pthread_key_create(&key, NULL));
  ASSERT_EQ(0, pthread_key_delete(key));
  // Can't delete a key that's already been deleted.
  ASSERT_EQ(EINVAL, pthread_key_delete(key));
}

static void* IdFn(void* arg) {
  return arg;
}

static void* SleepFn(void* arg) {
  sleep(reinterpret_cast<unsigned int>(arg));
  return NULL;
}

static void* JoinFn(void* arg) {
  return reinterpret_cast<void*>(pthread_join(reinterpret_cast<pthread_t>(arg), NULL));
}

TEST(pthread, pthread_create) {
  void* expected_result = reinterpret_cast<void*>(123);
  // Can we create a thread?
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, NULL, IdFn, expected_result));
  // If we join, do we get the expected value back?
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
  ASSERT_EQ(expected_result, result);
}

TEST(pthread, pthread_no_join_after_detach) {
  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, SleepFn, reinterpret_cast<void*>(5)));

  // After a pthread_detach...
  ASSERT_EQ(0, pthread_detach(t1));

  // ...pthread_join should fail.
  void* result;
  ASSERT_EQ(EINVAL, pthread_join(t1, &result));
}

TEST(pthread, pthread_no_op_detach_after_join) {
  pthread_t t1;
  ASSERT_EQ(0, pthread_create(&t1, NULL, SleepFn, reinterpret_cast<void*>(1)));

  // If thread 2 is already waiting to join thread 1...
  pthread_t t2;
  ASSERT_EQ(0, pthread_create(&t2, NULL, JoinFn, reinterpret_cast<void*>(t1)));

  // ...a call to pthread_detach on thread 1 will "succeed"...
  ASSERT_EQ(0, pthread_detach(t1));

  // ...but the join still goes ahead.
  void* join_result;
  ASSERT_EQ(0, pthread_join(t2, &join_result));
  ASSERT_EQ(EINVAL, reinterpret_cast<int>(join_result));
}
