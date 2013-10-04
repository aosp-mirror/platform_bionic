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

#include <asm/unistd.h>
#include <machine/asm.h>

// int  __pthread_clone(void* (*fn)(void*), void* tls, int flags, void* arg);
ENTRY(__pthread_clone)
        # save tls
        movq    %rsi, %r11
        # 16-byte alignment on child stack
        andq    $~15, %rsi

        # insert arguments onto the child stack
        movq    %rdi, -32(%rsi)
        movq    %rcx, -24(%rsi)
        movq    %r11, -16(%rsi)

        subq    $32, %rsi
        movq    %rdx, %rdi
        movl    $__NR_clone, %eax
        syscall
        testl   %eax, %eax
        jns     1f

        # an error occurred, set errno and return -1
        negl    %eax
        movl    %eax, %edi
        call    __set_errno
        orl     $-1, %eax
        jmp     2f
1:
        jnz     2f

        # we're in the child thread now, call __thread_entry
        # with the appropriate arguments on the child stack
        # we already placed most of them
        call    __thread_entry
        hlt
2:
        ret

/*
 * int  __bionic_clone(unsigned long clone_flags,
 *                     void*         newsp,
 *                     int           *parent_tidptr,
 *                     void          *new_tls,
 *                     int           *child_tidptr,
 *                     int           (*fn)(void *),
 *                     void          *arg);
 */
ENTRY(__bionic_clone)
        # insert arguments onto the child stack
        andq    $~15, %rsi
        movq    %r9, -16(%rsi)
        # 7th argument (arg) goes through stack
        movq    8(%rsp), %rax
        movq    %rax, -8(%rsi)

        subq    $16, %rsi
        movq    %r8, %r10
        movq    %rcx, %r8
        movl    $__NR_clone, %eax
        syscall
        testl   %eax, %eax
        jns     1f

        # an error occurred, set errno and return -1
        negl    %eax
        movl    %eax, %edi
        call    __set_errno
        orl     $-1, %eax
        jmp     2f

1:
        jnz     2f

        # we're in the child now, call __bionic_clone_entry
        # with the appropriate arguments on the child stack
        # we already placed most of them
        call    __bionic_clone_entry
        hlt

2:
        ret

