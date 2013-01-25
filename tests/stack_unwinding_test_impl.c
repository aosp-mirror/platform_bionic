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

/*
 * Contributed by: Intel Corporation
 */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unwind.h>

#define noinline __attribute__((__noinline__))
#define unused __attribute__((__unused__))

static noinline _Unwind_Reason_Code stop_fn(int a unused,
    _Unwind_Action action,
    _Unwind_Exception_Class b unused, struct _Unwind_Exception* c unused,
    struct _Unwind_Context* d unused, void* e unused) {
  if ((action & _UA_END_OF_STACK) != 0) {
    // We reached the end of the stack without executing foo_cleanup. Test failed.
    abort();
  }
  return _URC_NO_REASON;
}

static void noinline foo_cleanup(char* param unused) {
  exit(42);
}

static void noinline do_crash() {
  char* ptr = NULL;
  *ptr = 0; // Deliberately cause a SIGSEGV.
}

static void noinline foo() {
  char c1 __attribute__((cleanup(foo_cleanup)));
  do_crash();
}

// It's SEGSEGV handler. We start forced stack unwinding here.
// If libgcc don't find dso for signal frame stack unwinding will be finished.
// libgcc pass to stop_fn _UA_END_OF_STACK flag.
// Test pass condition: stack unwinding through signal frame and foo1_handler execution.
static void noinline sigsegv_handler(int param unused) {
  struct _Unwind_Exception* exception = (struct _Unwind_Exception*) malloc(sizeof(*exception));
  memset(&exception->exception_class, 0, sizeof(exception->exception_class));
  exception->exception_cleanup = 0;
  _Unwind_ForcedUnwind(exception, stop_fn, 0);
}

void do_test() {
  signal(SIGSEGV, &sigsegv_handler);
  foo();
}
