// Copyright (C) 2017 The Android Open Source Project
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>

#include "header_checks.h"

static void stdbool_h() {
  TYPE(bool);
  bool t = true;
  bool f = false;
  MACRO_VALUE(__bool_true_false_are_defined, 1);
}
