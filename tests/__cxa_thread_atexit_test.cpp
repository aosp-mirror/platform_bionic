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

#include <stdint.h>

#include <string>

static std::string class_with_dtor_output;

class ClassWithDtor {
 public:
  void set_message(const std::string& msg) {
    message = msg;
  }

  ~ClassWithDtor() {
    class_with_dtor_output += message;
  }
 private:
  std::string message;
};

#if defined(__clang__) && defined(__aarch64__)
// b/25642296, aarch64 clang compiled "thread_local" does not link.
static ClassWithDtor class_with_dtor;
#else
static thread_local ClassWithDtor class_with_dtor;
#endif

static void* thread_nop(void* arg) {
  class_with_dtor.set_message(*static_cast<std::string*>(arg));
  return nullptr;
}

TEST(thread_local, smoke) {
  std::string msg("dtor called.");
  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, thread_nop, &msg));
  ASSERT_EQ(0, pthread_join(t, nullptr));
#if defined(__clang__) && defined(__aarch64__)
  GTEST_LOG_(INFO) << "Skipping test, b/25642296, "
                   << "thread_local does not work with aarch64 clang/llvm.\n";
#else
  ASSERT_EQ("dtor called.", class_with_dtor_output);
#endif
}

class ClassWithDtorForMainThread {
 public:
  void set_message(const std::string& msg) {
    message = msg;
  }

  ~ClassWithDtorForMainThread() {
    fprintf(stderr, "%s", message.c_str());
  }
 private:
  std::string message;
};

static void thread_atexit_main() {
#if defined(__clang__) && defined(__aarch64__)
  static ClassWithDtorForMainThread class_with_dtor_for_main_thread;
  GTEST_LOG_(INFO) << "Skipping test, b/25642296, "
                   << "thread_local does not work with aarch64 clang/llvm.\n";
#else
  static thread_local ClassWithDtorForMainThread class_with_dtor_for_main_thread;
#endif
  class_with_dtor_for_main_thread.set_message("d-tor for main thread called.");
  exit(0);
}

TEST(thread_local, dtor_for_main_thread) {
  ASSERT_EXIT(thread_atexit_main(), testing::ExitedWithCode(0), "d-tor for main thread called.");
}

extern "C" int __cxa_thread_atexit_impl(void (*fn)(void*), void* arg, void* dso_handle);

static void thread_atexit_fn1(void* arg) {
  std::string* call_sequence = static_cast<std::string*>(arg);
  *call_sequence += "one, ";
}

static void thread_atexit_fn2(void* arg) {
  std::string* call_sequence = static_cast<std::string*>(arg);
  *call_sequence += "two, ";
}

static void thread_atexit_from_atexit(void* arg) {
  std::string* call_sequence = static_cast<std::string*>(arg);
  *call_sequence += "oops, ";
}

static void thread_atexit_fn3(void* arg) {
  __cxa_thread_atexit_impl(thread_atexit_from_atexit, arg, nullptr);
  std::string* call_sequence = static_cast<std::string*>(arg);
  *call_sequence += "three, ";
}

static void thread_atexit_fn4(void* arg) {
  std::string* call_sequence = static_cast<std::string*>(arg);
  *call_sequence += "four, ";
}

static void thread_atexit_fn5(void* arg) {
  std::string* call_sequence = static_cast<std::string*>(arg);
  *call_sequence += "five.";
}

static void* thread_main(void* arg) {
  __cxa_thread_atexit_impl(thread_atexit_fn5, arg, nullptr);
  __cxa_thread_atexit_impl(thread_atexit_fn4, arg, nullptr);
  __cxa_thread_atexit_impl(thread_atexit_fn3, arg, nullptr);
  __cxa_thread_atexit_impl(thread_atexit_fn2, arg, nullptr);
  __cxa_thread_atexit_impl(thread_atexit_fn1, arg, nullptr);
  return nullptr;
}

TEST(__cxa_thread_atexit_impl, smoke) {
  std::string atexit_call_sequence;

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, nullptr, thread_main, &atexit_call_sequence));
  ASSERT_EQ(0, pthread_join(t, nullptr));
  ASSERT_EQ("one, two, three, oops, four, five.", atexit_call_sequence);
}


