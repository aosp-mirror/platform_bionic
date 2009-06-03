/*
 * Copyright (C) 2009 The Android Open Source Project
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

#ifndef _BIONIC_PREINIT_H
#define _BIONIC_PREINIT_H

#include "pthread_internal.h"
#include "bionic_tls.h"
#include <asm/page.h>

/* this function is used to perform a minimal initialization of the
 * the C library. This must be performed before any other call to
 * other functions, in either the dynamic linker's startup code
 * or libc_init_static.c
 *
 * 'main_thread' and 'tls_area' must be persistent variables,
 * which means they must be either global, or allocated in the
 * main thread's stack frame and never, ever, freed before
 * program exit.
 *
 * VERY IMPORTANT NOTE:
 *
 *   THIS IMPLEMENTATION SHOULD NOT USE GLOBAL VARIABLES.
 *
 *   The reason is the dynamic linker's tricky usage of C library
 *   functions and later renaming of their symbols. See the
 *   "C Library Usage Restriction" section in bionic/linker/README.TXT
 *   for details.
 */

extern unsigned __get_sp(void);
extern pid_t    gettid(void);

static __inline__ void
__libc_preinit( pthread_internal_t*  main_thread,
                void*                tls_area[BIONIC_TLS_SLOTS] )
{
    pthread_attr_t  thread_attr;

    /* Setup the main thread's information */
    unsigned stacktop = (__get_sp() & ~(PAGE_SIZE - 1)) + PAGE_SIZE;
    unsigned stacksize = 128 * 1024;
    unsigned stackbottom = stacktop - stacksize;

    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, (void*)stackbottom, stacksize);
    _init_thread(main_thread, gettid(), &thread_attr, (void*)stackbottom);
    __init_tls(tls_area, main_thread);
}

#endif /* _BIONIC_INIT_H */
