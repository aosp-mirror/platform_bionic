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

#include <android/api-level.h>
#include <elf.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/mman.h>

#include "libc_init_common.h"
#include "pthread_internal.h"

#include "platform/bionic/page.h"
#include "private/bionic_call_ifunc_resolver.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "platform/bionic/macros.h"
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"

#if __has_feature(hwaddress_sanitizer)
#include <sanitizer/hwasan_interface.h>
#endif

// Leave the variable uninitialized for the sake of the dynamic loader, which
// links in this file. The loader will initialize this variable before
// relocating itself.
#if defined(__i386__)
__LIBC_HIDDEN__ void* __libc_sysinfo;
#endif

extern "C" int __cxa_atexit(void (*)(void *), void *, void *);

static void call_array(void(**list)()) {
  // First element is -1, list is null-terminated
  while (*++list) {
    (*list)();
  }
}

#if defined(__aarch64__) || defined(__x86_64__)
extern __LIBC_HIDDEN__ __attribute__((weak)) ElfW(Rela) __rela_iplt_start[], __rela_iplt_end[];

static void call_ifunc_resolvers() {
  if (__rela_iplt_start == nullptr || __rela_iplt_end == nullptr) {
    // These symbols are not emitted by gold. Gold has code to do so, but for
    // whatever reason it is not being run. In these cases ifuncs cannot be
    // resolved, so we do not support using ifuncs in static executables linked
    // with gold.
    //
    // Since they are weak, they will be non-null when linked with bfd/lld and
    // null when linked with gold.
    return;
  }

  for (ElfW(Rela) *r = __rela_iplt_start; r != __rela_iplt_end; ++r) {
    ElfW(Addr)* offset = reinterpret_cast<ElfW(Addr)*>(r->r_offset);
    ElfW(Addr) resolver = r->r_addend;
    *offset = __bionic_call_ifunc_resolver(resolver);
  }
}
#else
extern __LIBC_HIDDEN__ __attribute__((weak)) ElfW(Rel) __rel_iplt_start[], __rel_iplt_end[];

static void call_ifunc_resolvers() {
  if (__rel_iplt_start == nullptr || __rel_iplt_end == nullptr) {
    // These symbols are not emitted by gold. Gold has code to do so, but for
    // whatever reason it is not being run. In these cases ifuncs cannot be
    // resolved, so we do not support using ifuncs in static executables linked
    // with gold.
    //
    // Since they are weak, they will be non-null when linked with bfd/lld and
    // null when linked with gold.
    return;
  }

  for (ElfW(Rel) *r = __rel_iplt_start; r != __rel_iplt_end; ++r) {
    ElfW(Addr)* offset = reinterpret_cast<ElfW(Addr)*>(r->r_offset);
    ElfW(Addr) resolver = *offset;
    *offset = __bionic_call_ifunc_resolver(resolver);
  }
}
#endif

static void apply_gnu_relro() {
  ElfW(Phdr)* phdr_start = reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR));
  unsigned long int phdr_ct = getauxval(AT_PHNUM);

  for (ElfW(Phdr)* phdr = phdr_start; phdr < (phdr_start + phdr_ct); phdr++) {
    if (phdr->p_type != PT_GNU_RELRO) {
      continue;
    }

    ElfW(Addr) seg_page_start = PAGE_START(phdr->p_vaddr);
    ElfW(Addr) seg_page_end = PAGE_END(phdr->p_vaddr + phdr->p_memsz);

    // Check return value here? What do we do if we fail?
    mprotect(reinterpret_cast<void*>(seg_page_start), seg_page_end - seg_page_start, PROT_READ);
  }
}

static void layout_static_tls(KernelArgumentBlock& args) {
  StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  layout.reserve_bionic_tls();

  const char* progname = args.argv[0];
  ElfW(Phdr)* phdr_start = reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR));
  size_t phdr_ct = getauxval(AT_PHNUM);

  static TlsModule mod;
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  if (__bionic_get_tls_segment(phdr_start, phdr_ct, 0, &mod.segment)) {
    if (!__bionic_check_tls_alignment(&mod.segment.alignment)) {
      async_safe_fatal("error: TLS segment alignment in \"%s\" is not a power of 2: %zu\n",
                       progname, mod.segment.alignment);
    }
    mod.static_offset = layout.reserve_exe_segment_and_tcb(&mod.segment, progname);
    mod.first_generation = kTlsGenerationFirst;

    modules.module_count = 1;
    modules.module_table = &mod;
  } else {
    layout.reserve_exe_segment_and_tcb(nullptr, progname);
  }
  // Enable the fast path in __tls_get_addr.
  __libc_tls_generation_copy = modules.generation;

  layout.finish_layout();
}

__noreturn static void __real_libc_init(void *raw_args,
                                        void (*onexit)(void) __unused,
                                        int (*slingshot)(int, char**, char**),
                                        structors_array_t const * const structors,
                                        bionic_tcb* temp_tcb) {
  BIONIC_STOP_UNWIND;

  // Initialize TLS early so system calls and errno work.
  KernelArgumentBlock args(raw_args);
  __libc_init_main_thread_early(args, temp_tcb);
  __libc_init_main_thread_late();
  __libc_init_globals();
  __libc_shared_globals()->init_progname = args.argv[0];
  __libc_init_AT_SECURE(args.envp);
  layout_static_tls(args);
  __libc_init_main_thread_final();
  __libc_init_common();
  __libc_init_fork_handler();

  call_ifunc_resolvers();
  apply_gnu_relro();

  // Several Linux ABIs don't pass the onexit pointer, and the ones that
  // do never use it.  Therefore, we ignore it.

  call_array(structors->preinit_array);
  call_array(structors->init_array);

  // The executable may have its own destructors listed in its .fini_array
  // so we need to ensure that these are called when the program exits
  // normally.
  if (structors->fini_array != nullptr) {
    __cxa_atexit(__libc_fini,structors->fini_array,nullptr);
  }

  exit(slingshot(args.argc, args.argv, args.envp));
}

extern "C" void __hwasan_init_static();

// This __libc_init() is only used for static executables, and is called from crtbegin.c.
//
// The 'structors' parameter contains pointers to various initializer
// arrays that must be run before the program's 'main' routine is launched.
__attribute__((no_sanitize("hwaddress")))
__noreturn void __libc_init(void* raw_args,
                            void (*onexit)(void) __unused,
                            int (*slingshot)(int, char**, char**),
                            structors_array_t const * const structors) {
  bionic_tcb temp_tcb = {};
#if __has_feature(hwaddress_sanitizer)
  // Install main thread TLS early. It will be initialized later in __libc_init_main_thread. For now
  // all we need is access to TLS_SLOT_SANITIZER.
  __set_tls(&temp_tcb.tls_slot(0));
  // Initialize HWASan enough to run instrumented code. This sets up TLS_SLOT_SANITIZER, among other
  // things.
  __hwasan_init_static();
  // We are ready to run HWASan-instrumented code, proceed with libc initialization...
#endif
  __real_libc_init(raw_args, onexit, slingshot, structors, &temp_tcb);
}

static int g_target_sdk_version{__ANDROID_API__};

extern "C" int android_get_application_target_sdk_version() {
  return g_target_sdk_version;
}

extern "C" void android_set_application_target_sdk_version(int target) {
  g_target_sdk_version = target;
}

// This function is called in the dynamic linker before ifunc resolvers have run, so this file is
// compiled with -ffreestanding to avoid implicit string.h function calls. (It shouldn't strictly
// be necessary, though.)
__LIBC_HIDDEN__ libc_shared_globals* __libc_shared_globals() {
  static libc_shared_globals globals;
  return &globals;
}
