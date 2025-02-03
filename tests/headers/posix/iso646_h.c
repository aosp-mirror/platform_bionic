// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <iso646.h>

#include "header_checks.h"

static void iso646_h() {
#if !defined(and)
#error and
#endif
#if !defined(and_eq)
#error and_eq
#endif
#if !defined(bitand)
#error bitand
#endif
#if !defined(bitor)
#error bitor
#endif
#if !defined(compl)
#error compl
#endif
#if !defined(not)
#error not
#endif
#if !defined(not_eq)
#error not_eq
#endif
#if !defined(or)
#error or
#endif
#if !defined(or_eq)
#error or_eq
#endif
#if !defined(xor)
#error xor
#endif
#if !defined(xor_eq)
#error xor_eq
#endif
}
