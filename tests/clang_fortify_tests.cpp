/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifndef __clang__
#error "Non-clang isn't supported"
#endif

//
// Clang compile-time and run-time tests for Bionic's FORTIFY.
//

// This file is compiled in two configurations to give us reasonable coverage of clang's
// FORTIFY implementation:
//
// 1. For compile-time checks, we use clang's diagnostic consumer
// (https://clang.llvm.org/doxygen/classclang_1_1VerifyDiagnosticConsumer.html#details)
// to check diagnostics (e.g. the expected-* comments everywhere).
//
// 2. For run-time checks, we build and run as regular gtests.

// Note that these tests do things like leaking memory. That's WAI.

//
// Configuration for the compile-time checks. (These comments have side effects!)
//
// Silence all "from 'diagnose_if'" `note`s from anywhere, including headers; they're uninteresting
// for this test case, and their line numbers may change over time.
// expected-note@* 0+{{from 'diagnose_if'}}
//
// Similarly, there are a few overload tricks we have to emit errors. Ignore any notes from those.
// expected-note@* 0+{{candidate function}}
//
// And finally, all explicitly-unavailable-here complaints from headers are
// uninteresting
// expected-note@* 0+{{has been explicitly marked unavailable here}}
//
// Note that some of these diagnostics come from clang itself, while others come from
// `diagnose_if`s sprinkled throughout Bionic.

#ifndef _FORTIFY_SOURCE
#error "_FORTIFY_SOURCE must be defined"
#endif

#include <sys/cdefs.h>

// This is a test specifically of bionic's FORTIFY machinery. Other stdlibs need not apply.
#ifndef __BIONIC__
// expected-no-diagnostics
#else

// As alluded to above, we're going to be doing some obviously very broken things in this file.
// FORTIFY helpfully flags a lot of it at compile-time, but we want it to *actually* crash, too. So
// let's wipe out any build-time errors.
#ifndef COMPILATION_TESTS
#undef __clang_error_if
#define __clang_error_if(...)
#undef __clang_warning_if
#define __clang_warning_if(...)
#pragma clang diagnostic ignored "-Wfortify-source"

// SOMETIMES_CONST allows clang to emit eager diagnostics when we're doing compilation tests, but
// blocks them otherwise. This is needed for diagnostics emitted with __enable_if.
#define SOMETIMES_CONST volatile
#else
#define SOMETIMES_CONST const
#endif

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>
#include <wchar.h>

#ifndef COMPILATION_TESTS
#include <android-base/silent_death_test.h>
#include <gtest/gtest.h>

#define CONCAT2(x, y) x##y
#define CONCAT(x, y) CONCAT2(x, y)
#define FORTIFY_TEST_NAME CONCAT(CONCAT(clang_fortify_test_, _FORTIFY_SOURCE), _DeathTest)

using FORTIFY_TEST_NAME = SilentDeathTest;

template <typename Fn>
__attribute__((noreturn)) static void ExitAfter(Fn&& f) {
  f();
  // No need to tear things down; our parent process should handle that.
  _exit(0);
}

// In any case (including failing tests), we always want to die after this.
#define DIE_WITH(expr, cond, regex) EXPECT_EXIT(ExitAfter([&] { (expr); }), cond, regex)

// EXPECT_NO_DEATH forks so that the test remains alive on a bug, and so that the environment
// doesn't get modified on no bug. (Environment modification is especially tricky to deal with given
// the *_STRUCT variants below.)
#define EXPECT_NO_DEATH(expr) DIE_WITH(expr, testing::ExitedWithCode(0), "")
#define EXPECT_FORTIFY_DEATH(expr) DIE_WITH(expr, testing::KilledBySignal(SIGABRT), "FORTIFY")
// Expecting death, but only if we're doing a "strict" struct-checking mode.
#if _FORTIFY_SOURCE > 1
#define EXPECT_FORTIFY_DEATH_STRUCT EXPECT_FORTIFY_DEATH
#else
#define EXPECT_FORTIFY_DEATH_STRUCT EXPECT_NO_DEATH
#endif

#define FORTIFY_TEST(test_name) TEST_F(FORTIFY_TEST_NAME, test_name)

#else  // defined(COMPILATION_TESTS)

#define EXPECT_NO_DEATH(expr) expr
#define EXPECT_FORTIFY_DEATH(expr) expr
#define EXPECT_FORTIFY_DEATH_STRUCT EXPECT_FORTIFY_DEATH
#define FORTIFY_TEST(test_name) void test_name()
#endif

const static int kBogusFD = -1;

FORTIFY_TEST(string) {
  char small_buffer[8] = {};

  {
    char large_buffer[sizeof(small_buffer) + 1] = {};
    // expected-error@+1{{will always overflow}}
    EXPECT_FORTIFY_DEATH(memcpy(small_buffer, large_buffer, sizeof(large_buffer)));
    // expected-error@+1{{will always overflow}}
    EXPECT_FORTIFY_DEATH(memmove(small_buffer, large_buffer, sizeof(large_buffer)));
    // FIXME(gbiv): look into removing mempcpy's diagnose_if bits once the b/149839606 roll sticks.
    // expected-error@+2{{will always overflow}}
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(mempcpy(small_buffer, large_buffer, sizeof(large_buffer)));
    // expected-error@+1{{will always overflow}}
    EXPECT_FORTIFY_DEATH(memset(small_buffer, 0, sizeof(large_buffer)));
    // expected-warning@+1{{arguments got flipped?}}
    EXPECT_NO_DEATH(memset(small_buffer, sizeof(small_buffer), 0));
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(bcopy(large_buffer, small_buffer, sizeof(large_buffer)));
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(bzero(small_buffer, sizeof(large_buffer)));
  }

  {
    const char large_string[] = "Hello!!!";
    static_assert(sizeof(large_string) > sizeof(small_buffer), "");

#if __clang_major__ > 13
    // expected-error@+3{{will always overflow}}
#endif
    // expected-error@+1{{string bigger than buffer}}
    EXPECT_FORTIFY_DEATH(strcpy(small_buffer, large_string));
    // expected-error@+1{{string bigger than buffer}}
    EXPECT_FORTIFY_DEATH(stpcpy(small_buffer, large_string));
    // expected-error@+1{{size argument is too large}}
    EXPECT_FORTIFY_DEATH(strncpy(small_buffer, large_string, sizeof(large_string)));
    // expected-error@+1{{size argument is too large}}
    EXPECT_FORTIFY_DEATH(stpncpy(small_buffer, large_string, sizeof(large_string)));
    // expected-error@+1{{string bigger than buffer}}
    EXPECT_FORTIFY_DEATH(strcat(small_buffer, large_string));
    // expected-error@+1{{size argument is too large}}
    EXPECT_FORTIFY_DEATH(strncat(small_buffer, large_string, sizeof(large_string)));
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(strlcpy(small_buffer, large_string, sizeof(large_string)));
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(strlcat(small_buffer, large_string, sizeof(large_string)));
  }

  {
    struct {
      char tiny_buffer[4];
      char tiny_buffer2[4];
    } split = {};

    EXPECT_NO_DEATH(memcpy(split.tiny_buffer, &split, sizeof(split)));
    EXPECT_NO_DEATH(memcpy(split.tiny_buffer, &split, sizeof(split)));
    EXPECT_NO_DEATH(memmove(split.tiny_buffer, &split, sizeof(split)));
    EXPECT_NO_DEATH(mempcpy(split.tiny_buffer, &split, sizeof(split)));
    EXPECT_NO_DEATH(memset(split.tiny_buffer, 0, sizeof(split)));

    EXPECT_NO_DEATH(bcopy(&split, split.tiny_buffer, sizeof(split)));
    EXPECT_NO_DEATH(bzero(split.tiny_buffer, sizeof(split)));

    const char small_string[] = "Hi!!";
    static_assert(sizeof(small_string) > sizeof(split.tiny_buffer), "");

#if _FORTIFY_SOURCE > 1
#if __clang_major__ > 13
    // expected-error@+4{{will always overflow}}
#endif
    // expected-error@+2{{string bigger than buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(strcpy(split.tiny_buffer, small_string));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{string bigger than buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(stpcpy(split.tiny_buffer, small_string));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{size argument is too large}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(strncpy(split.tiny_buffer, small_string, sizeof(small_string)));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{size argument is too large}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(stpncpy(split.tiny_buffer, small_string, sizeof(small_string)));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{string bigger than buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(strcat(split.tiny_buffer, small_string));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{size argument is too large}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(strncat(split.tiny_buffer, small_string, sizeof(small_string)));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{size bigger than buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(strlcat(split.tiny_buffer, small_string, sizeof(small_string)));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{size bigger than buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(strlcpy(split.tiny_buffer, small_string, sizeof(small_string)));
  }
}

FORTIFY_TEST(fcntl) {
  const char target[] = "/dev/null";
  int dirfd = 0;

  // These all emit hard errors without diagnose_if, so running them is a bit
  // more involved.
#ifdef COMPILATION_TESTS
  // expected-error@+1{{too many arguments}}
  open("/", 0, 0, 0);
  // expected-error@+1{{too many arguments}}
  open64("/", 0, 0, 0);
  // expected-error@+1{{too many arguments}}
  openat(0, "/", 0, 0, 0);
  // expected-error@+1{{too many arguments}}
  openat64(0, "/", 0, 0, 0);
#endif

  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(open(target, O_CREAT));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(open(target, O_TMPFILE));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(open64(target, O_CREAT));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(open64(target, O_TMPFILE));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(openat(dirfd, target, O_CREAT));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(openat(dirfd, target, O_TMPFILE));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(openat64(dirfd, target, O_CREAT));
  // expected-error@+1{{missing mode}}
  EXPECT_FORTIFY_DEATH(openat64(dirfd, target, O_TMPFILE));

  // expected-warning@+1{{superfluous mode bits}}
  EXPECT_NO_DEATH(open(target, O_RDONLY, 0777));
  // expected-warning@+1{{superfluous mode bits}}
  EXPECT_NO_DEATH(open64(target, O_RDONLY, 0777));
  // expected-warning@+1{{superfluous mode bits}}
  EXPECT_NO_DEATH(openat(dirfd, target, O_RDONLY, 0777));
  // expected-warning@+1{{superfluous mode bits}}
  EXPECT_NO_DEATH(openat64(dirfd, target, O_RDONLY, 0777));
}

// Since these emit hard errors, it's sort of hard to run them...
#ifdef COMPILATION_TESTS
namespace compilation_tests {
template <typename T>
static T declval() {
  __builtin_unreachable();
}

static void testFormatStrings() {
  const auto unsigned_value = declval<unsigned long long>();
  const auto* unknown_string = declval<const char*>();
  const auto va = *declval<va_list*>();

  {
    auto some_fd = declval<int>();
    // expected-warning@+1{{format specifies type 'int'}}
    dprintf(some_fd, "%d", unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    dprintf(some_fd, unknown_string, unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    vdprintf(1, unknown_string, va);
  }

  {
    auto* retval = declval<char*>();
#if 0
    // expected-error@+2{{ignoring return value}}
#endif
    // expected-warning@+1{{format specifies type 'int'}}
    asprintf(&retval, "%d", unsigned_value);
#if 0
    // expected-error@+2{{ignoring return value}}
#endif
    // expected-warning@+1{{format string is not a string literal}}
    asprintf(&retval, unknown_string, unsigned_value);
#if 0
    // expected-error@+2{{ignoring return value}}
#endif
    // expected-warning@+1{{format string is not a string literal}}
    vasprintf(&retval, unknown_string, va);
  }

  // expected-warning@+1{{format specifies type 'int'}}
  syslog(0, "%d", unsigned_value);
  // expected-warning@+1{{format string is not a string literal}}
  syslog(0, unknown_string, unsigned_value);
  // expected-warning@+1{{format string is not a string literal}}
  vsyslog(0, unknown_string, va);

  {
    auto* file = declval<FILE*>();
    // expected-warning@+1{{format specifies type 'int'}}
    fprintf(file, "%d", unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    fprintf(file, unknown_string, unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    vfprintf(file, unknown_string, va);
  }

  // expected-warning@+1{{format specifies type 'int'}}
  printf("%d", unsigned_value);
  // expected-warning@+1{{format string is not a string literal}}
  printf(unknown_string, unsigned_value);
  // expected-warning@+1{{format string is not a string literal}}
  vprintf(unknown_string, va);

  {
    char buf[128];
    // expected-warning@+1{{format specifies type 'int'}}
    sprintf(buf, "%d", unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    sprintf(buf, unknown_string, unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    sprintf(buf, unknown_string, va);

    // expected-warning@+1{{format specifies type 'int'}}
    snprintf(buf, sizeof(buf), "%d", unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    snprintf(buf, sizeof(buf), unknown_string, unsigned_value);
    // expected-warning@+1{{format string is not a string literal}}
    vsnprintf(buf, sizeof(buf), unknown_string, va);
  }

  // FIXME: below are general format string cases where clang should probably try to warn.
  {
    char buf[4];
    sprintf(buf, "%s", "1234");
    sprintf(buf, "1%s4", "23");
    sprintf(buf, "%d", 1234);

    // Similar thoughts for strncpy, etc.
  }
}

static void testStdlib() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnonnull"
  char path_buffer[PATH_MAX - 1];
  // expected-warning@+2{{ignoring return value of function}}
  // expected-error@+1{{must be NULL or a pointer to a buffer with >= PATH_MAX bytes}}
  realpath("/", path_buffer);
  // expected-warning@+1{{ignoring return value of function}}
  realpath("/", nullptr);

  // expected-warning@+2{{ignoring return value of function}}
  // expected-error@+1{{flipped arguments?}}
  realpath(nullptr, path_buffer);

  // expected-warning@+2{{ignoring return value of function}}
  // expected-error@+1{{flipped arguments?}}
  realpath(nullptr, nullptr);
#pragma clang diagnostic pop
}
}  // namespace compilation_tests
#endif

FORTIFY_TEST(poll) {
  int pipe_fds[2];
  if (pipe(pipe_fds)) err(1, "pipe failed");

  // after this, pipe_fds[0] should always report RDHUP
  if (close(pipe_fds[1])) err(1, "close failed");

  struct pollfd poll_fd = { pipe_fds[0], POLLRDHUP, 0 };
  {
    struct pollfd few_fds[] = { poll_fd, poll_fd };
    // expected-error@+1{{fd_count is larger than the given buffer}}
    EXPECT_FORTIFY_DEATH(poll(few_fds, 3, 0));
    // expected-error@+1{{fd_count is larger than the given buffer}}
    EXPECT_FORTIFY_DEATH(ppoll(few_fds, 3, 0, 0));
    // expected-error@+1{{fd_count is larger than the given buffer}}
    EXPECT_FORTIFY_DEATH(ppoll64(few_fds, 3, 0, nullptr));
  }

  {
    struct {
      struct pollfd few[2];
      struct pollfd extra[1];
    } fds = { { poll_fd, poll_fd }, { poll_fd } };
    static_assert(sizeof(fds) >= sizeof(struct pollfd) * 3, "");

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{fd_count is larger than the given buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(poll(fds.few, 3, 0));

    struct timespec timeout = {};
#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{fd_count is larger than the given buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(ppoll(fds.few, 3, &timeout, 0));

#if _FORTIFY_SOURCE > 1
    // expected-error@+2{{fd_count is larger than the given buffer}}
#endif
    EXPECT_FORTIFY_DEATH_STRUCT(ppoll64(fds.few, 3, 0, nullptr));
  }
}

FORTIFY_TEST(socket) {
  {
    char small_buffer[8];
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(recv(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0));
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(recvfrom(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0, 0, 0));

    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(send(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0));
    // expected-error@+1{{size bigger than buffer}}
    EXPECT_FORTIFY_DEATH(sendto(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0, 0, 0));
  }

  {
    struct {
      char tiny_buffer[4];
      char tiny_buffer2;
    } split = {};

    EXPECT_NO_DEATH(recv(kBogusFD, split.tiny_buffer, sizeof(split), 0));
    EXPECT_NO_DEATH(recvfrom(kBogusFD, split.tiny_buffer, sizeof(split), 0, 0, 0));
  }
}

FORTIFY_TEST(sys_stat) {
  // expected-error@+1{{'umask' called with invalid mode}}
  EXPECT_FORTIFY_DEATH(umask(01777));
}

FORTIFY_TEST(stdio) {
  char small_buffer[8] = {};
  {
    // expected-error@+1{{size argument is too large}}
    EXPECT_FORTIFY_DEATH(snprintf(small_buffer, sizeof(small_buffer) + 1, ""));

    va_list va;
    // expected-error@+2{{size argument is too large}}
    // expected-warning@+1{{format string is empty}}
    EXPECT_FORTIFY_DEATH(vsnprintf(small_buffer, sizeof(small_buffer) + 1, "", va));

    const char *SOMETIMES_CONST format_string = "aaaaaaaaa";

    // expected-error@+1{{format string will always overflow}}
    EXPECT_FORTIFY_DEATH(sprintf(small_buffer, format_string));
  }

  // expected-error@+1{{size should not be negative}}
  EXPECT_FORTIFY_DEATH(fgets(small_buffer, -1, stdin));
  // expected-error@+1{{size is larger than the destination buffer}}
  EXPECT_FORTIFY_DEATH(fgets(small_buffer, sizeof(small_buffer) + 1, stdin));

  // expected-error@+1{{size * count overflows}}
  EXPECT_NO_DEATH(fread(small_buffer, 2, (size_t)-1, stdin));
  // expected-error@+1{{size * count is too large for the given buffer}}
  EXPECT_FORTIFY_DEATH(fread(small_buffer, 1, sizeof(small_buffer) + 1, stdin));

  // expected-error@+1{{size * count overflows}}
  EXPECT_NO_DEATH(fwrite(small_buffer, 2, (size_t)-1, stdout));
  // expected-error@+1{{size * count is too large for the given buffer}}
  EXPECT_FORTIFY_DEATH(fwrite(small_buffer, 1, sizeof(small_buffer) + 1, stdout));
}

FORTIFY_TEST(unistd) {
  char small_buffer[8];

  // Return value warnings are (sort of) a part of FORTIFY, so we don't ignore them.
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(read(kBogusFD, small_buffer, sizeof(small_buffer) + 1));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(pread(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(pread64(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(write(kBogusFD, small_buffer, sizeof(small_buffer) + 1));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(pwrite(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(pwrite64(kBogusFD, small_buffer, sizeof(small_buffer) + 1, 0));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(readlink("/", small_buffer, sizeof(small_buffer) + 1));
#if 0
  // expected-error@+2{{ignoring return value of function}}
#endif
  // expected-error@+1{{bytes overflows the given object}}
  EXPECT_FORTIFY_DEATH(getcwd(small_buffer, sizeof(small_buffer) + 1));

  // getcwd allocates and returns a buffer if you pass null to getcwd
  EXPECT_NO_DEATH(getcwd(nullptr, 0));
  EXPECT_NO_DEATH(getcwd(nullptr, 4096));

  struct {
    char tiny_buffer[4];
    char tiny_buffer2[4];
  } split;

  EXPECT_NO_DEATH(read(kBogusFD, split.tiny_buffer, sizeof(split)));
  EXPECT_NO_DEATH(pread(kBogusFD, split.tiny_buffer, sizeof(split), 0));
  EXPECT_NO_DEATH(pread64(kBogusFD, split.tiny_buffer, sizeof(split), 0));
  EXPECT_NO_DEATH(write(kBogusFD, split.tiny_buffer, sizeof(split)));
  EXPECT_NO_DEATH(pwrite(kBogusFD, split.tiny_buffer, sizeof(split), 0));
  EXPECT_NO_DEATH(pwrite64(kBogusFD, split.tiny_buffer, sizeof(split), 0));

#if _FORTIFY_SOURCE > 1
  // expected-error@+2{{bytes overflows the given object}}
#endif
  EXPECT_FORTIFY_DEATH_STRUCT(readlink("/", split.tiny_buffer, sizeof(split)));
#if _FORTIFY_SOURCE > 1
  // expected-error@+2{{bytes overflows the given object}}
#endif
  EXPECT_FORTIFY_DEATH_STRUCT(getcwd(split.tiny_buffer, sizeof(split)));

  {
    char* volatile unknown = small_buffer;
    const size_t count = static_cast<size_t>(SSIZE_MAX) + 1;
    // expected-error@+1{{'count' must be <= SSIZE_MAX}}
    EXPECT_FORTIFY_DEATH(read(kBogusFD, unknown, count));
    // expected-error@+1{{'count' must be <= SSIZE_MAX}}
    EXPECT_FORTIFY_DEATH(pread(kBogusFD, unknown, count, 0));
    // expected-error@+1{{'count' must be <= SSIZE_MAX}}
    EXPECT_FORTIFY_DEATH(pread64(kBogusFD, unknown, count, 0));
    // expected-error@+1{{'count' must be <= SSIZE_MAX}}
    EXPECT_FORTIFY_DEATH(write(kBogusFD, unknown, count));
    // expected-error@+1{{'count' must be <= SSIZE_MAX}}
    EXPECT_FORTIFY_DEATH(pwrite(kBogusFD, unknown, count, 0));
    // expected-error@+1{{'count' must be <= SSIZE_MAX}}
    EXPECT_FORTIFY_DEATH(pwrite64(kBogusFD, unknown, count, 0));
  }
}

#endif  // defined(__BIONIC__)
