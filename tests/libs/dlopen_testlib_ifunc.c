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

const char* foo() __attribute__ ((ifunc ("foo_ifunc")));

const char* f1() {
  return "unset";
}

const char* f2() {
  return "set";
}

void* foo_ifunc() {
   char* choice = getenv("IFUNC_CHOICE");
   return choice == NULL ? f1 : f2;
}

const char* foo_library() {
   return foo();
}