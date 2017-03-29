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

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/auxv.h>

#include "libs_utils.h"

static unsigned long g_AT_RANDOM;
static unsigned long g_AT_PAGESZ;

static void preinit_ctor() {
  g_AT_RANDOM = getauxval(AT_RANDOM);
  g_AT_PAGESZ = getauxval(AT_PAGESZ);
}

__attribute__((section(".preinit_array"), used)) void (*preinit_ctor_p)(void) = preinit_ctor;

int main() {
  // Did getauxval during preinit get the same results as getauxval now?
  CHECK(getauxval(AT_RANDOM) == g_AT_RANDOM);
  CHECK(getauxval(AT_PAGESZ) == g_AT_PAGESZ);
  return 0;
}
