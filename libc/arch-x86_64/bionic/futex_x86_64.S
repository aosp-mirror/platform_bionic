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

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1

// int __futex_wait(volatile void* ftx, int val, const struct timespec* timeout)
ENTRY_PRIVATE(__futex_wait)
    mov     %rdx, %r10           /* timeout */
    mov     %esi, %edx           /* val */
    mov     $FUTEX_WAIT, %esi    /* op */
    mov     $__NR_futex, %eax
    syscall
    ret
END(__futex_wait)

// int __futex_wake(volatile void* ftx, int count)
ENTRY_PRIVATE(__futex_wake)
    mov     %esi, %edx
    mov     $FUTEX_WAKE, %esi
    mov     $__NR_futex, %eax
    syscall
    ret
END(__futex_wake)

// int __futex_syscall3(volatile void* ftx, int op, int count)
ENTRY(__futex_syscall3)
    mov     $__NR_futex, %eax
    syscall
    ret
END(__futex_syscall3)

// int __futex_syscall4(volatile void* ftx, int op, int val, const struct timespec* timeout)
ENTRY(__futex_syscall4)
    mov     %rcx, %r10      /* timeout */
    mov     $__NR_futex, %eax
    syscall
    ret
END(__futex_syscall4)
