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
#include <sys/linux-syscalls.h>
#include <machine/asm.h>
#include <machine/cpu-features.h>

#define FUTEX_WAIT 0
#define FUTEX_WAKE 1

/* __futex_wait(*ftx, val, *timespec) */
/* __futex_wake(*ftx, counter) */
/* __futex_syscall3(*ftx, op, val) */
/* __futex_syscall4(*ftx, op, val, *timespec) */

.global __futex_wait
.type __futex_wait, %function

.global __futex_wake
.type __futex_wake, %function

.global __futex_syscall3
.type __futex_syscall3, %function

.global __futex_syscall4
.type __futex_syscall4, %function

#if __ARM_EABI__

ENTRY(__futex_syscall3)
    stmdb   sp!, {r4, r7}
    .save   {r4, r7}
    ldr     r7, =__NR_futex
    swi     #0
    ldmia   sp!, {r4, r7}
    bx      lr
END(__futex_syscall3)

ENTRY(__futex_wait)
    stmdb   sp!, {r4, r7}
    .save   {r4, r7}
    mov     r3, r2
    mov     r2, r1
    mov     r1, #FUTEX_WAIT
    ldr     r7, =__NR_futex
    swi     #0
    ldmia   sp!, {r4, r7}
    bx      lr
END(__futex_wait)

ENTRY(__futex_wake)
    .save   {r4, r7}
    stmdb   sp!, {r4, r7}
    mov     r2, r1
    mov     r1, #FUTEX_WAKE
    ldr     r7, =__NR_futex
    swi     #0
    ldmia   sp!, {r4, r7}
    bx      lr
END(__futex_wake)

#else

ENTRY(__futex_syscall3)
    swi     #__NR_futex
    bx      lr
END(__futex_syscall3)

ENTRY(__futex_wait)
    mov     r3, r2
    mov     r2, r1
    mov     r1, #FUTEX_WAIT
    swi     #__NR_futex
    bx      lr
END(__futex_wait)

ENTRY(__futex_wake)
    mov     r2, r1
    mov     r1, #FUTEX_WAKE
    swi     #__NR_futex
    bx      lr
END(__futex_wake)

#endif

ENTRY(__futex_syscall4)
    b __futex_syscall3
END(__futex_syscall4)
