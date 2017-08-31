/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <stdarg.h>

TEST(stdarg, smoke) {
#if !defined(va_start)
#error va_start
#endif
#if !defined(va_arg)
#error va_arg
#endif
#if !defined(va_copy)
#error va_copy
#endif
#if !defined(va_end)
#error va_end
#endif
  va_list l __attribute__((__unused__));
}
