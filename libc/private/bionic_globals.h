/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef _PRIVATE_BIONIC_GLOBALS_H
#define _PRIVATE_BIONIC_GLOBALS_H

#include <sys/cdefs.h>
#include <link.h>
#include <pthread.h>

#include "private/bionic_allocator.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_fdsan.h"
#include "private/bionic_malloc_dispatch.h"
#include "private/bionic_vdso.h"
#include "private/WriteProtected.h"

struct libc_globals {
  vdso_entry vdso[VDSO_END];
  long setjmp_cookie;
  MallocDispatch malloc_dispatch;
};

__LIBC_HIDDEN__ extern WriteProtected<libc_globals> __libc_globals;

struct abort_msg_t;

// Globals shared between the dynamic linker and libc.so.
struct libc_shared_globals {
  // Construct the shared globals using a constexpr constructor to ensure that
  // the object doesn't need dynamic initialization. The object is accessed
  // before the dynamic linker has relocated itself.
  constexpr libc_shared_globals() {}

  FdTable fd_table;

  // When the linker is invoked on a binary (e.g. `linker64 /system/bin/date`),
  // record the number of arguments passed to the linker itself rather than to
  // the program it's loading. Typically 0, sometimes 1.
  int initial_linker_arg_count = 0;

  ElfW(auxv_t)* auxv = nullptr;

  pthread_mutex_t abort_msg_lock = PTHREAD_MUTEX_INITIALIZER;
  abort_msg_t* abort_msg = nullptr;

  StaticTlsLayout static_tls_layout;
  TlsModules tls_modules;
  BionicAllocator tls_allocator;

  // Values passed from the linker to libc.so.
  const char* init_progname = nullptr;
  char** init_environ = nullptr;
};

__LIBC_HIDDEN__ libc_shared_globals* __libc_shared_globals();
__LIBC_HIDDEN__ void __libc_init_fdsan();

__LIBC_HIDDEN__ void __libc_init_malloc(libc_globals* globals);
__LIBC_HIDDEN__ void __libc_init_setjmp_cookie(libc_globals* globals);
__LIBC_HIDDEN__ void __libc_init_vdso(libc_globals* globals);

#if defined(__i386__)
__LIBC_HIDDEN__ extern void* __libc_sysinfo;
__LIBC_HIDDEN__ void __libc_init_sysinfo();
#endif

#endif
