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

// TODO: switch to the V variants when qemu is fixed.
FUNCTION_DELEGATE(memchr, memchr_gc)
FUNCTION_DELEGATE(memcmp, memcmp_gc)
FUNCTION_DELEGATE(memcpy, memcpy_gc)
FUNCTION_DELEGATE(memmove, memmove_gc)
FUNCTION_DELEGATE(memset, memset_gc)
FUNCTION_DELEGATE(stpcpy, stpcpy_gc)
FUNCTION_DELEGATE(strcat, strcat_gc)
FUNCTION_DELEGATE(strchr, strchr_gc)
FUNCTION_DELEGATE(strcmp, strcmp_gc)
FUNCTION_DELEGATE(strcpy, strcpy_gc)
FUNCTION_DELEGATE(strlen, strlen_gc)
FUNCTION_DELEGATE(strncat, strncat_gc)
FUNCTION_DELEGATE(strncmp, strncmp_gc)
FUNCTION_DELEGATE(strncpy, strncpy_gc)
FUNCTION_DELEGATE(strnlen, strnlen_gc)
