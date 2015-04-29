/*
 * Copyright (C) 2015 The Android Open Source Project
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
#include <sys/cdefs.h>

struct thread_local_dtor {
  void (*func) (void *);
  void *arg;
  void *dso_handle; // unused...
  thread_local_dtor* next;
};

static __thread thread_local_dtor* thread_local_dtors = nullptr;

extern "C" int __cxa_thread_atexit_impl(void (*func) (void *), void *arg, void *dso_handle) {
  thread_local_dtor* dtor = new thread_local_dtor();

  dtor->func = func;
  dtor->arg = arg;
  dtor->dso_handle = dso_handle;
  dtor->next = thread_local_dtors;

  thread_local_dtors = dtor;

  return 0;
}

extern "C" __LIBC_HIDDEN__ void __cxa_thread_finalize() {
  while (thread_local_dtors != nullptr) {
    thread_local_dtor* current = thread_local_dtors;
    thread_local_dtors = current->next;

    current->func(current->arg);
    delete current;
  }
}
