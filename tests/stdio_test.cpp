/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

TEST(stdio, tmpfile_fileno_fprintf_rewind_fgets) {
  FILE* fp = tmpfile();
  ASSERT_TRUE(fp != NULL);

  int fd = fileno(fp);
  ASSERT_NE(fd, -1);

  struct stat sb;
  int rc = fstat(fd, &sb);
  ASSERT_NE(rc, -1);
  ASSERT_EQ(sb.st_mode & 0777, 0600U);

  rc = fprintf(fp, "hello\n");
  ASSERT_EQ(rc, 6);

  rewind(fp);

  char buf[16];
  char* s = fgets(buf, sizeof(buf), fp);
  ASSERT_TRUE(s != NULL);
  ASSERT_STREQ("hello\n", s);

  fclose(fp);
}
