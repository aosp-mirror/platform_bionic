/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <setjmp.h>

#include "header_checks.h"

// POSIX says it's undefined whether `setjmp` is a macro or a function,
// but C11 says it's a macro, and the C standard always wins.
#if !defined(setjmp)
#error setjmp
#endif

static void setjmp_h() {
  TYPE(jmp_buf);
  TYPE(sigjmp_buf);

  FUNCTION(_longjmp, void (*f)(jmp_buf, int));
  FUNCTION(longjmp, void (*f)(jmp_buf, int));
  FUNCTION(siglongjmp, void (*f)(sigjmp_buf, int));

  FUNCTION(_setjmp, int (*f)(jmp_buf));
  FUNCTION(setjmp, int (*f)(jmp_buf));
#if defined(__GLIBC__)
  FUNCTION(__sigsetjmp, int (*f)(sigjmp_buf, int));
#else
  FUNCTION(sigsetjmp, int (*f)(sigjmp_buf, int));
#endif
}
