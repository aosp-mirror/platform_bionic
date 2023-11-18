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

#define iResult a0

#define pStr1 a0
#define pStr2 a1

#define iVL a2
#define iTemp1 a3
#define iTemp2 a4
#define iLMUL1 a5
#define iLMUL2 a6
#define iLMUL4 a7

#define iLMUL t0

#define vStr1 v0
#define vStr2 v8
#define vMask1 v16
#define vMask2 v17

ENTRY(strcmp_v)

    # increase the lmul using the following sequences:
    # 1/2, 1/2, 1, 2, 4, 4, 4, ...

    # lmul=1/2
    vsetvli iVL, zero, e8, mf2, ta, ma

    vle8ff.v vStr1, (pStr1)
     # check if vStr1[i] == 0
    vmseq.vx vMask1, vStr1, zero

    vle8ff.v vStr2, (pStr2)
    # check if vStr1[i] != vStr2[i]
    vmsne.vv vMask2, vStr1, vStr2

    # find the index x for vStr1[x]==0
    vfirst.m iTemp1, vMask1
    # find the index x for vStr1[x]!=vStr2[x]
    vfirst.m iTemp2, vMask2

    bgez iTemp1, L(check1)
    bgez iTemp2, L(check2)

    # get the current vl updated by vle8ff.
    csrr iVL, vl
    add pStr1, pStr1, iVL
    add pStr2, pStr2, iVL

    vsetvli iVL, zero, e8, mf2, ta, ma
    addi iLMUL1, zero, 1
    addi iLMUL, zero, 1
    j L(loop)
L(m1):
    vsetvli iVL, zero, e8, m1, ta, ma
    addi iLMUL2, zero, 2
    addi iLMUL, zero, 2
    j L(loop)
L(m2):
    vsetvli iVL, zero, e8, m2, ta, ma
    addi iLMUL4, zero, 4
    addi iLMUL, zero, 4
    j L(loop)
L(m4):
    vsetvli iVL, zero, e8, m4, ta, ma

L(loop):
    vle8ff.v vStr1, (pStr1)
    vmseq.vx vMask1, vStr1, zero

    vle8ff.v vStr2, (pStr2)
    vmsne.vv vMask2, vStr1, vStr2

    vfirst.m iTemp1, vMask1
    vfirst.m iTemp2, vMask2

    bgez iTemp1, L(check1)
    bgez iTemp2, L(check2)

    csrr iVL, vl
    add pStr1, pStr1, iVL
    add pStr2, pStr2, iVL

    beq iLMUL, iLMUL1, L(m1)
    beq iLMUL, iLMUL2, L(m2)
    beq iLMUL, iLMUL4, L(m4)
    j L(loop)

    // iTemp1>=0
L(check1):
    bltz iTemp2, 1f
    blt iTemp2, iTemp1, L(check2)
1:
    // iTemp2<0
    // iTemp2>=0 && iTemp1<iTemp2
    add pStr1, pStr1, iTemp1
    add pStr2, pStr2, iTemp1
    lbu iTemp1, 0(pStr1)
    lbu iTemp2, 0(pStr2)
    sub iResult, iTemp1, iTemp2
    ret

    // iTemp1<0
    // iTemp2>=0
L(check2):
    add pStr1, pStr1, iTemp2
    add pStr2, pStr2, iTemp2
    lbu iTemp1, 0(pStr1)
    lbu iTemp2, 0(pStr2)
    sub iResult, iTemp1, iTemp2
    ret

END(strcmp_v)
