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
#include <stdint.h>
#include <string.h>

#if defined(__GNUC__) && !defined(__clang__) && \
    (defined(__arm__) || defined(__aarch64__))
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

// Test emutls runtime data structures and __emutls_get_address function.
typedef unsigned int gcc_word __attribute__((mode(word)));
typedef unsigned int gcc_pointer __attribute__((mode(pointer)));
struct gcc_emutls_object {  // for libgcc
  gcc_word size;
  gcc_word align;
  union {
    gcc_pointer offset;
    void* ptr;
  } loc;
  void* templ;
};

typedef struct __emutls_control {  // for clang/llvm
  size_t size;
  size_t align;
  union {
    uintptr_t index;
    void* address;
  } object;
  void* value;
} __emutls_control;

TEST(thread_local_storage, type_size) {
  static_assert(sizeof(size_t) == sizeof(gcc_word),
                "size_t != gcc_word");
  static_assert(sizeof(uintptr_t) == sizeof(gcc_pointer),
                "uintptr_t != gcc_pointer");
  static_assert(sizeof(uintptr_t) == sizeof(void*),
                "sizoeof(uintptr_t) != sizeof(void*)");
  static_assert(sizeof(__emutls_control) == sizeof(struct gcc_emutls_object),
                "sizeof(__emutls_control) != sizeof(struct gcc_emutls_object)");
}

extern "C" void* __emutls_get_address(__emutls_control*);

TEST(thread_local_storage, init_value) {
  char tls_value1[] = "123456789";
  char tls_value2[] = "abcdefghi";
  constexpr size_t num_saved_values = 10;
  __emutls_control tls_var[num_saved_values];
  size_t prev_index = 0;
  void* saved_gap[num_saved_values];
  void* saved_p[num_saved_values];
  ASSERT_TRUE(strlen(tls_value2) <= strlen(tls_value1));
  __emutls_control c =
      {strlen(tls_value1) + 1, 1, {0}, tls_value1};
  for (size_t n = 0; n < num_saved_values; n++) {
    memcpy(&tls_var[n], &c, sizeof(c));
    tls_var[n].align = (1 << n);
  }
  for (size_t n = 0; n < num_saved_values; n++) {
    // Try to mess up malloc space so that the next malloc will not have the
    // required alignment, but __emutls_get_address should still return an
    // aligned address.
    saved_gap[n] = malloc(1);
    void* p = __emutls_get_address(&tls_var[n]);
    saved_p[n] = p;
    ASSERT_TRUE(p != nullptr);
    ASSERT_TRUE(tls_var[n].object.index != 0);
    // check if p is a new object.
    if (n > 0) {
      // In single-thread environment, object.address == p.
      // In multi-threads environment, object.index is increased.
      ASSERT_TRUE(prev_index + 1 == tls_var[n].object.index ||
                  p == tls_var[n].object.address);
      ASSERT_TRUE(p != saved_p[n - 1]);
    }
    prev_index = tls_var[n].object.index;
    // check if p is aligned
    uintptr_t align = (1 << n);
    uintptr_t address= reinterpret_cast<uintptr_t>(p);
    ASSERT_EQ((address & ~(align - 1)), address);
    // check if *p is initialized
    ASSERT_STREQ(tls_value1, static_cast<char*>(p));
    // change value in *p
    memcpy(p, tls_value2, strlen(tls_value2) + 1);
  }
  for (size_t n = 0; n < num_saved_values; n++) {
    free(saved_gap[n]);
  }
  for (size_t n = 0; n < num_saved_values; n++) {
    void* p = __emutls_get_address(&tls_var[n]);
    ASSERT_EQ(p, saved_p[n]);
    // check if *p has the new value
    ASSERT_STREQ(tls_value2, static_cast<char*>(p));
  }
}
