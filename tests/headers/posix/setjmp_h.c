// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

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
