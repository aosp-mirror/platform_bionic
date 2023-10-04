/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <nl_types.h>

#include <errno.h>
#include <gtest/gtest.h>

#include "utils.h"

TEST(nl_types, smoke) {
  nl_catd cat = catopen("/does/not/exist", NL_CAT_LOCALE);
  ASSERT_EQ(reinterpret_cast<nl_catd>(-1), cat);

  ASSERT_STREQ("hello, world!", catgets(cat, NL_SETD, 0, "hello, world!"));

  errno = 0;
  ASSERT_EQ(-1, catclose(cat));
  ASSERT_ERRNO(EBADF);
}
