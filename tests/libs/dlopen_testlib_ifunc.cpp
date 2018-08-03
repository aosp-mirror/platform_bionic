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

#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uintptr_t g_flag = 0;

static void __attribute__((constructor)) init_flag() {
  g_flag = reinterpret_cast<uintptr_t>(dlsym(RTLD_DEFAULT, "dlsym"));
}

static const char* is_ctor_called() __attribute__ ((ifunc("is_ctor_called_ifun")));

extern "C" const char* foo() __attribute__ ((ifunc ("foo_ifunc")));

// Static linker creates GLOBAL/IFUNC symbol and JUMP_SLOT relocation type for plt segment
extern "C" const char* is_ctor_called_jump_slot() __attribute__ ((ifunc("is_ctor_called_ifun")));

extern "C" const char* is_ctor_called_irelative() {
  // Call internal ifunc-resolved function with IRELATIVE reloc
  return is_ctor_called();
}

extern "C" const char* return_true() {
  return "true";
}

extern "C" const char* return_false() {
  return "false";
}

extern "C" const char* f1() {
  return "unset";
}

extern "C" const char* f2() {
  return "set";
}

typedef const char* (*fn_ptr)();

extern "C" fn_ptr is_ctor_called_ifun() {
  return g_flag == 0 ? return_false : return_true;
}

extern "C" fn_ptr foo_ifunc() {
   char* choice = getenv("IFUNC_CHOICE");
   return choice == nullptr ? f1 : f2;
}

extern "C" const char* foo_library() {
   return foo();
}
