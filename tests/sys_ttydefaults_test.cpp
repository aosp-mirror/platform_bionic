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

#include <sys/ttydefaults.h>
#include <termios.h>

TEST(sys_ttydefaults, flags) {
  int i;
  i = TTYDEF_IFLAG;
  i = TTYDEF_OFLAG;
  i = TTYDEF_LFLAG;
  i = TTYDEF_CFLAG;
  i = TTYDEF_SPEED;
}

TEST(sys_ttydefaults, correct_CEOL) {
  ASSERT_EQ(_POSIX_VDISABLE, CEOL);
}
