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

/* implement flockfile(), ftrylockfile() and funlockfile()
 *
 * we can't use the OpenBSD implementation which uses kernel-specific
 * APIs not available on Linux.
 *
 * Instead, we use a pthread_mutex_t within the FILE* internal state.
 * See fileext.h for details.
 *
 * the behaviour, if fclose() is called while the corresponding
 * file is locked is totally undefined.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "fileext.h"


void
flockfile(FILE * fp)
{
    if (fp != NULL) {
        _FLOCK_LOCK(fp);
    }
}


int
ftrylockfile(FILE *fp)
{
    /* The specification for ftrylockfile() says it returns 0 on success,
     * or non-zero on error. So return an errno code directly on error.
     */
    int  ret = EINVAL;

    if (fp != NULL) {
        ret = _FLOCK_TRYLOCK(fp);
    }
    return ret;
}

void
funlockfile(FILE * fp)
{
    if (fp != NULL) {
        _FLOCK_UNLOCK(fp);
    }
}
