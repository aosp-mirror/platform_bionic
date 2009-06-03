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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>
#include <asm/page.h>
#include "pthread_internal.h"
#include "atexit.h"
#include "libc_init_common.h"

#include <bionic_tls.h>
#include <errno.h>

/* This contains the common C library initialization code.
 * To understand what happens here, you should read the
 * "Initialization and Finalization" section of the file
 * named bionic/linker/README.TXT
 */

static void call_array(void(**list)())
{
    // First element is -1, list is null-terminated
    while (*++list) {
        (*list)();
    }
}

char*  __progname;
char **environ;

/* from asm/page.h */
unsigned int __page_size = PAGE_SIZE;
unsigned int __page_shift = PAGE_SHIFT;


int __system_properties_init(void);

/* This function can be run under two different contexts:
 *
 * - for statically linked executables (i.e. those who do
 *   not depend on shared libraries at all), it will be
 *   called from the __libc_init() function defined in
 *   bionic/libc_init_static.c
 *
 * - for dynamic executables, it will be called from the
 *   __libc_init() function defined in bionic/libc_init_dynamic.c
 *
 */
void __libc_init_common(uintptr_t *elfdata,
                       void (*onexit)(void),
                       int (*slingshot)(int, char**, char**),
                       structors_array_t const * const structors,
                       void (*pre_ctor_hook)())
{
    int argc;
    char **argv, **envp, **envend;
    struct auxentry *auxentry;
    unsigned int page_size = 0, page_shift = 0;

    argc = (int) *elfdata++;
    argv = (char**) elfdata;
    envp = argv+(argc+1);
    environ = envp;

    __progname = argv[0] ? argv[0] : "<unknown>";

    errno = 0;

    __system_properties_init();

    if (pre_ctor_hook) pre_ctor_hook();

    if (structors != NULL) {
        // pre-init array.
        call_array(structors->preinit_array);

        // for compatibility with non-eabi binary, call the .ctors section
        // this is only useful for static non-ARM (e.g. x86) executables.
        call_array(structors->ctors_array);

        // call static constructors
        call_array(structors->init_array);
    }

    exit(slingshot(argc, argv, envp));
}
