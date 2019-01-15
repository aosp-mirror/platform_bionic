/*
 * Copyright (C) 2007 The Android Open Source Project
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
#include <link.h>
#include <stdlib.h>
#include <android/dlext.h>

// These functions are exported by the loader
// TODO(dimitry): replace these with reference to libc.so

extern "C" {

__attribute__((__weak__, visibility("default")))
void* __loader_dlopen(const char* filename, int flags, const void* caller_addr);

__attribute__((__weak__, visibility("default")))
char* __loader_dlerror();

__attribute__((__weak__, visibility("default")))
void* __loader_dlsym(void* handle, const char* symbol, const void* caller_addr);

__attribute__((__weak__, visibility("default")))
void* __loader_dlvsym(void* handle,
                      const char* symbol,
                      const char* version,
                      const void* caller_addr);

__attribute__((__weak__, visibility("default")))
int __loader_dladdr(const void* addr, Dl_info* info);

__attribute__((__weak__, visibility("default")))
int __loader_dlclose(void* handle);

#if defined(__arm__)
__attribute__((__weak__, visibility("default")))
_Unwind_Ptr __loader_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount);
#endif

__attribute__((__weak__, visibility("default")))
int __loader_dl_iterate_phdr(int (*cb)(struct dl_phdr_info* info, size_t size, void* data),
                             void* data);

__attribute__((__weak__, visibility("default")))
void __loader_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size);

__attribute__((__weak__, visibility("default")))
void __loader_android_update_LD_LIBRARY_PATH(const char* ld_library_path);

__attribute__((__weak__, visibility("default")))
void* __loader_android_dlopen_ext(const char* filename,
                                  int flag,
                                  const android_dlextinfo* extinfo,
                                  const void* caller_addr);

__attribute__((__weak__, visibility("default")))
void __loader_android_set_application_target_sdk_version(int target);

__attribute__((__weak__, visibility("default")))
int __loader_android_get_application_target_sdk_version();

__attribute__((__weak__, visibility("default")))
bool __loader_android_init_anonymous_namespace(const char* shared_libs_sonames,
                                               const char* library_search_path);

__attribute__((__weak__, visibility("default")))
struct android_namespace_t* __loader_android_create_namespace(
                                const char* name,
                                const char* ld_library_path,
                                const char* default_library_path,
                                uint64_t type,
                                const char* permitted_when_isolated_path,
                                struct android_namespace_t* parent,
                                const void* caller_addr);

__attribute__((__weak__, visibility("default")))
bool __loader_android_link_namespaces(
                                struct android_namespace_t* namespace_from,
                                struct android_namespace_t* namespace_to,
                                const char* shared_libs_sonames);

__attribute__((__weak__, visibility("default")))
void __loader_android_dlwarning(void* obj, void (*f)(void*, const char*));

__attribute__((__weak__, visibility("default")))
struct android_namespace_t* __loader_android_get_exported_namespace(const char* name);

// Proxy calls to bionic loader
__attribute__((__weak__))
void* dlopen(const char* filename, int flag) {
  const void* caller_addr = __builtin_return_address(0);
  return __loader_dlopen(filename, flag, caller_addr);
}

__attribute__((__weak__))
char* dlerror() {
  return __loader_dlerror();
}

__attribute__((__weak__))
void* dlsym(void* handle, const char* symbol) {
  const void* caller_addr = __builtin_return_address(0);
  return __loader_dlsym(handle, symbol, caller_addr);
}

__attribute__((__weak__))
void* dlvsym(void* handle, const char* symbol, const char* version) {
  const void* caller_addr = __builtin_return_address(0);
  return __loader_dlvsym(handle, symbol, version, caller_addr);
}

__attribute__((__weak__))
int dladdr(const void* addr, Dl_info* info) {
  return __loader_dladdr(addr, info);
}

__attribute__((__weak__))
int dlclose(void* handle) {
  return __loader_dlclose(handle);
}

#if defined(__arm__)
__attribute__((__weak__))
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
  return __loader_dl_unwind_find_exidx(pc, pcount);
}
#endif

/*
 * This needs to be defined as weak because it is also defined in libc.a.
 * Without this, static executables will have a multiple definition error.
 */
__attribute__((__weak__))
int dl_iterate_phdr(int (*cb)(struct dl_phdr_info* info, size_t size, void* data), void* data) {
  return __loader_dl_iterate_phdr(cb, data);
}

__attribute__((__weak__))
void android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  __loader_android_get_LD_LIBRARY_PATH(buffer, buffer_size);
}

__attribute__((__weak__))
void android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  __loader_android_update_LD_LIBRARY_PATH(ld_library_path);
}

__attribute__((__weak__))
void* android_dlopen_ext(const char* filename, int flag, const android_dlextinfo* extinfo) {
  const void* caller_addr = __builtin_return_address(0);
  return __loader_android_dlopen_ext(filename, flag, extinfo, caller_addr);
}

__attribute__((__weak__))
void android_set_application_target_sdk_version(int target) {
  __loader_android_set_application_target_sdk_version(target);
}

__attribute__((__weak__))
int android_get_application_target_sdk_version() {
  return __loader_android_get_application_target_sdk_version();
}

__attribute__((__weak__))
bool android_init_anonymous_namespace(const char* shared_libs_sonames,
                                      const char* library_search_path) {
  return __loader_android_init_anonymous_namespace(shared_libs_sonames, library_search_path);
}

__attribute__((__weak__))
struct android_namespace_t* android_create_namespace(const char* name,
                                                     const char* ld_library_path,
                                                     const char* default_library_path,
                                                     uint64_t type,
                                                     const char* permitted_when_isolated_path,
                                                     struct android_namespace_t* parent) {
  const void* caller_addr = __builtin_return_address(0);
  return __loader_android_create_namespace(name,
                                           ld_library_path,
                                           default_library_path,
                                           type,
                                           permitted_when_isolated_path,
                                           parent,
                                           caller_addr);
}

__attribute__((__weak__))
bool android_link_namespaces(struct android_namespace_t* namespace_from,
                             struct android_namespace_t* namespace_to,
                             const char* shared_libs_sonames) {
  return __loader_android_link_namespaces(namespace_from, namespace_to, shared_libs_sonames);
}

__attribute__((__weak__))
void android_dlwarning(void* obj, void (*f)(void*, const char*)) {
  __loader_android_dlwarning(obj, f);
}

__attribute__((__weak__))
struct android_namespace_t* android_get_exported_namespace(const char* name) {
  return __loader_android_get_exported_namespace(name);
}

#if defined(__arm__)
// An arm32 unwinding table has an R_ARM_NONE relocation to
// __aeabi_unwind_cpp_pr0. This shared library will never invoke the unwinder,
// so it doesn't actually need the routine. Define a dummy version here,
// because the real version calls libc functions (e.g. memcpy, abort), which
// would create a dependency cycle with libc.so.
__attribute__((visibility("hidden")))
void __aeabi_unwind_cpp_pr0() {
  __builtin_trap();
}
#endif

} // extern "C"
