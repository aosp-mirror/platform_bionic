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

#include <private/bionic_asm.h>

#define FUNCTION_DELEGATE(name, impl) \
ENTRY(name); \
    j impl; \
END(name)

#if defined(__riscv_v)

FUNCTION_DELEGATE(memchr, memchr_vext)
FUNCTION_DELEGATE(memcmp, memcmp_vext)
FUNCTION_DELEGATE(memcpy, memcpy_vext)
FUNCTION_DELEGATE(memmove, memmove_vext)
FUNCTION_DELEGATE(memset, memset_vext)
FUNCTION_DELEGATE(stpcpy, stpcpy_vext)
FUNCTION_DELEGATE(strcat, strcat_vext)
FUNCTION_DELEGATE(strchr, strchr_vext)
FUNCTION_DELEGATE(strcmp, strcmp_vext)
FUNCTION_DELEGATE(strcpy, strcpy_vext)
FUNCTION_DELEGATE(strlen, strlen_vext)
FUNCTION_DELEGATE(strncat, strncat_vext)
FUNCTION_DELEGATE(strncmp, strncmp_vext)
FUNCTION_DELEGATE(strncpy, strncpy_vext)
FUNCTION_DELEGATE(strnlen, strnlen_vext)

#endif

NOTE_GNU_PROPERTY()
