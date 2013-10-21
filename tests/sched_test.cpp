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

#include <gtest/gtest.h>

#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

static int child_fn(void* i_ptr) {
  *reinterpret_cast<int*>(i_ptr) = 42;
  return 123;
}

TEST(sched, clone) {
  void* child_stack[1024];

  int i = 0;
  pid_t tid = clone(child_fn, &child_stack[1024], /*CLONE_FILES | CLONE_FS | */CLONE_VM/* | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM*/, &i);

  int status;
  ASSERT_EQ(tid, TEMP_FAILURE_RETRY(waitpid(tid, &status, __WCLONE)));

  ASSERT_EQ(42, i);

  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(123, WEXITSTATUS(status));
}
