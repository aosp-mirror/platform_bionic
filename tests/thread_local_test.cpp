/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifdef __GNUC__
// Gcc has a bug with -O -fdata-section for the arm target: http://b/22772147.
// Until that bug is fixed, disable optimization since
// it is not essential for this test.
#pragma GCC optimize("-O0")
#endif

__thread int local_var = 100;
int shared_var = 200;

static void reset_vars() {
  local_var = 1000;
  shared_var = 2000;
  // local_var should be reset by threads
}

typedef void* (*MyThread)(void*);

static void* inc_shared_var(void* p) {
  int *data = reinterpret_cast<int*>(p);
  shared_var++;
  *data = shared_var;
  return nullptr;
}

static void* inc_local_var(void* p) {
  int *data = reinterpret_cast<int*>(p);
  local_var++;
  *data = local_var;
  return nullptr;
}

static int run_one_thread(MyThread foo) {
  pthread_t t;
  int data;
  int error = pthread_create(&t, nullptr, foo, &data);
  if (!error)
      error = pthread_join(t, nullptr);
  return error ? error : data;
}

TEST(thread_local_storage, shared) {
  reset_vars();
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  // Update shared_var, local_var remains 1000.
  ASSERT_EQ(run_one_thread(inc_shared_var), 2001);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2001);

  ASSERT_EQ(run_one_thread(inc_shared_var), 2002);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2002);

  ASSERT_EQ(run_one_thread(inc_shared_var), 2003);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2003);
}

TEST(thread_local_storage, local) {
  reset_vars();
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  // When a child thread updates its own TLS variable,
  // this thread's local_var and shared_var are not changed.
  // TLS local_var is initialized to 100 in a thread.
  ASSERT_EQ(run_one_thread(inc_local_var), 101);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  ASSERT_EQ(run_one_thread(inc_local_var), 101);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  ASSERT_EQ(run_one_thread(inc_local_var), 101);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);
}

// Test TLS initialization of more complicated type, array of struct.
struct Point {
  int x, y;
};

typedef Point Triangle[3];

__thread Triangle local_triangle = {{10,10}, {20,20}, {30,30}};
Triangle shared_triangle = {{1,1}, {2,2}, {3,3}};

static void reset_triangle() {
  static const Triangle t1 = {{3,3}, {4,4}, {5,5}};
  static const Triangle t2 = {{2,2}, {3,3}, {4,4}};
  memcpy(local_triangle, t1, sizeof(local_triangle));
  memcpy(shared_triangle, t2, sizeof(shared_triangle));
}

static void* move_shared_triangle(void* p) {
  int *data = reinterpret_cast<int*>(p);
  shared_triangle[1].y++;
  *data = shared_triangle[1].y;
  return nullptr;
}

static void* move_local_triangle(void* p) {
  int *data = reinterpret_cast<int*>(p);
  local_triangle[1].y++;
  *data = local_triangle[1].y;
  return nullptr;
}

TEST(thread_local_storage, shared_triangle) {
  reset_triangle();
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  // Update shared_triangle, local_triangle remains 1000.
  ASSERT_EQ(run_one_thread(move_shared_triangle), 4);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 4);

  ASSERT_EQ(run_one_thread(move_shared_triangle), 5);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 5);

  ASSERT_EQ(run_one_thread(move_shared_triangle), 6);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 6);
}

TEST(thread_local_storage, local_triangle) {
  reset_triangle();
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  // Update local_triangle, parent thread's
  // shared_triangle and local_triangle are unchanged.
  ASSERT_EQ(run_one_thread(move_local_triangle), 21);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  ASSERT_EQ(run_one_thread(move_local_triangle), 21);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  ASSERT_EQ(run_one_thread(move_local_triangle), 21);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);
}
