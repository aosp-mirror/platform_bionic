/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <gtest/gtest.h>

#if __has_include(<threads.h>)

#define HAVE_THREADS_H
#include <threads.h>

static int g_call_once_call_count;

static void increment_call_count() {
  ++g_call_once_call_count;
}

static int g_dtor_call_count;

static void tss_dtor(void* ptr) {
  ++g_dtor_call_count;
  free(ptr);
}

static int return_arg(void* arg) {
  return static_cast<int>(reinterpret_cast<uintptr_t>(arg));
}

static int exit_arg(void* arg) {
  thrd_exit(static_cast<int>(reinterpret_cast<uintptr_t>(arg)));
}

#endif

#include <time.h>

#include <thread>

#include "BionicDeathTest.h"
#include "SignalUtils.h"

TEST(threads, call_once) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  once_flag flag = ONCE_FLAG_INIT;
  call_once(&flag, increment_call_count);
  call_once(&flag, increment_call_count);
  std::thread([&flag] {
    call_once(&flag, increment_call_count);
  }).join();
  ASSERT_EQ(1, g_call_once_call_count);
#endif
}

TEST(threads, cnd_broadcast__cnd_wait) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain));

  cnd_t c;
  ASSERT_EQ(thrd_success, cnd_init(&c));

  std::atomic_int i = 0;

  auto waiter = [&c, &i, &m] {
    ASSERT_EQ(thrd_success, mtx_lock(&m));
    while (i != 1) ASSERT_EQ(thrd_success, cnd_wait(&c, &m));
    ASSERT_EQ(thrd_success, mtx_unlock(&m));
  };
  std::thread t1(waiter);
  std::thread t2(waiter);
  std::thread t3(waiter);

  ASSERT_EQ(thrd_success, mtx_lock(&m));
  i = 1;
  ASSERT_EQ(thrd_success, mtx_unlock(&m));

  ASSERT_EQ(thrd_success, cnd_broadcast(&c));

  t1.join();
  t2.join();
  t3.join();

  mtx_destroy(&m);
  cnd_destroy(&c);
#endif
}

TEST(threads, cnd_init__cnd_destroy) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  cnd_t c;
  ASSERT_EQ(thrd_success, cnd_init(&c));
  cnd_destroy(&c);
#endif
}

TEST(threads, cnd_signal__cnd_wait) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain));
  cnd_t c;
  ASSERT_EQ(thrd_success, cnd_init(&c));

  std::atomic_int count = 0;
  auto waiter = [&c, &m, &count] {
    ASSERT_EQ(thrd_success, mtx_lock(&m));
    ASSERT_EQ(thrd_success, cnd_wait(&c, &m));
    ASSERT_EQ(thrd_success, mtx_unlock(&m));
    ++count;
  };
  std::thread t1(waiter);
  std::thread t2(waiter);
  std::thread t3(waiter);

  // This is inherently racy, but attempts to distinguish between cnd_signal and
  // cnd_broadcast.
  usleep(100000);
  ASSERT_EQ(thrd_success, cnd_signal(&c));
  while (count == 0) {
  }
  usleep(100000);
  ASSERT_EQ(1, count);

  ASSERT_EQ(thrd_success, cnd_signal(&c));
  while (count == 1) {
  }
  usleep(100000);
  ASSERT_EQ(2, count);

  ASSERT_EQ(thrd_success, cnd_signal(&c));
  while (count == 2) {
  }
  usleep(100000);
  ASSERT_EQ(3, count);

  t1.join();
  t2.join();
  t3.join();

  mtx_destroy(&m);
  cnd_destroy(&c);
#endif
}

TEST(threads, cnd_timedwait_timedout) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_timed));
  ASSERT_EQ(thrd_success, mtx_lock(&m));

  cnd_t c;
  ASSERT_EQ(thrd_success, cnd_init(&c));

  timespec ts = {};
  ASSERT_EQ(thrd_timedout, cnd_timedwait(&c, &m, &ts));
#endif
}

TEST(threads, cnd_timedwait) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_timed));

  cnd_t c;
  ASSERT_EQ(thrd_success, cnd_init(&c));

  std::atomic_bool done = false;
  std::thread t([&c, &m, &done] {
    ASSERT_EQ(thrd_success, mtx_lock(&m));

    // cnd_timewait's time is *absolute*.
    timespec ts;
    ASSERT_EQ(TIME_UTC, timespec_get(&ts, TIME_UTC));
    ts.tv_sec += 666;

    ASSERT_EQ(thrd_success, cnd_timedwait(&c, &m, &ts));
    done = true;
    ASSERT_EQ(thrd_success, mtx_unlock(&m));
  });

  while (!done) ASSERT_EQ(thrd_success, cnd_signal(&c));

  t.join();
#endif
}

TEST(threads, mtx_init) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain));
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_timed));
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain | mtx_recursive));
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_timed | mtx_recursive));
  ASSERT_EQ(thrd_error, mtx_init(&m, 123));
  ASSERT_EQ(thrd_error, mtx_init(&m, mtx_recursive));
#endif
}

TEST(threads, mtx_destroy) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain));
  mtx_destroy(&m);
#endif
}

TEST(threads, mtx_lock_plain) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain));

  ASSERT_EQ(thrd_success, mtx_lock(&m));
  ASSERT_EQ(thrd_busy, mtx_trylock(&m));
  ASSERT_EQ(thrd_success, mtx_unlock(&m));

  mtx_destroy(&m);
#endif
}

TEST(threads, mtx_lock_recursive) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain | mtx_recursive));

  ASSERT_EQ(thrd_success, mtx_lock(&m));
  ASSERT_EQ(thrd_success, mtx_trylock(&m));
  ASSERT_EQ(thrd_success, mtx_unlock(&m));
  ASSERT_EQ(thrd_success, mtx_unlock(&m));

  mtx_destroy(&m);
#endif
}

TEST(threads, mtx_timedlock) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_timed));

  timespec ts = {};
  ASSERT_EQ(thrd_success, mtx_timedlock(&m, &ts));

  std::thread([&m] {
    timespec ts = { .tv_nsec = 500000 };
    ASSERT_EQ(thrd_timedout, mtx_timedlock(&m, &ts));
  }).join();

  ASSERT_EQ(thrd_success, mtx_unlock(&m));
  mtx_destroy(&m);
#endif
}


TEST(threads, mtx_unlock) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  mtx_t m;
  ASSERT_EQ(thrd_success, mtx_init(&m, mtx_plain));
  ASSERT_EQ(thrd_success, mtx_lock(&m));
  std::thread([&m] {
    ASSERT_EQ(thrd_busy, mtx_trylock(&m));
  }).join();
  ASSERT_EQ(thrd_success, mtx_unlock(&m));
  std::thread([&m] {
    ASSERT_EQ(thrd_success, mtx_trylock(&m));
  }).join();
#endif
}

TEST(threads, thrd_current__thrd_equal) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  thrd_t t1 = thrd_current();
  // (As a side-effect, this demonstrates interoperability with std::thread.)
  std::thread([&t1] {
    thrd_t t2 = thrd_current();
    ASSERT_FALSE(thrd_equal(t1, t2));
    thrd_t t2_2 = thrd_current();
    ASSERT_TRUE(thrd_equal(t2, t2_2));
  }).join();
  thrd_t t1_2 = thrd_current();
  ASSERT_TRUE(thrd_equal(t1, t1_2));
#endif
}

TEST(threads, thrd_create__thrd_detach) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  thrd_t t;
  ASSERT_EQ(thrd_success, thrd_create(&t, exit_arg, reinterpret_cast<void*>(1)));
  ASSERT_EQ(thrd_success, thrd_detach(t));
#endif
}

TEST(threads, thrd_create__thrd_exit) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  // Similar to the thrd_join test, but with a function that calls thrd_exit
  // instead.
  thrd_t t;
  int result;
  ASSERT_EQ(thrd_success, thrd_create(&t, exit_arg, reinterpret_cast<void*>(1)));
  ASSERT_EQ(thrd_success, thrd_join(t, &result));
  ASSERT_EQ(1, result);

  ASSERT_EQ(thrd_success, thrd_create(&t, exit_arg, reinterpret_cast<void*>(2)));
  ASSERT_EQ(thrd_success, thrd_join(t, &result));
  ASSERT_EQ(2, result);

  // The `result` argument can be null if you don't care...
  ASSERT_EQ(thrd_success, thrd_create(&t, exit_arg, reinterpret_cast<void*>(3)));
  ASSERT_EQ(thrd_success, thrd_join(t, nullptr));
#endif
}

class threads_DeathTest : public BionicDeathTest {};

TEST(threads_DeathTest, thrd_exit_main_thread) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  // "The program terminates normally after the last thread has been terminated.
  // The behavior is as if the program called the exit function with the status
  // EXIT_SUCCESS at thread termination time." (ISO/IEC 9899:2018)
  ASSERT_EXIT(thrd_exit(12), ::testing::ExitedWithCode(EXIT_SUCCESS), "");
#endif
}

TEST(threads, thrd_create__thrd_join) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  // Similar to the thrd_exit test, but with a function that calls return
  // instead.
  thrd_t t;
  int result;
  ASSERT_EQ(thrd_success, thrd_create(&t, return_arg, reinterpret_cast<void*>(1)));
  ASSERT_EQ(thrd_success, thrd_join(t, &result));
  ASSERT_EQ(1, result);

  ASSERT_EQ(thrd_success, thrd_create(&t, return_arg, reinterpret_cast<void*>(2)));
  ASSERT_EQ(thrd_success, thrd_join(t, &result));
  ASSERT_EQ(2, result);

  // The `result` argument can be null if you don't care...
  ASSERT_EQ(thrd_success, thrd_create(&t, return_arg, reinterpret_cast<void*>(3)));
  ASSERT_EQ(thrd_success, thrd_join(t, nullptr));
#endif
}

TEST(threads, thrd_sleep_signal) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  ScopedSignalHandler ssh{SIGALRM, [](int) {}};
  std::thread t([] {
    timespec long_time = { .tv_sec = 666 };
    timespec remaining = {};
    ASSERT_EQ(-1, thrd_sleep(&long_time, &remaining));
    uint64_t t = remaining.tv_sec * 1000000000 + remaining.tv_nsec;
    ASSERT_LE(t, 666ULL * 1000000000);
  });
  usleep(100000); // 0.1s
  pthread_kill(t.native_handle(), SIGALRM);
  t.join();
#endif
}

TEST(threads, thrd_sleep_signal_nullptr) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  ScopedSignalHandler ssh{SIGALRM, [](int) {}};
  std::thread t([] {
    timespec long_time = { .tv_sec = 666 };
    ASSERT_EQ(-1, thrd_sleep(&long_time, nullptr));
  });
  usleep(100000); // 0.1s
  pthread_kill(t.native_handle(), SIGALRM);
  t.join();
#endif
}

TEST(threads, thrd_sleep_error) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  timespec invalid = { .tv_sec = -1 };
  ASSERT_EQ(-2, thrd_sleep(&invalid, nullptr));
#endif
}

TEST(threads, thrd_yield) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  thrd_yield();
#endif
}

TEST(threads, TSS_DTOR_ITERATIONS_macro) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  ASSERT_EQ(PTHREAD_DESTRUCTOR_ITERATIONS, TSS_DTOR_ITERATIONS);
#endif
}

TEST(threads, tss_create) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  tss_t key;
  ASSERT_EQ(thrd_success, tss_create(&key, nullptr));
  tss_delete(key);
#endif
}

TEST(threads, tss_create_dtor) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  tss_dtor_t dtor = tss_dtor;
  tss_t key;
  ASSERT_EQ(thrd_success, tss_create(&key, dtor));

  ASSERT_EQ(thrd_success, tss_set(key, strdup("hello")));
  std::thread([&key] {
    ASSERT_EQ(thrd_success, tss_set(key, strdup("world")));
  }).join();
  // Thread exit calls the destructor...
  ASSERT_EQ(1, g_dtor_call_count);

  // "[A call to tss_set] will not invoke the destructor associated with the
  // key on the value being replaced" (ISO/IEC 9899:2018).
  g_dtor_call_count = 0;
  ASSERT_EQ(thrd_success, tss_set(key, strdup("hello")));
  ASSERT_EQ(0, g_dtor_call_count);

  // "Calling tss_delete will not result in the invocation of any
  // destructors" (ISO/IEC 9899:2018).
  // The destructor for "hello" won't be called until *this* thread exits.
  g_dtor_call_count = 0;
  tss_delete(key);
  ASSERT_EQ(0, g_dtor_call_count);
#endif
}

TEST(threads, tss_get__tss_set) {
#if !defined(HAVE_THREADS_H)
  GTEST_SKIP() << "<threads.h> unavailable";
#else
  tss_t key;
  ASSERT_EQ(thrd_success, tss_create(&key, nullptr));

  ASSERT_EQ(thrd_success, tss_set(key, const_cast<char*>("hello")));
  ASSERT_STREQ("hello", reinterpret_cast<char*>(tss_get(key)));
  std::thread([&key] {
      ASSERT_EQ(nullptr, tss_get(key));
      ASSERT_EQ(thrd_success, tss_set(key, const_cast<char*>("world")));
      ASSERT_STREQ("world", reinterpret_cast<char*>(tss_get(key)));
  }).join();
  ASSERT_STREQ("hello", reinterpret_cast<char*>(tss_get(key)));

  tss_delete(key);
#endif
}
