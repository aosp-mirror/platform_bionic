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

#include <sys/types.h>
#include <stdio.h>

#include <async_safe/log.h>

/*
 * This source file should only be included by libc.so, its purpose is
 * to support legacy ARM binaries by exporting a publicly visible
 * implementation of atexit().
 */

extern int __cxa_atexit(void (*func)(void *), void *arg, void *dso);

/*
 * Register a function to be performed at exit.
 */
int
atexit(void (*func)(void))
{
    /*
     * Exit functions queued by this version of atexit will not be called
     * on dlclose(), and when they are called (at program exit), the
     * calling library may have been dlclose()'d, causing the program to
     * crash.
     */
    static char const warning[] = "WARNING: generic atexit() called from legacy shared library\n";

    async_safe_format_log(ANDROID_LOG_WARN, "libc", warning);
    fprintf(stderr, warning);

    return (__cxa_atexit((void (*)(void *))func, NULL, NULL));
}
