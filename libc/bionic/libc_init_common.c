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

extern unsigned __get_sp(void);
extern pid_t    gettid(void);

char*  __progname;
char **environ;

/* from asm/page.h */
unsigned int __page_size = PAGE_SIZE;
unsigned int __page_shift = PAGE_SHIFT;


int __system_properties_init(void);

void __libc_init_common(uintptr_t *elfdata)
{
    int     argc = *elfdata;
    char**  argv = (char**)(elfdata + 1);
    char**  envp = argv + argc + 1;

    pthread_attr_t             thread_attr;
    static pthread_internal_t  thread;
    static void*               tls_area[BIONIC_TLS_SLOTS];

    /* setup pthread runtime and main thread descriptor */
    unsigned stacktop = (__get_sp() & ~(PAGE_SIZE - 1)) + PAGE_SIZE;
    unsigned stacksize = 128 * 1024;
    unsigned stackbottom = stacktop - stacksize;

    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, (void*)stackbottom, stacksize);
    _init_thread(&thread, gettid(), &thread_attr, (void*)stackbottom);
    __init_tls(tls_area, &thread);

    /* clear errno - requires TLS area */
    errno = 0;

    /* set program name */
    __progname = argv[0] ? argv[0] : "<unknown>";

    /* setup environment pointer */
    environ = envp;

    /* setup system properties - requires environment */
    __system_properties_init();
}

/* This function will be called during normal program termination
 * to run the destructors that are listed in the .fini_array section
 * of the executable, if any.
 *
 * 'fini_array' points to a list of function addresses. The first
 * entry in the list has value -1, the last one has value 0.
 */
void __libc_fini(void* array)
{
    int count;
    void** fini_array = array;
    const size_t  minus1 = ~(size_t)0; /* ensure proper sign extension */

    /* Sanity check - first entry must be -1 */
    if (array == NULL || (size_t)fini_array[0] != minus1) {
        return;
    }

    /* skip over it */
    fini_array += 1;

    /* Count the number of destructors. */
    for (count = 0; fini_array[count] != NULL; count++);

    /* Now call each destructor in reverse order. */
    while (count > 0) {
        void (*func)() = (void (*)) fini_array[--count];

        /* Sanity check, any -1 in the list is ignored */
        if ((size_t)func == minus1)
            continue;

        func();
    }
}
