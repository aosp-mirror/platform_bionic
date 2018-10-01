/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include <float.h>

TEST(float_h, macros) {
#if !defined(FLT_RADIX)
#error FLT_RADIX
#endif
#if !defined(DECIMAL_DIG)
#error DECIMAL_DIG
#endif
#if !defined(FLT_DECIMAL_DIG)
#error FLT_DECIMAL_DIG
#endif
#if !defined(DBL_DECIMAL_DIG)
#error DBL_DECIMAL_DIG
#endif
#if !defined(LDBL_DECIMAL_DIG)
#error LDBL_DECIMAL_DIG
#endif
#if !defined(FLT_MIN)
#error FLT_MIN
#endif
#if !defined(DBL_MIN)
#error DBL_MIN
#endif
#if !defined(LDBL_MIN)
#error LDBL_MIN
#endif
#if !defined(FLT_EPSILON)
#error FLT_EPSILON
#endif
#if !defined(DBL_EPSILON)
#error DBL_EPSILON
#endif
#if !defined(LDBL_EPSILON)
#error LDBL_EPSILON
#endif
#if !defined(FLT_DIG)
#error FLT_DIG
#endif
#if !defined(DBL_DIG)
#error DBL_DIG
#endif
#if !defined(LDBL_DIG)
#error LDBL_DIG
#endif
#if !defined(FLT_MANT_DIG)
#error FLT_MANT_DIG
#endif
#if !defined(DBL_MANT_DIG)
#error DBL_MANT_DIG
#endif
#if !defined(LDBL_MANT_DIG)
#error LDBL_MANT_DIG
#endif
#if !defined(FLT_MIN_EXP)
#error FLT_MIN_EXP
#endif
#if !defined(DBL_MIN_EXP)
#error DBL_MIN_EXP
#endif
#if !defined(LDBL_MIN_EXP)
#error LDBL_MIN_EXP
#endif
#if !defined(FLT_MIN_10_EXP)
#error FLT_MIN_10_EXP
#endif
#if !defined(DBL_MIN_10_EXP)
#error DBL_MIN_10_EXP
#endif
#if !defined(LDBL_MIN_10_EXP)
#error LDBL_MIN_10_EXP
#endif
#if !defined(FLT_MAX_EXP)
#error FLT_MAX_EXP
#endif
#if !defined(DBL_MAX_EXP)
#error DBL_MAX_EXP
#endif
#if !defined(LDBL_MAX_EXP)
#error LDBL_MAX_EXP
#endif
#if !defined(FLT_MAX_10_EXP)
#error FLT_MAX_10_EXP
#endif
#if !defined(DBL_MAX_10_EXP)
#error DBL_MAX_10_EXP
#endif
#if !defined(LDBL_MAX_10_EXP)
#error LDBL_MAX_10_EXP
#endif
#if !defined(FLT_ROUNDS)
#error FLT_ROUNDS
#endif
#if !defined(FLT_EVAL_METHOD)
#error FLT_EVAL_METHOD
#endif
#if !defined(FLT_HAS_SUBNORM)
#error FLT_HAS_SUBNORM
#endif
#if !defined(DBL_HAS_SUBNORM)
#error DBL_HAS_SUBNORM
#endif
#if !defined(LDBL_HAS_SUBNORM)
#error LDBL_HAS_SUBNORM
#endif
}
