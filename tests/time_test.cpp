/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <time.h>

#include <errno.h>
#include <gtest/gtest.h>
#include <pthread.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>

#include "ScopedSignalHandler.h"

#include "private/bionic_constants.h"

TEST(time, gmtime) {
  time_t t = 0;
  tm* broken_down = gmtime(&t);
  ASSERT_TRUE(broken_down != NULL);
  ASSERT_EQ(0, broken_down->tm_sec);
  ASSERT_EQ(0, broken_down->tm_min);
  ASSERT_EQ(0, broken_down->tm_hour);
  ASSERT_EQ(1, broken_down->tm_mday);
  ASSERT_EQ(0, broken_down->tm_mon);
  ASSERT_EQ(1970, broken_down->tm_year + 1900);
}

static void* gmtime_no_stack_overflow_14313703_fn(void*) {
  const char* original_tz = getenv("TZ");
  // Ensure we'll actually have to enter tzload by using a time zone that doesn't exist.
  setenv("TZ", "gmtime_stack_overflow_14313703", 1);
  tzset();
  if (original_tz != NULL) {
    setenv("TZ", original_tz, 1);
  }
  tzset();
  return NULL;
}

TEST(time, gmtime_no_stack_overflow_14313703) {
  // Is it safe to call tzload on a thread with a small stack?
  // http://b/14313703
  // https://code.google.com/p/android/issues/detail?id=61130
  pthread_attr_t attributes;
  ASSERT_EQ(0, pthread_attr_init(&attributes));
#if defined(__BIONIC__)
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, PTHREAD_STACK_MIN));
#else
  // PTHREAD_STACK_MIN not currently in the host GCC sysroot.
  ASSERT_EQ(0, pthread_attr_setstacksize(&attributes, 4 * getpagesize()));
#endif

  pthread_t t;
  ASSERT_EQ(0, pthread_create(&t, &attributes, gmtime_no_stack_overflow_14313703_fn, NULL));
  void* result;
  ASSERT_EQ(0, pthread_join(t, &result));
}

TEST(time, mktime_empty_TZ) {
  // tzcode used to have a bug where it didn't reinitialize some internal state.

  // Choose a time where DST is set.
  struct tm t;
  memset(&t, 0, sizeof(tm));
  t.tm_year = 1980 - 1900;
  t.tm_mon = 6;
  t.tm_mday = 2;

  setenv("TZ", "America/Los_Angeles", 1);
  tzset();
  ASSERT_EQ(static_cast<time_t>(331372800U), mktime(&t));

  memset(&t, 0, sizeof(tm));
  t.tm_year = 1980 - 1900;
  t.tm_mon = 6;
  t.tm_mday = 2;

  setenv("TZ", "", 1); // Implies UTC.
  tzset();
  ASSERT_EQ(static_cast<time_t>(331344000U), mktime(&t));
}

TEST(time, mktime_10310929) {
  struct tm t;
  memset(&t, 0, sizeof(tm));
  t.tm_year = 200;
  t.tm_mon = 2;
  t.tm_mday = 10;

#if !defined(__LP64__)
  // 32-bit bionic stupidly had a signed 32-bit time_t.
  ASSERT_EQ(-1, mktime(&t));
#else
  // Everyone else should be using a signed 64-bit time_t.
  ASSERT_GE(sizeof(time_t) * 8, 64U);

  setenv("TZ", "America/Los_Angeles", 1);
  tzset();
  ASSERT_EQ(static_cast<time_t>(4108348800U), mktime(&t));

  setenv("TZ", "UTC", 1);
  tzset();
  ASSERT_EQ(static_cast<time_t>(4108320000U), mktime(&t));
#endif
}

TEST(time, strftime) {
  setenv("TZ", "UTC", 1);

  struct tm t;
  memset(&t, 0, sizeof(tm));
  t.tm_year = 200;
  t.tm_mon = 2;
  t.tm_mday = 10;

  char buf[64];

  // Seconds since the epoch.
#if defined(__BIONIC__) || defined(__LP64__) // Not 32-bit glibc.
  EXPECT_EQ(10U, strftime(buf, sizeof(buf), "%s", &t));
  EXPECT_STREQ("4108320000", buf);
#endif

  // Date and time as text.
  EXPECT_EQ(24U, strftime(buf, sizeof(buf), "%c", &t));
  EXPECT_STREQ("Sun Mar 10 00:00:00 2100", buf);
}

TEST(time, strptime) {
  setenv("TZ", "UTC", 1);

  struct tm t;
  char buf[64];

  memset(&t, 0, sizeof(t));
  strptime("11:14", "%R", &t);
  strftime(buf, sizeof(buf), "%H:%M", &t);
  EXPECT_STREQ("11:14", buf);

  memset(&t, 0, sizeof(t));
  strptime("09:41:53", "%T", &t);
  strftime(buf, sizeof(buf), "%H:%M:%S", &t);
  EXPECT_STREQ("09:41:53", buf);
}

void SetTime(timer_t t, time_t value_s, time_t value_ns, time_t interval_s, time_t interval_ns) {
  itimerspec ts;
  ts.it_value.tv_sec = value_s;
  ts.it_value.tv_nsec = value_ns;
  ts.it_interval.tv_sec = interval_s;
  ts.it_interval.tv_nsec = interval_ns;
  ASSERT_EQ(0, timer_settime(t, 0, &ts, NULL));
}

static void NoOpNotifyFunction(sigval_t) {
}

TEST(time, timer_create) {
  sigevent_t se;
  memset(&se, 0, sizeof(se));
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_notify_function = NoOpNotifyFunction;
  timer_t timer_id;
  ASSERT_EQ(0, timer_create(CLOCK_MONOTONIC, &se, &timer_id));

  int pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    // Timers are not inherited by the child.
    ASSERT_EQ(-1, timer_delete(timer_id));
    ASSERT_EQ(EINVAL, errno);
    _exit(0);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status));

  ASSERT_EQ(0, timer_delete(timer_id));
}

static int timer_create_SIGEV_SIGNAL_signal_handler_invocation_count;
static void timer_create_SIGEV_SIGNAL_signal_handler(int signal_number) {
  ++timer_create_SIGEV_SIGNAL_signal_handler_invocation_count;
  ASSERT_EQ(SIGUSR1, signal_number);
}

TEST(time, timer_create_SIGEV_SIGNAL) {
  sigevent_t se;
  memset(&se, 0, sizeof(se));
  se.sigev_notify = SIGEV_SIGNAL;
  se.sigev_signo = SIGUSR1;

  timer_t timer_id;
  ASSERT_EQ(0, timer_create(CLOCK_MONOTONIC, &se, &timer_id));

  timer_create_SIGEV_SIGNAL_signal_handler_invocation_count = 0;
  ScopedSignalHandler ssh(SIGUSR1, timer_create_SIGEV_SIGNAL_signal_handler);

  ASSERT_EQ(0, timer_create_SIGEV_SIGNAL_signal_handler_invocation_count);

  itimerspec ts;
  ts.it_value.tv_sec =  0;
  ts.it_value.tv_nsec = 1;
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;
  ASSERT_EQ(0, timer_settime(timer_id, 0, &ts, NULL));

  usleep(500000);
  ASSERT_EQ(1, timer_create_SIGEV_SIGNAL_signal_handler_invocation_count);
}

struct Counter {
 private:
  std::atomic<int> value;
  timer_t timer_id;
  sigevent_t se;
  bool timer_valid;

  void Create() {
    ASSERT_FALSE(timer_valid);
    ASSERT_EQ(0, timer_create(CLOCK_REALTIME, &se, &timer_id));
    timer_valid = true;
  }

 public:
  Counter(void (*fn)(sigval_t)) : value(0), timer_valid(false) {
    memset(&se, 0, sizeof(se));
    se.sigev_notify = SIGEV_THREAD;
    se.sigev_notify_function = fn;
    se.sigev_value.sival_ptr = this;
    Create();
  }
  void DeleteTimer() {
    ASSERT_TRUE(timer_valid);
    ASSERT_EQ(0, timer_delete(timer_id));
    timer_valid = false;
  }

  ~Counter() {
    if (timer_valid) {
      DeleteTimer();
    }
  }

  int Value() const {
    return value;
  }

  void SetTime(time_t value_s, time_t value_ns, time_t interval_s, time_t interval_ns) {
    ::SetTime(timer_id, value_s, value_ns, interval_s, interval_ns);
  }

  bool ValueUpdated() {
    int current_value = value;
    time_t start = time(NULL);
    while (current_value == value && (time(NULL) - start) < 5) {
    }
    return current_value != value;
  }

  static void CountNotifyFunction(sigval_t value) {
    Counter* cd = reinterpret_cast<Counter*>(value.sival_ptr);
    ++cd->value;
  }

  static void CountAndDisarmNotifyFunction(sigval_t value) {
    Counter* cd = reinterpret_cast<Counter*>(value.sival_ptr);
    ++cd->value;

    // Setting the initial expiration time to 0 disarms the timer.
    cd->SetTime(0, 0, 1, 0);
  }
};

TEST(time, timer_settime_0) {
  Counter counter(Counter::CountAndDisarmNotifyFunction);
  ASSERT_EQ(0, counter.Value());

  counter.SetTime(0, 500000000, 1, 0);
  sleep(1);

  // The count should just be 1 because we disarmed the timer the first time it fired.
  ASSERT_EQ(1, counter.Value());
}

TEST(time, timer_settime_repeats) {
  Counter counter(Counter::CountNotifyFunction);
  ASSERT_EQ(0, counter.Value());

  counter.SetTime(0, 1, 0, 10);
  ASSERT_TRUE(counter.ValueUpdated());
  ASSERT_TRUE(counter.ValueUpdated());
  ASSERT_TRUE(counter.ValueUpdated());
  counter.DeleteTimer();
  // Add a sleep as other threads may be calling the callback function when the timer is deleted.
  usleep(500000);
}

static int timer_create_NULL_signal_handler_invocation_count;
static void timer_create_NULL_signal_handler(int signal_number) {
  ++timer_create_NULL_signal_handler_invocation_count;
  ASSERT_EQ(SIGALRM, signal_number);
}

TEST(time, timer_create_NULL) {
  // A NULL sigevent* is equivalent to asking for SIGEV_SIGNAL for SIGALRM.
  timer_t timer_id;
  ASSERT_EQ(0, timer_create(CLOCK_MONOTONIC, NULL, &timer_id));

  timer_create_NULL_signal_handler_invocation_count = 0;
  ScopedSignalHandler ssh(SIGALRM, timer_create_NULL_signal_handler);

  ASSERT_EQ(0, timer_create_NULL_signal_handler_invocation_count);

  SetTime(timer_id, 0, 1, 0, 0);
  usleep(500000);

  ASSERT_EQ(1, timer_create_NULL_signal_handler_invocation_count);
}

TEST(time, timer_create_EINVAL) {
  clockid_t invalid_clock = 16;

  // A SIGEV_SIGNAL timer is easy; the kernel does all that.
  timer_t timer_id;
  ASSERT_EQ(-1, timer_create(invalid_clock, NULL, &timer_id));
  ASSERT_EQ(EINVAL, errno);

  // A SIGEV_THREAD timer is more interesting because we have stuff to clean up.
  sigevent_t se;
  memset(&se, 0, sizeof(se));
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_notify_function = NoOpNotifyFunction;
  ASSERT_EQ(-1, timer_create(invalid_clock, &se, &timer_id));
  ASSERT_EQ(EINVAL, errno);
}

TEST(time, timer_delete_multiple) {
  timer_t timer_id;
  ASSERT_EQ(0, timer_create(CLOCK_MONOTONIC, NULL, &timer_id));
  ASSERT_EQ(0, timer_delete(timer_id));
  ASSERT_EQ(-1, timer_delete(timer_id));
  ASSERT_EQ(EINVAL, errno);

  sigevent_t se;
  memset(&se, 0, sizeof(se));
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_notify_function = NoOpNotifyFunction;
  ASSERT_EQ(0, timer_create(CLOCK_MONOTONIC, &se, &timer_id));
  ASSERT_EQ(0, timer_delete(timer_id));
  ASSERT_EQ(-1, timer_delete(timer_id));
  ASSERT_EQ(EINVAL, errno);
}

TEST(time, timer_create_multiple) {
  Counter counter1(Counter::CountNotifyFunction);
  Counter counter2(Counter::CountNotifyFunction);
  Counter counter3(Counter::CountNotifyFunction);

  ASSERT_EQ(0, counter1.Value());
  ASSERT_EQ(0, counter2.Value());
  ASSERT_EQ(0, counter3.Value());

  counter2.SetTime(0, 500000000, 0, 0);
  sleep(1);

  EXPECT_EQ(0, counter1.Value());
  EXPECT_EQ(1, counter2.Value());
  EXPECT_EQ(0, counter3.Value());
}

// Test to verify that disarming a repeatable timer disables the callbacks.
TEST(time, timer_disarm_terminates) {
  Counter counter(Counter::CountNotifyFunction);
  ASSERT_EQ(0, counter.Value());

  counter.SetTime(0, 1, 0, 1);
  ASSERT_TRUE(counter.ValueUpdated());
  ASSERT_TRUE(counter.ValueUpdated());
  ASSERT_TRUE(counter.ValueUpdated());

  counter.SetTime(0, 0, 0, 0);
  // Add a sleep as the kernel may have pending events when the timer is disarmed.
  usleep(500000);
  int value = counter.Value();
  usleep(500000);

  // Verify the counter has not been incremented.
  ASSERT_EQ(value, counter.Value());
}

// Test to verify that deleting a repeatable timer disables the callbacks.
TEST(time, timer_delete_terminates) {
  Counter counter(Counter::CountNotifyFunction);
  ASSERT_EQ(0, counter.Value());

  counter.SetTime(0, 1, 0, 1);
  ASSERT_TRUE(counter.ValueUpdated());
  ASSERT_TRUE(counter.ValueUpdated());
  ASSERT_TRUE(counter.ValueUpdated());

  counter.DeleteTimer();
  // Add a sleep as other threads may be calling the callback function when the timer is deleted.
  usleep(500000);
  int value = counter.Value();
  usleep(500000);

  // Verify the counter has not been incremented.
  ASSERT_EQ(value, counter.Value());
}

struct TimerDeleteData {
  timer_t timer_id;
  pthread_t thread_id;
  volatile bool complete;
};

static void TimerDeleteCallback(sigval_t value) {
  TimerDeleteData* tdd = reinterpret_cast<TimerDeleteData*>(value.sival_ptr);

  tdd->thread_id = pthread_self();
  timer_delete(tdd->timer_id);
  tdd->complete = true;
}

TEST(time, timer_delete_from_timer_thread) {
  TimerDeleteData tdd;
  sigevent_t se;

  memset(&se, 0, sizeof(se));
  se.sigev_notify = SIGEV_THREAD;
  se.sigev_notify_function = TimerDeleteCallback;
  se.sigev_value.sival_ptr = &tdd;

  tdd.complete = false;
  ASSERT_EQ(0, timer_create(CLOCK_REALTIME, &se, &tdd.timer_id));

  itimerspec ts;
  ts.it_value.tv_sec = 1;
  ts.it_value.tv_nsec = 0;
  ts.it_interval.tv_sec = 0;
  ts.it_interval.tv_nsec = 0;
  ASSERT_EQ(0, timer_settime(tdd.timer_id, 0, &ts, NULL));

  time_t cur_time = time(NULL);
  while (!tdd.complete && (time(NULL) - cur_time) < 5);
  ASSERT_TRUE(tdd.complete);

#if defined(__BIONIC__)
  // Since bionic timers are implemented by creating a thread to handle the
  // callback, verify that the thread actually completes.
  cur_time = time(NULL);
  while (pthread_detach(tdd.thread_id) != ESRCH && (time(NULL) - cur_time) < 5);
  ASSERT_EQ(ESRCH, pthread_detach(tdd.thread_id));
#endif
}

TEST(time, clock_gettime) {
  // Try to ensure that our vdso clock_gettime is working.
  timespec ts1;
  ASSERT_EQ(0, clock_gettime(CLOCK_MONOTONIC, &ts1));
  timespec ts2;
  ASSERT_EQ(0, syscall(__NR_clock_gettime, CLOCK_MONOTONIC, &ts2));

  // What's the difference between the two?
  ts2.tv_sec -= ts1.tv_sec;
  ts2.tv_nsec -= ts1.tv_nsec;
  if (ts2.tv_nsec < 0) {
    --ts2.tv_sec;
    ts2.tv_nsec += NS_PER_S;
  }

  // Should be less than (a very generous, to try to avoid flakiness) 1000000ns.
  ASSERT_EQ(0, ts2.tv_sec);
  ASSERT_LT(ts2.tv_nsec, 1000000);
}

TEST(time, clock) {
  // clock(3) is hard to test, but a 1s sleep should cost less than 1ms.
  clock_t t0 = clock();
  sleep(1);
  clock_t t1 = clock();
  ASSERT_LT(t1 - t0, CLOCKS_PER_SEC / 1000);
}

pid_t GetInvalidPid() {
  FILE* fp = fopen("/proc/sys/kernel/pid_max", "r");
  long pid_max;
  fscanf(fp, "%ld", &pid_max);
  pid_t invalid_pid = static_cast<pid_t>(pid_max + 1);
  fclose(fp);
  return invalid_pid;
}

TEST(time, clock_getcpuclockid) {
  // For current process.
  clockid_t clockid;
  ASSERT_EQ(0, clock_getcpuclockid(getpid(), &clockid));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(clockid, &ts));

  // For parent process.
  ASSERT_EQ(0, clock_getcpuclockid(getppid(), &clockid));
  ASSERT_EQ(0, clock_gettime(clockid, &ts));

  // For invalid process.
  // We can't use -1 for invalid pid here, because clock_getcpuclockid() can't detect it.
  errno = 0;
  ASSERT_EQ(ESRCH, clock_getcpuclockid(GetInvalidPid(), &clockid));
  ASSERT_EQ(0, errno);
}

TEST(time, clock_settime) {
  errno = 0;
  timespec ts;
  ASSERT_EQ(-1, clock_settime(-1, &ts));
  ASSERT_EQ(EINVAL, errno);
}

TEST(time, clock_nanosleep) {
  timespec in;
  timespec out;
  ASSERT_EQ(EINVAL, clock_nanosleep(-1, 0, &in, &out));
}
