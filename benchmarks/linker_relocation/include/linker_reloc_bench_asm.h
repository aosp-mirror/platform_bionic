/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#if defined(__arm__)

#define GOT_RELOC(sym) .long sym(GOT_PREL)
#define CALL(sym) bl sym
#define DATA_WORD(val) .long val
#define MAIN .globl main; main: mov r0, #0; bx lr

#elif defined(__aarch64__)

#define GOT_RELOC(sym) adrp x1, :got:sym
#define CALL(sym) bl sym
#define DATA_WORD(val) .quad val
#define MAIN .globl main; main: mov w0, wzr; ret

#elif defined(__i386__)

#define GOT_RELOC(sym) .long sym@got
#define CALL(sym) call sym@PLT
#define DATA_WORD(val) .long val
#define MAIN .globl main; main: xorl %eax, %eax; retl

#elif defined(__x86_64__)

#define GOT_RELOC(sym) .quad sym@got
#define CALL(sym) call sym@PLT
#define DATA_WORD(val) .quad val
#define MAIN .globl main; main: xorl %eax, %eax; retq

#else
#error "Unrecognized architecture"
#endif
