/*
 * Copyright (C) 2008 The Android Open Source Project
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

#pragma once

#include <dlfcn.h>
#include <android/dlext.h>
#include <elf.h>
#include <inttypes.h>
#include <link.h>
#include <sys/stat.h>
#include <unistd.h>

#include "platform/bionic/page.h"
#include "linked_list.h"
#include "linker_common_types.h"
#include "linker_logger.h"
#include "linker_soinfo.h"

#include <string>
#include <vector>

#if defined(__LP64__)
#define ELFW(what) ELF64_ ## what
#else
#define ELFW(what) ELF32_ ## what
#endif

#define SUPPORTED_DT_FLAGS_1 (DF_1_NOW | DF_1_GLOBAL | DF_1_NODELETE | DF_1_PIE | DF_1_ORIGIN)

// Class used construct version dependency graph.
class VersionTracker {
 public:
  VersionTracker() = default;
  bool init(const soinfo* si_from);

  const version_info* get_version_info(ElfW(Versym) source_symver) const;
 private:
  bool init_verneed(const soinfo* si_from);
  bool init_verdef(const soinfo* si_from);
  void add_version_info(size_t source_index, ElfW(Word) elf_hash,
      const char* ver_name, const soinfo* target_si);

  std::vector<version_info> version_infos;

  DISALLOW_COPY_AND_ASSIGN(VersionTracker);
};

static constexpr const char* kBionicChangesUrl =
    "https://android.googlesource.com/platform/bionic/+/main/"
    "android-changes-for-ndk-developers.md";

soinfo* get_libdl_info(const soinfo& linker_si);

soinfo* find_containing_library(const void* p);

int open_executable(const char* path, off64_t* file_offset, std::string* realpath);

void do_android_get_LD_LIBRARY_PATH(char*, size_t);
void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path);
void* do_dlopen(const char* name,
                int flags,
                const android_dlextinfo* extinfo,
                const void* caller_addr);

int do_dlclose(void* handle);

int do_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data);

#if defined(__arm__)
_Unwind_Ptr do_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount);
#endif

bool do_dlsym(void* handle, const char* sym_name,
              const char* sym_ver,
              const void* caller_addr,
              void** symbol);

int do_dladdr(const void* addr, Dl_info* info);

void set_application_target_sdk_version(int target);
int get_application_target_sdk_version();

bool get_transparent_hugepages_supported();

enum {
  /* A regular namespace is the namespace with a custom search path that does
   * not impose any restrictions on the location of native libraries.
   */
  ANDROID_NAMESPACE_TYPE_REGULAR = 0,

  /* An isolated namespace requires all the libraries to be on the search path
   * or under permitted_when_isolated_path. The search path is the union of
   * ld_library_path and default_library_path.
   */
  ANDROID_NAMESPACE_TYPE_ISOLATED = 1,

  /* The shared namespace clones the list of libraries of the caller namespace upon creation
   * which means that they are shared between namespaces - the caller namespace and the new one
   * will use the same copy of a library if it was loaded prior to android_create_namespace call.
   *
   * Note that libraries loaded after the namespace is created will not be shared.
   *
   * Shared namespaces can be isolated or regular. Note that they do not inherit the search path nor
   * permitted_path from the caller's namespace.
   */
  ANDROID_NAMESPACE_TYPE_SHARED = 2,

  /* This flag instructs linker to enable exempt-list workaround for the namespace.
   * See http://b/26394120 for details.
   */
  ANDROID_NAMESPACE_TYPE_EXEMPT_LIST_ENABLED = 0x08000000,

  /* This flag instructs linker to use this namespace as the anonymous
   * namespace. There can be only one anonymous namespace in a process. If there
   * already an anonymous namespace in the process, using this flag when
   * creating a new namespace causes an error
   */
  ANDROID_NAMESPACE_TYPE_ALSO_USED_AS_ANONYMOUS = 0x10000000,

  ANDROID_NAMESPACE_TYPE_SHARED_ISOLATED = ANDROID_NAMESPACE_TYPE_SHARED |
                                           ANDROID_NAMESPACE_TYPE_ISOLATED,
};

bool init_anonymous_namespace(const char* shared_lib_sonames, const char* library_search_path);
android_namespace_t* create_namespace(const void* caller_addr,
                                      const char* name,
                                      const char* ld_library_path,
                                      const char* default_library_path,
                                      uint64_t type,
                                      const char* permitted_when_isolated_path,
                                      android_namespace_t* parent_namespace);

bool link_namespaces(android_namespace_t* namespace_from,
                     android_namespace_t* namespace_to,
                     const char* shared_lib_sonames);

bool link_namespaces_all_libs(android_namespace_t* namespace_from,
                              android_namespace_t* namespace_to);

android_namespace_t* get_exported_namespace(const char* name);

void increment_dso_handle_reference_counter(void* dso_handle);
void decrement_dso_handle_reference_counter(void* dso_handle);

void purge_unused_memory();

struct address_space_params {
  void* start_addr = nullptr;
  size_t reserved_size = 0;
  bool must_use_address = false;
};

int get_application_target_sdk_version();
ElfW(Versym) find_verdef_version_index(const soinfo* si, const version_info* vi);
bool validate_verdef_section(const soinfo* si);

struct platform_properties {
#if defined(__aarch64__)
  bool bti_supported = false;
#endif
};
