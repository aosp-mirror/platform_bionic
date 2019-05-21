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

/*
 * Silence all notes about enable_if-related 'candidates'; they're nice to know
 * about for users, but this test doesn't care.
 */
// expected-note@* 0+{{candidate function}}

/* Similarly, ignore all "from 'diagnose_if'"s. */
// expected-note@* 0+{{from 'diagnose_if'}}


#undef _FORTIFY_SOURCE
#define _FORTIFY_SOURCE 2
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#if !defined(__BIONIC__)
#  error "This only works with Bionic."
#endif

void test_sprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-error@+1{{call to unavailable function 'sprintf': format string will always overflow destination buffer}}
  sprintf(buf, "foobar");  // NOLINT(runtime/printf)

  // TODO: clang should emit a warning, but doesn't
  sprintf(buf, "%s", "foobar");  // NOLINT(runtime/printf)
}

void test_snprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-error@+1{{call to unavailable function 'snprintf': format string will always overflow destination buffer}}
  snprintf(buf, 5, "foobar");  // NOLINT(runtime/printf)

  // TODO: clang should emit a warning, but doesn't
  snprintf(buf, 5, "%s", "foobar");  // NOLINT(runtime/printf)

  // TODO: clang should emit a warning, but doesn't
  snprintf(buf, 5, " %s ", "foobar");  // NOLINT(runtime/printf)

  // TODO: clang should emit a warning, but doesn't
  snprintf(buf, 5, "%d", 100000);  // NOLINT(runtime/printf)
}

void test_memcpy() {
  char buf[4];

  // expected-error@+1{{'memcpy' called with size bigger than buffer}}
  memcpy(buf, "foobar", sizeof("foobar") + 100);
}

void test_memmove() {
  char buf[4];

  // expected-error@+1{{'memmove' called with size bigger than buffer}}
  memmove(buf, "foobar", sizeof("foobar"));
}

void test_memset() {
  char buf[4];

  // expected-error@+1{{'memset' called with size bigger than buffer}}
  memset(buf, 0, 6);
}

void test_strcpy() {
  char buf[4];

  // expected-error@+1{{'strcpy' called with string bigger than buffer}}
  strcpy(buf, "foobar");  // NOLINT(runtime/printf)

  // expected-error@+1{{'strcpy' called with string bigger than buffer}}
  strcpy(buf, "quux");
}

void test_stpcpy() {
  char buf[4];

  // expected-error@+1{{'stpcpy' called with string bigger than buffer}}
  stpcpy(buf, "foobar");

  // expected-error@+1{{'stpcpy' called with string bigger than buffer}}
  stpcpy(buf, "quux");
}

void test_strncpy() {
  char buf[4];

  // TODO: clang should emit a warning, but doesn't
  strncpy(buf, "foobar", sizeof("foobar"));
}

void test_strcat() {
  char buf[4] = "";

  // TODO: clang should emit a warning, but doesn't
  strcat(buf, "foobar");  // NOLINT(runtime/printf)
}

void test_strncat() {
  char buf[4] = "";

  // TODO: clang should emit a warning, but doesn't
  strncat(buf, "foobar", sizeof("foobar"));
}

void test_vsprintf(const char* fmt, ...) {
  va_list va;
  char buf[4];
  va_start(va, fmt);

  // clang should emit a warning, but doesn't
  vsprintf(buf, "foobar", va);
  va_end(va);
}

void test_vsnprintf(const char* fmt, ...) {
  va_list va;
  char buf[4];
  va_start(va, fmt);

  // clang should emit a warning, but doesn't
  vsnprintf(buf, 5, "foobar", va);  // NOLINT(runtime/printf)

  va_end(va);
}

void test_fgets() {
  char buf[4];

  // expected-error@+1{{in call to 'fgets', size should not be negative}}
  fgets(buf, -1, stdin);

  // expected-error@+1{{in call to 'fgets', size is larger than the destination buffer}}
  fgets(buf, 6, stdin);
}

void test_recvfrom() {
  char buf[4];
  sockaddr_in addr;

  // expected-error@+1{{'recvfrom' called with size bigger than buffer}}
  recvfrom(0, buf, 6, 0, reinterpret_cast<sockaddr*>(&addr), nullptr);
}

void test_recv() {
  char buf[4] = {0};

  // expected-error@+1{{'recv' called with size bigger than buffer}}
  recv(0, buf, 6, 0);
}

void test_umask() {
  // expected-error@+1{{'umask' called with invalid mode}}
  umask(01777);
}

void test_read() {
  char buf[4];
  // expected-error@+1{{in call to 'read', 'count' bytes overflows the given object}}
  read(0, buf, 6);
}

void test_open() {
  // expected-error@+1{{'open' called with O_CREAT or O_TMPFILE, but missing mode}}
  open("/dev/null", O_CREAT);

  // expected-error@+1{{'open' called with O_CREAT or O_TMPFILE, but missing mode}}
  open("/dev/null", O_TMPFILE);

  // expected-error@+1{{call to unavailable function 'open': too many arguments}}
  open("/dev/null", O_CREAT, 0, 0);

  // expected-error@+1{{call to unavailable function 'open': too many arguments}}
  open("/dev/null", O_TMPFILE, 0, 0);

  // expected-warning@+1{{'open' has superfluous mode bits; missing O_CREAT?}}
  open("/dev/null", O_RDONLY, 0644);

  // expected-warning@+1{{'open' has superfluous mode bits; missing O_CREAT?}}
  open("/dev/null", O_DIRECTORY, 0644);
}

void test_poll() {
  pollfd fds[1];
  // expected-error@+1{{in call to 'poll', fd_count is larger than the given buffer}}
  poll(fds, 2, 0);
}

void test_ppoll() {
  pollfd fds[1];
  timespec timeout;
  // expected-error@+1{{in call to 'ppoll', fd_count is larger than the given buffer}}
  ppoll(fds, 2, &timeout, nullptr);
}

void test_ppoll64() {
  pollfd fds[1];
  timespec timeout;
  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-error@+1{{in call to 'ppoll64', fd_count is larger than the given buffer}}
  ppoll64(fds, 2, &timeout, nullptr);
}

void test_fread_overflow() {
  char buf[4];
  // expected-error@+1{{in call to 'fread', size * count overflows}}
  fread(buf, 2, (size_t)-1, stdin);
}

void test_fread_too_big() {
  char buf[4];
  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-error@+1{{in call to 'fread', size * count is too large for the given buffer}}
  fread(buf, 1, 5, stdin);
}

void test_fwrite_overflow() {
  char buf[4] = {0};
  // expected-error@+1{{in call to 'fwrite', size * count overflows}}
  fwrite(buf, 2, (size_t)-1, stdout);
}

void test_fwrite_too_big() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-error@+1{{in call to 'fwrite', size * count is too large for the given buffer}}
  fwrite(buf, 1, 5, stdout);
}

void test_getcwd() {
  char buf[4];
  // expected-error@+1{{in call to 'getcwd', 'size' bytes overflows the given object}}
  getcwd(buf, 5);
}

void test_pwrite64_size() {
  char buf[4] = {0};
  // expected-error@+1{{in call to 'pwrite64', 'count' bytes overflows the given object}}
  pwrite64(STDOUT_FILENO, buf, 5, 0);
}

void test_pwrite64_too_big_malloc() {
  void *buf = calloc(atoi("5"), 1);
  // expected-error@+1{{in call to 'pwrite64', 'count' must be <= SSIZE_MAX}}
  pwrite64(STDOUT_FILENO, buf, SIZE_MAX, 0);
}

void test_pwrite64_too_big() {
  char buf[4] = {0};
  // expected-error@+1{{in call to 'pwrite64', 'count' must be <= SSIZE_MAX}}
  pwrite64(STDOUT_FILENO, buf, SIZE_MAX, 0);
}

void test_write_size() {
  char buf[4] = {0};
  // expected-error@+1{{in call to 'write', 'count' bytes overflows the given object}}
  write(STDOUT_FILENO, buf, 5);
}

void test_memset_args_flipped() {
  char from[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-warning@+1{{'memset' will set 0 bytes; maybe the arguments got flipped?}}
  memset(from, sizeof(from), 0);
}

void test_sendto() {
  char buf[4] = {0};
  sockaddr_in addr;

  // expected-error@+1{{'sendto' called with size bigger than buffer}}
  sendto(0, buf, 6, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
}

void test_send() {
  char buf[4] = {0};

  // expected-error@+1{{'send' called with size bigger than buffer}}
  send(0, buf, 6, 0);
}

void test_realpath() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // expected-error@+1{{'realpath' output parameter must be NULL or a pointer to a buffer with >= PATH_MAX bytes}}
  realpath(".", buf);

  // This is fine.
  realpath(".", nullptr);

  char bigbuf[PATH_MAX];
  // expected-error@+1{{'realpath': NULL path is never correct; flipped arguments?}}
  realpath(nullptr, bigbuf);
}
