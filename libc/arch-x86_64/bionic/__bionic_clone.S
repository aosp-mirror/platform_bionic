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
        # Copy 'fn' and 'arg' onto the child stack.
        movq    %r9, -16(%rsi)  # fn
        movq    8(%rsp), %rax   # Read 'arg'.
        movq    %rax, -8(%rsi)  # Write 'arg'.

        subq    $16, %rsi

        # Translate to the kernel calling convention and swap the 'tls' and 'child_tid' arguments.
        # They're flipped for x86-64 compared to all our other architectures and __bionic_clone.
        movq    %r8, %r10
        movq    %rcx, %r8

        # Make the system call.
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
END(__bionic_clone)
.hidden __bionic_clone
