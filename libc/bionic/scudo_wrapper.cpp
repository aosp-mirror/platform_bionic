/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <errno.h>
#include <stdio.h>

#include "scudo.h"
#include "private/bionic_globals.h"
#include "private/WriteProtected.h"

__LIBC_HIDDEN__ WriteProtected<libc_globals> __libc_globals;

// Call the libc malloc initialisers.
__attribute__((constructor(1))) static void __scudo_preinit() {
  __libc_globals.mutate(__libc_init_malloc);
}

extern "C" libc_shared_globals* __loader_shared_globals();

__LIBC_HIDDEN__ libc_shared_globals* __libc_shared_globals() {
  return __loader_shared_globals();
}

#if defined(__i386__)
__LIBC_HIDDEN__ void* __libc_sysinfo = reinterpret_cast<void*>(__libc_int0x80);
#endif

extern "C" void scudo_malloc_disable_memory_tagging() {}

int scudo_mallopt(int /*param*/, int /*value*/) {
  return 0;
}

int scudo_malloc_info(int /*options*/, FILE* /*fp*/) {
  errno = ENOTSUP;
  return -1;
}

int scudo_malloc_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*) {
  return 0;
}

void scudo_malloc_disable() {
}

void scudo_malloc_enable() {
}
