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
 * libc_init_static.c
 *
 * This function takes the raw data block set up by the ELF loader
 * in the kernel and parses it.  It is invoked by crt0.S which makes
 * any necessary adjustments and passes calls this function using
 * the standard C calling convention.
 *
 * The arguments are:
 *  uintptr_t *elfdata	 -- The ELF loader data block; usually from the stack.
 *                          Basically a pointer to argc.
 *  void (*onexit)(void) -- Function to install into onexit
 */

/*
 * Several Linux ABIs don't pass the onexit pointer, and the ones that
 * do never use it.  Therefore, unless USE_ONEXIT is defined, we just
 * ignore the onexit pointer.
 */
/* #define USE_ONEXIT */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>
#include "pthread_internal.h"
#include "atexit.h"
#include "libc_init_common.h"

#include <bionic_tls.h>
#include <errno.h>

__noreturn void __libc_init(uintptr_t *elfdata,
                       void (*onexit)(void),
                       int (*slingshot)(int, char**, char**),
                       structors_array_t const * const structors)
{
/* 
 * To enable malloc checks for statically linked programs, add
 * "WITH_MALLOC_CHECK_LIBC_A := true" in device/buildspec.mk
 */
#ifdef MALLOC_LEAK_CHECK
    extern void malloc_debug_init();
    __libc_init_common(elfdata, onexit, slingshot, structors, malloc_debug_init);
#else
    __libc_init_common(elfdata, onexit, slingshot, structors, NULL);
#endif
}
