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

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <gtest/gtest.h>

#include "../linker_config.h"

#include <unistd.h>

#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>
#include <android-base/file.h>
#include <android-base/test_utils.h>


static const char* config_str =
  "# comment \n"
  "dir.test = /data/local/tmp\n"
  "\n"
  "[test]\n"
  "\n"
  "enable.target.sdk.version = true\n"
  "additional.namespaces=system\n"
  "namespace.default.isolated = true\n"
  "namespace.default.search.paths = /vendor/${LIB}\n"
  "namespace.default.permitted.paths = /vendor/${LIB}\n"
  "namespace.default.asan.search.paths = /data:/vendor/${LIB}\n"
  "namespace.default.asan.permitted.paths = /data:/vendor\n"
  "namespace.default.links = system\n"
  "namespace.default.link.system.shared_libs = libc.so:libm.so:libdl.so:libstdc++.so\n"
  "namespace.system.isolated = true\n"
  "namespace.system.search.paths = /system/${LIB}\n"
  "namespace.system.permitted.paths = /system/${LIB}\n"
  "namespace.system.asan.search.paths = /data:/system/${LIB}\n"
  "namespace.system.asan.permitted.paths = /data:/system\n"
  "\n";

static bool write_version(const std::string& path, uint32_t version) {
  std::string content = android::base::StringPrintf("%d", version);
  return android::base::WriteStringToFile(content, path);
}

static void run_linker_config_smoke_test(bool is_asan) {
#if defined(__LP64__)
  const std::vector<std::string> kExpectedDefaultSearchPath = is_asan ?
        std::vector<std::string>({ "/data", "/vendor/lib64"}) :
        std::vector<std::string>({ "/vendor/lib64" });

  const std::vector<std::string> kExpectedDefaultPermittedPath = is_asan ?
        std::vector<std::string>({ "/data", "/vendor" }) :
        std::vector<std::string>({ "/vendor/lib64" });

  const std::vector<std::string> kExpectedSystemSearchPath = is_asan ?
        std::vector<std::string>({ "/data", "/system/lib64" }) :
        std::vector<std::string>({ "/system/lib64" });

  const std::vector<std::string> kExpectedSystemPermittedPath = is_asan ?
        std::vector<std::string>({ "/data", "/system" }) :
        std::vector<std::string>({ "/system/lib64" });
#else
  const std::vector<std::string> kExpectedDefaultSearchPath = is_asan ?
        std::vector<std::string>({ "/data", "/vendor/lib"}) :
        std::vector<std::string>({ "/vendor/lib" });

  const std::vector<std::string> kExpectedDefaultPermittedPath = is_asan ?
        std::vector<std::string>({ "/data", "/vendor" }) :
        std::vector<std::string>({ "/vendor/lib" });

  const std::vector<std::string> kExpectedSystemSearchPath = is_asan ?
        std::vector<std::string>({ "/data", "/system/lib" }) :
        std::vector<std::string>({ "/system/lib" });

  const std::vector<std::string> kExpectedSystemPermittedPath = is_asan ?
        std::vector<std::string>({ "/data", "/system" }) :
        std::vector<std::string>({ "/system/lib" });
#endif

  TemporaryFile tmp_file;
  close(tmp_file.fd);
  tmp_file.fd = -1;

  android::base::WriteStringToFile(config_str, tmp_file.path);

  TemporaryDir tmp_dir;

  std::string executable_path = std::string(tmp_dir.path) + "/some-binary";
  std::string version_file = std::string(tmp_dir.path) + "/.version";

  auto file_guard =
      android::base::make_scope_guard([&version_file] { unlink(version_file.c_str()); });

  ASSERT_TRUE(write_version(version_file, 113U)) << strerror(errno);

  // read config
  const Config* config = nullptr;
  std::string error_msg;
  ASSERT_TRUE(Config::read_binary_config(tmp_file.path,
                                         executable_path.c_str(),
                                         is_asan,
                                         &config,
                                         &error_msg)) << error_msg;
  ASSERT_TRUE(config != nullptr);
  ASSERT_TRUE(error_msg.empty());

  ASSERT_EQ(113U, config->target_sdk_version());

  const NamespaceConfig* default_ns_config = config->default_namespace_config();
  ASSERT_TRUE(default_ns_config != nullptr);

  ASSERT_TRUE(default_ns_config->isolated());
  ASSERT_EQ(kExpectedDefaultSearchPath, default_ns_config->search_paths());
  ASSERT_EQ(kExpectedDefaultPermittedPath, default_ns_config->permitted_paths());

  const auto& default_ns_links = default_ns_config->links();
  ASSERT_EQ(1U, default_ns_links.size());
  ASSERT_EQ("system", default_ns_links[0].ns_name());
  ASSERT_EQ("libc.so:libm.so:libdl.so:libstdc++.so", default_ns_links[0].shared_libs());

  auto& ns_configs = config->namespace_configs();
  ASSERT_EQ(2U, ns_configs.size());

  // find second namespace
  const NamespaceConfig* ns_system = nullptr;
  for (auto& ns : ns_configs) {
    std::string ns_name = ns->name();
    ASSERT_TRUE(ns_name == "system" || ns_name == "default")
        << "unexpected ns name: " << ns->name();

    if (ns_name == "system") {
      ns_system = ns.get();
    }
  }

  ASSERT_TRUE(ns_system != nullptr) << "system namespace was not found";

  ASSERT_TRUE(ns_system->isolated());
  ASSERT_EQ(kExpectedSystemSearchPath, ns_system->search_paths());
  ASSERT_EQ(kExpectedSystemPermittedPath, ns_system->permitted_paths());
}

TEST(linker_config, smoke) {
  run_linker_config_smoke_test(false);
}

TEST(linker_config, asan_smoke) {
  run_linker_config_smoke_test(true);
}
