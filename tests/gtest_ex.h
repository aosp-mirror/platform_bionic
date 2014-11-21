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

#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

template<typename F>
void test_isolated(F test) {
  int pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    test();
    _exit(testing::Test::HasFailure() ? 1 : 0);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status)) << "Forked test has failed, see above..";
}
