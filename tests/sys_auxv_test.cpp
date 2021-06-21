/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <sys/auxv.h>

TEST(sys_auxv, getauxval_HWCAP) {
  __attribute__((__unused__)) unsigned long hwcap = getauxval(AT_HWCAP);

  // Check that the constants for *using* AT_HWCAP are also available.
#if defined(__arm__)
  ASSERT_NE(0, HWCAP_THUMB);
#elif defined(__aarch64__)
  ASSERT_NE(0, HWCAP_FP);
#endif
}

TEST(sys_auxv, getauxval_HWCAP2) {
#if defined(AT_HWCAP2)
  __attribute__((__unused__)) unsigned long hwcap = getauxval(AT_HWCAP2);

  // Check that the constants for *using* AT_HWCAP2 are also available.
#if defined(__arm__)
  ASSERT_NE(0, HWCAP2_AES);
#elif defined(__aarch64__)
  ASSERT_NE(0, HWCAP2_SVE2);
#endif
#else
  GTEST_SKIP() << "No AT_HWCAP2 for this architecture.";
#endif
}
