/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <thread>

// Specify the LE access model explicitly. This file is compiled into the
// bionic-unit-tests executable, but the compiler sees an -fpic object file
// output into a static library, so it defaults to dynamic TLS accesses.

// This variable will be zero-initialized (.tbss)
__attribute__((tls_model("local-exec"))) static __thread int tlsvar_le_zero;

// This variable will have an initializer (.tdata)
__attribute__((tls_model("local-exec"))) static __thread int tlsvar_le_init = 10;

// Access libtest_elftls_shared_var's TLS variable using an IE access.
__attribute__((tls_model("initial-exec"))) extern "C" __thread int elftls_shared_var;

TEST(elftls, basic_le) {
  // Check the variables on the main thread.
  ASSERT_EQ(11, ++tlsvar_le_init);
  ASSERT_EQ(1, ++tlsvar_le_zero);

  // Check variables on a new thread.
  std::thread([] {
    ASSERT_EQ(11, ++tlsvar_le_init);
    ASSERT_EQ(1, ++tlsvar_le_zero);
  }).join();
}

TEST(elftls, shared_ie) {
  ASSERT_EQ(21, ++elftls_shared_var);
  std::thread([] {
    ASSERT_EQ(21, ++elftls_shared_var);
  }).join();
}

extern "C" int bump_static_tls_var_1();
extern "C" int bump_static_tls_var_2();

TEST(elftls, tprel_addend) {
  ASSERT_EQ(4, bump_static_tls_var_1());
  ASSERT_EQ(8, bump_static_tls_var_2());
  std::thread([] {
    ASSERT_EQ(4, bump_static_tls_var_1());
    ASSERT_EQ(8, bump_static_tls_var_2());
  }).join();
}

// Because this C++ source file is built with -fpic, the compiler will access
// this variable using a GD model. Typically, the static linker will relax the
// GD to LE, but the arm32 linker doesn't do TLS relaxations, so we can test
// calling __tls_get_addr in a static executable. The static linker knows that
// the main executable's TlsIndex::module_id is 1 and writes that into the GOT.
__thread int tlsvar_general = 30;

TEST(elftls, general) {
  ASSERT_EQ(31, ++tlsvar_general);
  std::thread([] {
    ASSERT_EQ(31, ++tlsvar_general);
  }).join();
}
