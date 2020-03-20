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

#include <android/dlext.h>
#include <dlfcn.h>
#include <stdlib.h>

#include <string>

#include "../core_shared_libs.h"
#include "../dlext_private.h"

extern "C" void global_function();
extern "C" void internal_function();

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s NS_PATH\n", argv[0]);
    fprintf(stderr, "NS_PATH   path to the ns_hidden_child_app directory\n");
    exit(1);
  }

  // Ensure that -Wl,--needed doesn't break the test by removing DT_NEEDED entries.
  global_function();
  internal_function();

  const char* app_lib_dir = argv[1];
  android_namespace_t* app_ns =
      android_create_namespace("app", nullptr, app_lib_dir, ANDROID_NAMESPACE_TYPE_ISOLATED,
                               nullptr, nullptr);
  if (app_ns == nullptr) {
    fprintf(stderr, "android_create_namespace failed: %s\n", dlerror());
    exit(1);
  }

  std::string public_libs = std::string(kCoreSharedLibs) + ":libns_hidden_child_public.so";
  if (!android_link_namespaces(app_ns, nullptr, public_libs.c_str())) {
    fprintf(stderr, "android_link_namespaces failed: %s\n", dlerror());
    exit(1);
  }

  android_dlextinfo ext = {
    .flags = ANDROID_DLEXT_USE_NAMESPACE,
    .library_namespace = app_ns,
  };
  void* app_lib = android_dlopen_ext("libns_hidden_child_app.so", RTLD_NOW | RTLD_LOCAL, &ext);
  if (app_lib == nullptr) {
    fprintf(stderr, "android_dlopen_ext failed: %s\n", dlerror());
    exit(1);
  }

  auto app_function = reinterpret_cast<void(*)()>(dlsym(app_lib, "app_function"));
  if (app_function == nullptr) {
    fprintf(stderr, "dlsym failed to find app_function: %s\n", dlerror());
    exit(1);
  }

  app_function();
  return 0;
}
