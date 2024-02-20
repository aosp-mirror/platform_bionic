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

/*
 * This source files provides two important functions for dynamic
 * executables:
 *
 * - a C runtime initializer (__libc_preinit), which is called by
 *   the dynamic linker when libc.so is loaded. This happens before
 *   any other initializer (e.g. static C++ constructors in other
 *   shared libraries the program depends on).
 *
 * - a program launch function (__libc_init), which is called after
 *   all dynamic linking has been performed.
 */

#include <elf.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "bionic/pthread_internal.h"
#include "libc_init_common.h"

#include "private/bionic_defs.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "platform/bionic/macros.h"
#include "private/bionic_ssp.h"
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"

extern "C" {
  extern void netdClientInit(void);
  extern int __cxa_atexit(void (*)(void *), void *, void *);
};

void memtag_stack_dlopen_callback() {
  async_safe_format_log(ANDROID_LOG_INFO, "libc", "remapping stacks as PROT_MTE");
  __pthread_internal_remap_stack_with_mte();
}

// Use an initializer so __libc_sysinfo will have a fallback implementation
// while .preinit_array constructors run.
#if defined(__i386__)
__LIBC_HIDDEN__ void* __libc_sysinfo = reinterpret_cast<void*>(__libc_int0x80);
#endif

extern "C" __attribute__((weak)) void __hwasan_library_loaded(ElfW(Addr) base,
                                                              const ElfW(Phdr)* phdr,
                                                              ElfW(Half) phnum);
extern "C" __attribute__((weak)) void __hwasan_library_unloaded(ElfW(Addr) base,
                                                                const ElfW(Phdr)* phdr,
                                                                ElfW(Half) phnum);

// We need a helper function for __libc_preinit because compiling with LTO may
// inline functions requiring a stack protector check, but __stack_chk_guard is
// not initialized at the start of __libc_preinit. __libc_preinit_impl will run
// after __stack_chk_guard is initialized and therefore can safely have a stack
// protector.
__attribute__((noinline))
static void __libc_preinit_impl() {
#if defined(__i386__)
  __libc_init_sysinfo();
#endif

  // Register libc.so's copy of the TLS generation variable so the linker can
  // update it when it loads or unloads a shared object.
  TlsModules& tls_modules = __libc_shared_globals()->tls_modules;
  tls_modules.generation_libc_so = &__libc_tls_generation_copy;
  __libc_tls_generation_copy = tls_modules.generation;

  __libc_init_globals();
  __libc_init_common();
  __libc_init_scudo();

#if __has_feature(hwaddress_sanitizer)
  // Notify the HWASan runtime library whenever a library is loaded or unloaded
  // so that it can update its shadow memory.
  // This has to happen before _libc_init_malloc which might dlopen to load
  // profiler libraries.
  __libc_shared_globals()->load_hook = __hwasan_library_loaded;
  __libc_shared_globals()->unload_hook = __hwasan_library_unloaded;
#endif

  // Hooks for various libraries to let them know that we're starting up.
  __libc_globals.mutate(__libc_init_malloc);

  // Install reserved signal handlers for assisting the platform's profilers.
  __libc_init_profiling_handlers();

  __libc_init_fork_handler();

  __libc_shared_globals()->set_target_sdk_version_hook = __libc_set_target_sdk_version;

  netdClientInit();
}

// We flag the __libc_preinit function as a constructor to ensure that
// its address is listed in libc.so's .init_array section.
// This ensures that the function is called by the dynamic linker as
// soon as the shared library is loaded.
// We give this constructor priority 1 because we want libc's constructor
// to run before any others (such as the jemalloc constructor), and lower
// is better (http://b/68046352).
__attribute__((constructor(1))) static void __libc_preinit() {
  // The linker has initialized its copy of the global stack_chk_guard, and filled in the main
  // thread's TLS slot with that value. Initialize the local global stack guard with its value.
  __stack_chk_guard = reinterpret_cast<uintptr_t>(__get_tls()[TLS_SLOT_STACK_GUARD]);

  __libc_preinit_impl();
}

// This function is called from the executable's _start entry point
// (see arch-$ARCH/bionic/crtbegin.c), which is itself called by the dynamic
// linker after it has loaded all shared libraries the executable depends on.
//
// Note that the dynamic linker has also run all constructors in the
// executable at this point.
__noreturn void __libc_init(void* raw_args,
                            void (*onexit)(void) __unused,
                            int (*slingshot)(int, char**, char**),
                            structors_array_t const * const structors) {
  BIONIC_STOP_UNWIND;

  KernelArgumentBlock args(raw_args);

  // Several Linux ABIs don't pass the onexit pointer, and the ones that
  // do never use it.  Therefore, we ignore it.

  // The executable may have its own destructors listed in its .fini_array
  // so we need to ensure that these are called when the program exits
  // normally.
  if (structors->fini_array) {
    __cxa_atexit(__libc_fini,structors->fini_array,nullptr);
  }

  __libc_init_mte_late();

  // This roundabout way is needed so we don't use the static libc linked into the linker, which
  // will not affect the process.
  __libc_shared_globals()->memtag_stack_dlopen_callback = memtag_stack_dlopen_callback;

  exit(slingshot(args.argc - __libc_shared_globals()->initial_linker_arg_count,
                 args.argv + __libc_shared_globals()->initial_linker_arg_count,
                 args.envp));
}

extern "C" libc_shared_globals* __loader_shared_globals();

__LIBC_HIDDEN__ libc_shared_globals* __libc_shared_globals() {
  return __loader_shared_globals();
}
