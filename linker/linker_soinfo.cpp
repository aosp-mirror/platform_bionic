/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "linker_soinfo.h"

#include <dlfcn.h>
#include <elf.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <async_safe/log.h>

#include "linker.h"
#include "linker_config.h"
#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_gnu_hash.h"
#include "linker_logger.h"
#include "linker_relocate.h"
#include "linker_utils.h"

// Enable the slow lookup path if symbol lookups should be logged.
static bool is_lookup_tracing_enabled() {
  return g_ld_debug_verbosity > LINKER_VERBOSITY_TRACE && DO_TRACE_LOOKUP;
}

SymbolLookupList::SymbolLookupList(soinfo* si)
    : sole_lib_(si->get_lookup_lib()), begin_(&sole_lib_), end_(&sole_lib_ + 1) {
  CHECK(si != nullptr);
  slow_path_count_ += is_lookup_tracing_enabled();
  slow_path_count_ += sole_lib_.needs_sysv_lookup();
}

SymbolLookupList::SymbolLookupList(const soinfo_list_t& global_group, const soinfo_list_t& local_group) {
  slow_path_count_ += is_lookup_tracing_enabled();
  libs_.reserve(1 + global_group.size() + local_group.size());

  // Reserve a space in front for DT_SYMBOLIC lookup.
  libs_.push_back(SymbolLookupLib {});

  global_group.for_each([this](soinfo* si) {
    libs_.push_back(si->get_lookup_lib());
    slow_path_count_ += libs_.back().needs_sysv_lookup();
  });

  local_group.for_each([this](soinfo* si) {
    libs_.push_back(si->get_lookup_lib());
    slow_path_count_ += libs_.back().needs_sysv_lookup();
  });

  begin_ = &libs_[1];
  end_ = &libs_[0] + libs_.size();
}

/* "This element's presence in a shared object library alters the dynamic linker's
 * symbol resolution algorithm for references within the library. Instead of starting
 * a symbol search with the executable file, the dynamic linker starts from the shared
 * object itself. If the shared object fails to supply the referenced symbol, the
 * dynamic linker then searches the executable file and other shared objects as usual."
 *
 * http://www.sco.com/developers/gabi/2012-12-31/ch5.dynamic.html
 *
 * Note that this is unlikely since static linker avoids generating
 * relocations for -Bsymbolic linked dynamic executables.
 */
void SymbolLookupList::set_dt_symbolic_lib(soinfo* lib) {
  CHECK(!libs_.empty());
  slow_path_count_ -= libs_[0].needs_sysv_lookup();
  libs_[0] = lib ? lib->get_lookup_lib() : SymbolLookupLib();
  slow_path_count_ += libs_[0].needs_sysv_lookup();
  begin_ = lib ? &libs_[0] : &libs_[1];
}

// Check whether a requested version matches the version on a symbol definition. There are a few
// special cases:
//  - If the defining DSO has no version info at all, then any version matches.
//  - If no version is requested (vi==nullptr, verneed==kVersymNotNeeded), then any non-hidden
//    version matches.
//  - If the requested version is not defined by the DSO, then verneed is kVersymGlobal, and only
//    global symbol definitions match. (This special case is handled as part of the ordinary case
//    where the version must match exactly.)
static inline bool check_symbol_version(const ElfW(Versym)* ver_table, uint32_t sym_idx,
                                        const ElfW(Versym) verneed) {
  if (ver_table == nullptr) return true;
  const uint32_t verdef = ver_table[sym_idx];
  return (verneed == kVersymNotNeeded) ?
      !(verdef & kVersymHiddenBit) :
      verneed == (verdef & ~kVersymHiddenBit);
}

template <bool IsGeneral>
__attribute__((noinline)) static const ElfW(Sym)*
soinfo_do_lookup_impl(const char* name, const version_info* vi,
                      soinfo** si_found_in, const SymbolLookupList& lookup_list) {
  const auto [ hash, name_len ] = calculate_gnu_hash(name);
  constexpr uint32_t kBloomMaskBits = sizeof(ElfW(Addr)) * 8;
  SymbolName elf_symbol_name(name);

  const SymbolLookupLib* end = lookup_list.end();
  const SymbolLookupLib* it = lookup_list.begin();

  while (true) {
    const SymbolLookupLib* lib;
    uint32_t sym_idx;

    // Iterate over libraries until we find one whose Bloom filter matches the symbol we're
    // searching for.
    while (true) {
      if (it == end) return nullptr;
      lib = it++;

      if (IsGeneral && lib->needs_sysv_lookup()) {
        if (const ElfW(Sym)* sym = lib->si_->find_symbol_by_name(elf_symbol_name, vi)) {
          *si_found_in = lib->si_;
          return sym;
        }
        continue;
      }

      if (IsGeneral) {
        TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p (gnu)",
                   name, lib->si_->get_realpath(), reinterpret_cast<void*>(lib->si_->base));
      }

      const uint32_t word_num = (hash / kBloomMaskBits) & lib->gnu_maskwords_;
      const ElfW(Addr) bloom_word = lib->gnu_bloom_filter_[word_num];
      const uint32_t h1 = hash % kBloomMaskBits;
      const uint32_t h2 = (hash >> lib->gnu_shift2_) % kBloomMaskBits;

      if ((1 & (bloom_word >> h1) & (bloom_word >> h2)) == 1) {
        sym_idx = lib->gnu_bucket_[hash % lib->gnu_nbucket_];
        if (sym_idx != 0) {
          break;
        }
      }

      if (IsGeneral) {
        TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
                   name, lib->si_->get_realpath(), reinterpret_cast<void*>(lib->si_->base));
      }
    }

    // Search the library's hash table chain.
    ElfW(Versym) verneed = kVersymNotNeeded;
    bool calculated_verneed = false;

    uint32_t chain_value = 0;
    const ElfW(Sym)* sym = nullptr;

    do {
      sym = lib->symtab_ + sym_idx;
      chain_value = lib->gnu_chain_[sym_idx];
      if ((chain_value >> 1) == (hash >> 1)) {
        if (vi != nullptr && !calculated_verneed) {
          calculated_verneed = true;
          verneed = find_verdef_version_index(lib->si_, vi);
        }
        if (check_symbol_version(lib->versym_, sym_idx, verneed) &&
            static_cast<size_t>(sym->st_name) + name_len + 1 <= lib->strtab_size_ &&
            memcmp(lib->strtab_ + sym->st_name, name, name_len + 1) == 0 &&
            is_symbol_global_and_defined(lib->si_, sym)) {
          *si_found_in = lib->si_;
          if (IsGeneral) {
            TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
                       name, lib->si_->get_realpath(), reinterpret_cast<void*>(sym->st_value),
                       static_cast<size_t>(sym->st_size));
          }
          return sym;
        }
      }
      ++sym_idx;
    } while ((chain_value & 1) == 0);

    if (IsGeneral) {
      TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
                 name, lib->si_->get_realpath(), reinterpret_cast<void*>(lib->si_->base));
    }
  }
}

const ElfW(Sym)* soinfo_do_lookup(const char* name, const version_info* vi,
                                  soinfo** si_found_in, const SymbolLookupList& lookup_list) {
  return lookup_list.needs_slow_path() ?
      soinfo_do_lookup_impl<true>(name, vi, si_found_in, lookup_list) :
      soinfo_do_lookup_impl<false>(name, vi, si_found_in, lookup_list);
}

soinfo::soinfo(android_namespace_t* ns, const char* realpath, const struct stat* file_stat,
               off64_t file_offset, int rtld_flags) {
  if (realpath != nullptr) {
    realpath_ = realpath;
  }

  flags_ = FLAG_NEW_SOINFO;
  version_ = SOINFO_VERSION;

  if (file_stat != nullptr) {
    this->st_dev_ = file_stat->st_dev;
    this->st_ino_ = file_stat->st_ino;
    this->file_offset_ = file_offset;
  }

  this->rtld_flags_ = rtld_flags;
  this->primary_namespace_ = ns;
}

soinfo::~soinfo() {
  g_soinfo_handles_map.erase(handle_);
}

void soinfo::set_dt_runpath(const char* path) {
  if (!has_min_version(3)) {
    return;
  }

  std::vector<std::string> runpaths;

  split_path(path, ":", &runpaths);

  std::string origin = dirname(get_realpath());
  // FIXME: add $PLATFORM.
  std::vector<std::pair<std::string, std::string>> params = {
    {"ORIGIN", origin},
    {"LIB", kLibPath},
  };
  for (auto&& s : runpaths) {
    format_string(&s, params);
  }

  resolve_paths(runpaths, &dt_runpath_);
}

const ElfW(Versym)* soinfo::get_versym(size_t n) const {
  auto table = get_versym_table();
  return table ? table + n : nullptr;
}

ElfW(Addr) soinfo::get_verneed_ptr() const {
  if (has_min_version(2)) {
    return verneed_ptr_;
  }

  return 0;
}

size_t soinfo::get_verneed_cnt() const {
  if (has_min_version(2)) {
    return verneed_cnt_;
  }

  return 0;
}

ElfW(Addr) soinfo::get_verdef_ptr() const {
  if (has_min_version(2)) {
    return verdef_ptr_;
  }

  return 0;
}

size_t soinfo::get_verdef_cnt() const {
  if (has_min_version(2)) {
    return verdef_cnt_;
  }

  return 0;
}

SymbolLookupLib soinfo::get_lookup_lib() {
  SymbolLookupLib result {};
  result.si_ = this;

  // For libs that only have SysV hashes, leave the gnu_bloom_filter_ field NULL to signal that
  // the fallback code path is needed.
  if (!is_gnu_hash()) {
    return result;
  }

  result.gnu_maskwords_ = gnu_maskwords_;
  result.gnu_shift2_ = gnu_shift2_;
  result.gnu_bloom_filter_ = gnu_bloom_filter_;

  result.strtab_ = strtab_;
  result.strtab_size_ = strtab_size_;
  result.symtab_ = symtab_;
  result.versym_ = get_versym_table();

  result.gnu_chain_ = gnu_chain_;
  result.gnu_nbucket_ = gnu_nbucket_;
  result.gnu_bucket_ = gnu_bucket_;

  return result;
}

const ElfW(Sym)* soinfo::find_symbol_by_name(SymbolName& symbol_name,
                                             const version_info* vi) const {
  return is_gnu_hash() ? gnu_lookup(symbol_name, vi) : elf_lookup(symbol_name, vi);
}

const ElfW(Sym)* soinfo::gnu_lookup(SymbolName& symbol_name, const version_info* vi) const {
  const uint32_t hash = symbol_name.gnu_hash();

  constexpr uint32_t kBloomMaskBits = sizeof(ElfW(Addr)) * 8;
  const uint32_t word_num = (hash / kBloomMaskBits) & gnu_maskwords_;
  const ElfW(Addr) bloom_word = gnu_bloom_filter_[word_num];
  const uint32_t h1 = hash % kBloomMaskBits;
  const uint32_t h2 = (hash >> gnu_shift2_) % kBloomMaskBits;

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p (gnu)",
      symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  // test against bloom filter
  if ((1 & (bloom_word >> h1) & (bloom_word >> h2)) == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return nullptr;
  }

  // bloom test says "probably yes"...
  uint32_t n = gnu_bucket_[hash % gnu_nbucket_];

  if (n == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return nullptr;
  }

  const ElfW(Versym) verneed = find_verdef_version_index(this, vi);
  const ElfW(Versym)* versym = get_versym_table();

  do {
    ElfW(Sym)* s = symtab_ + n;
    if (((gnu_chain_[n] ^ hash) >> 1) == 0 &&
        check_symbol_version(versym, n, verneed) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(this, s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
          symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(s->st_value),
          static_cast<size_t>(s->st_size));
      return symtab_ + n;
    }
  } while ((gnu_chain_[n++] & 1) == 0);

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
             symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  return nullptr;
}

const ElfW(Sym)* soinfo::elf_lookup(SymbolName& symbol_name, const version_info* vi) const {
  uint32_t hash = symbol_name.elf_hash();

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p h=%x(elf) %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  const ElfW(Versym) verneed = find_verdef_version_index(this, vi);
  const ElfW(Versym)* versym = get_versym_table();

  for (uint32_t n = bucket_[hash % nbucket_]; n != 0; n = chain_[n]) {
    ElfW(Sym)* s = symtab_ + n;

    if (check_symbol_version(versym, n, verneed) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(this, s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
                 symbol_name.get_name(), get_realpath(),
                 reinterpret_cast<void*>(s->st_value),
                 static_cast<size_t>(s->st_size));
      return symtab_ + n;
    }
  }

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p %x %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  return nullptr;
}

ElfW(Sym)* soinfo::find_symbol_by_address(const void* addr) {
  return is_gnu_hash() ? gnu_addr_lookup(addr) : elf_addr_lookup(addr);
}

static bool symbol_matches_soaddr(const ElfW(Sym)* sym, ElfW(Addr) soaddr) {
  // Skip TLS symbols. A TLS symbol's value is relative to the start of the TLS segment rather than
  // to the start of the solib. The solib only reserves space for the initialized part of the TLS
  // segment. (i.e. .tdata is followed by .tbss, and .tbss overlaps other sections.)
  return sym->st_shndx != SHN_UNDEF &&
      ELF_ST_TYPE(sym->st_info) != STT_TLS &&
      soaddr >= sym->st_value &&
      soaddr < sym->st_value + sym->st_size;
}

ElfW(Sym)* soinfo::gnu_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  for (size_t i = 0; i < gnu_nbucket_; ++i) {
    uint32_t n = gnu_bucket_[i];

    if (n == 0) {
      continue;
    }

    do {
      ElfW(Sym)* sym = symtab_ + n;
      if (symbol_matches_soaddr(sym, soaddr)) {
        return sym;
      }
    } while ((gnu_chain_[n++] & 1) == 0);
  }

  return nullptr;
}

ElfW(Sym)* soinfo::elf_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  // Search the library's symbol table for any defined symbol which
  // contains this address.
  for (size_t i = 0; i < nchain_; ++i) {
    ElfW(Sym)* sym = symtab_ + i;
    if (symbol_matches_soaddr(sym, soaddr)) {
      return sym;
    }
  }

  return nullptr;
}

static void call_function(const char* function_name __unused,
                          linker_ctor_function_t function,
                          const char* realpath __unused) {
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
  function(g_argc, g_argv, g_envp);
  TRACE("[ Done calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

static void call_function(const char* function_name __unused,
                          linker_dtor_function_t function,
                          const char* realpath __unused) {
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
  function();
  TRACE("[ Done calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

template <typename F>
static inline void call_array(const char* array_name __unused, F* functions, size_t count,
                              bool reverse, const char* realpath) {
  if (functions == nullptr) {
    return;
  }

  TRACE("[ Calling %s (size %zd) @ %p for '%s' ]", array_name, count, functions, realpath);

  int begin = reverse ? (count - 1) : 0;
  int end = reverse ? -1 : count;
  int step = reverse ? -1 : 1;

  for (int i = begin; i != end; i += step) {
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);
    call_function("function", functions[i], realpath);
  }

  TRACE("[ Done calling %s for '%s' ]", array_name, realpath);
}

void soinfo::call_pre_init_constructors() {
  if (g_is_ldd) return;

  // DT_PREINIT_ARRAY functions are called before any other constructors for executables,
  // but ignored in a shared library.
  call_array("DT_PREINIT_ARRAY", preinit_array_, preinit_array_count_, false, get_realpath());
}

void soinfo::call_constructors() {
  if (constructors_called || g_is_ldd) {
    return;
  }

  // We set constructors_called before actually calling the constructors, otherwise it doesn't
  // protect against recursive constructor calls. One simple example of constructor recursion
  // is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:
  // 1. The program depends on libc, so libc's constructor is called here.
  // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
  // 3. dlopen() calls the constructors on the newly created
  //    soinfo for libc_malloc_debug_leak.so.
  // 4. The debug .so depends on libc, so CallConstructors is
  //    called again with the libc soinfo. If it doesn't trigger the early-
  //    out above, the libc constructor will be called again (recursively!).
  constructors_called = true;

  if (!is_main_executable() && preinit_array_ != nullptr) {
    // The GNU dynamic linker silently ignores these, but we warn the developer.
    PRINT("\"%s\": ignoring DT_PREINIT_ARRAY in shared library!", get_realpath());
  }

  get_children().for_each([] (soinfo* si) {
    si->call_constructors();
  });

  if (!is_linker()) {
    bionic_trace_begin((std::string("calling constructors: ") + get_realpath()).c_str());
  }

  // DT_INIT should be called before DT_INIT_ARRAY if both are present.
  call_function("DT_INIT", init_func_, get_realpath());
  call_array("DT_INIT_ARRAY", init_array_, init_array_count_, false, get_realpath());

  if (!is_linker()) {
    bionic_trace_end();
  }
}

void soinfo::call_destructors() {
  if (!constructors_called) {
    return;
  }

  ScopedTrace trace((std::string("calling destructors: ") + get_realpath()).c_str());

  // DT_FINI_ARRAY must be parsed in reverse order.
  call_array("DT_FINI_ARRAY", fini_array_, fini_array_count_, true, get_realpath());

  // DT_FINI should be called after DT_FINI_ARRAY if both are present.
  call_function("DT_FINI", fini_func_, get_realpath());
}

void soinfo::add_child(soinfo* child) {
  if (has_min_version(0)) {
    child->parents_.push_back(this);
    this->children_.push_back(child);
  }
}

void soinfo::remove_all_links() {
  if (!has_min_version(0)) {
    return;
  }

  // 1. Untie connected soinfos from 'this'.
  children_.for_each([&] (soinfo* child) {
    child->parents_.remove_if([&] (const soinfo* parent) {
      return parent == this;
    });
  });

  parents_.for_each([&] (soinfo* parent) {
    parent->children_.remove_if([&] (const soinfo* child) {
      return child == this;
    });
  });

  // 2. Remove from the primary namespace
  primary_namespace_->remove_soinfo(this);
  primary_namespace_ = nullptr;

  // 3. Remove from secondary namespaces
  secondary_namespaces_.for_each([&](android_namespace_t* ns) {
    ns->remove_soinfo(this);
  });


  // 4. Once everything untied - clear local lists.
  parents_.clear();
  children_.clear();
  secondary_namespaces_.clear();
}

dev_t soinfo::get_st_dev() const {
  if (has_min_version(0)) {
    return st_dev_;
  }

  return 0;
};

ino_t soinfo::get_st_ino() const {
  if (has_min_version(0)) {
    return st_ino_;
  }

  return 0;
}

off64_t soinfo::get_file_offset() const {
  if (has_min_version(1)) {
    return file_offset_;
  }

  return 0;
}

uint32_t soinfo::get_rtld_flags() const {
  if (has_min_version(1)) {
    return rtld_flags_;
  }

  return 0;
}

uint32_t soinfo::get_dt_flags_1() const {
  if (has_min_version(1)) {
    return dt_flags_1_;
  }

  return 0;
}

void soinfo::set_dt_flags_1(uint32_t dt_flags_1) {
  if (has_min_version(1)) {
    if ((dt_flags_1 & DF_1_GLOBAL) != 0) {
      rtld_flags_ |= RTLD_GLOBAL;
    }

    if ((dt_flags_1 & DF_1_NODELETE) != 0) {
      rtld_flags_ |= RTLD_NODELETE;
    }

    dt_flags_1_ = dt_flags_1;
  }
}

void soinfo::set_nodelete() {
  rtld_flags_ |= RTLD_NODELETE;
}

void soinfo::set_realpath(const char* path) {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    realpath_ = path;
  }
#else
  realpath_ = path;
#endif
}

const char* soinfo::get_realpath() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return realpath_.c_str();
  } else {
    return old_name_;
  }
#else
  return realpath_.c_str();
#endif
}

void soinfo::set_soname(const char* soname) {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    soname_ = soname;
  }
  strlcpy(old_name_, soname_.c_str(), sizeof(old_name_));
#else
  soname_ = soname;
#endif
}

const char* soinfo::get_soname() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return soname_.c_str();
  } else {
    return old_name_;
  }
#else
  return soname_.c_str();
#endif
}

// This is a return on get_children()/get_parents() if
// 'this->flags' does not have FLAG_NEW_SOINFO set.
static soinfo_list_t g_empty_list;

soinfo_list_t& soinfo::get_children() {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

const soinfo_list_t& soinfo::get_children() const {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

soinfo_list_t& soinfo::get_parents() {
  if (has_min_version(0)) {
    return parents_;
  }

  return g_empty_list;
}

static std::vector<std::string> g_empty_runpath;

const std::vector<std::string>& soinfo::get_dt_runpath() const {
  if (has_min_version(3)) {
    return dt_runpath_;
  }

  return g_empty_runpath;
}

android_namespace_t* soinfo::get_primary_namespace() {
  if (has_min_version(3)) {
    return primary_namespace_;
  }

  return &g_default_namespace;
}

void soinfo::add_secondary_namespace(android_namespace_t* secondary_ns) {
  CHECK(has_min_version(3));
  secondary_namespaces_.push_back(secondary_ns);
}

android_namespace_list_t& soinfo::get_secondary_namespaces() {
  CHECK(has_min_version(3));
  return secondary_namespaces_;
}

const char* soinfo::get_string(ElfW(Word) index) const {
  if (has_min_version(1) && (index >= strtab_size_)) {
    async_safe_fatal("%s: strtab out of bounds error; STRSZ=%zd, name=%d",
        get_realpath(), strtab_size_, index);
  }

  return strtab_ + index;
}

bool soinfo::is_gnu_hash() const {
  return (flags_ & FLAG_GNU_HASH) != 0;
}

bool soinfo::can_unload() const {
  return !is_linked() ||
         (
             (get_rtld_flags() & (RTLD_NODELETE | RTLD_GLOBAL)) == 0
         );
}

bool soinfo::is_linked() const {
  return (flags_ & FLAG_LINKED) != 0;
}

bool soinfo::is_image_linked() const {
  return (flags_ & FLAG_IMAGE_LINKED) != 0;
}

bool soinfo::is_main_executable() const {
  return (flags_ & FLAG_EXE) != 0;
}

bool soinfo::is_linker() const {
  return (flags_ & FLAG_LINKER) != 0;
}

void soinfo::set_linked() {
  flags_ |= FLAG_LINKED;
}

void soinfo::set_image_linked() {
  flags_ |= FLAG_IMAGE_LINKED;
}

void soinfo::set_linker_flag() {
  flags_ |= FLAG_LINKER;
}

void soinfo::set_main_executable() {
  flags_ |= FLAG_EXE;
}

size_t soinfo::increment_ref_count() {
  return ++local_group_root_->ref_count_;
}

size_t soinfo::decrement_ref_count() {
  return --local_group_root_->ref_count_;
}

size_t soinfo::get_ref_count() const {
  return local_group_root_->ref_count_;
}

soinfo* soinfo::get_local_group_root() const {
  return local_group_root_;
}

void soinfo::set_mapped_by_caller(bool mapped_by_caller) {
  if (mapped_by_caller) {
    flags_ |= FLAG_MAPPED_BY_CALLER;
  } else {
    flags_ &= ~FLAG_MAPPED_BY_CALLER;
  }
}

bool soinfo::is_mapped_by_caller() const {
  return (flags_ & FLAG_MAPPED_BY_CALLER) != 0;
}

// This function returns api-level at the time of
// dlopen/load. Note that libraries opened by system
// will always have 'current' api level.
int soinfo::get_target_sdk_version() const {
  if (!has_min_version(2)) {
    return __ANDROID_API__;
  }

  return local_group_root_->target_sdk_version_;
}

uintptr_t soinfo::get_handle() const {
  CHECK(has_min_version(3));
  CHECK(handle_ != 0);
  return handle_;
}

void* soinfo::to_handle() {
  if (get_application_target_sdk_version() < 24 || !has_min_version(3)) {
    return this;
  }

  return reinterpret_cast<void*>(get_handle());
}

void soinfo::generate_handle() {
  CHECK(has_min_version(3));
  CHECK(handle_ == 0); // Make sure this is the first call

  // Make sure the handle is unique and does not collide
  // with special values which are RTLD_DEFAULT and RTLD_NEXT.
  do {
    if (!is_first_stage_init()) {
      arc4random_buf(&handle_, sizeof(handle_));
    } else {
      // arc4random* is not available in init because /dev/urandom hasn't yet been
      // created. So, when running with init, use the monotonically increasing
      // numbers as handles
      handle_ += 2;
    }
    // the least significant bit for the handle is always 1
    // making it easy to test the type of handle passed to
    // dl* functions.
    handle_ = handle_ | 1;
  } while (handle_ == reinterpret_cast<uintptr_t>(RTLD_DEFAULT) ||
           handle_ == reinterpret_cast<uintptr_t>(RTLD_NEXT) ||
           g_soinfo_handles_map.find(handle_) != g_soinfo_handles_map.end());

  g_soinfo_handles_map[handle_] = this;
}

void soinfo::set_gap_start(ElfW(Addr) gap_start) {
  CHECK(has_min_version(6));
  gap_start_ = gap_start;
}
ElfW(Addr) soinfo::get_gap_start() const {
  CHECK(has_min_version(6));
  return gap_start_;
}

void soinfo::set_gap_size(size_t gap_size) {
  CHECK(has_min_version(6));
  gap_size_ = gap_size;
}
size_t soinfo::get_gap_size() const {
  CHECK(has_min_version(6));
  return gap_size_;
}

// TODO(dimitry): Move SymbolName methods to a separate file.

uint32_t calculate_elf_hash(const char* name) {
  const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(name);
  uint32_t h = 0, g;

  while (*name_bytes) {
    h = (h << 4) + *name_bytes++;
    g = h & 0xf0000000;
    h ^= g;
    h ^= g >> 24;
  }

  return h;
}

uint32_t SymbolName::elf_hash() {
  if (!has_elf_hash_) {
    elf_hash_ = calculate_elf_hash(name_);
    has_elf_hash_ = true;
  }

  return elf_hash_;
}

uint32_t SymbolName::gnu_hash() {
  if (!has_gnu_hash_) {
    gnu_hash_ = calculate_gnu_hash(name_).first;
    has_gnu_hash_ = true;
  }

  return gnu_hash_;
}
