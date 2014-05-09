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
#include <machine/setjmp.h>

/*
 * C library - _setjmp, _longjmp
 *
 * _longjmp(jmp_buf state, int value)
 * will generate a "return(v)" from the last call to _setjmp(state) by restoring
 * registers from the stack. The previous signal state is NOT restored.
 *
 * NOTE: x0 return value
 *       x9-x15 temporary registers
 */

ENTRY(setjmp)
    /* block all signals an retrieve signal mask */
    stp     x0, x30, [sp, #-16]!

    mov     x0, xzr
    bl      PIC_SYM(sigblock, PLT)
    mov     w1, w0

    ldp     x0, x30, [sp], #16

    /* store signal mask */
    str     w1, [x0, #(_JB_SIGMASK *4)]

    /* store magic number */
    ldr     w9, .L_setjmp_magic
    str     w9, [x0, #(_JB_MAGIC * 4)]

    /* store core registers */
    mov     x10, sp
    stp     x30, x10, [x0, #(_JB_CORE_BASE * 4 + 16 * 0)]
    stp     x28, x29, [x0, #(_JB_CORE_BASE * 4 + 16 * 1)]
    stp     x26, x27, [x0, #(_JB_CORE_BASE * 4 + 16 * 2)]
    stp     x24, x25, [x0, #(_JB_CORE_BASE * 4 + 16 * 3)]
    stp     x22, x23, [x0, #(_JB_CORE_BASE * 4 + 16 * 4)]
    stp     x20, x21, [x0, #(_JB_CORE_BASE * 4 + 16 * 5)]
    str     x19,      [x0, #(_JB_CORE_BASE * 4 + 16 * 6)]

    /* store floating point registers */
    stp     d14, d15, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 0)]
    stp     d12, d13, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 1)]
    stp     d10, d11, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 2)]
    stp     d8,  d9,  [x0, #(_JB_FLOAT_BASE * 4 + 16 * 3)]

    mov     w0, wzr
    ret
END(setjmp)

.L_setjmp_magic:
    .word   _JB_MAGIC__SETJMP

ENTRY(longjmp)
    /* check magic */
    ldr     w9, .L_setjmp_magic
    ldr     w10, [x0, #(_JB_MAGIC * 4)]
    cmp     w9, w10
    b.ne    .L_fail

    /* restore core registers */
    ldp     x30, x10, [x0, #(_JB_CORE_BASE * 4 + 16 * 0)]
    mov     sp, x10
    ldp     x28, x29, [x0, #(_JB_CORE_BASE * 4 + 16 * 1)]
    ldp     x26, x27, [x0, #(_JB_CORE_BASE * 4 + 16 * 2)]
    ldp     x24, x25, [x0, #(_JB_CORE_BASE * 4 + 16 * 3)]
    ldp     x22, x23, [x0, #(_JB_CORE_BASE * 4 + 16 * 4)]
    ldp     x20, x21, [x0, #(_JB_CORE_BASE * 4 + 16 * 5)]
    ldr     x19,      [x0, #(_JB_CORE_BASE * 4 + 16 * 6)]

    /* restore floating point registers */
    ldp     d14, d15, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 0)]
    ldp     d12, d13, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 1)]
    ldp     d10, d11, [x0, #(_JB_FLOAT_BASE * 4 + 16 * 2)]
    ldp     d8,  d9,  [x0, #(_JB_FLOAT_BASE * 4 + 16 * 3)]

    /* validate sp (sp mod 16 = 0) and lr (lr mod 4 = 0) */
    tst     x30, #3
    b.ne    .L_fail
    mov     x10, sp
    tst     x10, #15
    b.ne    .L_fail

    /* set return value */
    cmp     w1, wzr
    csinc   w0, w1, wzr, ne
    ret

    /* validation failed, die die die */
.L_fail:
    bl      PIC_SYM(longjmperror, PLT)
    bl      PIC_SYM(abort, PLT)
    b       . - 8       /* Cannot get here */
END(longjmp)
