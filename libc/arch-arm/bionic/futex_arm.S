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

#include <asm/unistd.h>
#include <machine/asm.h>

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1

// __futex_syscall3(*ftx, op, val)
ENTRY(__futex_syscall3)
    mov     ip, r7
    ldr     r7, =__NR_futex
    swi     #0
    mov     r7, ip
    bx      lr
END(__futex_syscall3)

// __futex_syscall4(*ftx, op, val, *timespec)
ENTRY(__futex_syscall4)
    b __futex_syscall3
END(__futex_syscall4)

// __futex_wait(*ftx, val, *timespec)
ENTRY(__futex_wait)
    mov     ip, r7
    mov     r3, r2
    mov     r2, r1
    mov     r1, #FUTEX_WAIT
    ldr     r7, =__NR_futex
    swi     #0
    mov     r7, ip
    bx      lr
END(__futex_wait)

// __futex_wake(*ftx, counter)
ENTRY(__futex_wake)
    mov     ip, r7
    mov     r2, r1
    mov     r1, #FUTEX_WAKE
    ldr     r7, =__NR_futex
    swi     #0
    mov     r7, ip
    bx      lr
END(__futex_wake)
