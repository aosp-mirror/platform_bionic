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

#include <async_safe/log.h>

#include "private/KernelArgumentBlock.h"
#include "private/bionic_arc4random.h"
#include "private/bionic_defs.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/bionic_ssp.h"
#include "pthread_internal.h"

extern "C" pid_t __getpid();
extern "C" int __set_tid_address(int* tid_address);

// Declared in "private/bionic_ssp.h".
uintptr_t __stack_chk_guard = 0;

static pthread_internal_t main_thread;

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
// thread's TLS (which stack protector relies on). It's also built with
// -ffreestanding because the early init function runs in the linker before
// ifunc resolvers have run.

// Do enough setup to:
//  - Let the dynamic linker invoke system calls (and access errno)
//  - Ensure that TLS access functions (__get_{tls,thread}) never return NULL
//  - Allow the stack protector to work (with a zero cookie)
// Avoid doing much more because, when this code is called within the dynamic
// linker, the linker binary hasn't been relocated yet, so certain kinds of code
// are hazardous, such as accessing non-hidden global variables or calling
// string.h functions.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" void __libc_init_main_thread_early(const KernelArgumentBlock& args,
                                              bionic_tcb* temp_tcb) {
  __libc_shared_globals()->auxv = args.auxv;
#if defined(__i386__)
  __libc_init_sysinfo(); // uses AT_SYSINFO auxv entry
#endif
  __init_tcb(temp_tcb, &main_thread);
  __init_tcb_dtv(temp_tcb);
  __set_tls(&temp_tcb->tls_slot(0));
  main_thread.tid = __getpid();
  main_thread.set_cached_pid(main_thread.tid);
  main_thread.stack_top = reinterpret_cast<uintptr_t>(args.argv);
}

// This code is used both by each new pthread and the code that initializes the main thread.
void __init_tcb(bionic_tcb* tcb, pthread_internal_t* thread) {
#ifdef TLS_SLOT_SELF
  // On x86, slot 0 must point to itself so code can read the thread pointer by
  // loading %fs:0 or %gs:0.
  tcb->tls_slot(TLS_SLOT_SELF) = &tcb->tls_slot(TLS_SLOT_SELF);
#endif
  tcb->tls_slot(TLS_SLOT_THREAD_ID) = thread;
}

void __init_tcb_dtv(bionic_tcb* tcb) {
  // Initialize the DTV slot to a statically-allocated empty DTV. The first
  // access to a dynamic TLS variable allocates a new DTV.
  static const TlsDtv zero_dtv = {};
  __set_tcb_dtv(tcb, const_cast<TlsDtv*>(&zero_dtv));
}

// Finish initializing the main thread.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" void __libc_init_main_thread_late() {
  __init_bionic_tls_ptrs(__get_bionic_tcb(), __allocate_temp_bionic_tls());

  // Tell the kernel to clear our tid field when we exit, so we're like any other pthread.
  // For threads created by pthread_create, this setup happens during the clone syscall (i.e.
  // CLONE_CHILD_CLEARTID).
  __set_tid_address(&main_thread.tid);

  pthread_attr_init(&main_thread.attr);
  // We don't want to explicitly set the main thread's scheduler attributes (http://b/68328561).
  pthread_attr_setinheritsched(&main_thread.attr, PTHREAD_INHERIT_SCHED);
  // The main thread has no guard page.
  pthread_attr_setguardsize(&main_thread.attr, 0);
  // User code should never see this; we'll compute it when asked.
  pthread_attr_setstacksize(&main_thread.attr, 0);

  // The TLS stack guard is set from the global, so ensure that we've initialized the global
  // before we initialize the TLS. Dynamic executables will initialize their copy of the global
  // stack protector from the one in the main thread's TLS.
  __libc_safe_arc4random_buf(&__stack_chk_guard, sizeof(__stack_chk_guard));
  __init_tcb_stack_guard(__get_bionic_tcb());

  __init_thread(&main_thread);

  __init_additional_stacks(&main_thread);
}

// Once all ELF modules are loaded, allocate the final copy of the main thread's
// static TLS memory.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" void __libc_init_main_thread_final() {
  bionic_tcb* temp_tcb = __get_bionic_tcb();
  bionic_tls* temp_tls = &__get_bionic_tls();

  // Allocate the main thread's static TLS. (This mapping doesn't include a
  // stack.)
  ThreadMapping mapping = __allocate_thread_mapping(0, PTHREAD_GUARD_SIZE);
  if (mapping.mmap_base == nullptr) {
    async_safe_fatal("failed to mmap main thread static TLS: %s", strerror(errno));
  }

  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  auto new_tcb = reinterpret_cast<bionic_tcb*>(mapping.static_tls + layout.offset_bionic_tcb());
  auto new_tls = reinterpret_cast<bionic_tls*>(mapping.static_tls + layout.offset_bionic_tls());

  __init_static_tls(mapping.static_tls);
  new_tcb->copy_from_bootstrap(temp_tcb);
  new_tls->copy_from_bootstrap(temp_tls);
  __init_tcb(new_tcb, &main_thread);
  __init_bionic_tls_ptrs(new_tcb, new_tls);

  main_thread.mmap_base = mapping.mmap_base;
  main_thread.mmap_size = mapping.mmap_size;
  main_thread.mmap_base_unguarded = mapping.mmap_base_unguarded;
  main_thread.mmap_size_unguarded = mapping.mmap_size_unguarded;

  __set_tls(&new_tcb->tls_slot(0));

  __set_stack_and_tls_vma_name(true);
  __free_temp_bionic_tls(temp_tls);
}
