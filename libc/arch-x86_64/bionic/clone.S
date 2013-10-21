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
        # Save tls.
        movq    %rsi, %r11
        # Enforce 16-byte alignment for child stack.
        andq    $~15, %rsi

        # Copy 'fn', 'arg', and 'tls' onto the child stack.
        movq    %rdi, -32(%rsi)  # fn
        movq    %rcx, -24(%rsi)  # arg
        movq    %r11, -16(%rsi)  # tls
        subq    $32, %rsi

        movq    %rdx, %rdi
        movl    $__NR_clone, %eax
        syscall
        testl   %eax, %eax
        jns     1f

        # An error occurred, set errno and return -1.
        negl    %eax
        movl    %eax, %edi
        call    __set_errno
        orl     $-1, %eax
        jmp     2f
1:
        jnz     2f

        # We're in the child now, so call __thread_entry
        # with the arguments from the child stack moved into
        # the appropriate registers. We avoid pop here to keep
        # the required 16-byte stack alignment.
        movq    (%rsp), %rdi    # fn
        movq    8(%rsp), %rsi   # arg
        movq    16(%rsp), %rdx  # tls
        call    __thread_entry
        hlt
2:
        ret

// int __bionic_clone(unsigned long clone_flags,
//                    void* new_sp,
//                    int* parent_tid_ptr,
//                    void* new_tls,
//                    int* child_tid_ptr,
//                    int (*fn)(void*),
//                    void* arg);
ENTRY(__bionic_clone)
        # Enforce 16-byte alignment for child stack.
        andq    $~15, %rsi

        # Copy 'fn' and 'arg' onto the child stack.
        movq    %r9, -16(%rsi)  # fn
        movq    8(%rsp), %rax   # Read 'arg'.
        movq    %rax, -8(%rsi)  # Write 'arg'.

        subq    $16, %rsi
        movq    %r8, %r10
        movq    %rcx, %r8
        movl    $__NR_clone, %eax
        syscall
        testl   %eax, %eax
        jns     1f

        # An error occurred, set errno and return -1.
        negl    %eax
        movl    %eax, %edi
        call    __set_errno
        orl     $-1, %eax
        jmp     2f
1:
        jnz     2f

        # We're in the child now, so call __bionic_clone_entry
        # with the arguments from the child stack moved into
        # the appropriate registers.
        popq    %rdi  # fn
        popq    %rsi  # arg
        call    __bionic_clone_entry
        hlt
2:
        ret
