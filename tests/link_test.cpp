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

#include <gtest/gtest.h>

#include <link.h>
#if __has_include(<sys/auxv.h>)
#include <sys/auxv.h>
#endif

#include <string>
#include <unordered_map>

TEST(link, dl_iterate_phdr_early_exit) {
  static size_t call_count = 0;
  ASSERT_EQ(123, dl_iterate_phdr([](dl_phdr_info*, size_t, void*) { ++call_count; return 123; },
                                 nullptr));
  ASSERT_EQ(1u, call_count);
}

TEST(link, dl_iterate_phdr) {
  struct Functor {
    static int Callback(dl_phdr_info* i, size_t s, void* data) {
      static_cast<Functor*>(data)->DoChecks(i, s);
      return 0;
    }
    void DoChecks(dl_phdr_info* info, size_t s) {
      ASSERT_EQ(sizeof(dl_phdr_info), s);

      ASSERT_TRUE(info->dlpi_name != nullptr);

      // An ELF file must have at least a PT_LOAD program header.
      ASSERT_NE(nullptr, info->dlpi_phdr);
      ASSERT_NE(0, info->dlpi_phnum);

      // Find the first PT_LOAD program header so we can find the ELF header.
      bool found_load = false;
      for (ElfW(Half) i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)* phdr = reinterpret_cast<const ElfW(Phdr)*>(&info->dlpi_phdr[i]);
        if (phdr->p_type == PT_LOAD) {
          const ElfW(Ehdr)* ehdr = reinterpret_cast<const ElfW(Ehdr)*>(info->dlpi_addr +
                                                                       phdr->p_vaddr);
          // Does it look like an ELF file?
          ASSERT_EQ(0, memcmp(ehdr, ELFMAG, SELFMAG));
          // Does the e_phnum match what dl_iterate_phdr told us?
          ASSERT_EQ(info->dlpi_phnum, ehdr->e_phnum);
          found_load = true;
          break;
        }
      }
      ASSERT_EQ(true, found_load);
    }
    size_t count;
  } f = {};
  ASSERT_EQ(0, dl_iterate_phdr(Functor::Callback, &f));
}

struct ProgHdr {
  const ElfW(Phdr)* table;
  size_t size;
};

__attribute__((__unused__))
static ElfW(Addr) find_exe_load_bias(const ProgHdr& phdr) {
  for (size_t i = 0; i < phdr.size; ++i) {
    if (phdr.table[i].p_type == PT_PHDR) {
      return reinterpret_cast<ElfW(Addr)>(phdr.table) - phdr.table[i].p_vaddr;
    }
  }
  return 0;
}

__attribute__((__unused__))
static ElfW(Dyn)* find_dynamic(const ProgHdr& phdr, ElfW(Addr) load_bias) {
  for (size_t i = 0; i < phdr.size; ++i) {
    if (phdr.table[i].p_type == PT_DYNAMIC) {
      return reinterpret_cast<ElfW(Dyn)*>(phdr.table[i].p_vaddr + load_bias);
    }
  }
  return nullptr;
}

__attribute__((__unused__))
static r_debug* find_exe_r_debug(ElfW(Dyn)* dynamic) {
  for (ElfW(Dyn)* d = dynamic; d->d_tag != DT_NULL; ++d) {
    if (d->d_tag == DT_DEBUG) {
      return reinterpret_cast<r_debug*>(d->d_un.d_val);
    }
  }
  return nullptr;
}

// Walk the DT_DEBUG/_r_debug global module list and compare it with the same
// information from dl_iterate_phdr. Verify that the executable appears first
// in _r_debug.
TEST(link, r_debug) {
#if __has_include(<sys/auxv.h>)
  // Find the executable's PT_DYNAMIC segment and DT_DEBUG value. The linker
  // will write the address of its _r_debug global into the .dynamic section.
  ProgHdr exe_phdr = {
    .table = reinterpret_cast<ElfW(Phdr)*>(getauxval(AT_PHDR)),
    .size = getauxval(AT_PHNUM)
  };
  ASSERT_NE(nullptr, exe_phdr.table);
  ElfW(Addr) exe_load_bias = find_exe_load_bias(exe_phdr);
  ASSERT_NE(0u, exe_load_bias);
  ElfW(Dyn)* exe_dynamic = find_dynamic(exe_phdr, exe_load_bias);
  ASSERT_NE(nullptr, exe_dynamic);
  r_debug* dbg = find_exe_r_debug(exe_dynamic);
  ASSERT_NE(nullptr, dbg);

  // Use dl_iterate_phdr to build a table mapping from load bias values to
  // solib names and PT_DYNAMIC segments.
  struct DlIterateInfo {
    std::string name;
    ElfW(Dyn)* dynamic;
  };
  struct Functor {
    std::unordered_map<ElfW(Addr), DlIterateInfo> dl_iter_mods;
    static int Callback(dl_phdr_info* i, size_t s, void* data) {
      static_cast<Functor*>(data)->AddModule(i, s);
      return 0;
    }
    void AddModule(dl_phdr_info* info, size_t s) {
      ASSERT_EQ(sizeof(dl_phdr_info), s);
      ASSERT_TRUE(dl_iter_mods.find(info->dlpi_addr) == dl_iter_mods.end());
      ASSERT_TRUE(info->dlpi_name != nullptr);
      dl_iter_mods[info->dlpi_addr] = {
        .name = info->dlpi_name,
        .dynamic = find_dynamic({ info->dlpi_phdr, info->dlpi_phnum }, info->dlpi_addr)
      };
    }
  } f = {};
  ASSERT_EQ(0, dl_iterate_phdr(Functor::Callback, &f));

  size_t map_size = 0;

  for (link_map* map = dbg->r_map; map != nullptr; map = map->l_next) {
    ASSERT_NE(0u, map->l_addr);
    ASSERT_NE(nullptr, map->l_ld);
    ASSERT_NE(nullptr, map->l_name);

    auto it = f.dl_iter_mods.find(map->l_addr);
    ASSERT_TRUE(it != f.dl_iter_mods.end());
    const DlIterateInfo& info = it->second;
    ASSERT_EQ(info.name, map->l_name);
    ASSERT_EQ(info.dynamic, map->l_ld);

    ++map_size;
  }

  // _r_debug and dl_iterate_phdr should report the same set of modules. We
  // verified above that every _r_debug module was reported by dl_iterate_phdr,
  // so checking the sizes verifies the converse.
  ASSERT_EQ(f.dl_iter_mods.size(), map_size);

  // Make sure the first entry is the executable. gdbserver assumes this and
  // removes the first entry from its list of shared objects that it sends back
  // to gdb.
  ASSERT_EQ(exe_load_bias, dbg->r_map->l_addr);
  ASSERT_EQ(exe_dynamic, dbg->r_map->l_ld);
#endif
}

#if __arm__
static uintptr_t read_exidx_func(uintptr_t* entry) {
  int32_t offset = *entry;
  // Sign-extend from int31 to int32.
  if ((offset & 0x40000000) != 0) {
    offset += -0x7fffffff - 1;
  }
  return reinterpret_cast<uintptr_t>(entry) + offset;
}
__attribute__((__unused__)) static void another_function_in_same_ELF_file() {}
#endif

TEST(link, dl_unwind_find_exidx) {
#if __arm__
  int count = 0;
  struct eit_entry_t {
    uintptr_t one;
    uintptr_t two;
  };
  eit_entry_t* entries = reinterpret_cast<eit_entry_t*>(dl_unwind_find_exidx(
      reinterpret_cast<_Unwind_Ptr>(read_exidx_func), &count));
  ASSERT_TRUE(entries != nullptr);
  ASSERT_GT(count, 0);

  // Sanity checks
  uintptr_t func = reinterpret_cast<uintptr_t>(read_exidx_func);
  bool found = false;
  for (int i = 0; i < count; ++i) {
    // Entries must have bit 31 clear.
    ASSERT_TRUE((entries[i].one & (1<<31)) == 0);

    uintptr_t exidx_func = read_exidx_func(&entries[i].one);

    // If our function is compiled for thumb, exception table contains our address - 1.
    if (func == exidx_func || func == exidx_func + 1) found = true;

    // Entries must be sorted. Some addresses may appear twice if function
    // is compiled for arm.
    if (i > 0) {
      EXPECT_GE(exidx_func, read_exidx_func(&entries[i - 1].one)) << i;
    }
  }
  ASSERT_TRUE(found);
#else
  GTEST_LOG_(INFO) << "dl_unwind_find_exidx is an ARM-only API\n";
#endif
}
