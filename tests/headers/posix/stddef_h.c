// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <stddef.h>

#include "header_checks.h"

static void stddef_h() {
  void* null = NULL;

#if !defined(offsetof)
#error offsetof
#endif

  TYPE(ptrdiff_t);
  TYPE(wchar_t);
  TYPE(size_t);
}
