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

#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#if __has_include(<sys/auxv.h>)
#include <sys/auxv.h>
#endif
#include <unistd.h>

extern int get_value_from_lib();

int main() {
  bool skip_vdso_check = false;
#if __has_include(<sys/auxv.h>)
  if (getauxval(AT_SYSINFO_EHDR) == 0) {
    skip_vdso_check = true;
  }
#endif

  if (!skip_vdso_check) {
    const char* vdso_name = "linux-vdso.so.1";
#if defined(__i386__)
    vdso_name = "linux-gate.so.1";
#endif
    void* handle = dlopen(vdso_name, RTLD_NOW);
    if (handle == nullptr) {
      printf("%s", dlerror());
      return 1;
    }
    dlclose(handle);
  }

  printf("%d", get_value_from_lib());
  return 0;
}
