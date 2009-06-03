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

extern void _init(void);
extern void _fini(void);

static void call_array(void(**list)())
{
    // First element is -1, list is null-terminated
    while (*++list) {
        (*list)();
    }
}

static void __bionic_do_global_dtors(structors_array_t const * const p)
{
    call_array(p->fini_array);
    //_fini();
}

extern unsigned __get_sp(void);
extern pid_t    gettid(void);

char*  __progname;
char **environ;

/* from asm/page.h */
unsigned int __page_size = PAGE_SIZE;
unsigned int __page_shift = PAGE_SHIFT;


int __system_properties_init(void);

void __libc_init_common(uintptr_t *elfdata,
                       void (*onexit)(void),
                       int (*slingshot)(int, char**, char**),
                       structors_array_t const * const structors,
                       void (*pre_ctor_hook)())
{
    pthread_internal_t thread;
    pthread_attr_t thread_attr;
    void *tls_area[BIONIC_TLS_SLOTS];
    int argc;
    char **argv, **envp, **envend;
    struct auxentry *auxentry;
    unsigned int page_size = 0, page_shift = 0;

    /* The main thread's stack has empirically shown to be 84k */
    unsigned stacktop = (__get_sp() & ~(PAGE_SIZE - 1)) + PAGE_SIZE;
    unsigned stacksize = 128 * 1024; //84 * 1024;
    unsigned stackbottom = stacktop - stacksize;

    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, (void*)stackbottom, stacksize);
    _init_thread(&thread, gettid(), &thread_attr, (void*)stackbottom);
    __init_tls(tls_area, &thread);

    argc = (int) *elfdata++;
    argv = (char**) elfdata;
    envp = argv+(argc+1);
    environ = envp;

    __progname = argv[0] ? argv[0] : "<unknown>";

    errno = 0;

    __system_properties_init();

    if (pre_ctor_hook) pre_ctor_hook();

    // XXX: we should execute the .fini_array upon exit

    // pre-init array.
    // XXX: I'm not sure what's the different with the init array.
    call_array(structors->preinit_array);

    // for compatibility with non-eabi binary, call the .ctors section
    call_array(structors->ctors_array);

    // call static constructors
    call_array(structors->init_array);

    exit(slingshot(argc, argv, envp));
}
