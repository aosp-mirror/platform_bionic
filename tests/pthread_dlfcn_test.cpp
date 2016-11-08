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

#include <dlfcn.h>

#include "utils.h"

static int g_atfork_prepare_calls = 0;
static void AtForkPrepare1() { g_atfork_prepare_calls = (g_atfork_prepare_calls * 10) + 1; }
static void AtForkPrepare2() { g_atfork_prepare_calls = (g_atfork_prepare_calls * 10) + 2; }
static void AtForkPrepare3() { g_atfork_prepare_calls = (g_atfork_prepare_calls * 10) + 3; }
static void AtForkPrepare4() { g_atfork_prepare_calls = (g_atfork_prepare_calls * 10) + 4; }

static int g_atfork_parent_calls = 0;
static void AtForkParent1() { g_atfork_parent_calls = (g_atfork_parent_calls * 10) + 1; }
static void AtForkParent2() { g_atfork_parent_calls = (g_atfork_parent_calls * 10) + 2; }
static void AtForkParent3() { g_atfork_parent_calls = (g_atfork_parent_calls * 10) + 3; }
static void AtForkParent4() { g_atfork_parent_calls = (g_atfork_parent_calls * 10) + 4; }

static int g_atfork_child_calls = 0;
static void AtForkChild1() { g_atfork_child_calls = (g_atfork_child_calls * 10) + 1; }
static void AtForkChild2() { g_atfork_child_calls = (g_atfork_child_calls * 10) + 2; }
static void AtForkChild3() { g_atfork_child_calls = (g_atfork_child_calls * 10) + 3; }
static void AtForkChild4() { g_atfork_child_calls = (g_atfork_child_calls * 10) + 4; }

static void* g_atfork_test_handle = nullptr;
static void AtForkPrepare() {}
static void AtForkParent() {}
static void AtForkChild() { dlclose(g_atfork_test_handle); g_atfork_test_handle = dlopen("libtest_pthread_atfork.so", RTLD_NOW | RTLD_LOCAL); }

TEST(pthread, pthread_atfork_with_dlclose) {
  ASSERT_EQ(0, pthread_atfork(AtForkPrepare1, AtForkParent1, AtForkChild1));

  void* handle = dlopen("libtest_pthread_atfork.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(handle != nullptr) << dlerror();
  typedef int (*fn_t)(void (*)(void), void (*)(void), void (*)(void));
  fn_t fn = reinterpret_cast<fn_t>(dlsym(handle, "proxy_pthread_atfork"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  // the library registers 2 additional atfork handlers in a constructor
  ASSERT_EQ(0, fn(AtForkPrepare2, AtForkParent2, AtForkChild2));
  ASSERT_EQ(0, fn(AtForkPrepare3, AtForkParent3, AtForkChild3));

  ASSERT_EQ(0, pthread_atfork(AtForkPrepare4, AtForkParent4, AtForkChild4));

  pid_t pid = fork();

  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    ASSERT_EQ(1234, g_atfork_child_calls);
    _exit(0);
  }

  ASSERT_EQ(1234, g_atfork_parent_calls);
  ASSERT_EQ(4321, g_atfork_prepare_calls);

  EXPECT_EQ(0, dlclose(handle));
  g_atfork_prepare_calls = g_atfork_parent_calls = g_atfork_child_calls = 0;

  AssertChildExited(pid, 0);

  pid = fork();

  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    ASSERT_EQ(14, g_atfork_child_calls);
    _exit(0);
  }

  ASSERT_EQ(14, g_atfork_parent_calls);
  ASSERT_EQ(41, g_atfork_prepare_calls);

  AssertChildExited(pid, 0);
}

TEST(pthread, pthread_atfork_child_with_dlclose) {

  g_atfork_test_handle = dlopen("libtest_pthread_atfork.so", RTLD_NOW | RTLD_LOCAL);
  ASSERT_TRUE(g_atfork_test_handle != nullptr) << dlerror();
  typedef int (*fn_t)(void (*)(void), void (*)(void), void (*)(void));
  fn_t fn = reinterpret_cast<fn_t>(dlsym(g_atfork_test_handle, "proxy_pthread_atfork"));
  ASSERT_TRUE(fn != nullptr) << dlerror();
  // the library registers 2 additional atfork handlers in a constructor

  ASSERT_EQ(0, pthread_atfork(AtForkPrepare, AtForkParent, AtForkChild));

  pid_t pid = fork();

  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    _exit(0);
  }

  AssertChildExited(pid, 0);

  EXPECT_EQ(0, dlclose(g_atfork_test_handle));
  g_atfork_test_handle = nullptr;
}
