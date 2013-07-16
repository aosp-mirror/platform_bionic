/*
 * Copyright (C) 2008-2010 The Android Open Source Project
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

#include <linux/err.h>
#include <machine/asm.h>
#include <asm/unistd.h>

// int  __pthread_clone(void* (*fn)(void*), void* child_stack, int flags, void* arg);
ENTRY(__pthread_clone)
    # Push 'fn' and 'arg' onto 'child_stack'.
    stmdb   r1!, {r0, r3}

    # The sys_clone system call only takes two arguments: 'flags' and 'child_stack'.
    # 'child_stack' is already in r1, but we need to move 'flags' into position.
    mov     r0, r2

    # System call.
    mov     ip, r7
    ldr     r7, =__NR_clone
    swi     #0

    # Child?
    movs    r0, r0
    beq     1f

    # Parent.
    mov     r7, ip
    cmn     r0, #(MAX_ERRNO + 1)
    bxls    lr
    neg     r0, r0
    b       __set_errno

1:  # Child.
    # Pop 'fn' and 'arg' back off the stack and call __thread_entry.
    pop     {r0, r1}
    # __thread_entry also needs our stack pointer.
    mov     r2, sp
    b       __thread_entry
END(__pthread_clone)


    #
    # This function is defined as:
    #
    #   pid_t  __bionic_clone( int  flags, void *child_stack,
    #                          pid_t *pid, void *tls, pid_t *ctid,
    #                          int  (*fn)(void *), void* arg );
    #
    # NOTE: This is not the same signature as the glibc
    #       __clone function. Placing 'fn' and 'arg'
    #       at the end of the parameter list makes the
    #       implementation much simpler.
    #

ENTRY(__bionic_clone)
    mov     ip, sp
    .save   {r4, r5, r6, r7}

    # save registers to parent stack
    stmfd   sp!, {r4, r5, r6, r7}

    # load extra parameters
    ldmfd   ip, {r4, r5, r6}

    # store 'fn' and 'arg' to the child stack
    str     r5, [r1, #-4]
    str     r6, [r1, #-8]

    # System call
    ldr     r7, =__NR_clone
    swi     #0
    movs    r0, r0
    beq     1f

    # In the parent, reload saved registers then either return or set errno.
    ldmfd   sp!, {r4, r5, r6, r7}
    cmn     r0, #(MAX_ERRNO + 1)
    bxls    lr
    neg     r0, r0
    b       __set_errno

1:  # The child.
    ldr    r0, [sp, #-4]
    ldr    r1, [sp, #-8]
    b      __bionic_clone_entry
END(__bionic_clone)
