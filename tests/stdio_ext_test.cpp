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

#include <stdio_ext.h>

#include <gtest/gtest.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>

#include <android-base/file.h>

#include "utils.h"

TEST(stdio_ext, __fbufsize) {
  FILE* fp = fopen("/proc/version", "r");

  // Initially, there's no buffer in case the first thing you do is disable buffering.
  ASSERT_EQ(0U, __fbufsize(fp));

  // A read forces a buffer to be created.
  char buf[128];
  fgets(buf, sizeof(buf), fp);
  ASSERT_EQ(1024U, __fbufsize(fp));

  ASSERT_EQ(0, setvbuf(fp, buf, _IOFBF, 1));
  ASSERT_EQ(1U, __fbufsize(fp));

  ASSERT_EQ(0, setvbuf(fp, buf, _IOFBF, 8));
  ASSERT_EQ(8U, __fbufsize(fp));

  fclose(fp);
}

TEST(stdio_ext, __flbf) {
  FILE* fp = fopen("/proc/version", "r");

  ASSERT_FALSE(__flbf(fp));

  char buf[128];
  ASSERT_EQ(0, setvbuf(fp, buf, _IOLBF, sizeof(buf)));

  ASSERT_TRUE(__flbf(fp));

  fclose(fp);
}

TEST(stdio_ext, __fpending) {
  FILE* fp = fopen("/dev/null", "w");
  ASSERT_EQ(0U, __fpending(fp));
  ASSERT_EQ('x', fputc('x', fp));
  ASSERT_EQ(1U, __fpending(fp));
  ASSERT_EQ('y', fputc('y', fp));
  ASSERT_EQ(2U, __fpending(fp));
  fflush(fp);
  ASSERT_EQ(0U, __fpending(fp));
  fclose(fp);
}

TEST(stdio_ext, __fpurge) {
  FILE* fp = tmpfile();

  ASSERT_EQ('a', fputc('a', fp));
  ASSERT_EQ(1U, __fpending(fp));
  __fpurge(fp);
  ASSERT_EQ(0U, __fpending(fp));

  ASSERT_EQ('b', fputc('b', fp));
  ASSERT_EQ('\n', fputc('\n', fp));
  ASSERT_EQ(2U, __fpending(fp));

  rewind(fp);

  char buf[16];
  char* s = fgets(buf, sizeof(buf), fp);
  ASSERT_TRUE(s != nullptr);
  ASSERT_STREQ("b\n", s);

  fclose(fp);
}

TEST(stdio_ext, _flushlbf) {
  FILE* fp = fopen("/dev/null", "w");

  char buf[128];
  ASSERT_EQ(0, setvbuf(fp, buf, _IOLBF, sizeof(buf)));

  ASSERT_EQ('a', fputc('a', fp));
  ASSERT_EQ(1U, __fpending(fp));

  _flushlbf();

  ASSERT_EQ(0U, __fpending(fp));

  fclose(fp);
}

TEST(stdio_ext, __freadable__fwritable) {
  FILE* fp;

  // Read-only.
  fp = fopen("/dev/null", "r");
  ASSERT_TRUE(__freadable(fp));
  ASSERT_FALSE(__fwritable(fp));
  fclose(fp);

  // Write-only.
  fp = fopen("/dev/null", "w");
  ASSERT_FALSE(__freadable(fp));
  ASSERT_TRUE(__fwritable(fp));
  fclose(fp);

  // Append (aka write-only).
  fp = fopen("/dev/null", "a");
  ASSERT_FALSE(__freadable(fp));
  ASSERT_TRUE(__fwritable(fp));
  fclose(fp);

  // The three read-write modes.
  for (auto read_write_mode : {"r+", "w+", "a+"}) {
    fp = fopen("/dev/null", read_write_mode);
    ASSERT_TRUE(__freadable(fp));
    ASSERT_TRUE(__fwritable(fp));
    fclose(fp);
  }
}

TEST(stdio_ext, __freading__fwriting) {
  FILE* fp;

  // Append (aka write-only). Never reading. Always writing.
  fp = fopen("/dev/zero", "a");
  ASSERT_FALSE(__freading(fp)); // Not reading initially.
  ASSERT_TRUE(__fwriting(fp)); // Writing initially.
  ASSERT_TRUE(fputc('x', fp) != EOF);
  ASSERT_FALSE(__freading(fp)); // Not reading after write.
  ASSERT_TRUE(__fwriting(fp)); // Still writing after write.
  fclose(fp);

  // Write-only. Never reading. Always writing.
  fp = fopen("/dev/zero", "w");
  ASSERT_FALSE(__freading(fp)); // Not reading initially.
  ASSERT_TRUE(__fwriting(fp)); // Writing initially.
  ASSERT_TRUE(fputc('x', fp) != EOF);
  ASSERT_FALSE(__freading(fp)); // Not reading after write.
  ASSERT_TRUE(__fwriting(fp)); // Still writing after write.
  fclose(fp);

  // Read-only. Always reading. Never writing.
  fp = fopen("/dev/zero", "r");
  ASSERT_TRUE(__freading(fp)); // Reading initially.
  ASSERT_FALSE(__fwriting(fp)); // Not writing initially.
  ASSERT_TRUE(fgetc(fp) == 0);
  ASSERT_TRUE(__freading(fp)); // Still reading after read.
  ASSERT_FALSE(__fwriting(fp)); // Still not writing after read.
  fclose(fp);

  // The three read-write modes.
  for (auto read_write_mode : {"r+", "w+", "a+"}) {
    fp = fopen("/dev/zero", read_write_mode);
    ASSERT_FALSE(__freading(fp)); // Not reading initially.
    ASSERT_FALSE(__fwriting(fp)); // Not writing initially.
    ASSERT_TRUE(fgetc(fp) == 0);
    ASSERT_TRUE(__freading(fp)); // Reading after read.
    ASSERT_FALSE(__fwriting(fp)); // Not writing after read.
    ASSERT_TRUE(fputc('x', fp) != EOF);
    ASSERT_FALSE(__freading(fp)); // Not reading after write.
    ASSERT_TRUE(__fwriting(fp)); // Writing after write.
    fclose(fp);
  }
}

TEST(stdio_ext, __fseterr) {
#if defined(__GLIBC__)
  GTEST_SKIP() << "glibc doesn't have __fseterr, but gnulib will use it";
#else
  FILE* fp = fopen("/dev/null", "w");

  ASSERT_FALSE(ferror(fp));
  __fseterr(fp);
  ASSERT_TRUE(ferror(fp));
  clearerr(fp);
  ASSERT_FALSE(ferror(fp));

  fclose(fp);
#endif
}

TEST(stdio_ext, __fsetlocking) {
  FILE* fp = fopen("/proc/version", "r");
  ASSERT_EQ(FSETLOCKING_INTERNAL, __fsetlocking(fp, FSETLOCKING_QUERY));
  ASSERT_EQ(FSETLOCKING_INTERNAL, __fsetlocking(fp, FSETLOCKING_BYCALLER));
  ASSERT_EQ(FSETLOCKING_BYCALLER, __fsetlocking(fp, FSETLOCKING_QUERY));
  ASSERT_EQ(FSETLOCKING_BYCALLER, __fsetlocking(fp, FSETLOCKING_INTERNAL));
  ASSERT_EQ(FSETLOCKING_INTERNAL, __fsetlocking(fp, FSETLOCKING_QUERY));
  fclose(fp);
}

static void LockingByCallerHelper(std::atomic<pid_t>* pid) {
  *pid = gettid();
  flockfile(stdout);
  funlockfile(stdout);
}

TEST(stdio_ext, __fsetlocking_BYCALLER) {
  // Check if users can use flockfile/funlockfile to protect stdio operations.
  int old_state = __fsetlocking(stdout, FSETLOCKING_BYCALLER);
  flockfile(stdout);
  pthread_t thread;
  std::atomic<pid_t> pid(0);
  ASSERT_EQ(0, pthread_create(&thread, nullptr,
                              reinterpret_cast<void* (*)(void*)>(LockingByCallerHelper), &pid));
  WaitUntilThreadSleep(pid);
  funlockfile(stdout);

  ASSERT_EQ(0, pthread_join(thread, nullptr));
  __fsetlocking(stdout, old_state);
}
