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

#include <dlfcn.h>
#include <gtest/gtest.h>

#include <thread>

#include "gtest_globals.h"
#include "utils.h"

// Access libtest_elftls_shared_var.so's TLS variable using an IE access.
__attribute__((tls_model("initial-exec"))) extern "C" __thread int elftls_shared_var;

TEST(elftls_dl, dlopen_shared_var_ie) {
  // libtest_elftls_shared_var_ie.so can be dlopen'ed, even though it contains a
  // TLS IE access, because its IE access references a TLS variable from
  // libtest_elftls_shared_var.so, which is DT_NEEDED by the executable. This
  // pattern appears in sanitizers, which use TLS IE instrumentation in shared
  // objects to access special variables exported from the executable or from a
  // preloaded solib.
  void* lib = dlopen("libtest_elftls_shared_var_ie.so", RTLD_LOCAL | RTLD_NOW);
  ASSERT_NE(nullptr, lib);

  auto bump_shared_var = reinterpret_cast<int(*)()>(dlsym(lib, "bump_shared_var"));
  ASSERT_NE(nullptr, bump_shared_var);

  ASSERT_EQ(21, ++elftls_shared_var);
  ASSERT_EQ(22, bump_shared_var());

  std::thread([bump_shared_var] {
    ASSERT_EQ(21, ++elftls_shared_var);
    ASSERT_EQ(22, bump_shared_var());
  }).join();
}

TEST(elftls_dl, dlopen_ie_error) {
  std::string helper = GetTestlibRoot() +
      "/elftls_dlopen_ie_error_helper/elftls_dlopen_ie_error_helper";
  std::string src_path = GetTestlibRoot() + "/libtest_elftls_shared_var_ie.so";
  std::string dst_path = GetTestlibRoot() + "/libtest_elftls_shared_var.so";
#if defined(__BIONIC__)
  std::string error =
      "dlerror: dlopen failed: TLS symbol \"elftls_shared_var\" in dlopened \"" + dst_path + "\" " +
      "referenced from \"" + src_path + "\" using IE access model\n";
#else
  // glibc will reserve some surplus static TLS memory, allowing this test to pass.
  std::string error = "success\n";
#endif

  chmod(helper.c_str(), 0755); // TODO: "x" lost in CTS, b/34945607
  ExecTestHelper eth;
  eth.SetArgs({ helper.c_str(), nullptr });
  eth.Run([&]() { execve(helper.c_str(), eth.GetArgs(), eth.GetEnv()); }, 0, error.c_str());
}
