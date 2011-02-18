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

#if defined(__ARM_HAVE_LDREX_STREX)
/*
 * ===========================================================================
 *      ARMv6+ implementation
 * ===========================================================================
 */

/* r0(addr) -> r0(old) */
ENTRY(__atomic_dec)
    mov     r1, r0                      @ copy addr so we don't clobber it
1:  ldrex   r0, [r1]                    @ load current value into r0
    sub     r2, r0, #1                  @ generate new value into r2
    strex   r3, r2, [r1]                @ try to store new value; result in r3
    cmp     r3, #0                      @ success?
    bxeq    lr                          @ yes, return
    b       1b                          @ no, retry
END(__atomic_dec)

/* r0(addr) -> r0(old) */
ENTRY(__atomic_inc)
    mov     r1, r0
1:  ldrex   r0, [r1]
    add     r2, r0, #1
    strex   r3, r2, [r1]
    cmp     r3, #0
    bxeq    lr
    b       1b
END(__atomic_inc)

/* r0(old) r1(new) r2(addr) -> r0(zero_if_succeeded) */
ENTRY(__atomic_cmpxchg)
1:  mov     ip, #2                      @ ip=2 means "new != old"
    ldrex   r3, [r2]                    @ load current value into r3
    teq     r0, r3                      @ new == old?
    strexeq ip, r1, [r2]                @ yes, try store, set ip to 0 or 1
    teq     ip, #1                      @ strex failure?
    beq     1b                          @ yes, retry
    mov     r0, ip                      @ return 0 on success, 2 on failure
    bx      lr
END(__atomic_cmpxchg)

/* r0(new) r1(addr) -> r0(old) */
ENTRY(__atomic_swap)
1:  ldrex   r2, [r1]
    strex   r3, r0, [r1]
    teq     r3, #0
    bne     1b
    mov     r0, r2
    bx      lr
END(__atomic_swap)

#else /*not defined __ARM_HAVE_LDREX_STREX*/
/*
 * ===========================================================================
 *      Pre-ARMv6 implementation
 * ===========================================================================
 */

    /* int __kernel_cmpxchg(int oldval, int newval, int* ptr) */
    .equ    kernel_cmpxchg, 0xFFFF0FC0
    .equ    kernel_atomic_base, 0xFFFF0FFF

/* r0(addr) -> r0(old) */
ENTRY(__atomic_dec)
    .save {r4, lr}
    stmdb   sp!, {r4, lr}
    mov     r2, r0
1: @ atomic_dec
    ldr     r0, [r2]
    mov     r3, #kernel_atomic_base
    add     lr, pc, #4
    sub     r1, r0, #1
    add     pc, r3, #(kernel_cmpxchg - kernel_atomic_base)
    bcc     1b
    add     r0, r1, #1
    ldmia   sp!, {r4, lr}
    bx      lr
END(__atomic_dec)

/* r0(addr) -> r0(old) */
ENTRY(__atomic_inc)
    .save {r4, lr}
    stmdb   sp!, {r4, lr}
    mov     r2, r0
1: @ atomic_inc
    ldr     r0, [r2]
    mov     r3, #kernel_atomic_base
    add     lr, pc, #4
    add     r1, r0, #1
    add     pc, r3, #(kernel_cmpxchg - kernel_atomic_base)
    bcc     1b
    sub     r0, r1, #1
    ldmia   sp!, {r4, lr}
    bx      lr
END(__atomic_inc)

/* r0(old) r1(new) r2(addr) -> r0(zero_if_succeeded) */
ENTRY(__atomic_cmpxchg)
    .save {r4, lr}
    stmdb   sp!, {r4, lr}
    mov     r4, r0          /* r4 = save oldvalue */
1: @ atomic_cmpxchg
    mov     r3, #kernel_atomic_base
    add     lr, pc, #4
    mov     r0, r4          /* r0 = oldvalue */
    add     pc, r3, #(kernel_cmpxchg - kernel_atomic_base)
    bcs     2f              /* swap was made. we're good, return. */
    ldr     r3, [r2]        /* swap not made, see if it's because *ptr!=oldvalue */
    cmp     r3, r4
    beq     1b
2: @ atomic_cmpxchg
    ldmia   sp!, {r4, lr}
    bx      lr
END(__atomic_cmpxchg)

/* r0(new) r1(addr) -> r0(old) */
ENTRY(__atomic_swap)
    swp     r0, r0, [r1]
    bx      lr
END(__atomic_swap)

#endif /*not defined __ARM_HAVE_LDREX_STREX*/


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
