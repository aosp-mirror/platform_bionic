/*
 * Copyright (C) 2023 The Android Open Source Project
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
/*
 * Copyright (c) 2023 SiFive, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SIFIVE INC ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL SIFIVE INC BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <private/bionic_asm.h>

#define pDst a0
#define pSrc a1
#define pDstPtr a2

#define iVL a3
#define iCurrentVL a4
#define iActiveElemPos a5

#define ELEM_LMUL_SETTING m1
#define vMask1 v0
#define vMask2 v1
#define vStr1 v8
#define vStr2 v16

ENTRY(strcat_v)

    mv pDstPtr, pDst

    // the strlen of dst
L(strlen_loop):
    vsetvli iVL, zero, e8, ELEM_LMUL_SETTING, ta, ma

    vle8ff.v vStr1, (pDstPtr)
    // find the '\0'
    vmseq.vx vMask1, vStr1, zero
    csrr iCurrentVL, vl
    vfirst.m iActiveElemPos, vMask1
    add pDstPtr, pDstPtr, iCurrentVL
    bltz iActiveElemPos, L(strlen_loop)

    sub pDstPtr, pDstPtr, iCurrentVL
    add pDstPtr, pDstPtr, iActiveElemPos

    // copy pSrc to pDstPtr
L(strcpy_loop):
    vsetvli iVL, zero, e8, ELEM_LMUL_SETTING, ta, ma

    vle8ff.v vStr1, (pSrc)
    vmseq.vx vMask2, vStr1, zero
    csrr iCurrentVL, vl
    vfirst.m iActiveElemPos, vMask2
    vmsif.m vMask1, vMask2
    add pSrc, pSrc, iCurrentVL
    vse8.v vStr1, (pDstPtr), vMask1.t
    add pDstPtr, pDstPtr, iCurrentVL
    bltz iActiveElemPos, L(strcpy_loop)

    ret

END(strcat_v)
