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

#include <stdio.h>
#include <stdlib.h>

static int g_flag = 0;

static void __attribute__((constructor)) init_flag() {
  g_flag = 1;
}

static const char* is_ctor_called() __attribute__ ((ifunc("is_ctor_called_ifun")));

const char* foo() __attribute__ ((ifunc ("foo_ifunc")));

// Static linker creates GLOBAL/IFUNC symbol and JUMP_SLOT relocation type for plt segment
const char* is_ctor_called_jump_slot() __attribute__ ((ifunc("is_ctor_called_ifun")));

const char* is_ctor_called_irelative() {
  // Call internal ifunc-resolved function with IRELATIVE reloc
  return is_ctor_called();
}

const char* return_true() {
  return "true";
}

const char* return_false() {
  return "false";
}

const char* f1() {
  return "unset";
}

const char* f2() {
  return "set";
}

void* is_ctor_called_ifun() {
  return g_flag == 0 ? return_false : return_true;
}

void* foo_ifunc() {
   char* choice = getenv("IFUNC_CHOICE");
   return choice == NULL ? f1 : f2;
}

const char* foo_library() {
   return foo();
}
