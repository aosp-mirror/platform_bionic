/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "../../bionic/libc_init_common.h"
#include <stddef.h>
#include <stdint.h>

#define SECTION(name) __attribute__((__section__(name)))
SECTION(".preinit_array") void (*__PREINIT_ARRAY__)(void) = (void (*)(void)) -1;
SECTION(".init_array") void (*__INIT_ARRAY__)(void) = (void (*)(void)) -1;
SECTION(".fini_array") void (*__FINI_ARRAY__)(void) = (void (*)(void)) -1;
#undef SECTION

__used static void _start_main(void* raw_args) {
  structors_array_t array;
  array.preinit_array = &__PREINIT_ARRAY__;
  array.init_array = &__INIT_ARRAY__;
  array.fini_array = &__FINI_ARRAY__;

  __libc_init(raw_args, NULL, &main, &array);
}

#define PRE ".text; .global _start; .type _start,%function; _start:"
#define POST "; .size _start, .-_start"

#if defined(__aarch64__)
__asm__(PRE "mov x0,sp; b _start_main" POST);
#elif defined(__arm__)
__asm__(PRE "mov r0,sp; b _start_main" POST);
#elif defined(__i386__)
__asm__(PRE "movl %esp,%eax; andl $~0xf,%esp; subl $12,%esp; pushl %eax; calll _start_main" POST);
#elif defined(__x86_64__)
__asm__(PRE "movq %rsp,%rdi; andq $~0xf,%rsp; callq _start_main" POST);
#else
#error unsupported architecture
#endif

#undef PRE
#undef POST

// On arm32 and arm64, when targeting Q and up, overalign the TLS segment to
// (8 * sizeof(void*)), which reserves enough space between the thread pointer
// and the executable's TLS segment for Bionic's TLS slots. It has the side
// effect of placing a 0-sized TLS segment into Android executables that don't
// use TLS, but this should be harmless.
//
// To ensure that the .tdata input section isn't deleted (e.g. by
// --gc-sections), the .text input section (which contains _start) has a
// relocation to the .tdata input section.
#if __ANDROID_API__ >= 29
#if defined(__arm__)
asm("  .section .tdata,\"awT\",%progbits\n"
    "  .p2align 5\n"
    "  .text\n"
    "  .reloc 0, R_ARM_NONE, .tdata\n");
#elif defined(__aarch64__)
asm("  .section .tdata,\"awT\",@progbits\n"
    "  .p2align 6\n"
    "  .text\n"
    "  .reloc 0, R_AARCH64_NONE, .tdata\n");
#endif
#endif

#include "__dso_handle.h"
#include "atexit.h"
#include "pthread_atfork.h"
#ifdef __i386__
# include "../../arch-x86/bionic/__stack_chk_fail_local.h"
#endif
