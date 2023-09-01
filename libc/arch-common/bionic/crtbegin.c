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

extern init_func_t* __preinit_array_start[];
extern init_func_t* __preinit_array_end[];
extern init_func_t* __init_array_start[];
extern init_func_t* __init_array_end[];
extern fini_func_t* __fini_array_start[];
extern fini_func_t* __fini_array_end[];

#if !defined(CRTBEGIN_STATIC)
/* This function will be called during normal program termination
 * to run the destructors that are listed in the .fini_array section
 * of the executable, if any.
 *
 * 'fini_array' points to a list of function addresses.
 */
static void call_fini_array() {
  fini_func_t** array = __fini_array_start;
  size_t count = __fini_array_end - __fini_array_start;
  // Call fini functions in reverse order.
  while (count-- > 0) {
    fini_func_t* function = array[count];
    (*function)();
  }
}

// libc.so needs fini_array with sentinels. So create a fake fini_array with sentinels.
// It contains a function to call functions in real fini_array.
static fini_func_t* fini_array_with_sentinels[] = {
    (fini_func_t*)-1,
    &call_fini_array,
    (fini_func_t*)0,
};
#endif  // !defined(CRTBEGIN_STATIC)

__used static void _start_main(void* raw_args) {
  structors_array_t array = {};
#if defined(CRTBEGIN_STATIC)
  array.preinit_array = __preinit_array_start;
  array.preinit_array_count = __preinit_array_end - __preinit_array_start;
  array.init_array = __init_array_start;
  array.init_array_count = __init_array_end - __init_array_start;
  array.fini_array = __fini_array_start;
  array.fini_array_count = __fini_array_end - __fini_array_start;
#else
  if (__fini_array_end - __fini_array_start > 0) {
    array.fini_array = fini_array_with_sentinels;
  }
#endif  // !defined(CRTBEGIN_STATIC)

  __libc_init(raw_args, NULL, &main, &array);
}

#define PRE ".text; .global _start; .type _start,%function; _start:"
#define POST "; .size _start, .-_start"

#if defined(__aarch64__)
__asm__(PRE "bti j; mov x29,#0; mov x30,#0; mov x0,sp; b _start_main" POST);
#elif defined(__arm__)
__asm__(PRE "mov fp,#0; mov lr,#0; mov r0,sp; b _start_main" POST);
#elif defined(__i386__)
__asm__(PRE
        "xorl %ebp,%ebp; movl %esp,%eax; andl $~0xf,%esp; subl $12,%esp; pushl %eax;"
        "call _start_main" POST);
#elif defined(__riscv)
__asm__(PRE "li fp,0; li ra,0; mv a0,sp; tail _start_main" POST);
#elif defined(__x86_64__)
__asm__(PRE "xorl %ebp, %ebp; movq %rsp,%rdi; andq $~0xf,%rsp; callq _start_main" POST);
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
