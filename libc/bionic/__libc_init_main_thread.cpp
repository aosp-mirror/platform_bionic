/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "libc_init_common.h"

#include "private/bionic_auxv.h"
#include "private/bionic_globals.h"
#include "private/KernelArgumentBlock.h"
#include "pthread_internal.h"

extern "C" int __set_tls(void* ptr);
extern "C" int __set_tid_address(int* tid_address);

// Setup for the main thread. For dynamic executables, this is called by the
// linker _before_ libc is mapped in memory. This means that all writes to
// globals from this function will apply to linker-private copies and will not
// be visible from libc later on.
//
// Note: this function creates a pthread_internal_t for the initial thread and
// stores the pointer in TLS, but does not add it to pthread's thread list. This
// has to be done later from libc itself (see __libc_init_common).
//
// This is in a file by itself because it needs to be built with
// -fno-stack-protector because it's responsible for setting up the main
// thread's TLS (which stack protector relies on).

void __libc_init_main_thread(KernelArgumentBlock& args) {
  __libc_auxv = args.auxv;
#if defined(__i386__)
  __libc_init_sysinfo(args);
#endif

  static pthread_internal_t main_thread;

  // The -fstack-protector implementation uses TLS, so make sure that's
  // set up before we call any function that might get a stack check inserted.
  __set_tls(main_thread.tls);

  // Tell the kernel to clear our tid field when we exit, so we're like any other pthread.
  // As a side-effect, this tells us our pid (which is the same as the main thread's tid).
  main_thread.tid = __set_tid_address(&main_thread.tid);
  main_thread.set_cached_pid(main_thread.tid);

  // We don't want to free the main thread's stack even when the main thread exits
  // because things like environment variables with global scope live on it.
  // We also can't free the pthread_internal_t itself, since that lives on the main
  // thread's stack rather than on the heap.
  // The main thread has no mmap allocated space for stack or pthread_internal_t.
  main_thread.mmap_size = 0;
  pthread_attr_init(&main_thread.attr);
  main_thread.attr.guard_size = 0; // The main thread has no guard page.
  main_thread.attr.stack_size = 0; // User code should never see this; we'll compute it when asked.
  // TODO: the main thread's sched_policy and sched_priority need to be queried.

  // The TLS stack guard is set from the global, so ensure that we've initialized the global
  // before we initialize the TLS.
  __libc_init_global_stack_chk_guard(args);

  __init_thread(&main_thread);
  __init_tls(&main_thread);

  // Store a pointer to the kernel argument block in a TLS slot to be
  // picked up by the libc constructor.
  main_thread.tls[TLS_SLOT_BIONIC_PREINIT] = &args;

  __init_alternate_signal_stack(&main_thread);
}
