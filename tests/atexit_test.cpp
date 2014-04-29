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

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>

#include <string>

TEST(atexit, combined_test) {
  std::string atexit_call_sequence;
  bool valid_this_in_static_dtor = false;
  void* handle = dlopen("libtest_atexit.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);

  void* sym = dlsym(handle, "register_atexit");
  ASSERT_TRUE(sym != NULL);
  reinterpret_cast<void (*)(std::string*, bool*)>(sym)(&atexit_call_sequence, &valid_this_in_static_dtor);

  ASSERT_EQ(0, dlclose(handle));
  // this test verifies atexit call from atexit handler. as well as the order of calls
  ASSERT_EQ("Humpty Dumpty sat on a wall", atexit_call_sequence);
  ASSERT_TRUE(valid_this_in_static_dtor);
}

// TODO: test for static dtor calls from from exit(.) -> __cxa_finalize(NULL)

