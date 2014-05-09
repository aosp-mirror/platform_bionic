/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <private/bionic_asm.h>

// pid_t __bionic_clone(int flags, void* child_stack, pid_t* parent_tid, void* tls, pid_t* child_tid, int (*fn)(void*), void* arg);

ENTRY(__bionic_clone)
    stp     x29, x30, [sp, #-16]!
    mov     x29,  sp
    str     x8,       [sp, #-16]!

    /* store thread pointer & args in child stack */
    stp     x5, x6, [x1, #-16]

    /* sys_clone */
    uxtw    x0, w0
    mov     x8, __NR_clone
    svc     #0

    /* check for child/parent */
    cbz     x0,1f

    ldr     x8,       [sp], #16
    ldp     x29, x30, [sp], #16

    cmn     x0, #(MAX_ERRNO + 1)
    cneg    x0, x0, hi
    b.hi    __set_errno

    ret

    /* thread initialization - set the end of the frame record chain */
1:
    mov     x29, xzr
    ldp     x0, x1, [sp, #-16]
    b       __bionic_clone_entry
END(__bionic_clone)
.hidden __bionic_clone
