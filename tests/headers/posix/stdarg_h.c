// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <stdarg.h>

#include "header_checks.h"

static void stdarg_h() {
  TYPE(va_list);

#if !defined(va_start)
#error va_start
#endif
#if !defined(va_copy)
#error va_copy
#endif
#if !defined(va_arg)
#error va_arg
#endif
#if !defined(va_end)
#error va_end
#endif
}
