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
 * If you delete everything before clang++ and delete "CLANG" (or "GCC" if gcc is failing), then
 * you'll end up with:
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
  // GCC: warning: call to int __builtin___sprintf_chk(char*, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // CLANG: error: call to unavailable function 'sprintf': format string will always overflow destination buffer
  sprintf(buf, "foobar");  // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___sprintf_chk(char*, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  sprintf(buf, "%s", "foobar");  // NOLINT(runtime/printf)
}

void test_snprintf() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // CLANG: error: call to unavailable function 'snprintf': format string will always overflow destination buffer
  snprintf(buf, 5, "foobar");  // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "%s", "foobar");  // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, " %s ", "foobar");  // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___snprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, ...) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  snprintf(buf, 5, "%d", 100000);  // NOLINT(runtime/printf)
}

void test_memcpy() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to void* __builtin___memcpy_chk(void*, const void*, {{(long )?}}unsigned int, {{(long )?}}unsigned int) will always overflow destination buffer
  // CLANG: error: 'memcpy' called with size bigger than buffer
  memcpy(buf, "foobar", sizeof("foobar") + 100);
}

void test_memmove() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to void* __builtin___memmove_chk(void*, const void*, {{(long )?}}unsigned int, {{(long )?}}unsigned int) will always overflow destination buffer
  // CLANG: error: 'memmove' called with size bigger than buffer
  memmove(buf, "foobar", sizeof("foobar"));
}

void test_memset() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to void* __builtin___memset_chk(void*, int, {{(long )?}}unsigned int, {{(long )?}}unsigned int) will always overflow destination buffer
  // CLANG: error: 'memset' called with size bigger than buffer
  memset(buf, 0, 6);
}

void test_strcpy() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to {{(char\* __builtin___strcpy_chk\(char\*, const char\*, unsigned int\))|(void\* __builtin___memcpy_chk\(void\*, const void\*, (long )?unsigned int, (long )?unsigned int\))}} will always overflow destination buffer
  // CLANG: error: 'strcpy' called with string bigger than buffer
  strcpy(buf, "foobar");  // NOLINT(runtime/printf)

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to {{(char\* __builtin___strcpy_chk\(char\*, const char\*, unsigned int\))|(void\* __builtin___memcpy_chk\(void\*, const void\*, (long )?unsigned int, (long )?unsigned int\))}} will always overflow destination buffer
  // CLANG: error: 'strcpy' called with string bigger than buffer
  strcpy(buf, "quux");
}

void test_stpcpy() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to char* __builtin___stpcpy_chk(char*, const char*, {{(long )?}}unsigned int) will always overflow destination buffer
  // CLANG: error: 'stpcpy' called with string bigger than buffer
  stpcpy(buf, "foobar");

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to char* __builtin___stpcpy_chk(char*, const char*, {{(long )?}}unsigned int) will always overflow destination buffer
  // CLANG: error: 'stpcpy' called with string bigger than buffer
  stpcpy(buf, "quux");
}

void test_strncpy() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to char* __builtin___strncpy_chk(char*, const char*, {{(long )?}}unsigned int, {{(long )?}}unsigned int) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  strncpy(buf, "foobar", sizeof("foobar"));
}

void test_strcat() {
  char buf[4] = "";

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to {{(char\* __builtin___strcat_chk\(char\*, const char\*, unsigned int\))|(void\* __builtin___memcpy_chk\(void\*, const void\*, (long )?unsigned int, (long )?unsigned int\))}} will always overflow destination buffer
  // clang should emit a warning, but doesn't
  strcat(buf, "foobar");  // NOLINT(runtime/printf)
}

void test_strncat() {
  char buf[4] = "";

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to {{(char\* __builtin___strcat_chk\(char\*, const char\*, unsigned int\))|(void\* __builtin___memcpy_chk\(void\*, const void\*, (long )?unsigned int, (long )?unsigned int\))}} will always overflow destination buffer
  // gcc output warning with __builtin___strcat_chk for __builtin___strncat_chk.
  // clang should emit a warning, but doesn't
  strncat(buf, "foobar", sizeof("foobar"));
}

void test_vsprintf(const char* fmt, ...) {
  va_list va;
  char buf[4];
  va_start(va, fmt);

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___vsprintf_chk(char*, int, {{(long )?}}unsigned int, const char*, {{(__va_list)|(void\*)|(char\*)|(__va_list_tag\*)}}) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  vsprintf(buf, "foobar", va);
  va_end(va);
}

void test_vsnprintf(const char* fmt, ...) {
  va_list va;
  char buf[4];
  va_start(va, fmt);

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: warning: call to int __builtin___vsnprintf_chk(char*, {{(long )?}}unsigned int, int, {{(long )?}}unsigned int, const char*, {{(__va_list)|(void\*)|(char\*)|(__va_list_tag\*)}}) will always overflow destination buffer
  // clang should emit a warning, but doesn't
  vsnprintf(buf, 5, "foobar", va);  // NOLINT(runtime/printf)

  va_end(va);
}

void test_fgets() {
  char buf[4];

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__fgets_too_small_error' declared with attribute error: fgets called with size less than zero
  // CLANG: error: in call to 'fgets', size should not be negative
  fgets(buf, -1, stdin);

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__fgets_too_big_error' declared with attribute error: fgets called with size bigger than buffer
  // CLANG: error: in call to 'fgets', size is larger than the destination buffer
  fgets(buf, 6, stdin);
}

void test_recvfrom() {
  char buf[4];
  sockaddr_in addr;

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__recvfrom_error' declared with attribute error: 'recvfrom' called with size bigger than buffer
  // CLANG: error: 'recvfrom' called with size bigger than buffer
  recvfrom(0, buf, 6, 0, reinterpret_cast<sockaddr*>(&addr), NULL);
}

void test_recv() {
  char buf[4] = {0};

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__recvfrom_error' declared with attribute error: 'recvfrom' called with size bigger than buffer
  // CLANG: error: 'recv' called with size bigger than buffer
  recv(0, buf, 6, 0);
}

void test_umask() {
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__umask_invalid_mode' declared with attribute error: 'umask' called with invalid mode
  // CLANG: error: 'umask' called with invalid mode
  umask(01777);
}

void test_read() {
  char buf[4];
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__read_dest_size_error' declared with attribute error: read called with size bigger than destination
  // CLANG: error: in call to 'read', 'count' bytes overflows the given object
  read(0, buf, 6);
}

void test_open() {
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__creat_missing_mode' declared with attribute error: called with O_CREAT or O_TMPFILE, but missing mode
  // CLANG: error: 'open' called with O_CREAT or O_TMPFILE, but missing mode
  open("/dev/null", O_CREAT);

  // GCC: error: call to '__creat_missing_mode' declared with attribute error: called with O_CREAT or O_TMPFILE, but missing mode
  // CLANG: error: 'open' called with O_CREAT or O_TMPFILE, but missing mode
  open("/dev/null", O_TMPFILE);

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__creat_too_many_args' declared with attribute error: too many arguments
  // CLANG: error: call to unavailable function 'open': too many arguments
  open("/dev/null", O_CREAT, 0, 0);

  // GCC: error: call to '__creat_too_many_args' declared with attribute error: too many arguments
  // CLANG: error: call to unavailable function 'open': too many arguments
  open("/dev/null", O_TMPFILE, 0, 0);

  // CLANG: warning: 'open' has superfluous mode bits; missing O_CREAT?
  open("/dev/null", O_RDONLY, 0644);

  // CLANG: warning: 'open' has superfluous mode bits; missing O_CREAT?
  open("/dev/null", O_DIRECTORY, 0644);
}

void test_poll() {
  pollfd fds[1];
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__poll_too_small_error' declared with attribute error: poll: pollfd array smaller than fd count
  // CLANG: error: in call to 'poll', fd_count is larger than the given buffer
  poll(fds, 2, 0);
}

void test_ppoll() {
  pollfd fds[1];
  timespec timeout;
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__ppoll_too_small_error' declared with attribute error: ppoll: pollfd array smaller than fd count
  // CLANG: error: in call to 'ppoll', fd_count is larger than the given buffer
  ppoll(fds, 2, &timeout, NULL);
}

void test_fread_overflow() {
  char buf[4];
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__fread_overflow' declared with attribute error: fread called with overflowing size * count
  // CLANG: error: in call to 'fread', size * count overflows
  fread(buf, 2, (size_t)-1, stdin);
}

void test_fread_too_big() {
  char buf[4];
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__fread_too_big_error' declared with attribute error: fread called with size * count bigger than buffer
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: in call to 'fread', size * count is too large for the given buffer
  fread(buf, 1, 5, stdin);
}

void test_fwrite_overflow() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__fwrite_overflow' declared with attribute error: fwrite called with overflowing size * count
  // CLANG: error: in call to 'fwrite', size * count overflows
  fwrite(buf, 2, (size_t)-1, stdout);
}

void test_fwrite_too_big() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__fwrite_too_big_error' declared with attribute error: fwrite called with size * count bigger than buffer
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: in call to 'fwrite', size * count is too large for the given buffer
  fwrite(buf, 1, 5, stdout);
}

void test_getcwd() {
  char buf[4];
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__getcwd_dest_size_error' declared with attribute error: getcwd called with size bigger than destination
  // CLANG: error: in call to 'getcwd', 'size' bytes overflows the given object
  getcwd(buf, 5);
}

void test_pwrite64_size() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__pwrite64_dest_size_error' declared with attribute error: pwrite64 called with size bigger than destination
  // CLANG: error: in call to 'pwrite64', 'count' bytes overflows the given object
  pwrite64(STDOUT_FILENO, buf, 5, 0);
}

void test_pwrite64_too_big_malloc() {
  void *buf = calloc(atoi("5"), 1);
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__pwrite64_count_toobig_error' declared with attribute error: pwrite64 called with count > SSIZE_MAX
  // clang should emit a warning, but probably never will.
  pwrite64(STDOUT_FILENO, buf, SIZE_MAX, 0);
}

void test_pwrite64_too_big() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__pwrite64_count_toobig_error' declared with attribute error: pwrite64 called with count > SSIZE_MAX
  // CLANG: error: in call to 'pwrite64', 'count' must be <= SSIZE_MAX
  pwrite64(STDOUT_FILENO, buf, SIZE_MAX, 0);
}

void test_write_size() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__write_dest_size_error' declared with attribute error: write called with size bigger than destination
  // CLANG: error: in call to 'write', 'count' bytes overflows the given object
  write(STDOUT_FILENO, buf, 5);
}

void test_memset_args_flipped() {
  char from[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: 'memset' will set 0 bytes; maybe the arguments got flipped?
  memset(from, sizeof(from), 0);
}

void test_sendto() {
  char buf[4] = {0};
  sockaddr_in addr;

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__sendto_error' declared with attribute error: 'sendto' called with size bigger than buffer
  // CLANG: error: 'sendto' called with size bigger than buffer
  sendto(0, buf, 6, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
}

void test_send() {
  char buf[4] = {0};

  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__sendto_error' declared with attribute error: 'sendto' called with size bigger than buffer
  // CLANG: error: 'send' called with size bigger than buffer
  send(0, buf, 6, 0);
}

void test_realpath() {
  char buf[4] = {0};
  // NOLINTNEXTLINE(whitespace/line_length)
  // GCC: error: call to '__realpath_size_error' declared with attribute error: 'realpath' output parameter must be NULL or a pointer to a buffer with >= PATH_MAX bytes
  // NOLINTNEXTLINE(whitespace/line_length)
  // CLANG: error: 'realpath' output parameter must be NULL or a pointer to a buffer with >= PATH_MAX bytes
  realpath(".", buf);

  // This is fine.
  realpath(".", NULL);

  char bigbuf[PATH_MAX];
  // CLANG: error: 'realpath': NULL path is never correct; flipped arguments?
  realpath(NULL, bigbuf);
}
