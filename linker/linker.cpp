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

#include <android/api-level.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/vfs.h>
#include <unistd.h>

#include <new>
#include <string>
#include <unordered_map>
#include <vector>

#include <android-base/properties.h>
#include <android-base/scopeguard.h>
#include <async_safe/log.h>
#include <bionic/pthread_internal.h>

// Private C library headers.

#include "linker.h"
#include "linker_block_allocator.h"
#include "linker_cfi.h"
#include "linker_config.h"
#include "linker_gdb_support.h"
#include "linker_globals.h"
#include "linker_debug.h"
#include "linker_dlwarning.h"
#include "linker_main.h"
#include "linker_namespaces.h"
#include "linker_sleb128.h"
#include "linker_phdr.h"
#include "linker_relocate.h"
#include "linker_tls.h"
#include "linker_translate_path.h"
#include "linker_utils.h"

#include "private/bionic_call_ifunc_resolver.h"
#include "private/bionic_globals.h"
#include "android-base/macros.h"
#include "android-base/strings.h"
#include "android-base/stringprintf.h"
#include "ziparchive/zip_archive.h"

static std::unordered_map<void*, size_t> g_dso_handle_counters;

static bool g_anonymous_namespace_set = false;
static android_namespace_t* g_anonymous_namespace = &g_default_namespace;
static std::unordered_map<std::string, android_namespace_t*> g_exported_namespaces;

static LinkerTypeAllocator<soinfo> g_soinfo_allocator;
static LinkerTypeAllocator<LinkedListEntry<soinfo>> g_soinfo_links_allocator;

static LinkerTypeAllocator<android_namespace_t> g_namespace_allocator;
static LinkerTypeAllocator<LinkedListEntry<android_namespace_t>> g_namespace_list_allocator;

static uint64_t g_module_load_counter = 0;
static uint64_t g_module_unload_counter = 0;

static const char* const kLdConfigArchFilePath = "/system/etc/ld.config." ABI_STRING ".txt";

static const char* const kLdConfigFilePath = "/system/etc/ld.config.txt";
static const char* const kLdConfigVndkLiteFilePath = "/system/etc/ld.config.vndk_lite.txt";

static const char* const kLdGeneratedConfigFilePath = "/linkerconfig/ld.config.txt";

#if defined(__LP64__)
static const char* const kSystemLibDir        = "/system/lib64";
static const char* const kOdmLibDir           = "/odm/lib64";
static const char* const kVendorLibDir        = "/vendor/lib64";
static const char* const kAsanSystemLibDir    = "/data/asan/system/lib64";
static const char* const kAsanOdmLibDir       = "/data/asan/odm/lib64";
static const char* const kAsanVendorLibDir    = "/data/asan/vendor/lib64";
#else
static const char* const kSystemLibDir        = "/system/lib";
static const char* const kOdmLibDir           = "/odm/lib";
static const char* const kVendorLibDir        = "/vendor/lib";
static const char* const kAsanSystemLibDir    = "/data/asan/system/lib";
static const char* const kAsanOdmLibDir       = "/data/asan/odm/lib";
static const char* const kAsanVendorLibDir    = "/data/asan/vendor/lib";
#endif

static const char* const kAsanLibDirPrefix = "/data/asan";

static const char* const kDefaultLdPaths[] = {
  kSystemLibDir,
  kOdmLibDir,
  kVendorLibDir,
  nullptr
};

static const char* const kAsanDefaultLdPaths[] = {
  kAsanSystemLibDir,
  kSystemLibDir,
  kAsanOdmLibDir,
  kOdmLibDir,
  kAsanVendorLibDir,
  kVendorLibDir,
  nullptr
};

// Is ASAN enabled?
static bool g_is_asan = false;

static CFIShadowWriter g_cfi_shadow;

CFIShadowWriter* get_cfi_shadow() {
  return &g_cfi_shadow;
}

static bool is_system_library(const std::string& realpath) {
  for (const auto& dir : g_default_namespace.get_default_library_paths()) {
    if (file_is_in_dir(realpath, dir)) {
      return true;
    }
  }
  return false;
}

// Checks if the file exists and not a directory.
static bool file_exists(const char* path) {
  struct stat s;

  if (stat(path, &s) != 0) {
    return false;
  }

  return S_ISREG(s.st_mode);
}

static std::string resolve_soname(const std::string& name) {
  // We assume that soname equals to basename here

  // TODO(dimitry): consider having honest absolute-path -> soname resolution
  // note that since we might end up refusing to load this library because
  // it is not in shared libs list we need to get the soname without actually loading
  // the library.
  //
  // On the other hand there are several places where we already assume that
  // soname == basename in particular for any not-loaded library mentioned
  // in DT_NEEDED list.
  return basename(name.c_str());
}

static bool maybe_accessible_via_namespace_links(android_namespace_t* ns, const char* name) {
  std::string soname = resolve_soname(name);
  for (auto& ns_link : ns->linked_namespaces()) {
    if (ns_link.is_accessible(soname.c_str())) {
      return true;
    }
  }

  return false;
}

// TODO(dimitry): The exempt-list is a workaround for http://b/26394120 ---
// gradually remove libraries from this list until it is gone.
static bool is_exempt_lib(android_namespace_t* ns, const char* name, const soinfo* needed_by) {
  static const char* const kLibraryExemptList[] = {
    "libandroid_runtime.so",
    "libbinder.so",
    "libcrypto.so",
    "libcutils.so",
    "libexpat.so",
    "libgui.so",
    "libmedia.so",
    "libnativehelper.so",
    "libssl.so",
    "libstagefright.so",
    "libsqlite.so",
    "libui.so",
    "libutils.so",
    nullptr
  };

  // If you're targeting N, you don't get the exempt-list.
  if (get_application_target_sdk_version() >= 24) {
    return false;
  }

  // if the library needed by a system library - implicitly assume it
  // is exempt unless it is in the list of shared libraries for one or
  // more linked namespaces
  if (needed_by != nullptr && is_system_library(needed_by->get_realpath())) {
    return !maybe_accessible_via_namespace_links(ns, name);
  }

  // if this is an absolute path - make sure it points to /system/lib(64)
  if (name[0] == '/' && dirname(name) == kSystemLibDir) {
    // and reduce the path to basename
    name = basename(name);
  }

  for (size_t i = 0; kLibraryExemptList[i] != nullptr; ++i) {
    if (strcmp(name, kLibraryExemptList[i]) == 0) {
      return true;
    }
  }

  return false;
}
// END OF WORKAROUND

static std::vector<std::string> g_ld_preload_names;

static void notify_gdb_of_load(soinfo* info) {
  if (info->is_linker() || info->is_main_executable()) {
    // gdb already knows about the linker and the main executable.
    return;
  }

  link_map* map = &(info->link_map_head);

  map->l_addr = info->load_bias;
  // link_map l_name field is not const.
  map->l_name = const_cast<char*>(info->get_realpath());
  map->l_ld = info->dynamic;

  CHECK(map->l_name != nullptr);
  CHECK(map->l_name[0] != '\0');

  notify_gdb_of_load(map);
}

static void notify_gdb_of_unload(soinfo* info) {
  notify_gdb_of_unload(&(info->link_map_head));
}

LinkedListEntry<soinfo>* SoinfoListAllocator::alloc() {
  return g_soinfo_links_allocator.alloc();
}

void SoinfoListAllocator::free(LinkedListEntry<soinfo>* entry) {
  g_soinfo_links_allocator.free(entry);
}

LinkedListEntry<android_namespace_t>* NamespaceListAllocator::alloc() {
  return g_namespace_list_allocator.alloc();
}

void NamespaceListAllocator::free(LinkedListEntry<android_namespace_t>* entry) {
  g_namespace_list_allocator.free(entry);
}

soinfo* soinfo_alloc(android_namespace_t* ns, const char* name,
                     const struct stat* file_stat, off64_t file_offset,
                     uint32_t rtld_flags) {
  if (strlen(name) >= PATH_MAX) {
    async_safe_fatal("library name \"%s\" too long", name);
  }

  TRACE("name %s: allocating soinfo for ns=%p", name, ns);

  soinfo* si = new (g_soinfo_allocator.alloc()) soinfo(ns, name, file_stat,
                                                       file_offset, rtld_flags);

  solist_add_soinfo(si);

  si->generate_handle();
  ns->add_soinfo(si);

  TRACE("name %s: allocated soinfo @ %p", name, si);
  return si;
}

static void soinfo_free(soinfo* si) {
  if (si == nullptr) {
    return;
  }

  if (si->base != 0 && si->size != 0) {
    if (!si->is_mapped_by_caller()) {
      munmap(reinterpret_cast<void*>(si->base), si->size);
    } else {
      // remap the region as PROT_NONE, MAP_ANONYMOUS | MAP_NORESERVE
      mmap(reinterpret_cast<void*>(si->base), si->size, PROT_NONE,
           MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    }
  }

  if (si->has_min_version(6) && si->get_gap_size()) {
    munmap(reinterpret_cast<void*>(si->get_gap_start()), si->get_gap_size());
  }

  TRACE("name %s: freeing soinfo @ %p", si->get_realpath(), si);

  if (!solist_remove_soinfo(si)) {
    async_safe_fatal("soinfo=%p is not in soinfo_list (double unload?)", si);
  }

  // clear links to/from si
  si->remove_all_links();

  si->~soinfo();
  g_soinfo_allocator.free(si);
}

static void parse_path(const char* path, const char* delimiters,
                       std::vector<std::string>* resolved_paths) {
  std::vector<std::string> paths;
  split_path(path, delimiters, &paths);
  resolve_paths(paths, resolved_paths);
}

static void parse_LD_LIBRARY_PATH(const char* path) {
  std::vector<std::string> ld_libary_paths;
  parse_path(path, ":", &ld_libary_paths);
  g_default_namespace.set_ld_library_paths(std::move(ld_libary_paths));
}

static bool realpath_fd(int fd, std::string* realpath) {
  // proc_self_fd needs to be large enough to hold "/proc/self/fd/" plus an
  // integer, plus the NULL terminator.
  char proc_self_fd[32];
  // We want to statically allocate this large buffer so that we don't grow
  // the stack by too much.
  static char buf[PATH_MAX];

  async_safe_format_buffer(proc_self_fd, sizeof(proc_self_fd), "/proc/self/fd/%d", fd);
  auto length = readlink(proc_self_fd, buf, sizeof(buf));
  if (length == -1) {
    if (!is_first_stage_init()) {
      PRINT("readlink(\"%s\") failed: %s [fd=%d]", proc_self_fd, strerror(errno), fd);
    }
    return false;
  }

  realpath->assign(buf, length);
  return true;
}

// Returns the address of the current thread's copy of a TLS module. If the current thread doesn't
// have a copy yet, allocate one on-demand if should_alloc is true, and return nullptr otherwise.
static inline void* get_tls_block_for_this_thread(const soinfo_tls* si_tls, bool should_alloc) {
  const TlsModule& tls_mod = get_tls_module(si_tls->module_id);
  if (tls_mod.static_offset != SIZE_MAX) {
    const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
    char* static_tls = reinterpret_cast<char*>(__get_bionic_tcb()) - layout.offset_bionic_tcb();
    return static_tls + tls_mod.static_offset;
  } else if (should_alloc) {
    const TlsIndex ti { si_tls->module_id, 0 };
    return TLS_GET_ADDR(&ti);
  } else {
    TlsDtv* dtv = __get_tcb_dtv(__get_bionic_tcb());
    if (dtv->generation < tls_mod.first_generation) return nullptr;
    return dtv->modules[__tls_module_id_to_idx(si_tls->module_id)];
  }
}

#if defined(__arm__)

// For a given PC, find the .so that it belongs to.
// Returns the base address of the .ARM.exidx section
// for that .so, and the number of 8-byte entries
// in that section (via *pcount).
//
// Intended to be called by libc's __gnu_Unwind_Find_exidx().
_Unwind_Ptr do_dl_unwind_find_exidx(_Unwind_Ptr pc, int* pcount) {
  if (soinfo* si = find_containing_library(reinterpret_cast<void*>(pc))) {
    *pcount = si->ARM_exidx_count;
    return reinterpret_cast<_Unwind_Ptr>(si->ARM_exidx);
  }
  *pcount = 0;
  return 0;
}

#endif

// Here, we only have to provide a callback to iterate across all the
// loaded libraries. gcc_eh does the rest.
int do_dl_iterate_phdr(int (*cb)(dl_phdr_info* info, size_t size, void* data), void* data) {
  int rv = 0;
  for (soinfo* si = solist_get_head(); si != nullptr; si = si->next) {
    dl_phdr_info dl_info;
    dl_info.dlpi_addr = si->link_map_head.l_addr;
    dl_info.dlpi_name = si->link_map_head.l_name;
    dl_info.dlpi_phdr = si->phdr;
    dl_info.dlpi_phnum = si->phnum;
    dl_info.dlpi_adds = g_module_load_counter;
    dl_info.dlpi_subs = g_module_unload_counter;
    if (soinfo_tls* tls_module = si->get_tls()) {
      dl_info.dlpi_tls_modid = tls_module->module_id;
      dl_info.dlpi_tls_data = get_tls_block_for_this_thread(tls_module, /*should_alloc=*/false);
    } else {
      dl_info.dlpi_tls_modid = 0;
      dl_info.dlpi_tls_data = nullptr;
    }

    rv = cb(&dl_info, sizeof(dl_phdr_info), data);
    if (rv != 0) {
      break;
    }
  }
  return rv;
}

ProtectedDataGuard::ProtectedDataGuard() {
  if (ref_count_++ == 0) {
    protect_data(PROT_READ | PROT_WRITE);
  }

  if (ref_count_ == 0) { // overflow
    async_safe_fatal("Too many nested calls to dlopen()");
  }
}

ProtectedDataGuard::~ProtectedDataGuard() {
  if (--ref_count_ == 0) {
    protect_data(PROT_READ);
  }
}

void ProtectedDataGuard::protect_data(int protection) {
  g_soinfo_allocator.protect_all(protection);
  g_soinfo_links_allocator.protect_all(protection);
  g_namespace_allocator.protect_all(protection);
  g_namespace_list_allocator.protect_all(protection);
}

size_t ProtectedDataGuard::ref_count_ = 0;

// Each size has it's own allocator.
template<size_t size>
class SizeBasedAllocator {
 public:
  static void* alloc() {
    return allocator_.alloc();
  }

  static void free(void* ptr) {
    allocator_.free(ptr);
  }

  static void purge() {
    allocator_.purge();
  }

 private:
  static LinkerBlockAllocator allocator_;
};

template<size_t size>
LinkerBlockAllocator SizeBasedAllocator<size>::allocator_(size);

template<typename T>
class TypeBasedAllocator {
 public:
  static T* alloc() {
    return reinterpret_cast<T*>(SizeBasedAllocator<sizeof(T)>::alloc());
  }

  static void free(T* ptr) {
    SizeBasedAllocator<sizeof(T)>::free(ptr);
  }

  static void purge() {
    SizeBasedAllocator<sizeof(T)>::purge();
  }
};

class LoadTask {
 public:
  struct deleter_t {
    void operator()(LoadTask* t) {
      t->~LoadTask();
      TypeBasedAllocator<LoadTask>::free(t);
    }
  };

  static deleter_t deleter;

  // needed_by is NULL iff dlopen is called from memory that isn't part of any known soinfo.
  static LoadTask* create(const char* _Nonnull name, soinfo* _Nullable needed_by,
                          android_namespace_t* _Nonnull start_from,
                          std::unordered_map<const soinfo*, ElfReader>* _Nonnull readers_map) {
    LoadTask* ptr = TypeBasedAllocator<LoadTask>::alloc();
    return new (ptr) LoadTask(name, needed_by, start_from, readers_map);
  }

  const char* get_name() const {
    return name_;
  }

  soinfo* get_needed_by() const {
    return needed_by_;
  }

  soinfo* get_soinfo() const {
    return si_;
  }

  void set_soinfo(soinfo* si) {
    si_ = si;
  }

  off64_t get_file_offset() const {
    return file_offset_;
  }

  void set_file_offset(off64_t offset) {
    file_offset_ = offset;
  }

  int get_fd() const {
    return fd_;
  }

  void set_fd(int fd, bool assume_ownership) {
    if (fd_ != -1 && close_fd_) {
      close(fd_);
    }
    fd_ = fd;
    close_fd_ = assume_ownership;
  }

  const android_dlextinfo* get_extinfo() const {
    return extinfo_;
  }

  void set_extinfo(const android_dlextinfo* extinfo) {
    extinfo_ = extinfo;
  }

  bool is_dt_needed() const {
    return is_dt_needed_;
  }

  void set_dt_needed(bool is_dt_needed) {
    is_dt_needed_ = is_dt_needed;
  }

  // returns the namespace from where we need to start loading this.
  const android_namespace_t* get_start_from() const {
    return start_from_;
  }

  void remove_cached_elf_reader() {
    CHECK(si_ != nullptr);
    (*elf_readers_map_).erase(si_);
  }

  const ElfReader& get_elf_reader() const {
    CHECK(si_ != nullptr);
    return (*elf_readers_map_)[si_];
  }

  ElfReader& get_elf_reader() {
    CHECK(si_ != nullptr);
    return (*elf_readers_map_)[si_];
  }

  std::unordered_map<const soinfo*, ElfReader>* get_readers_map() {
    return elf_readers_map_;
  }

  bool read(const char* realpath, off64_t file_size) {
    ElfReader& elf_reader = get_elf_reader();
    return elf_reader.Read(realpath, fd_, file_offset_, file_size);
  }

  bool load(address_space_params* address_space) {
    ElfReader& elf_reader = get_elf_reader();
    if (!elf_reader.Load(address_space)) {
      return false;
    }

    si_->base = elf_reader.load_start();
    si_->size = elf_reader.load_size();
    si_->set_mapped_by_caller(elf_reader.is_mapped_by_caller());
    si_->load_bias = elf_reader.load_bias();
    si_->phnum = elf_reader.phdr_count();
    si_->phdr = elf_reader.loaded_phdr();
    si_->set_gap_start(elf_reader.gap_start());
    si_->set_gap_size(elf_reader.gap_size());

    return true;
  }

 private:
  LoadTask(const char* name,
           soinfo* needed_by,
           android_namespace_t* start_from,
           std::unordered_map<const soinfo*, ElfReader>* readers_map)
    : name_(name), needed_by_(needed_by), si_(nullptr),
      fd_(-1), close_fd_(false), file_offset_(0), elf_readers_map_(readers_map),
      is_dt_needed_(false), start_from_(start_from) {}

  ~LoadTask() {
    if (fd_ != -1 && close_fd_) {
      close(fd_);
    }
  }

  const char* name_;
  soinfo* needed_by_;
  soinfo* si_;
  const android_dlextinfo* extinfo_;
  int fd_;
  bool close_fd_;
  off64_t file_offset_;
  std::unordered_map<const soinfo*, ElfReader>* elf_readers_map_;
  // TODO(dimitry): needed by workaround for http://b/26394120 (the exempt-list)
  bool is_dt_needed_;
  // END OF WORKAROUND
  const android_namespace_t* const start_from_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(LoadTask);
};

LoadTask::deleter_t LoadTask::deleter;

template <typename T>
using linked_list_t = LinkedList<T, TypeBasedAllocator<LinkedListEntry<T>>>;

typedef linked_list_t<soinfo> SoinfoLinkedList;
typedef linked_list_t<const char> StringLinkedList;
typedef std::vector<LoadTask*> LoadTaskList;

enum walk_action_result_t : uint32_t {
  kWalkStop = 0,
  kWalkContinue = 1,
  kWalkSkip = 2
};

// This function walks down the tree of soinfo dependencies
// in breadth-first order and
//   * calls action(soinfo* si) for each node, and
//   * terminates walk if action returns kWalkStop
//   * skips children of the node if action
//     return kWalkSkip
//
// walk_dependencies_tree returns false if walk was terminated
// by the action and true otherwise.
template<typename F>
static bool walk_dependencies_tree(soinfo* root_soinfo, F action) {
  SoinfoLinkedList visit_list;
  SoinfoLinkedList visited;

  visit_list.push_back(root_soinfo);

  soinfo* si;
  while ((si = visit_list.pop_front()) != nullptr) {
    if (visited.contains(si)) {
      continue;
    }

    walk_action_result_t result = action(si);

    if (result == kWalkStop) {
      return false;
    }

    visited.push_back(si);

    if (result != kWalkSkip) {
      si->get_children().for_each([&](soinfo* child) {
        visit_list.push_back(child);
      });
    }
  }

  return true;
}


static const ElfW(Sym)* dlsym_handle_lookup_impl(android_namespace_t* ns,
                                                 soinfo* root,
                                                 soinfo* skip_until,
                                                 soinfo** found,
                                                 SymbolName& symbol_name,
                                                 const version_info* vi) {
  const ElfW(Sym)* result = nullptr;
  bool skip_lookup = skip_until != nullptr;

  walk_dependencies_tree(root, [&](soinfo* current_soinfo) {
    if (skip_lookup) {
      skip_lookup = current_soinfo != skip_until;
      return kWalkContinue;
    }

    if (!ns->is_accessible(current_soinfo)) {
      return kWalkSkip;
    }

    result = current_soinfo->find_symbol_by_name(symbol_name, vi);
    if (result != nullptr) {
      *found = current_soinfo;
      return kWalkStop;
    }

    return kWalkContinue;
  });

  return result;
}

/* This is used by dlsym(3) to performs a global symbol lookup. If the
   start value is null (for RTLD_DEFAULT), the search starts at the
   beginning of the global solist. Otherwise the search starts at the
   specified soinfo (for RTLD_NEXT).
 */
static const ElfW(Sym)* dlsym_linear_lookup(android_namespace_t* ns,
                                            const char* name,
                                            const version_info* vi,
                                            soinfo** found,
                                            soinfo* caller,
                                            void* handle) {
  SymbolName symbol_name(name);

  auto& soinfo_list = ns->soinfo_list();
  auto start = soinfo_list.begin();

  if (handle == RTLD_NEXT) {
    if (caller == nullptr) {
      return nullptr;
    } else {
      auto it = soinfo_list.find(caller);
      CHECK (it != soinfo_list.end());
      start = ++it;
    }
  }

  const ElfW(Sym)* s = nullptr;
  for (auto it = start, end = soinfo_list.end(); it != end; ++it) {
    soinfo* si = *it;
    // Do not skip RTLD_LOCAL libraries in dlsym(RTLD_DEFAULT, ...)
    // if the library is opened by application with target api level < M.
    // See http://b/21565766
    if ((si->get_rtld_flags() & RTLD_GLOBAL) == 0 && si->get_target_sdk_version() >= 23) {
      continue;
    }

    s = si->find_symbol_by_name(symbol_name, vi);
    if (s != nullptr) {
      *found = si;
      break;
    }
  }

  // If not found - use dlsym_handle_lookup_impl for caller's local_group
  if (s == nullptr && caller != nullptr) {
    soinfo* local_group_root = caller->get_local_group_root();

    return dlsym_handle_lookup_impl(local_group_root->get_primary_namespace(),
                                    local_group_root,
                                    (handle == RTLD_NEXT) ? caller : nullptr,
                                    found,
                                    symbol_name,
                                    vi);
  }

  if (s != nullptr) {
    TRACE_TYPE(LOOKUP, "%s s->st_value = %p, found->base = %p",
               name, reinterpret_cast<void*>(s->st_value), reinterpret_cast<void*>((*found)->base));
  }

  return s;
}

// This is used by dlsym(3).  It performs symbol lookup only within the
// specified soinfo object and its dependencies in breadth first order.
static const ElfW(Sym)* dlsym_handle_lookup(soinfo* si,
                                            soinfo** found,
                                            const char* name,
                                            const version_info* vi) {
  // According to man dlopen(3) and posix docs in the case when si is handle
  // of the main executable we need to search not only in the executable and its
  // dependencies but also in all libraries loaded with RTLD_GLOBAL.
  //
  // Since RTLD_GLOBAL is always set for the main executable and all dt_needed shared
  // libraries and they are loaded in breath-first (correct) order we can just execute
  // dlsym(RTLD_DEFAULT, ...); instead of doing two stage lookup.
  if (si == solist_get_somain()) {
    return dlsym_linear_lookup(&g_default_namespace, name, vi, found, nullptr, RTLD_DEFAULT);
  }

  SymbolName symbol_name(name);
  // note that the namespace is not the namespace associated with caller_addr
  // we use ns associated with root si intentionally here. Using caller_ns
  // causes problems when user uses dlopen_ext to open a library in the separate
  // namespace and then calls dlsym() on the handle.
  return dlsym_handle_lookup_impl(si->get_primary_namespace(), si, nullptr, found, symbol_name, vi);
}

soinfo* find_containing_library(const void* p) {
  // Addresses within a library may be tagged if they point to globals. Untag
  // them so that the bounds check succeeds.
  ElfW(Addr) address = reinterpret_cast<ElfW(Addr)>(untag_address(p));
  for (soinfo* si = solist_get_head(); si != nullptr; si = si->next) {
    if (address < si->base || address - si->base >= si->size) {
      continue;
    }
    ElfW(Addr) vaddr = address - si->load_bias;
    for (size_t i = 0; i != si->phnum; ++i) {
      const ElfW(Phdr)* phdr = &si->phdr[i];
      if (phdr->p_type != PT_LOAD) {
        continue;
      }
      if (vaddr >= phdr->p_vaddr && vaddr < phdr->p_vaddr + phdr->p_memsz) {
        return si;
      }
    }
  }
  return nullptr;
}

class ZipArchiveCache {
 public:
  ZipArchiveCache() {}
  ~ZipArchiveCache();

  bool get_or_open(const char* zip_path, ZipArchiveHandle* handle);
 private:
  DISALLOW_COPY_AND_ASSIGN(ZipArchiveCache);

  std::unordered_map<std::string, ZipArchiveHandle> cache_;
};

bool ZipArchiveCache::get_or_open(const char* zip_path, ZipArchiveHandle* handle) {
  std::string key(zip_path);

  auto it = cache_.find(key);
  if (it != cache_.end()) {
    *handle = it->second;
    return true;
  }

  int fd = TEMP_FAILURE_RETRY(open(zip_path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    return false;
  }

  if (OpenArchiveFd(fd, "", handle) != 0) {
    // invalid zip-file (?)
    CloseArchive(*handle);
    return false;
  }

  cache_[key] = *handle;
  return true;
}

ZipArchiveCache::~ZipArchiveCache() {
  for (const auto& it : cache_) {
    CloseArchive(it.second);
  }
}

static int open_library_in_zipfile(ZipArchiveCache* zip_archive_cache,
                                   const char* const input_path,
                                   off64_t* file_offset, std::string* realpath) {
  std::string normalized_path;
  if (!normalize_path(input_path, &normalized_path)) {
    return -1;
  }

  const char* const path = normalized_path.c_str();
  TRACE("Trying zip file open from path \"%s\" -> normalized \"%s\"", input_path, path);

  // Treat an '!/' separator inside a path as the separator between the name
  // of the zip file on disk and the subdirectory to search within it.
  // For example, if path is "foo.zip!/bar/bas/x.so", then we search for
  // "bar/bas/x.so" within "foo.zip".
  const char* const separator = strstr(path, kZipFileSeparator);
  if (separator == nullptr) {
    return -1;
  }

  char buf[512];
  if (strlcpy(buf, path, sizeof(buf)) >= sizeof(buf)) {
    PRINT("Warning: ignoring very long library path: %s", path);
    return -1;
  }

  buf[separator - path] = '\0';

  const char* zip_path = buf;
  const char* file_path = &buf[separator - path + 2];
  int fd = TEMP_FAILURE_RETRY(open(zip_path, O_RDONLY | O_CLOEXEC));
  if (fd == -1) {
    return -1;
  }

  ZipArchiveHandle handle;
  if (!zip_archive_cache->get_or_open(zip_path, &handle)) {
    // invalid zip-file (?)
    close(fd);
    return -1;
  }

  ZipEntry entry;

  if (FindEntry(handle, file_path, &entry) != 0) {
    // Entry was not found.
    close(fd);
    return -1;
  }

  // Check if it is properly stored
  if (entry.method != kCompressStored || (entry.offset % PAGE_SIZE) != 0) {
    close(fd);
    return -1;
  }

  *file_offset = entry.offset;

  if (realpath_fd(fd, realpath)) {
    *realpath += separator;
  } else {
    if (!is_first_stage_init()) {
      PRINT("warning: unable to get realpath for the library \"%s\". Will use given path.",
            normalized_path.c_str());
    }
    *realpath = normalized_path;
  }

  return fd;
}

static bool format_path(char* buf, size_t buf_size, const char* path, const char* name) {
  int n = async_safe_format_buffer(buf, buf_size, "%s/%s", path, name);
  if (n < 0 || n >= static_cast<int>(buf_size)) {
    PRINT("Warning: ignoring very long library path: %s/%s", path, name);
    return false;
  }

  return true;
}

static int open_library_at_path(ZipArchiveCache* zip_archive_cache,
                                const char* path, off64_t* file_offset,
                                std::string* realpath) {
  int fd = -1;
  if (strstr(path, kZipFileSeparator) != nullptr) {
    fd = open_library_in_zipfile(zip_archive_cache, path, file_offset, realpath);
  }

  if (fd == -1) {
    fd = TEMP_FAILURE_RETRY(open(path, O_RDONLY | O_CLOEXEC));
    if (fd != -1) {
      *file_offset = 0;
      if (!realpath_fd(fd, realpath)) {
        if (!is_first_stage_init()) {
          PRINT("warning: unable to get realpath for the library \"%s\". Will use given path.",
                path);
        }
        *realpath = path;
      }
    }
  }

  return fd;
}

static int open_library_on_paths(ZipArchiveCache* zip_archive_cache,
                                 const char* name, off64_t* file_offset,
                                 const std::vector<std::string>& paths,
                                 std::string* realpath) {
  for (const auto& path : paths) {
    char buf[512];
    if (!format_path(buf, sizeof(buf), path.c_str(), name)) {
      continue;
    }

    int fd = open_library_at_path(zip_archive_cache, buf, file_offset, realpath);
    if (fd != -1) {
      return fd;
    }
  }

  return -1;
}

static int open_library(android_namespace_t* ns,
                        ZipArchiveCache* zip_archive_cache,
                        const char* name, soinfo *needed_by,
                        off64_t* file_offset, std::string* realpath) {
  TRACE("[ opening %s from namespace %s ]", name, ns->get_name());

  // If the name contains a slash, we should attempt to open it directly and not search the paths.
  if (strchr(name, '/') != nullptr) {
    return open_library_at_path(zip_archive_cache, name, file_offset, realpath);
  }

  // LD_LIBRARY_PATH has the highest priority. We don't have to check accessibility when searching
  // the namespace's path lists, because anything found on a namespace path list should always be
  // accessible.
  int fd = open_library_on_paths(zip_archive_cache, name, file_offset, ns->get_ld_library_paths(), realpath);

  // Try the DT_RUNPATH, and verify that the library is accessible.
  if (fd == -1 && needed_by != nullptr) {
    fd = open_library_on_paths(zip_archive_cache, name, file_offset, needed_by->get_dt_runpath(), realpath);
    if (fd != -1 && !ns->is_accessible(*realpath)) {
      close(fd);
      fd = -1;
    }
  }

  // Finally search the namespace's main search path list.
  if (fd == -1) {
    fd = open_library_on_paths(zip_archive_cache, name, file_offset, ns->get_default_library_paths(), realpath);
  }

  return fd;
}

int open_executable(const char* path, off64_t* file_offset, std::string* realpath) {
  ZipArchiveCache zip_archive_cache;
  return open_library_at_path(&zip_archive_cache, path, file_offset, realpath);
}

const char* fix_dt_needed(const char* dt_needed, const char* sopath __unused) {
#if !defined(__LP64__)
  // Work around incorrect DT_NEEDED entries for old apps: http://b/21364029
  int app_target_api_level = get_application_target_sdk_version();
  if (app_target_api_level < 23) {
    const char* bname = basename(dt_needed);
    if (bname != dt_needed) {
      DL_WARN_documented_change(23,
                                "invalid-dt_needed-entries-enforced-for-api-level-23",
                                "library \"%s\" has invalid DT_NEEDED entry \"%s\"",
                                sopath, dt_needed, app_target_api_level);
      add_dlwarning(sopath, "invalid DT_NEEDED entry",  dt_needed);
    }

    return bname;
  }
#endif
  return dt_needed;
}

template<typename F>
static void for_each_dt_needed(const ElfReader& elf_reader, F action) {
  for (const ElfW(Dyn)* d = elf_reader.dynamic(); d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_NEEDED) {
      action(fix_dt_needed(elf_reader.get_string(d->d_un.d_val), elf_reader.name()));
    }
  }
}

static bool find_loaded_library_by_inode(android_namespace_t* ns,
                                         const struct stat& file_stat,
                                         off64_t file_offset,
                                         bool search_linked_namespaces,
                                         soinfo** candidate) {
  if (file_stat.st_dev == 0 || file_stat.st_ino == 0) {
    *candidate = nullptr;
    return false;
  }

  auto predicate = [&](soinfo* si) {
    return si->get_st_ino() == file_stat.st_ino &&
           si->get_st_dev() == file_stat.st_dev &&
           si->get_file_offset() == file_offset;
  };

  *candidate = ns->soinfo_list().find_if(predicate);

  if (*candidate == nullptr && search_linked_namespaces) {
    for (auto& link : ns->linked_namespaces()) {
      android_namespace_t* linked_ns = link.linked_namespace();
      soinfo* si = linked_ns->soinfo_list().find_if(predicate);

      if (si != nullptr && link.is_accessible(si->get_soname())) {
        *candidate = si;
        return true;
      }
    }
  }

  return *candidate != nullptr;
}

static bool find_loaded_library_by_realpath(android_namespace_t* ns, const char* realpath,
                                            bool search_linked_namespaces, soinfo** candidate) {
  auto predicate = [&](soinfo* si) { return strcmp(realpath, si->get_realpath()) == 0; };

  *candidate = ns->soinfo_list().find_if(predicate);

  if (*candidate == nullptr && search_linked_namespaces) {
    for (auto& link : ns->linked_namespaces()) {
      android_namespace_t* linked_ns = link.linked_namespace();
      soinfo* si = linked_ns->soinfo_list().find_if(predicate);

      if (si != nullptr && link.is_accessible(si->get_soname())) {
        *candidate = si;
        return true;
      }
    }
  }

  return *candidate != nullptr;
}

static bool load_library(android_namespace_t* ns,
                         LoadTask* task,
                         LoadTaskList* load_tasks,
                         int rtld_flags,
                         const std::string& realpath,
                         bool search_linked_namespaces) {
  off64_t file_offset = task->get_file_offset();
  const char* name = task->get_name();
  const android_dlextinfo* extinfo = task->get_extinfo();

  LD_LOG(kLogDlopen,
         "load_library(ns=%s, task=%s, flags=0x%x, realpath=%s, search_linked_namespaces=%d)",
         ns->get_name(), name, rtld_flags, realpath.c_str(), search_linked_namespaces);

  if ((file_offset % PAGE_SIZE) != 0) {
    DL_OPEN_ERR("file offset for the library \"%s\" is not page-aligned: %" PRId64, name, file_offset);
    return false;
  }
  if (file_offset < 0) {
    DL_OPEN_ERR("file offset for the library \"%s\" is negative: %" PRId64, name, file_offset);
    return false;
  }

  struct stat file_stat;
  if (TEMP_FAILURE_RETRY(fstat(task->get_fd(), &file_stat)) != 0) {
    DL_OPEN_ERR("unable to stat file for the library \"%s\": %s", name, strerror(errno));
    return false;
  }
  if (file_offset >= file_stat.st_size) {
    DL_OPEN_ERR("file offset for the library \"%s\" >= file size: %" PRId64 " >= %" PRId64,
        name, file_offset, file_stat.st_size);
    return false;
  }

  // Check for symlink and other situations where
  // file can have different names, unless ANDROID_DLEXT_FORCE_LOAD is set
  if (extinfo == nullptr || (extinfo->flags & ANDROID_DLEXT_FORCE_LOAD) == 0) {
    soinfo* si = nullptr;
    if (find_loaded_library_by_inode(ns, file_stat, file_offset, search_linked_namespaces, &si)) {
      LD_LOG(kLogDlopen,
             "load_library(ns=%s, task=%s): Already loaded under different name/path \"%s\" - "
             "will return existing soinfo",
             ns->get_name(), name, si->get_realpath());
      task->set_soinfo(si);
      return true;
    }
  }

  if ((rtld_flags & RTLD_NOLOAD) != 0) {
    DL_OPEN_ERR("library \"%s\" wasn't loaded and RTLD_NOLOAD prevented it", name);
    return false;
  }

  struct statfs fs_stat;
  if (TEMP_FAILURE_RETRY(fstatfs(task->get_fd(), &fs_stat)) != 0) {
    DL_OPEN_ERR("unable to fstatfs file for the library \"%s\": %s", name, strerror(errno));
    return false;
  }

  // do not check accessibility using realpath if fd is located on tmpfs
  // this enables use of memfd_create() for apps
  if ((fs_stat.f_type != TMPFS_MAGIC) && (!ns->is_accessible(realpath))) {
    // TODO(dimitry): workaround for http://b/26394120 - the exempt-list

    // TODO(dimitry) before O release: add a namespace attribute to have this enabled
    // only for classloader-namespaces
    const soinfo* needed_by = task->is_dt_needed() ? task->get_needed_by() : nullptr;
    if (is_exempt_lib(ns, name, needed_by)) {
      // print warning only if needed by non-system library
      if (needed_by == nullptr || !is_system_library(needed_by->get_realpath())) {
        const soinfo* needed_or_dlopened_by = task->get_needed_by();
        const char* sopath = needed_or_dlopened_by == nullptr ? "(unknown)" :
                                                      needed_or_dlopened_by->get_realpath();
        DL_WARN_documented_change(24,
                                  "private-api-enforced-for-api-level-24",
                                  "library \"%s\" (\"%s\") needed or dlopened by \"%s\" "
                                  "is not accessible by namespace \"%s\"",
                                  name, realpath.c_str(), sopath, ns->get_name());
        add_dlwarning(sopath, "unauthorized access to",  name);
      }
    } else {
      // do not load libraries if they are not accessible for the specified namespace.
      const char* needed_or_dlopened_by = task->get_needed_by() == nullptr ?
                                          "(unknown)" :
                                          task->get_needed_by()->get_realpath();

      DL_OPEN_ERR("library \"%s\" needed or dlopened by \"%s\" is not accessible for the namespace \"%s\"",
             name, needed_or_dlopened_by, ns->get_name());

      // do not print this if a library is in the list of shared libraries for linked namespaces
      if (!maybe_accessible_via_namespace_links(ns, name)) {
        PRINT("library \"%s\" (\"%s\") needed or dlopened by \"%s\" is not accessible for the"
              " namespace: [name=\"%s\", ld_library_paths=\"%s\", default_library_paths=\"%s\","
              " permitted_paths=\"%s\"]",
              name, realpath.c_str(),
              needed_or_dlopened_by,
              ns->get_name(),
              android::base::Join(ns->get_ld_library_paths(), ':').c_str(),
              android::base::Join(ns->get_default_library_paths(), ':').c_str(),
              android::base::Join(ns->get_permitted_paths(), ':').c_str());
      }
      return false;
    }
  }

  soinfo* si = soinfo_alloc(ns, realpath.c_str(), &file_stat, file_offset, rtld_flags);

  task->set_soinfo(si);

  // Read the ELF header and some of the segments.
  if (!task->read(realpath.c_str(), file_stat.st_size)) {
    task->remove_cached_elf_reader();
    task->set_soinfo(nullptr);
    soinfo_free(si);
    return false;
  }

  // Find and set DT_RUNPATH, DT_SONAME, and DT_FLAGS_1.
  // Note that these field values are temporary and are
  // going to be overwritten on soinfo::prelink_image
  // with values from PT_LOAD segments.
  const ElfReader& elf_reader = task->get_elf_reader();
  for (const ElfW(Dyn)* d = elf_reader.dynamic(); d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_RUNPATH) {
      si->set_dt_runpath(elf_reader.get_string(d->d_un.d_val));
    }
    if (d->d_tag == DT_SONAME) {
      si->set_soname(elf_reader.get_string(d->d_un.d_val));
    }
    // We need to identify a DF_1_GLOBAL library early so we can link it to namespaces.
    if (d->d_tag == DT_FLAGS_1) {
      si->set_dt_flags_1(d->d_un.d_val);
    }
  }

#if !defined(__ANDROID__)
  // Bionic on the host currently uses some Android prebuilts, which don't set
  // DT_RUNPATH with any relative paths, so they can't find their dependencies.
  // b/118058804
  if (si->get_dt_runpath().empty()) {
    si->set_dt_runpath("$ORIGIN/../lib64:$ORIGIN/lib64");
  }
#endif

  for_each_dt_needed(task->get_elf_reader(), [&](const char* name) {
    LD_LOG(kLogDlopen, "load_library(ns=%s, task=%s): Adding DT_NEEDED task: %s",
           ns->get_name(), task->get_name(), name);
    load_tasks->push_back(LoadTask::create(name, si, ns, task->get_readers_map()));
  });

  return true;
}

static bool load_library(android_namespace_t* ns,
                         LoadTask* task,
                         ZipArchiveCache* zip_archive_cache,
                         LoadTaskList* load_tasks,
                         int rtld_flags,
                         bool search_linked_namespaces) {
  const char* name = task->get_name();
  soinfo* needed_by = task->get_needed_by();
  const android_dlextinfo* extinfo = task->get_extinfo();

  if (extinfo != nullptr && (extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD) != 0) {
    off64_t file_offset = 0;
    if ((extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET) != 0) {
      file_offset = extinfo->library_fd_offset;
    }

    std::string realpath;
    if (!realpath_fd(extinfo->library_fd, &realpath)) {
      if (!is_first_stage_init()) {
        PRINT(
            "warning: unable to get realpath for the library \"%s\" by extinfo->library_fd. "
            "Will use given name.",
            name);
      }
      realpath = name;
    }

    task->set_fd(extinfo->library_fd, false);
    task->set_file_offset(file_offset);
    return load_library(ns, task, load_tasks, rtld_flags, realpath, search_linked_namespaces);
  }

  LD_LOG(kLogDlopen,
         "load_library(ns=%s, task=%s, flags=0x%x, search_linked_namespaces=%d): calling "
         "open_library",
         ns->get_name(), name, rtld_flags, search_linked_namespaces);

  // Open the file.
  off64_t file_offset;
  std::string realpath;
  int fd = open_library(ns, zip_archive_cache, name, needed_by, &file_offset, &realpath);
  if (fd == -1) {
    if (task->is_dt_needed()) {
      if (needed_by->is_main_executable()) {
        DL_OPEN_ERR("library \"%s\" not found: needed by main executable", name);
      } else {
        DL_OPEN_ERR("library \"%s\" not found: needed by %s in namespace %s", name,
                    needed_by->get_realpath(), task->get_start_from()->get_name());
      }
    } else {
      DL_OPEN_ERR("library \"%s\" not found", name);
    }
    return false;
  }

  task->set_fd(fd, true);
  task->set_file_offset(file_offset);

  return load_library(ns, task, load_tasks, rtld_flags, realpath, search_linked_namespaces);
}

static bool find_loaded_library_by_soname(android_namespace_t* ns,
                                          const char* name,
                                          soinfo** candidate) {
  return !ns->soinfo_list().visit([&](soinfo* si) {
    if (strcmp(name, si->get_soname()) == 0) {
      *candidate = si;
      return false;
    }

    return true;
  });
}

// Returns true if library was found and false otherwise
static bool find_loaded_library_by_soname(android_namespace_t* ns,
                                         const char* name,
                                         bool search_linked_namespaces,
                                         soinfo** candidate) {
  *candidate = nullptr;

  // Ignore filename with path.
  if (strchr(name, '/') != nullptr) {
    return false;
  }

  bool found = find_loaded_library_by_soname(ns, name, candidate);

  if (!found && search_linked_namespaces) {
    // if a library was not found - look into linked namespaces
    for (auto& link : ns->linked_namespaces()) {
      if (!link.is_accessible(name)) {
        continue;
      }

      android_namespace_t* linked_ns = link.linked_namespace();

      if (find_loaded_library_by_soname(linked_ns, name, candidate)) {
        return true;
      }
    }
  }

  return found;
}

static bool find_library_in_linked_namespace(const android_namespace_link_t& namespace_link,
                                             LoadTask* task) {
  android_namespace_t* ns = namespace_link.linked_namespace();

  soinfo* candidate;
  bool loaded = false;

  std::string soname;
  if (find_loaded_library_by_soname(ns, task->get_name(), false, &candidate)) {
    loaded = true;
    soname = candidate->get_soname();
  } else {
    soname = resolve_soname(task->get_name());
  }

  if (!namespace_link.is_accessible(soname.c_str())) {
    // the library is not accessible via namespace_link
    LD_LOG(kLogDlopen,
           "find_library_in_linked_namespace(ns=%s, task=%s): Not accessible (soname=%s)",
           ns->get_name(), task->get_name(), soname.c_str());
    return false;
  }

  // if library is already loaded - return it
  if (loaded) {
    LD_LOG(kLogDlopen, "find_library_in_linked_namespace(ns=%s, task=%s): Already loaded",
           ns->get_name(), task->get_name());
    task->set_soinfo(candidate);
    return true;
  }

  // returning true with empty soinfo means that the library is okay to be
  // loaded in the namespace but has not yet been loaded there before.
  LD_LOG(kLogDlopen, "find_library_in_linked_namespace(ns=%s, task=%s): Ok to load", ns->get_name(),
         task->get_name());
  task->set_soinfo(nullptr);
  return true;
}

static bool find_library_internal(android_namespace_t* ns,
                                  LoadTask* task,
                                  ZipArchiveCache* zip_archive_cache,
                                  LoadTaskList* load_tasks,
                                  int rtld_flags) {
  soinfo* candidate;

  if (find_loaded_library_by_soname(ns, task->get_name(), true /* search_linked_namespaces */,
                                    &candidate)) {
    LD_LOG(kLogDlopen,
           "find_library_internal(ns=%s, task=%s): Already loaded (by soname): %s",
           ns->get_name(), task->get_name(), candidate->get_realpath());
    task->set_soinfo(candidate);
    return true;
  }

  // Library might still be loaded, the accurate detection
  // of this fact is done by load_library.
  TRACE("[ \"%s\" find_loaded_library_by_soname failed (*candidate=%s@%p). Trying harder... ]",
        task->get_name(), candidate == nullptr ? "n/a" : candidate->get_realpath(), candidate);

  if (load_library(ns, task, zip_archive_cache, load_tasks, rtld_flags,
                   true /* search_linked_namespaces */)) {
    return true;
  }

  // TODO(dimitry): workaround for http://b/26394120 (the exempt-list)
  if (ns->is_exempt_list_enabled() && is_exempt_lib(ns, task->get_name(), task->get_needed_by())) {
    // For the libs in the exempt-list, switch to the default namespace and then
    // try the load again from there. The library could be loaded from the
    // default namespace or from another namespace (e.g. runtime) that is linked
    // from the default namespace.
    LD_LOG(kLogDlopen,
           "find_library_internal(ns=%s, task=%s): Exempt system library - trying namespace %s",
           ns->get_name(), task->get_name(), g_default_namespace.get_name());
    ns = &g_default_namespace;
    if (load_library(ns, task, zip_archive_cache, load_tasks, rtld_flags,
                     true /* search_linked_namespaces */)) {
      return true;
    }
  }
  // END OF WORKAROUND

  // if a library was not found - look into linked namespaces
  // preserve current dlerror in the case it fails.
  DlErrorRestorer dlerror_restorer;
  LD_LOG(kLogDlopen, "find_library_internal(ns=%s, task=%s): Trying %zu linked namespaces",
         ns->get_name(), task->get_name(), ns->linked_namespaces().size());
  for (auto& linked_namespace : ns->linked_namespaces()) {
    if (find_library_in_linked_namespace(linked_namespace, task)) {
      // Library is already loaded.
      if (task->get_soinfo() != nullptr) {
        // n.b. This code path runs when find_library_in_linked_namespace found an already-loaded
        // library by soname. That should only be possible with a exempt-list lookup, where we
        // switch the namespace, because otherwise, find_library_in_linked_namespace is duplicating
        // the soname scan done in this function's first call to find_loaded_library_by_soname.
        return true;
      }

      if (load_library(linked_namespace.linked_namespace(), task, zip_archive_cache, load_tasks,
                       rtld_flags, false /* search_linked_namespaces */)) {
        LD_LOG(kLogDlopen, "find_library_internal(ns=%s, task=%s): Found in linked namespace %s",
               ns->get_name(), task->get_name(), linked_namespace.linked_namespace()->get_name());
        return true;
      }
    }
  }

  return false;
}

static void soinfo_unload(soinfo* si);

static void shuffle(std::vector<LoadTask*>* v) {
  if (is_first_stage_init()) {
    // arc4random* is not available in first stage init because /dev/random
    // hasn't yet been created.
    return;
  }
  for (size_t i = 0, size = v->size(); i < size; ++i) {
    size_t n = size - i;
    size_t r = arc4random_uniform(n);
    std::swap((*v)[n-1], (*v)[r]);
  }
}

// add_as_children - add first-level loaded libraries (i.e. library_names[], but
// not their transitive dependencies) as children of the start_with library.
// This is false when find_libraries is called for dlopen(), when newly loaded
// libraries must form a disjoint tree.
bool find_libraries(android_namespace_t* ns,
                    soinfo* start_with,
                    const char* const library_names[],
                    size_t library_names_count,
                    soinfo* soinfos[],
                    std::vector<soinfo*>* ld_preloads,
                    size_t ld_preloads_count,
                    int rtld_flags,
                    const android_dlextinfo* extinfo,
                    bool add_as_children,
                    std::vector<android_namespace_t*>* namespaces) {
  // Step 0: prepare.
  std::unordered_map<const soinfo*, ElfReader> readers_map;
  LoadTaskList load_tasks;

  for (size_t i = 0; i < library_names_count; ++i) {
    const char* name = library_names[i];
    load_tasks.push_back(LoadTask::create(name, start_with, ns, &readers_map));
  }

  // If soinfos array is null allocate one on stack.
  // The array is needed in case of failure; for example
  // when library_names[] = {libone.so, libtwo.so} and libone.so
  // is loaded correctly but libtwo.so failed for some reason.
  // In this case libone.so should be unloaded on return.
  // See also implementation of failure_guard below.

  if (soinfos == nullptr) {
    size_t soinfos_size = sizeof(soinfo*)*library_names_count;
    soinfos = reinterpret_cast<soinfo**>(alloca(soinfos_size));
    memset(soinfos, 0, soinfos_size);
  }

  // list of libraries to link - see step 2.
  size_t soinfos_count = 0;

  auto scope_guard = android::base::make_scope_guard([&]() {
    for (LoadTask* t : load_tasks) {
      LoadTask::deleter(t);
    }
  });

  ZipArchiveCache zip_archive_cache;
  soinfo_list_t new_global_group_members;

  // Step 1: expand the list of load_tasks to include
  // all DT_NEEDED libraries (do not load them just yet)
  for (size_t i = 0; i<load_tasks.size(); ++i) {
    LoadTask* task = load_tasks[i];
    soinfo* needed_by = task->get_needed_by();

    bool is_dt_needed = needed_by != nullptr && (needed_by != start_with || add_as_children);
    task->set_extinfo(is_dt_needed ? nullptr : extinfo);
    task->set_dt_needed(is_dt_needed);

    LD_LOG(kLogDlopen, "find_libraries(ns=%s): task=%s, is_dt_needed=%d", ns->get_name(),
           task->get_name(), is_dt_needed);

    // Note: start from the namespace that is stored in the LoadTask. This namespace
    // is different from the current namespace when the LoadTask is for a transitive
    // dependency and the lib that created the LoadTask is not found in the
    // current namespace but in one of the linked namespace.
    if (!find_library_internal(const_cast<android_namespace_t*>(task->get_start_from()),
                               task,
                               &zip_archive_cache,
                               &load_tasks,
                               rtld_flags)) {
      return false;
    }

    soinfo* si = task->get_soinfo();

    if (is_dt_needed) {
      needed_by->add_child(si);
    }

    // When ld_preloads is not null, the first
    // ld_preloads_count libs are in fact ld_preloads.
    bool is_ld_preload = false;
    if (ld_preloads != nullptr && soinfos_count < ld_preloads_count) {
      ld_preloads->push_back(si);
      is_ld_preload = true;
    }

    if (soinfos_count < library_names_count) {
      soinfos[soinfos_count++] = si;
    }

    // Add the new global group members to all initial namespaces. Do this secondary namespace setup
    // at the same time that libraries are added to their primary namespace so that the order of
    // global group members is the same in the every namespace. Only add a library to a namespace
    // once, even if it appears multiple times in the dependency graph.
    if (is_ld_preload || (si->get_dt_flags_1() & DF_1_GLOBAL) != 0) {
      if (!si->is_linked() && namespaces != nullptr && !new_global_group_members.contains(si)) {
        new_global_group_members.push_back(si);
        for (auto linked_ns : *namespaces) {
          if (si->get_primary_namespace() != linked_ns) {
            linked_ns->add_soinfo(si);
            si->add_secondary_namespace(linked_ns);
          }
        }
      }
    }
  }

  // Step 2: Load libraries in random order (see b/24047022)
  LoadTaskList load_list;
  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    auto pred = [&](const LoadTask* t) {
      return t->get_soinfo() == si;
    };

    if (!si->is_linked() &&
        std::find_if(load_list.begin(), load_list.end(), pred) == load_list.end() ) {
      load_list.push_back(task);
    }
  }
  bool reserved_address_recursive = false;
  if (extinfo) {
    reserved_address_recursive = extinfo->flags & ANDROID_DLEXT_RESERVED_ADDRESS_RECURSIVE;
  }
  if (!reserved_address_recursive) {
    // Shuffle the load order in the normal case, but not if we are loading all
    // the libraries to a reserved address range.
    shuffle(&load_list);
  }

  // Set up address space parameters.
  address_space_params extinfo_params, default_params;
  size_t relro_fd_offset = 0;
  if (extinfo) {
    if (extinfo->flags & ANDROID_DLEXT_RESERVED_ADDRESS) {
      extinfo_params.start_addr = extinfo->reserved_addr;
      extinfo_params.reserved_size = extinfo->reserved_size;
      extinfo_params.must_use_address = true;
    } else if (extinfo->flags & ANDROID_DLEXT_RESERVED_ADDRESS_HINT) {
      extinfo_params.start_addr = extinfo->reserved_addr;
      extinfo_params.reserved_size = extinfo->reserved_size;
    }
  }

  for (auto&& task : load_list) {
    address_space_params* address_space =
        (reserved_address_recursive || !task->is_dt_needed()) ? &extinfo_params : &default_params;
    if (!task->load(address_space)) {
      return false;
    }
  }

  // Step 3: pre-link all DT_NEEDED libraries in breadth first order.
  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    if (!si->is_linked() && !si->prelink_image()) {
      return false;
    }
    register_soinfo_tls(si);
  }

  // Step 4: Construct the global group. DF_1_GLOBAL bit is force set for LD_PRELOADed libs because
  // they must be added to the global group. Note: The DF_1_GLOBAL bit for a library is normally set
  // in step 3.
  if (ld_preloads != nullptr) {
    for (auto&& si : *ld_preloads) {
      si->set_dt_flags_1(si->get_dt_flags_1() | DF_1_GLOBAL);
    }
  }

  // Step 5: Collect roots of local_groups.
  // Whenever needed_by->si link crosses a namespace boundary it forms its own local_group.
  // Here we collect new roots to link them separately later on. Note that we need to avoid
  // collecting duplicates. Also the order is important. They need to be linked in the same
  // BFS order we link individual libraries.
  std::vector<soinfo*> local_group_roots;
  if (start_with != nullptr && add_as_children) {
    local_group_roots.push_back(start_with);
  } else {
    CHECK(soinfos_count == 1);
    local_group_roots.push_back(soinfos[0]);
  }

  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    soinfo* needed_by = task->get_needed_by();
    bool is_dt_needed = needed_by != nullptr && (needed_by != start_with || add_as_children);
    android_namespace_t* needed_by_ns =
        is_dt_needed ? needed_by->get_primary_namespace() : ns;

    if (!si->is_linked() && si->get_primary_namespace() != needed_by_ns) {
      auto it = std::find(local_group_roots.begin(), local_group_roots.end(), si);
      LD_LOG(kLogDlopen,
             "Crossing namespace boundary (si=%s@%p, si_ns=%s@%p, needed_by=%s@%p, ns=%s@%p, needed_by_ns=%s@%p) adding to local_group_roots: %s",
             si->get_realpath(),
             si,
             si->get_primary_namespace()->get_name(),
             si->get_primary_namespace(),
             needed_by == nullptr ? "(nullptr)" : needed_by->get_realpath(),
             needed_by,
             ns->get_name(),
             ns,
             needed_by_ns->get_name(),
             needed_by_ns,
             it == local_group_roots.end() ? "yes" : "no");

      if (it == local_group_roots.end()) {
        local_group_roots.push_back(si);
      }
    }
  }

  // Step 6: Link all local groups
  for (auto root : local_group_roots) {
    soinfo_list_t local_group;
    android_namespace_t* local_group_ns = root->get_primary_namespace();

    walk_dependencies_tree(root,
      [&] (soinfo* si) {
        if (local_group_ns->is_accessible(si)) {
          local_group.push_back(si);
          return kWalkContinue;
        } else {
          return kWalkSkip;
        }
      });

    soinfo_list_t global_group = local_group_ns->get_global_group();
    SymbolLookupList lookup_list(global_group, local_group);
    soinfo* local_group_root = local_group.front();

    bool linked = local_group.visit([&](soinfo* si) {
      // Even though local group may contain accessible soinfos from other namespaces
      // we should avoid linking them (because if they are not linked -> they
      // are in the local_group_roots and will be linked later).
      if (!si->is_linked() && si->get_primary_namespace() == local_group_ns) {
        const android_dlextinfo* link_extinfo = nullptr;
        if (si == soinfos[0] || reserved_address_recursive) {
          // Only forward extinfo for the first library unless the recursive
          // flag is set.
          link_extinfo = extinfo;
        }
        if (__libc_shared_globals()->load_hook) {
          __libc_shared_globals()->load_hook(si->load_bias, si->phdr, si->phnum);
        }
        lookup_list.set_dt_symbolic_lib(si->has_DT_SYMBOLIC ? si : nullptr);
        if (!si->link_image(lookup_list, local_group_root, link_extinfo, &relro_fd_offset) ||
            !get_cfi_shadow()->AfterLoad(si, solist_get_head())) {
          return false;
        }
      }

      return true;
    });

    if (!linked) {
      return false;
    }
  }

  // Step 7: Mark all load_tasks as linked and increment refcounts
  // for references between load_groups (at this point it does not matter if
  // referenced load_groups were loaded by previous dlopen or as part of this
  // one on step 6)
  if (start_with != nullptr && add_as_children) {
    start_with->set_linked();
  }

  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    si->set_linked();
  }

  for (auto&& task : load_tasks) {
    soinfo* si = task->get_soinfo();
    soinfo* needed_by = task->get_needed_by();
    if (needed_by != nullptr &&
        needed_by != start_with &&
        needed_by->get_local_group_root() != si->get_local_group_root()) {
      si->increment_ref_count();
    }
  }


  return true;
}

static soinfo* find_library(android_namespace_t* ns,
                            const char* name, int rtld_flags,
                            const android_dlextinfo* extinfo,
                            soinfo* needed_by) {
  soinfo* si = nullptr;

  if (name == nullptr) {
    si = solist_get_somain();
  } else if (!find_libraries(ns,
                             needed_by,
                             &name,
                             1,
                             &si,
                             nullptr,
                             0,
                             rtld_flags,
                             extinfo,
                             false /* add_as_children */)) {
    if (si != nullptr) {
      soinfo_unload(si);
    }
    return nullptr;
  }

  si->increment_ref_count();

  return si;
}

static void soinfo_unload_impl(soinfo* root) {
  ScopedTrace trace((std::string("unload ") + root->get_realpath()).c_str());
  bool is_linked = root->is_linked();

  if (!root->can_unload()) {
    LD_LOG(kLogDlopen,
           "... dlclose(root=\"%s\"@%p) ... not unloading - the load group is flagged with NODELETE",
           root->get_realpath(),
           root);
    return;
  }


  soinfo_list_t unload_list;
  unload_list.push_back(root);

  soinfo_list_t local_unload_list;
  soinfo_list_t external_unload_list;
  soinfo* si = nullptr;

  while ((si = unload_list.pop_front()) != nullptr) {
    if (local_unload_list.contains(si)) {
      continue;
    }

    local_unload_list.push_back(si);

    if (si->has_min_version(0)) {
      soinfo* child = nullptr;
      while ((child = si->get_children().pop_front()) != nullptr) {
        TRACE("%s@%p needs to unload %s@%p", si->get_realpath(), si,
            child->get_realpath(), child);

        child->get_parents().remove(si);

        if (local_unload_list.contains(child)) {
          continue;
        } else if (child->is_linked() && child->get_local_group_root() != root) {
          external_unload_list.push_back(child);
        } else if (child->get_parents().empty()) {
          unload_list.push_back(child);
        }
      }
    } else {
      async_safe_fatal("soinfo for \"%s\"@%p has no version", si->get_realpath(), si);
    }
  }

  local_unload_list.for_each([](soinfo* si) {
    LD_LOG(kLogDlopen,
           "... dlclose: calling destructors for \"%s\"@%p ... ",
           si->get_realpath(),
           si);
    si->call_destructors();
    LD_LOG(kLogDlopen,
           "... dlclose: calling destructors for \"%s\"@%p ... done",
           si->get_realpath(),
           si);
  });

  while ((si = local_unload_list.pop_front()) != nullptr) {
    LD_LOG(kLogDlopen,
           "... dlclose: unloading \"%s\"@%p ...",
           si->get_realpath(),
           si);
    ++g_module_unload_counter;
    notify_gdb_of_unload(si);
    unregister_soinfo_tls(si);
    if (__libc_shared_globals()->unload_hook) {
      __libc_shared_globals()->unload_hook(si->load_bias, si->phdr, si->phnum);
    }
    get_cfi_shadow()->BeforeUnload(si);
    soinfo_free(si);
  }

  if (is_linked) {
    while ((si = external_unload_list.pop_front()) != nullptr) {
      LD_LOG(kLogDlopen,
             "... dlclose: unloading external reference \"%s\"@%p ...",
             si->get_realpath(),
             si);
      soinfo_unload(si);
    }
  } else {
      LD_LOG(kLogDlopen,
             "... dlclose: unload_si was not linked - not unloading external references ...");
  }
}

static void soinfo_unload(soinfo* unload_si) {
  // Note that the library can be loaded but not linked;
  // in which case there is no root but we still need
  // to walk the tree and unload soinfos involved.
  //
  // This happens on unsuccessful dlopen, when one of
  // the DT_NEEDED libraries could not be linked/found.
  bool is_linked = unload_si->is_linked();
  soinfo* root = is_linked ? unload_si->get_local_group_root() : unload_si;

  LD_LOG(kLogDlopen,
         "... dlclose(realpath=\"%s\"@%p) ... load group root is \"%s\"@%p",
         unload_si->get_realpath(),
         unload_si,
         root->get_realpath(),
         root);


  size_t ref_count = is_linked ? root->decrement_ref_count() : 0;
  if (ref_count > 0) {
    LD_LOG(kLogDlopen,
           "... dlclose(root=\"%s\"@%p) ... not unloading - decrementing ref_count to %zd",
           root->get_realpath(),
           root,
           ref_count);
    return;
  }

  soinfo_unload_impl(root);
}

void increment_dso_handle_reference_counter(void* dso_handle) {
  if (dso_handle == nullptr) {
    return;
  }

  auto it = g_dso_handle_counters.find(dso_handle);
  if (it != g_dso_handle_counters.end()) {
    CHECK(++it->second != 0);
  } else {
    soinfo* si = find_containing_library(dso_handle);
    if (si != nullptr) {
      ProtectedDataGuard guard;
      si->increment_ref_count();
    } else {
      async_safe_fatal(
          "increment_dso_handle_reference_counter: Couldn't find soinfo by dso_handle=%p",
          dso_handle);
    }
    g_dso_handle_counters[dso_handle] = 1U;
  }
}

void decrement_dso_handle_reference_counter(void* dso_handle) {
  if (dso_handle == nullptr) {
    return;
  }

  auto it = g_dso_handle_counters.find(dso_handle);
  CHECK(it != g_dso_handle_counters.end());
  CHECK(it->second != 0);

  if (--it->second == 0) {
    soinfo* si = find_containing_library(dso_handle);
    if (si != nullptr) {
      ProtectedDataGuard guard;
      soinfo_unload(si);
    } else {
      async_safe_fatal(
          "decrement_dso_handle_reference_counter: Couldn't find soinfo by dso_handle=%p",
          dso_handle);
    }
    g_dso_handle_counters.erase(it);
  }
}

static std::string symbol_display_name(const char* sym_name, const char* sym_ver) {
  if (sym_ver == nullptr) {
    return sym_name;
  }

  return std::string(sym_name) + ", version " + sym_ver;
}

static android_namespace_t* get_caller_namespace(soinfo* caller) {
  return caller != nullptr ? caller->get_primary_namespace() : g_anonymous_namespace;
}

void do_android_get_LD_LIBRARY_PATH(char* buffer, size_t buffer_size) {
  // Use basic string manipulation calls to avoid snprintf.
  // snprintf indirectly calls pthread_getspecific to get the size of a buffer.
  // When debug malloc is enabled, this call returns 0. This in turn causes
  // snprintf to do nothing, which causes libraries to fail to load.
  // See b/17302493 for further details.
  // Once the above bug is fixed, this code can be modified to use
  // snprintf again.
  const auto& default_ld_paths = g_default_namespace.get_default_library_paths();

  size_t required_size = 0;
  for (const auto& path : default_ld_paths) {
    required_size += path.size() + 1;
  }

  if (buffer_size < required_size) {
    async_safe_fatal("android_get_LD_LIBRARY_PATH failed, buffer too small: "
                     "buffer len %zu, required len %zu", buffer_size, required_size);
  }

  char* end = buffer;
  for (size_t i = 0; i < default_ld_paths.size(); ++i) {
    if (i > 0) *end++ = ':';
    end = stpcpy(end, default_ld_paths[i].c_str());
  }
}

void do_android_update_LD_LIBRARY_PATH(const char* ld_library_path) {
  parse_LD_LIBRARY_PATH(ld_library_path);
}

static std::string android_dlextinfo_to_string(const android_dlextinfo* info) {
  if (info == nullptr) {
    return "(null)";
  }

  return android::base::StringPrintf("[flags=0x%" PRIx64 ","
                                     " reserved_addr=%p,"
                                     " reserved_size=0x%zx,"
                                     " relro_fd=%d,"
                                     " library_fd=%d,"
                                     " library_fd_offset=0x%" PRIx64 ","
                                     " library_namespace=%s@%p]",
                                     info->flags,
                                     info->reserved_addr,
                                     info->reserved_size,
                                     info->relro_fd,
                                     info->library_fd,
                                     info->library_fd_offset,
                                     (info->flags & ANDROID_DLEXT_USE_NAMESPACE) != 0 ?
                                        (info->library_namespace != nullptr ?
                                          info->library_namespace->get_name() : "(null)") : "(n/a)",
                                     (info->flags & ANDROID_DLEXT_USE_NAMESPACE) != 0 ?
                                        info->library_namespace : nullptr);
}

void* do_dlopen(const char* name, int flags,
                const android_dlextinfo* extinfo,
                const void* caller_addr) {
  std::string trace_prefix = std::string("dlopen: ") + (name == nullptr ? "(nullptr)" : name);
  ScopedTrace trace(trace_prefix.c_str());
  ScopedTrace loading_trace((trace_prefix + " - loading and linking").c_str());
  soinfo* const caller = find_containing_library(caller_addr);
  android_namespace_t* ns = get_caller_namespace(caller);

  LD_LOG(kLogDlopen,
         "dlopen(name=\"%s\", flags=0x%x, extinfo=%s, caller=\"%s\", caller_ns=%s@%p, targetSdkVersion=%i) ...",
         name,
         flags,
         android_dlextinfo_to_string(extinfo).c_str(),
         caller == nullptr ? "(null)" : caller->get_realpath(),
         ns == nullptr ? "(null)" : ns->get_name(),
         ns,
         get_application_target_sdk_version());

  auto purge_guard = android::base::make_scope_guard([&]() { purge_unused_memory(); });

  auto failure_guard = android::base::make_scope_guard(
      [&]() { LD_LOG(kLogDlopen, "... dlopen failed: %s", linker_get_error_buffer()); });

  if ((flags & ~(RTLD_NOW|RTLD_LAZY|RTLD_LOCAL|RTLD_GLOBAL|RTLD_NODELETE|RTLD_NOLOAD)) != 0) {
    DL_OPEN_ERR("invalid flags to dlopen: %x", flags);
    return nullptr;
  }

  if (extinfo != nullptr) {
    if ((extinfo->flags & ~(ANDROID_DLEXT_VALID_FLAG_BITS)) != 0) {
      DL_OPEN_ERR("invalid extended flags to android_dlopen_ext: 0x%" PRIx64, extinfo->flags);
      return nullptr;
    }

    if ((extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD) == 0 &&
        (extinfo->flags & ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET) != 0) {
      DL_OPEN_ERR("invalid extended flag combination (ANDROID_DLEXT_USE_LIBRARY_FD_OFFSET without "
          "ANDROID_DLEXT_USE_LIBRARY_FD): 0x%" PRIx64, extinfo->flags);
      return nullptr;
    }

    if ((extinfo->flags & ANDROID_DLEXT_USE_NAMESPACE) != 0) {
      if (extinfo->library_namespace == nullptr) {
        DL_OPEN_ERR("ANDROID_DLEXT_USE_NAMESPACE is set but extinfo->library_namespace is null");
        return nullptr;
      }
      ns = extinfo->library_namespace;
    }
  }

  // Workaround for dlopen(/system/lib/<soname>) when .so is in /apex. http://b/121248172
  // The workaround works only when targetSdkVersion < Q.
  std::string name_to_apex;
  if (translateSystemPathToApexPath(name, &name_to_apex)) {
    const char* new_name = name_to_apex.c_str();
    LD_LOG(kLogDlopen, "dlopen considering translation from %s to APEX path %s",
           name,
           new_name);
    // Some APEXs could be optionally disabled. Only translate the path
    // when the old file is absent and the new file exists.
    // TODO(b/124218500): Re-enable it once app compat issue is resolved
    /*
    if (file_exists(name)) {
      LD_LOG(kLogDlopen, "dlopen %s exists, not translating", name);
    } else
    */
    if (!file_exists(new_name)) {
      LD_LOG(kLogDlopen, "dlopen %s does not exist, not translating",
             new_name);
    } else {
      LD_LOG(kLogDlopen, "dlopen translation accepted: using %s", new_name);
      name = new_name;
    }
  }
  // End Workaround for dlopen(/system/lib/<soname>) when .so is in /apex.

  std::string asan_name_holder;

  const char* translated_name = name;
  if (g_is_asan && translated_name != nullptr && translated_name[0] == '/') {
    char original_path[PATH_MAX];
    if (realpath(name, original_path) != nullptr) {
      asan_name_holder = std::string(kAsanLibDirPrefix) + original_path;
      if (file_exists(asan_name_holder.c_str())) {
        soinfo* si = nullptr;
        if (find_loaded_library_by_realpath(ns, original_path, true, &si)) {
          PRINT("linker_asan dlopen NOT translating \"%s\" -> \"%s\": library already loaded", name,
                asan_name_holder.c_str());
        } else {
          PRINT("linker_asan dlopen translating \"%s\" -> \"%s\"", name, translated_name);
          translated_name = asan_name_holder.c_str();
        }
      }
    }
  }

  ProtectedDataGuard guard;
  soinfo* si = find_library(ns, translated_name, flags, extinfo, caller);
  loading_trace.End();

  if (si != nullptr) {
    void* handle = si->to_handle();
    LD_LOG(kLogDlopen,
           "... dlopen calling constructors: realpath=\"%s\", soname=\"%s\", handle=%p",
           si->get_realpath(), si->get_soname(), handle);
    si->call_constructors();
    failure_guard.Disable();
    LD_LOG(kLogDlopen,
           "... dlopen successful: realpath=\"%s\", soname=\"%s\", handle=%p",
           si->get_realpath(), si->get_soname(), handle);
    return handle;
  }

  return nullptr;
}

int do_dladdr(const void* addr, Dl_info* info) {
  // Determine if this address can be found in any library currently mapped.
  soinfo* si = find_containing_library(addr);
  if (si == nullptr) {
    return 0;
  }

  memset(info, 0, sizeof(Dl_info));

  info->dli_fname = si->get_realpath();
  // Address at which the shared object is loaded.
  info->dli_fbase = reinterpret_cast<void*>(si->base);

  // Determine if any symbol in the library contains the specified address.
  ElfW(Sym)* sym = si->find_symbol_by_address(addr);
  if (sym != nullptr) {
    info->dli_sname = si->get_string(sym->st_name);
    info->dli_saddr = reinterpret_cast<void*>(si->resolve_symbol_address(sym));
  }

  return 1;
}

static soinfo* soinfo_from_handle(void* handle) {
  if ((reinterpret_cast<uintptr_t>(handle) & 1) != 0) {
    auto it = g_soinfo_handles_map.find(reinterpret_cast<uintptr_t>(handle));
    if (it == g_soinfo_handles_map.end()) {
      return nullptr;
    } else {
      return it->second;
    }
  }

  return static_cast<soinfo*>(handle);
}

bool do_dlsym(void* handle,
              const char* sym_name,
              const char* sym_ver,
              const void* caller_addr,
              void** symbol) {
  ScopedTrace trace("dlsym");
#if !defined(__LP64__)
  if (handle == nullptr) {
    DL_SYM_ERR("dlsym failed: library handle is null");
    return false;
  }
#endif

  soinfo* found = nullptr;
  const ElfW(Sym)* sym = nullptr;
  soinfo* caller = find_containing_library(caller_addr);
  android_namespace_t* ns = get_caller_namespace(caller);
  soinfo* si = nullptr;
  if (handle != RTLD_DEFAULT && handle != RTLD_NEXT) {
    si = soinfo_from_handle(handle);
  }

  LD_LOG(kLogDlsym,
         "dlsym(handle=%p(\"%s\"), sym_name=\"%s\", sym_ver=\"%s\", caller=\"%s\", caller_ns=%s@%p) ...",
         handle,
         si != nullptr ? si->get_realpath() : "n/a",
         sym_name,
         sym_ver,
         caller == nullptr ? "(null)" : caller->get_realpath(),
         ns == nullptr ? "(null)" : ns->get_name(),
         ns);

  auto failure_guard = android::base::make_scope_guard(
      [&]() { LD_LOG(kLogDlsym, "... dlsym failed: %s", linker_get_error_buffer()); });

  if (sym_name == nullptr) {
    DL_SYM_ERR("dlsym failed: symbol name is null");
    return false;
  }

  version_info vi_instance;
  version_info* vi = nullptr;

  if (sym_ver != nullptr) {
    vi_instance.name = sym_ver;
    vi_instance.elf_hash = calculate_elf_hash(sym_ver);
    vi = &vi_instance;
  }

  if (handle == RTLD_DEFAULT || handle == RTLD_NEXT) {
    sym = dlsym_linear_lookup(ns, sym_name, vi, &found, caller, handle);
  } else {
    if (si == nullptr) {
      DL_SYM_ERR("dlsym failed: invalid handle: %p", handle);
      return false;
    }
    sym = dlsym_handle_lookup(si, &found, sym_name, vi);
  }

  if (sym != nullptr) {
    uint32_t bind = ELF_ST_BIND(sym->st_info);
    uint32_t type = ELF_ST_TYPE(sym->st_info);

    if ((bind == STB_GLOBAL || bind == STB_WEAK) && sym->st_shndx != 0) {
      if (type == STT_TLS) {
        // For a TLS symbol, dlsym returns the address of the current thread's
        // copy of the symbol.
        const soinfo_tls* tls_module = found->get_tls();
        if (tls_module == nullptr) {
          DL_SYM_ERR("TLS symbol \"%s\" in solib \"%s\" with no TLS segment",
                     sym_name, found->get_realpath());
          return false;
        }
        void* tls_block = get_tls_block_for_this_thread(tls_module, /*should_alloc=*/true);
        *symbol = static_cast<char*>(tls_block) + sym->st_value;
      } else {
        *symbol = reinterpret_cast<void*>(found->resolve_symbol_address(sym));
      }
      failure_guard.Disable();
      LD_LOG(kLogDlsym,
             "... dlsym successful: sym_name=\"%s\", sym_ver=\"%s\", found in=\"%s\", address=%p",
             sym_name, sym_ver, found->get_soname(), *symbol);
      return true;
    }

    DL_SYM_ERR("symbol \"%s\" found but not global", symbol_display_name(sym_name, sym_ver).c_str());
    return false;
  }

  DL_SYM_ERR("undefined symbol: %s", symbol_display_name(sym_name, sym_ver).c_str());
  return false;
}

int do_dlclose(void* handle) {
  ScopedTrace trace("dlclose");
  ProtectedDataGuard guard;
  soinfo* si = soinfo_from_handle(handle);
  if (si == nullptr) {
    DL_OPEN_ERR("invalid handle: %p", handle);
    return -1;
  }

  LD_LOG(kLogDlopen,
         "dlclose(handle=%p, realpath=\"%s\"@%p) ...",
         handle,
         si->get_realpath(),
         si);
  soinfo_unload(si);
  LD_LOG(kLogDlopen,
         "dlclose(handle=%p) ... done",
         handle);
  return 0;
}

// Make ns as the anonymous namespace that is a namespace used when
// we fail to determine the caller address (e.g., call from mono-jited code)
// Since there can be multiple anonymous namespace in a process, subsequent
// call to this function causes an error.
static bool set_anonymous_namespace(android_namespace_t* ns) {
  if (!g_anonymous_namespace_set && ns != nullptr) {
    CHECK(ns->is_also_used_as_anonymous());
    g_anonymous_namespace = ns;
    g_anonymous_namespace_set = true;
    return true;
  }
  return false;
}

// TODO(b/130388701) remove this. Currently, this is used only for testing
// where we don't have classloader namespace.
bool init_anonymous_namespace(const char* shared_lib_sonames, const char* library_search_path) {
  ProtectedDataGuard guard;

  // Test-only feature: we need to change the anonymous namespace multiple times
  // while the test is running.
  g_anonymous_namespace_set = false;

  // create anonymous namespace
  // When the caller is nullptr - create_namespace will take global group
  // from the anonymous namespace, which is fine because anonymous namespace
  // is still pointing to the default one.
  android_namespace_t* anon_ns =
      create_namespace(nullptr,
                       "(anonymous)",
                       nullptr,
                       library_search_path,
                       ANDROID_NAMESPACE_TYPE_ISOLATED |
                       ANDROID_NAMESPACE_TYPE_ALSO_USED_AS_ANONYMOUS,
                       nullptr,
                       &g_default_namespace);

  CHECK(anon_ns != nullptr);

  if (!link_namespaces(anon_ns, &g_default_namespace, shared_lib_sonames)) {
    // TODO: delete anon_ns
    return false;
  }

  return true;
}

static void add_soinfos_to_namespace(const soinfo_list_t& soinfos, android_namespace_t* ns) {
  ns->add_soinfos(soinfos);
  for (auto si : soinfos) {
    si->add_secondary_namespace(ns);
  }
}

std::vector<std::string> fix_lib_paths(std::vector<std::string> paths) {
  // For the bootstrap linker, insert /system/${LIB}/bootstrap in front of /system/${LIB} in any
  // namespace search path. The bootstrap linker should prefer to use the bootstrap bionic libraries
  // (e.g. libc.so).
#if !defined(__ANDROID_APEX__)
  for (size_t i = 0; i < paths.size(); ++i) {
    if (paths[i] == kSystemLibDir) {
      paths.insert(paths.begin() + i, std::string(kSystemLibDir) + "/bootstrap");
      ++i;
    }
  }
#endif
  return paths;
}

android_namespace_t* create_namespace(const void* caller_addr,
                                      const char* name,
                                      const char* ld_library_path,
                                      const char* default_library_path,
                                      uint64_t type,
                                      const char* permitted_when_isolated_path,
                                      android_namespace_t* parent_namespace) {
  if (parent_namespace == nullptr) {
    // if parent_namespace is nullptr -> set it to the caller namespace
    soinfo* caller_soinfo = find_containing_library(caller_addr);

    parent_namespace = caller_soinfo != nullptr ?
                       caller_soinfo->get_primary_namespace() :
                       g_anonymous_namespace;
  }

  ProtectedDataGuard guard;
  std::vector<std::string> ld_library_paths;
  std::vector<std::string> default_library_paths;
  std::vector<std::string> permitted_paths;

  parse_path(ld_library_path, ":", &ld_library_paths);
  parse_path(default_library_path, ":", &default_library_paths);
  parse_path(permitted_when_isolated_path, ":", &permitted_paths);

  android_namespace_t* ns = new (g_namespace_allocator.alloc()) android_namespace_t();
  ns->set_name(name);
  ns->set_isolated((type & ANDROID_NAMESPACE_TYPE_ISOLATED) != 0);
  ns->set_exempt_list_enabled((type & ANDROID_NAMESPACE_TYPE_EXEMPT_LIST_ENABLED) != 0);
  ns->set_also_used_as_anonymous((type & ANDROID_NAMESPACE_TYPE_ALSO_USED_AS_ANONYMOUS) != 0);

  if ((type & ANDROID_NAMESPACE_TYPE_SHARED) != 0) {
    // append parent namespace paths.
    std::copy(parent_namespace->get_ld_library_paths().begin(),
              parent_namespace->get_ld_library_paths().end(),
              back_inserter(ld_library_paths));

    std::copy(parent_namespace->get_default_library_paths().begin(),
              parent_namespace->get_default_library_paths().end(),
              back_inserter(default_library_paths));

    std::copy(parent_namespace->get_permitted_paths().begin(),
              parent_namespace->get_permitted_paths().end(),
              back_inserter(permitted_paths));

    // If shared - clone the parent namespace
    add_soinfos_to_namespace(parent_namespace->soinfo_list(), ns);
    // and copy parent namespace links
    for (auto& link : parent_namespace->linked_namespaces()) {
      ns->add_linked_namespace(link.linked_namespace(), link.shared_lib_sonames(),
                               link.allow_all_shared_libs());
    }
  } else {
    // If not shared - copy only the shared group
    add_soinfos_to_namespace(parent_namespace->get_shared_group(), ns);
  }

  ns->set_ld_library_paths(std::move(ld_library_paths));
  ns->set_default_library_paths(std::move(default_library_paths));
  ns->set_permitted_paths(std::move(permitted_paths));

  if (ns->is_also_used_as_anonymous() && !set_anonymous_namespace(ns)) {
    DL_ERR("failed to set namespace: [name=\"%s\", ld_library_path=\"%s\", default_library_paths=\"%s\""
           " permitted_paths=\"%s\"] as the anonymous namespace",
           ns->get_name(),
           android::base::Join(ns->get_ld_library_paths(), ':').c_str(),
           android::base::Join(ns->get_default_library_paths(), ':').c_str(),
           android::base::Join(ns->get_permitted_paths(), ':').c_str());
    return nullptr;
  }

  return ns;
}

bool link_namespaces(android_namespace_t* namespace_from,
                     android_namespace_t* namespace_to,
                     const char* shared_lib_sonames) {
  if (namespace_to == nullptr) {
    namespace_to = &g_default_namespace;
  }

  if (namespace_from == nullptr) {
    DL_ERR("error linking namespaces: namespace_from is null.");
    return false;
  }

  if (shared_lib_sonames == nullptr || shared_lib_sonames[0] == '\0') {
    DL_ERR("error linking namespaces \"%s\"->\"%s\": the list of shared libraries is empty.",
           namespace_from->get_name(), namespace_to->get_name());
    return false;
  }

  auto sonames = android::base::Split(shared_lib_sonames, ":");
  std::unordered_set<std::string> sonames_set(sonames.begin(), sonames.end());

  ProtectedDataGuard guard;
  namespace_from->add_linked_namespace(namespace_to, sonames_set, false);

  return true;
}

bool link_namespaces_all_libs(android_namespace_t* namespace_from,
                              android_namespace_t* namespace_to) {
  if (namespace_from == nullptr) {
    DL_ERR("error linking namespaces: namespace_from is null.");
    return false;
  }

  if (namespace_to == nullptr) {
    DL_ERR("error linking namespaces: namespace_to is null.");
    return false;
  }

  ProtectedDataGuard guard;
  namespace_from->add_linked_namespace(namespace_to, std::unordered_set<std::string>(), true);

  return true;
}

ElfW(Addr) call_ifunc_resolver(ElfW(Addr) resolver_addr) {
  if (g_is_ldd) return 0;

  ElfW(Addr) ifunc_addr = __bionic_call_ifunc_resolver(resolver_addr);
  TRACE_TYPE(RELO, "Called ifunc_resolver@%p. The result is %p",
      reinterpret_cast<void *>(resolver_addr), reinterpret_cast<void*>(ifunc_addr));

  return ifunc_addr;
}

const version_info* VersionTracker::get_version_info(ElfW(Versym) source_symver) const {
  if (source_symver < 2 ||
      source_symver >= version_infos.size() ||
      version_infos[source_symver].name == nullptr) {
    return nullptr;
  }

  return &version_infos[source_symver];
}

void VersionTracker::add_version_info(size_t source_index,
                                      ElfW(Word) elf_hash,
                                      const char* ver_name,
                                      const soinfo* target_si) {
  if (source_index >= version_infos.size()) {
    version_infos.resize(source_index+1);
  }

  version_infos[source_index].elf_hash = elf_hash;
  version_infos[source_index].name = ver_name;
  version_infos[source_index].target_si = target_si;
}

bool VersionTracker::init_verneed(const soinfo* si_from) {
  uintptr_t verneed_ptr = si_from->get_verneed_ptr();

  if (verneed_ptr == 0) {
    return true;
  }

  size_t verneed_cnt = si_from->get_verneed_cnt();

  for (size_t i = 0, offset = 0; i<verneed_cnt; ++i) {
    const ElfW(Verneed)* verneed = reinterpret_cast<ElfW(Verneed)*>(verneed_ptr + offset);
    size_t vernaux_offset = offset + verneed->vn_aux;
    offset += verneed->vn_next;

    if (verneed->vn_version != 1) {
      DL_ERR("unsupported verneed[%zd] vn_version: %d (expected 1)", i, verneed->vn_version);
      return false;
    }

    const char* target_soname = si_from->get_string(verneed->vn_file);
    // find it in dependencies
    soinfo* target_si = si_from->get_children().find_if(
        [&](const soinfo* si) { return strcmp(si->get_soname(), target_soname) == 0; });

    if (target_si == nullptr) {
      DL_ERR("cannot find \"%s\" from verneed[%zd] in DT_NEEDED list for \"%s\"",
          target_soname, i, si_from->get_realpath());
      return false;
    }

    for (size_t j = 0; j<verneed->vn_cnt; ++j) {
      const ElfW(Vernaux)* vernaux = reinterpret_cast<ElfW(Vernaux)*>(verneed_ptr + vernaux_offset);
      vernaux_offset += vernaux->vna_next;

      const ElfW(Word) elf_hash = vernaux->vna_hash;
      const char* ver_name = si_from->get_string(vernaux->vna_name);
      ElfW(Half) source_index = vernaux->vna_other;

      add_version_info(source_index, elf_hash, ver_name, target_si);
    }
  }

  return true;
}

template <typename F>
static bool for_each_verdef(const soinfo* si, F functor) {
  if (!si->has_min_version(2)) {
    return true;
  }

  uintptr_t verdef_ptr = si->get_verdef_ptr();
  if (verdef_ptr == 0) {
    return true;
  }

  size_t offset = 0;

  size_t verdef_cnt = si->get_verdef_cnt();
  for (size_t i = 0; i<verdef_cnt; ++i) {
    const ElfW(Verdef)* verdef = reinterpret_cast<ElfW(Verdef)*>(verdef_ptr + offset);
    size_t verdaux_offset = offset + verdef->vd_aux;
    offset += verdef->vd_next;

    if (verdef->vd_version != 1) {
      DL_ERR("unsupported verdef[%zd] vd_version: %d (expected 1) library: %s",
          i, verdef->vd_version, si->get_realpath());
      return false;
    }

    if ((verdef->vd_flags & VER_FLG_BASE) != 0) {
      // "this is the version of the file itself.  It must not be used for
      //  matching a symbol. It can be used to match references."
      //
      // http://www.akkadia.org/drepper/symbol-versioning
      continue;
    }

    if (verdef->vd_cnt == 0) {
      DL_ERR("invalid verdef[%zd] vd_cnt == 0 (version without a name)", i);
      return false;
    }

    const ElfW(Verdaux)* verdaux = reinterpret_cast<ElfW(Verdaux)*>(verdef_ptr + verdaux_offset);

    if (functor(i, verdef, verdaux) == true) {
      break;
    }
  }

  return true;
}

ElfW(Versym) find_verdef_version_index(const soinfo* si, const version_info* vi) {
  if (vi == nullptr) {
    return kVersymNotNeeded;
  }

  ElfW(Versym) result = kVersymGlobal;

  if (!for_each_verdef(si,
    [&](size_t, const ElfW(Verdef)* verdef, const ElfW(Verdaux)* verdaux) {
      if (verdef->vd_hash == vi->elf_hash &&
          strcmp(vi->name, si->get_string(verdaux->vda_name)) == 0) {
        result = verdef->vd_ndx;
        return true;
      }

      return false;
    }
  )) {
    // verdef should have already been validated in prelink_image.
    async_safe_fatal("invalid verdef after prelinking: %s, %s",
                     si->get_realpath(), linker_get_error_buffer());
  }

  return result;
}

// Validate the library's verdef section. On error, returns false and invokes DL_ERR.
bool validate_verdef_section(const soinfo* si) {
  return for_each_verdef(si,
    [&](size_t, const ElfW(Verdef)*, const ElfW(Verdaux)*) {
      return false;
    });
}

bool VersionTracker::init_verdef(const soinfo* si_from) {
  return for_each_verdef(si_from,
    [&](size_t, const ElfW(Verdef)* verdef, const ElfW(Verdaux)* verdaux) {
      add_version_info(verdef->vd_ndx, verdef->vd_hash,
          si_from->get_string(verdaux->vda_name), si_from);
      return false;
    }
  );
}

bool VersionTracker::init(const soinfo* si_from) {
  if (!si_from->has_min_version(2)) {
    return true;
  }

  return init_verneed(si_from) && init_verdef(si_from);
}

// TODO (dimitry): Methods below need to be moved out of soinfo
// and in more isolated file in order minimize dependencies on
// unnecessary object in the linker binary. Consider making them
// independent from soinfo (?).
bool soinfo::lookup_version_info(const VersionTracker& version_tracker, ElfW(Word) sym,
                                 const char* sym_name, const version_info** vi) {
  const ElfW(Versym)* sym_ver_ptr = get_versym(sym);
  ElfW(Versym) sym_ver = sym_ver_ptr == nullptr ? 0 : *sym_ver_ptr;

  if (sym_ver != VER_NDX_LOCAL && sym_ver != VER_NDX_GLOBAL) {
    *vi = version_tracker.get_version_info(sym_ver);

    if (*vi == nullptr) {
      DL_ERR("cannot find verneed/verdef for version index=%d "
          "referenced by symbol \"%s\" at \"%s\"", sym_ver, sym_name, get_realpath());
      return false;
    }
  } else {
    // there is no version info
    *vi = nullptr;
  }

  return true;
}

void soinfo::apply_relr_reloc(ElfW(Addr) offset) {
  ElfW(Addr) address = offset + load_bias;
  *reinterpret_cast<ElfW(Addr)*>(address) += load_bias;
}

// Process relocations in SHT_RELR section (experimental).
// Details of the encoding are described in this post:
//   https://groups.google.com/d/msg/generic-abi/bX460iggiKg/Pi9aSwwABgAJ
bool soinfo::relocate_relr() {
  ElfW(Relr)* begin = relr_;
  ElfW(Relr)* end = relr_ + relr_count_;
  constexpr size_t wordsize = sizeof(ElfW(Addr));

  ElfW(Addr) base = 0;
  for (ElfW(Relr)* current = begin; current < end; ++current) {
    ElfW(Relr) entry = *current;
    ElfW(Addr) offset;

    if ((entry&1) == 0) {
      // Even entry: encodes the offset for next relocation.
      offset = static_cast<ElfW(Addr)>(entry);
      apply_relr_reloc(offset);
      // Set base offset for subsequent bitmap entries.
      base = offset + wordsize;
      continue;
    }

    // Odd entry: encodes bitmap for relocations starting at base.
    offset = base;
    while (entry != 0) {
      entry >>= 1;
      if ((entry&1) != 0) {
        apply_relr_reloc(offset);
      }
      offset += wordsize;
    }

    // Advance base offset by 63 words for 64-bit platforms,
    // or 31 words for 32-bit platforms.
    base += (8*wordsize - 1) * wordsize;
  }
  return true;
}

// An empty list of soinfos
static soinfo_list_t g_empty_list;

bool soinfo::prelink_image() {
  if (flags_ & FLAG_PRELINKED) return true;
  /* Extract dynamic section */
  ElfW(Word) dynamic_flags = 0;
  phdr_table_get_dynamic_section(phdr, phnum, load_bias, &dynamic, &dynamic_flags);

  /* We can't log anything until the linker is relocated */
  bool relocating_linker = (flags_ & FLAG_LINKER) != 0;
  if (!relocating_linker) {
    INFO("[ Linking \"%s\" ]", get_realpath());
    DEBUG("si->base = %p si->flags = 0x%08x", reinterpret_cast<void*>(base), flags_);
  }

  if (dynamic == nullptr) {
    if (!relocating_linker) {
      DL_ERR("missing PT_DYNAMIC in \"%s\"", get_realpath());
    }
    return false;
  } else {
    if (!relocating_linker) {
      DEBUG("dynamic = %p", dynamic);
    }
  }

#if defined(__arm__)
  (void) phdr_table_get_arm_exidx(phdr, phnum, load_bias,
                                  &ARM_exidx, &ARM_exidx_count);
#endif

  TlsSegment tls_segment;
  if (__bionic_get_tls_segment(phdr, phnum, load_bias, &tls_segment)) {
    if (!__bionic_check_tls_alignment(&tls_segment.alignment)) {
      if (!relocating_linker) {
        DL_ERR("TLS segment alignment in \"%s\" is not a power of 2: %zu",
               get_realpath(), tls_segment.alignment);
      }
      return false;
    }
    tls_ = std::make_unique<soinfo_tls>();
    tls_->segment = tls_segment;
  }

  // Extract useful information from dynamic section.
  // Note that: "Except for the DT_NULL element at the end of the array,
  // and the relative order of DT_NEEDED elements, entries may appear in any order."
  //
  // source: http://www.sco.com/developers/gabi/1998-04-29/ch5.dynamic.html
  uint32_t needed_count = 0;
  for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
    DEBUG("d = %p, d[0](tag) = %p d[1](val) = %p",
          d, reinterpret_cast<void*>(d->d_tag), reinterpret_cast<void*>(d->d_un.d_val));
    switch (d->d_tag) {
      case DT_SONAME:
        // this is parsed after we have strtab initialized (see below).
        break;

      case DT_HASH:
        nbucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[0];
        nchain_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[1];
        bucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr + 8);
        chain_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr + 8 + nbucket_ * 4);
        break;

      case DT_GNU_HASH:
        gnu_nbucket_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[0];
        // skip symndx
        gnu_maskwords_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[2];
        gnu_shift2_ = reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[3];

        gnu_bloom_filter_ = reinterpret_cast<ElfW(Addr)*>(load_bias + d->d_un.d_ptr + 16);
        gnu_bucket_ = reinterpret_cast<uint32_t*>(gnu_bloom_filter_ + gnu_maskwords_);
        // amend chain for symndx = header[1]
        gnu_chain_ = gnu_bucket_ + gnu_nbucket_ -
            reinterpret_cast<uint32_t*>(load_bias + d->d_un.d_ptr)[1];

        if (!powerof2(gnu_maskwords_)) {
          DL_ERR("invalid maskwords for gnu_hash = 0x%x, in \"%s\" expecting power to two",
              gnu_maskwords_, get_realpath());
          return false;
        }
        --gnu_maskwords_;

        flags_ |= FLAG_GNU_HASH;
        break;

      case DT_STRTAB:
        strtab_ = reinterpret_cast<const char*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_STRSZ:
        strtab_size_ = d->d_un.d_val;
        break;

      case DT_SYMTAB:
        symtab_ = reinterpret_cast<ElfW(Sym)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_SYMENT:
        if (d->d_un.d_val != sizeof(ElfW(Sym))) {
          DL_ERR("invalid DT_SYMENT: %zd in \"%s\"",
              static_cast<size_t>(d->d_un.d_val), get_realpath());
          return false;
        }
        break;

      case DT_PLTREL:
#if defined(USE_RELA)
        if (d->d_un.d_val != DT_RELA) {
          DL_ERR("unsupported DT_PLTREL in \"%s\"; expected DT_RELA", get_realpath());
          return false;
        }
#else
        if (d->d_un.d_val != DT_REL) {
          DL_ERR("unsupported DT_PLTREL in \"%s\"; expected DT_REL", get_realpath());
          return false;
        }
#endif
        break;

      case DT_JMPREL:
#if defined(USE_RELA)
        plt_rela_ = reinterpret_cast<ElfW(Rela)*>(load_bias + d->d_un.d_ptr);
#else
        plt_rel_ = reinterpret_cast<ElfW(Rel)*>(load_bias + d->d_un.d_ptr);
#endif
        break;

      case DT_PLTRELSZ:
#if defined(USE_RELA)
        plt_rela_count_ = d->d_un.d_val / sizeof(ElfW(Rela));
#else
        plt_rel_count_ = d->d_un.d_val / sizeof(ElfW(Rel));
#endif
        break;

      case DT_PLTGOT:
        // Ignored (because RTLD_LAZY is not supported).
        break;

      case DT_DEBUG:
        // Set the DT_DEBUG entry to the address of _r_debug for GDB
        // if the dynamic table is writable
        if ((dynamic_flags & PF_W) != 0) {
          d->d_un.d_val = reinterpret_cast<uintptr_t>(&_r_debug);
        }
        break;
#if defined(USE_RELA)
      case DT_RELA:
        rela_ = reinterpret_cast<ElfW(Rela)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_RELASZ:
        rela_count_ = d->d_un.d_val / sizeof(ElfW(Rela));
        break;

      case DT_ANDROID_RELA:
        android_relocs_ = reinterpret_cast<uint8_t*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_ANDROID_RELASZ:
        android_relocs_size_ = d->d_un.d_val;
        break;

      case DT_ANDROID_REL:
        DL_ERR("unsupported DT_ANDROID_REL in \"%s\"", get_realpath());
        return false;

      case DT_ANDROID_RELSZ:
        DL_ERR("unsupported DT_ANDROID_RELSZ in \"%s\"", get_realpath());
        return false;

      case DT_RELAENT:
        if (d->d_un.d_val != sizeof(ElfW(Rela))) {
          DL_ERR("invalid DT_RELAENT: %zd", static_cast<size_t>(d->d_un.d_val));
          return false;
        }
        break;

      // Ignored (see DT_RELCOUNT comments for details).
      case DT_RELACOUNT:
        break;

      case DT_REL:
        DL_ERR("unsupported DT_REL in \"%s\"", get_realpath());
        return false;

      case DT_RELSZ:
        DL_ERR("unsupported DT_RELSZ in \"%s\"", get_realpath());
        return false;

#else
      case DT_REL:
        rel_ = reinterpret_cast<ElfW(Rel)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_RELSZ:
        rel_count_ = d->d_un.d_val / sizeof(ElfW(Rel));
        break;

      case DT_RELENT:
        if (d->d_un.d_val != sizeof(ElfW(Rel))) {
          DL_ERR("invalid DT_RELENT: %zd", static_cast<size_t>(d->d_un.d_val));
          return false;
        }
        break;

      case DT_ANDROID_REL:
        android_relocs_ = reinterpret_cast<uint8_t*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_ANDROID_RELSZ:
        android_relocs_size_ = d->d_un.d_val;
        break;

      case DT_ANDROID_RELA:
        DL_ERR("unsupported DT_ANDROID_RELA in \"%s\"", get_realpath());
        return false;

      case DT_ANDROID_RELASZ:
        DL_ERR("unsupported DT_ANDROID_RELASZ in \"%s\"", get_realpath());
        return false;

      // "Indicates that all RELATIVE relocations have been concatenated together,
      // and specifies the RELATIVE relocation count."
      //
      // TODO: Spec also mentions that this can be used to optimize relocation process;
      // Not currently used by bionic linker - ignored.
      case DT_RELCOUNT:
        break;

      case DT_RELA:
        DL_ERR("unsupported DT_RELA in \"%s\"", get_realpath());
        return false;

      case DT_RELASZ:
        DL_ERR("unsupported DT_RELASZ in \"%s\"", get_realpath());
        return false;

#endif
      case DT_RELR:
      case DT_ANDROID_RELR:
        relr_ = reinterpret_cast<ElfW(Relr)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_RELRSZ:
      case DT_ANDROID_RELRSZ:
        relr_count_ = d->d_un.d_val / sizeof(ElfW(Relr));
        break;

      case DT_RELRENT:
      case DT_ANDROID_RELRENT:
        if (d->d_un.d_val != sizeof(ElfW(Relr))) {
          DL_ERR("invalid DT_RELRENT: %zd", static_cast<size_t>(d->d_un.d_val));
          return false;
        }
        break;

      // Ignored (see DT_RELCOUNT comments for details).
      // There is no DT_RELRCOUNT specifically because it would only be ignored.
      case DT_ANDROID_RELRCOUNT:
        break;

      case DT_INIT:
        init_func_ = reinterpret_cast<linker_ctor_function_t>(load_bias + d->d_un.d_ptr);
        DEBUG("%s constructors (DT_INIT) found at %p", get_realpath(), init_func_);
        break;

      case DT_FINI:
        fini_func_ = reinterpret_cast<linker_dtor_function_t>(load_bias + d->d_un.d_ptr);
        DEBUG("%s destructors (DT_FINI) found at %p", get_realpath(), fini_func_);
        break;

      case DT_INIT_ARRAY:
        init_array_ = reinterpret_cast<linker_ctor_function_t*>(load_bias + d->d_un.d_ptr);
        DEBUG("%s constructors (DT_INIT_ARRAY) found at %p", get_realpath(), init_array_);
        break;

      case DT_INIT_ARRAYSZ:
        init_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
        break;

      case DT_FINI_ARRAY:
        fini_array_ = reinterpret_cast<linker_dtor_function_t*>(load_bias + d->d_un.d_ptr);
        DEBUG("%s destructors (DT_FINI_ARRAY) found at %p", get_realpath(), fini_array_);
        break;

      case DT_FINI_ARRAYSZ:
        fini_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
        break;

      case DT_PREINIT_ARRAY:
        preinit_array_ = reinterpret_cast<linker_ctor_function_t*>(load_bias + d->d_un.d_ptr);
        DEBUG("%s constructors (DT_PREINIT_ARRAY) found at %p", get_realpath(), preinit_array_);
        break;

      case DT_PREINIT_ARRAYSZ:
        preinit_array_count_ = static_cast<uint32_t>(d->d_un.d_val) / sizeof(ElfW(Addr));
        break;

      case DT_TEXTREL:
#if defined(__LP64__)
        DL_ERR("\"%s\" has text relocations", get_realpath());
        return false;
#else
        has_text_relocations = true;
        break;
#endif

      case DT_SYMBOLIC:
        has_DT_SYMBOLIC = true;
        break;

      case DT_NEEDED:
        ++needed_count;
        break;

      case DT_FLAGS:
        if (d->d_un.d_val & DF_TEXTREL) {
#if defined(__LP64__)
          DL_ERR("\"%s\" has text relocations", get_realpath());
          return false;
#else
          has_text_relocations = true;
#endif
        }
        if (d->d_un.d_val & DF_SYMBOLIC) {
          has_DT_SYMBOLIC = true;
        }
        break;

      case DT_FLAGS_1:
        set_dt_flags_1(d->d_un.d_val);

        if ((d->d_un.d_val & ~SUPPORTED_DT_FLAGS_1) != 0) {
          DL_WARN("Warning: \"%s\" has unsupported flags DT_FLAGS_1=%p "
                  "(ignoring unsupported flags)",
                  get_realpath(), reinterpret_cast<void*>(d->d_un.d_val));
        }
        break;

      // Ignored: "Its use has been superseded by the DF_BIND_NOW flag"
      case DT_BIND_NOW:
        break;

      case DT_VERSYM:
        versym_ = reinterpret_cast<ElfW(Versym)*>(load_bias + d->d_un.d_ptr);
        break;

      case DT_VERDEF:
        verdef_ptr_ = load_bias + d->d_un.d_ptr;
        break;
      case DT_VERDEFNUM:
        verdef_cnt_ = d->d_un.d_val;
        break;

      case DT_VERNEED:
        verneed_ptr_ = load_bias + d->d_un.d_ptr;
        break;

      case DT_VERNEEDNUM:
        verneed_cnt_ = d->d_un.d_val;
        break;

      case DT_RUNPATH:
        // this is parsed after we have strtab initialized (see below).
        break;

      case DT_TLSDESC_GOT:
      case DT_TLSDESC_PLT:
        // These DT entries are used for lazy TLSDESC relocations. Bionic
        // resolves everything eagerly, so these can be ignored.
        break;

#if defined(__aarch64__)
      case DT_AARCH64_BTI_PLT:
      case DT_AARCH64_PAC_PLT:
      case DT_AARCH64_VARIANT_PCS:
        // Ignored: AArch64 processor-specific dynamic array tags.
        break;
#endif

      default:
        if (!relocating_linker) {
          const char* tag_name;
          if (d->d_tag == DT_RPATH) {
            tag_name = "DT_RPATH";
          } else if (d->d_tag == DT_ENCODING) {
            tag_name = "DT_ENCODING";
          } else if (d->d_tag >= DT_LOOS && d->d_tag <= DT_HIOS) {
            tag_name = "unknown OS-specific";
          } else if (d->d_tag >= DT_LOPROC && d->d_tag <= DT_HIPROC) {
            tag_name = "unknown processor-specific";
          } else {
            tag_name = "unknown";
          }
          DL_WARN("Warning: \"%s\" unused DT entry: %s (type %p arg %p) (ignoring)",
                  get_realpath(),
                  tag_name,
                  reinterpret_cast<void*>(d->d_tag),
                  reinterpret_cast<void*>(d->d_un.d_val));
        }
        break;
    }
  }

  DEBUG("si->base = %p, si->strtab = %p, si->symtab = %p",
        reinterpret_cast<void*>(base), strtab_, symtab_);

  // Validity checks.
  if (relocating_linker && needed_count != 0) {
    DL_ERR("linker cannot have DT_NEEDED dependencies on other libraries");
    return false;
  }
  if (nbucket_ == 0 && gnu_nbucket_ == 0) {
    DL_ERR("empty/missing DT_HASH/DT_GNU_HASH in \"%s\" "
        "(new hash type from the future?)", get_realpath());
    return false;
  }
  if (strtab_ == nullptr) {
    DL_ERR("empty/missing DT_STRTAB in \"%s\"", get_realpath());
    return false;
  }
  if (symtab_ == nullptr) {
    DL_ERR("empty/missing DT_SYMTAB in \"%s\"", get_realpath());
    return false;
  }

  // Second pass - parse entries relying on strtab. Skip this while relocating the linker so as to
  // avoid doing heap allocations until later in the linker's initialization.
  if (!relocating_linker) {
    for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
      switch (d->d_tag) {
        case DT_SONAME:
          set_soname(get_string(d->d_un.d_val));
          break;
        case DT_RUNPATH:
          set_dt_runpath(get_string(d->d_un.d_val));
          break;
      }
    }
  }

  // Before M release, linker was using basename in place of soname. In the case when DT_SONAME is
  // absent some apps stop working because they can't find DT_NEEDED library by soname. This
  // workaround should keep them working. (Applies only for apps targeting sdk version < M.) Make
  // an exception for the main executable, which does not need to have DT_SONAME. The linker has an
  // DT_SONAME but the soname_ field is initialized later on.
  if (soname_.empty() && this != solist_get_somain() && !relocating_linker &&
      get_application_target_sdk_version() < 23) {
    soname_ = basename(realpath_.c_str());
    DL_WARN_documented_change(23, "missing-soname-enforced-for-api-level-23",
                              "\"%s\" has no DT_SONAME (will use %s instead)", get_realpath(),
                              soname_.c_str());

    // Don't call add_dlwarning because a missing DT_SONAME isn't important enough to show in the UI
  }

  // Validate each library's verdef section once, so we don't have to validate
  // it each time we look up a symbol with a version.
  if (!validate_verdef_section(this)) return false;

  flags_ |= FLAG_PRELINKED;
  return true;
}

bool soinfo::link_image(const SymbolLookupList& lookup_list, soinfo* local_group_root,
                        const android_dlextinfo* extinfo, size_t* relro_fd_offset) {
  if (is_image_linked()) {
    // already linked.
    return true;
  }

  if (g_is_ldd && !is_main_executable()) {
    async_safe_format_fd(STDOUT_FILENO, "\t%s => %s (%p)\n", get_soname(),
                         get_realpath(), reinterpret_cast<void*>(base));
  }

  local_group_root_ = local_group_root;
  if (local_group_root_ == nullptr) {
    local_group_root_ = this;
  }

  if ((flags_ & FLAG_LINKER) == 0 && local_group_root_ == this) {
    target_sdk_version_ = get_application_target_sdk_version();
  }

#if !defined(__LP64__)
  if (has_text_relocations) {
    // Fail if app is targeting M or above.
    int app_target_api_level = get_application_target_sdk_version();
    if (app_target_api_level >= 23) {
      DL_ERR_AND_LOG("\"%s\" has text relocations (%s#Text-Relocations-Enforced-for-API-level-23)",
                     get_realpath(), kBionicChangesUrl);
      return false;
    }
    // Make segments writable to allow text relocations to work properly. We will later call
    // phdr_table_protect_segments() after all of them are applied.
    DL_WARN_documented_change(23,
                              "Text-Relocations-Enforced-for-API-level-23",
                              "\"%s\" has text relocations",
                              get_realpath());
    add_dlwarning(get_realpath(), "text relocations");
    if (phdr_table_unprotect_segments(phdr, phnum, load_bias) < 0) {
      DL_ERR("can't unprotect loadable segments for \"%s\": %s", get_realpath(), strerror(errno));
      return false;
    }
  }
#endif

  if (!relocate(lookup_list)) {
    return false;
  }

  DEBUG("[ finished linking %s ]", get_realpath());

#if !defined(__LP64__)
  if (has_text_relocations) {
    // All relocations are done, we can protect our segments back to read-only.
    if (phdr_table_protect_segments(phdr, phnum, load_bias) < 0) {
      DL_ERR("can't protect segments for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  }
#endif

  // We can also turn on GNU RELRO protection if we're not linking the dynamic linker
  // itself --- it can't make system calls yet, and will have to call protect_relro later.
  if (!is_linker() && !protect_relro()) {
    return false;
  }

  /* Handle serializing/sharing the RELRO segment */
  if (extinfo && (extinfo->flags & ANDROID_DLEXT_WRITE_RELRO)) {
    if (phdr_table_serialize_gnu_relro(phdr, phnum, load_bias,
                                       extinfo->relro_fd, relro_fd_offset) < 0) {
      DL_ERR("failed serializing GNU RELRO section for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  } else if (extinfo && (extinfo->flags & ANDROID_DLEXT_USE_RELRO)) {
    if (phdr_table_map_gnu_relro(phdr, phnum, load_bias,
                                 extinfo->relro_fd, relro_fd_offset) < 0) {
      DL_ERR("failed mapping GNU RELRO section for \"%s\": %s",
             get_realpath(), strerror(errno));
      return false;
    }
  }

  ++g_module_load_counter;
  notify_gdb_of_load(this);
  set_image_linked();
  return true;
}

bool soinfo::protect_relro() {
  if (phdr_table_protect_gnu_relro(phdr, phnum, load_bias) < 0) {
    DL_ERR("can't enable GNU RELRO protection for \"%s\": %s",
           get_realpath(), strerror(errno));
    return false;
  }
  return true;
}

static std::vector<android_namespace_t*> init_default_namespace_no_config(bool is_asan) {
  g_default_namespace.set_isolated(false);
  auto default_ld_paths = is_asan ? kAsanDefaultLdPaths : kDefaultLdPaths;

  char real_path[PATH_MAX];
  std::vector<std::string> ld_default_paths;
  for (size_t i = 0; default_ld_paths[i] != nullptr; ++i) {
    if (realpath(default_ld_paths[i], real_path) != nullptr) {
      ld_default_paths.push_back(real_path);
    } else {
      ld_default_paths.push_back(default_ld_paths[i]);
    }
  }

  g_default_namespace.set_default_library_paths(std::move(ld_default_paths));

  std::vector<android_namespace_t*> namespaces;
  namespaces.push_back(&g_default_namespace);
  return namespaces;
}

// Given an `executable_path` starting with "/apex/<name>/bin/, return
// "/linkerconfig/<name>/ld.config.txt", which is the auto-generated config file for the APEX by the
// linkerconfig tool.
static std::string get_ld_config_file_apex_path(const char* executable_path) {
  std::vector<std::string> paths = android::base::Split(executable_path, "/");
  if (paths.size() >= 5 && paths[1] == "apex" && paths[3] == "bin") {
    std::string generated_apex_config = "/linkerconfig/" + paths[2] + "/ld.config.txt";
    if (file_exists(generated_apex_config.c_str())) {
      return generated_apex_config;
    }
  }
  return "";
}

static std::string get_ld_config_file_vndk_path() {
  if (android::base::GetBoolProperty("ro.vndk.lite", false)) {
    return kLdConfigVndkLiteFilePath;
  }

  std::string ld_config_file_vndk = kLdConfigFilePath;
  size_t insert_pos = ld_config_file_vndk.find_last_of('.');
  if (insert_pos == std::string::npos) {
    insert_pos = ld_config_file_vndk.length();
  }
  ld_config_file_vndk.insert(insert_pos, Config::get_vndk_version_string('.'));
  return ld_config_file_vndk;
}

bool is_linker_config_expected(const char* executable_path) {
  // Do not raise message from a host environment which is expected to miss generated linker
  // configuration.
#if !defined(__ANDROID__)
  return false;
#endif

  if (strcmp(executable_path, "/system/bin/init") == 0) {
    // Generated linker configuration can be missed from processes executed
    // with init binary
    return false;
  }

  return true;
}

static std::string get_ld_config_file_path(const char* executable_path) {
#ifdef USE_LD_CONFIG_FILE
  // This is a debugging/testing only feature. Must not be available on
  // production builds.
  const char* ld_config_file_env = getenv("LD_CONFIG_FILE");
  if (ld_config_file_env != nullptr && file_exists(ld_config_file_env)) {
    return ld_config_file_env;
  }
#endif

  std::string path = get_ld_config_file_apex_path(executable_path);
  if (!path.empty()) {
    if (file_exists(path.c_str())) {
      return path;
    }
    DL_WARN("Warning: couldn't read config file \"%s\" for \"%s\"",
            path.c_str(), executable_path);
  }

  path = kLdConfigArchFilePath;
  if (file_exists(path.c_str())) {
    return path;
  }

  if (file_exists(kLdGeneratedConfigFilePath)) {
    return kLdGeneratedConfigFilePath;
  }

  if (is_linker_config_expected(executable_path)) {
    DL_WARN("Warning: failed to find generated linker configuration from \"%s\"",
            kLdGeneratedConfigFilePath);
  }

  path = get_ld_config_file_vndk_path();
  if (file_exists(path.c_str())) {
    return path;
  }

  return kLdConfigFilePath;
}

std::vector<android_namespace_t*> init_default_namespaces(const char* executable_path) {
  g_default_namespace.set_name("(default)");

  soinfo* somain = solist_get_somain();

  const char *interp = phdr_table_get_interpreter_name(somain->phdr, somain->phnum,
                                                       somain->load_bias);
  const char* bname = (interp != nullptr) ? basename(interp) : nullptr;

  g_is_asan = bname != nullptr &&
              (strcmp(bname, "linker_asan") == 0 ||
               strcmp(bname, "linker_asan64") == 0);

  const Config* config = nullptr;

  {
    std::string ld_config_file_path = get_ld_config_file_path(executable_path);
    INFO("[ Reading linker config \"%s\" ]", ld_config_file_path.c_str());
    ScopedTrace trace(("linker config " + ld_config_file_path).c_str());
    std::string error_msg;
    if (!Config::read_binary_config(ld_config_file_path.c_str(), executable_path, g_is_asan,
                                    &config, &error_msg)) {
      if (!error_msg.empty()) {
        DL_WARN("Warning: couldn't read '%s' for '%s' (using default configuration instead): %s",
                ld_config_file_path.c_str(), executable_path, error_msg.c_str());
      }
      config = nullptr;
    }
  }

  if (config == nullptr) {
    return init_default_namespace_no_config(g_is_asan);
  }

  const auto& namespace_configs = config->namespace_configs();
  std::unordered_map<std::string, android_namespace_t*> namespaces;

  // 1. Initialize default namespace
  const NamespaceConfig* default_ns_config = config->default_namespace_config();

  g_default_namespace.set_isolated(default_ns_config->isolated());
  g_default_namespace.set_default_library_paths(default_ns_config->search_paths());
  g_default_namespace.set_permitted_paths(default_ns_config->permitted_paths());

  namespaces[default_ns_config->name()] = &g_default_namespace;
  if (default_ns_config->visible()) {
    g_exported_namespaces[default_ns_config->name()] = &g_default_namespace;
  }

  // 2. Initialize other namespaces

  for (auto& ns_config : namespace_configs) {
    if (namespaces.find(ns_config->name()) != namespaces.end()) {
      continue;
    }

    android_namespace_t* ns = new (g_namespace_allocator.alloc()) android_namespace_t();
    ns->set_name(ns_config->name());
    ns->set_isolated(ns_config->isolated());
    ns->set_default_library_paths(ns_config->search_paths());
    ns->set_permitted_paths(ns_config->permitted_paths());
    ns->set_allowed_libs(ns_config->allowed_libs());

    namespaces[ns_config->name()] = ns;
    if (ns_config->visible()) {
      g_exported_namespaces[ns_config->name()] = ns;
    }
  }

  // 3. Establish links between namespaces
  for (auto& ns_config : namespace_configs) {
    auto it_from = namespaces.find(ns_config->name());
    CHECK(it_from != namespaces.end());
    android_namespace_t* namespace_from = it_from->second;
    for (const NamespaceLinkConfig& ns_link : ns_config->links()) {
      auto it_to = namespaces.find(ns_link.ns_name());
      CHECK(it_to != namespaces.end());
      android_namespace_t* namespace_to = it_to->second;
      if (ns_link.allow_all_shared_libs()) {
        link_namespaces_all_libs(namespace_from, namespace_to);
      } else {
        link_namespaces(namespace_from, namespace_to, ns_link.shared_libs().c_str());
      }
    }
  }
  // we can no longer rely on the fact that libdl.so is part of default namespace
  // this is why we want to add ld-android.so to all namespaces from ld.config.txt
  soinfo* ld_android_so = solist_get_head();

  // we also need vdso to be available for all namespaces (if present)
  soinfo* vdso = solist_get_vdso();
  for (auto it : namespaces) {
    if (it.second != &g_default_namespace) {
      it.second->add_soinfo(ld_android_so);
      if (vdso != nullptr) {
        it.second->add_soinfo(vdso);
      }
      // somain and ld_preloads are added to these namespaces after LD_PRELOAD libs are linked
    }
  }

  set_application_target_sdk_version(config->target_sdk_version());

  std::vector<android_namespace_t*> created_namespaces;
  created_namespaces.reserve(namespaces.size());
  for (const auto& kv : namespaces) {
    created_namespaces.push_back(kv.second);
  }
  return created_namespaces;
}

// This function finds a namespace exported in ld.config.txt by its name.
// A namespace can be exported by setting .visible property to true.
android_namespace_t* get_exported_namespace(const char* name) {
  if (name == nullptr) {
    return nullptr;
  }
  auto it = g_exported_namespaces.find(std::string(name));
  if (it == g_exported_namespaces.end()) {
    return nullptr;
  }
  return it->second;
}

void purge_unused_memory() {
  // For now, we only purge the memory used by LoadTask because we know those
  // are temporary objects.
  //
  // Purging other LinkerBlockAllocator hardly yields much because they hold
  // information about namespaces and opened libraries, which are not freed
  // when the control leaves the linker.
  //
  // Purging BionicAllocator may give us a few dirty pages back, but those pages
  // would be already zeroed out, so they compress easily in ZRAM.  Therefore,
  // it is not worth munmap()'ing those pages.
  TypeBasedAllocator<LoadTask>::purge();
}
