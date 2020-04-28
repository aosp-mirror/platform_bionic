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
 * If this test fails, you can see the compiler's output by erasing a few args from the failing
 * command. Specifically, delete everything before the path/to/the/compiler, then delete the first
 * arg after the path/to/the/compiler. For example, given the following command:
 *
 * bionic/tests/file-check-cxx out/host/linux-x86/bin/FileCheck \
 * prebuilts/clang/host/linux-x86/clang-4053586/bin/clang++ CLANG    -I bionic/tests -I ...
 *
 * If you delete everything before clang++ and delete "CLANG", then you'll end up with:
 *
 * prebuilts/clang/host/linux-x86/clang-4053586/bin/clang++ -I bionic/tests -I ...
 *
 * Which is the command that FileCheck executes.
 */

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

void test_sprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: call to unavailable function 'sprintf': format string will always overflow destination buffer
  sprintf(buf, "foobar");  // NOLINT(runtime/printf)

  // TODO: clang should emit a warning, but doesn't
  sprintf(buf, "%s", "foobar");  // NOLINT(runtime/printf)
}

void test_snprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: call to unavailable function 'snprintf': format string will always overflow destination buffer
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

  // CLANG: error: 'memcpy' called with size bigger than buffer
  memcpy(buf, "foobar", sizeof("foobar") + 100);
}

void test_memmove() {
  char buf[4];

  // CLANG: error: 'memmove' called with size bigger than buffer
  memmove(buf, "foobar", sizeof("foobar"));
}

void test_memset() {
  char buf[4];

  // CLANG: error: 'memset' called with size bigger than buffer
  memset(buf, 0, 6);
}

void test_strcpy() {
  char buf[4];

  // CLANG: error: 'strcpy' called with string bigger than buffer
  strcpy(buf, "foobar");  // NOLINT(runtime/printf)

  // CLANG: error: 'strcpy' called with string bigger than buffer
  strcpy(buf, "quux");
}

void test_stpcpy() {
  char buf[4];

  // CLANG: error: 'stpcpy' called with string bigger than buffer
  stpcpy(buf, "foobar");

  // CLANG: error: 'stpcpy' called with string bigger than buffer
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

  // CLANG: error: in call to 'fgets', size should not be negative
  fgets(buf, -1, stdin);

  // CLANG: error: in call to 'fgets', size is larger than the destination buffer
  fgets(buf, 6, stdin);
}

void test_recvfrom() {
  char buf[4];
  sockaddr_in addr;

  // CLANG: error: 'recvfrom' called with size bigger than buffer
  recvfrom(0, buf, 6, 0, reinterpret_cast<sockaddr*>(&addr), nullptr);
}

void test_recv() {
  char buf[4] = {0};

  // CLANG: error: 'recv' called with size bigger than buffer
  recv(0, buf, 6, 0);
}

void test_umask() {
  // CLANG: error: 'umask' called with invalid mode
  umask(01777);
}

void test_read() {
  char buf[4];
  // CLANG: error: in call to 'read', 'count' bytes overflows the given object
  read(0, buf, 6);
}

void test_open() {
  // CLANG: error: 'open' called with O_CREAT or O_TMPFILE, but missing mode
  open("/dev/null", O_CREAT);

  // CLANG: error: 'open' called with O_CREAT or O_TMPFILE, but missing mode
  open("/dev/null", O_TMPFILE);

  // CLANG: error: call to unavailable function 'open': too many arguments
  open("/dev/null", O_CREAT, 0, 0);

  // CLANG: error: call to unavailable function 'open': too many arguments
  open("/dev/null", O_TMPFILE, 0, 0);

  // CLANG: warning: 'open' has superfluous mode bits; missing O_CREAT?
  open("/dev/null", O_RDONLY, 0644);

  // CLANG: warning: 'open' has superfluous mode bits; missing O_CREAT?
  open("/dev/null", O_DIRECTORY, 0644);
}

void test_poll() {
  pollfd fds[1];
  // CLANG: error: in call to 'poll', fd_count is larger than the given buffer
  poll(fds, 2, 0);
}

void test_ppoll() {
  pollfd fds[1];
  timespec timeout;
  // CLANG: error: in call to 'ppoll', fd_count is larger than the given buffer
  ppoll(fds, 2, &timeout, nullptr);
}

void test_ppoll64() {
  pollfd fds[1];
  timespec timeout;
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: in call to 'ppoll64', fd_count is larger than the given buffer
  ppoll64(fds, 2, &timeout, nullptr);
}

void test_fread_overflow() {
  char buf[4];
  // CLANG: error: in call to 'fread', size * count overflows
  fread(buf, 2, (size_t)-1, stdin);
}

void test_fread_too_big() {
  char buf[4];
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: in call to 'fread', size * count is too large for the given buffer
  fread(buf, 1, 5, stdin);
}

void test_fwrite_overflow() {
  char buf[4] = {0};
  // CLANG: error: in call to 'fwrite', size * count overflows
  fwrite(buf, 2, (size_t)-1, stdout);
}

void test_fwrite_too_big() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: in call to 'fwrite', size * count is too large for the given buffer
  fwrite(buf, 1, 5, stdout);
}

void test_getcwd() {
  char buf[4];
  // CLANG: error: in call to 'getcwd', 'size' bytes overflows the given object
  getcwd(buf, 5);
}

void test_pwrite64_size() {
  char buf[4] = {0};
  // CLANG: error: in call to 'pwrite64', 'count' bytes overflows the given object
  pwrite64(STDOUT_FILENO, buf, 5, 0);
}

void test_pwrite64_too_big_malloc() {
  void *buf = calloc(atoi("5"), 1);
  // clang should emit a warning, but probably never will.
  pwrite64(STDOUT_FILENO, buf, SIZE_MAX, 0);
}

void test_pwrite64_too_big() {
  char buf[4] = {0};
  // CLANG: error: in call to 'pwrite64', 'count' must be <= SSIZE_MAX
  pwrite64(STDOUT_FILENO, buf, SIZE_MAX, 0);
}

void test_write_size() {
  char buf[4] = {0};
  // CLANG: error: in call to 'write', 'count' bytes overflows the given object
  write(STDOUT_FILENO, buf, 5);
}

void test_memset_args_flipped() {
  char from[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: 'memset' will set 0 bytes; maybe the arguments got flipped?
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmemset-transposed-args"
  memset(from, sizeof(from), 0);
#pragma clang diagnostic pop
}

void test_sendto() {
  char buf[4] = {0};
  sockaddr_in addr;

  // CLANG: error: 'sendto' called with size bigger than buffer
  sendto(0, buf, 6, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
}

void test_send() {
  char buf[4] = {0};

  // CLANG: error: 'send' called with size bigger than buffer
  send(0, buf, 6, 0);
}

void test_realpath() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: 'realpath' output parameter must be NULL or a pointer to a buffer with >= PATH_MAX bytes
  realpath(".", buf);

  // This is fine.
  realpath(".", nullptr);

  char bigbuf[PATH_MAX];
  // CLANG: error: 'realpath': NULL path is never correct; flipped arguments?
  realpath(nullptr, bigbuf);
}
