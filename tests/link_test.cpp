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

TEST(link, dl_iterate_phdr_early_exit) {
  static size_t call_count = 0;
  ASSERT_EQ(123, dl_iterate_phdr([](dl_phdr_info*, size_t, void*) { ++call_count; return 123; },
                                 nullptr));
  ASSERT_EQ(1u, call_count);
}

TEST(link, dl_iterate_phdr) {
  struct Functor {
    static int Callback(dl_phdr_info* i, size_t s, void* data) {
      reinterpret_cast<Functor*>(data)->DoChecks(i, s);
      return 0;
    }
    void DoChecks(dl_phdr_info* info, size_t s) {
      ASSERT_EQ(sizeof(dl_phdr_info), s);

      ASSERT_TRUE(info->dlpi_name != nullptr);

      // Find the first PT_LOAD program header so we can find the ELF header.
      for (ElfW(Half) i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)* phdr = reinterpret_cast<const ElfW(Phdr)*>(&info->dlpi_phdr[i]);
        if (phdr->p_type == PT_LOAD) {
          const ElfW(Ehdr)* ehdr = reinterpret_cast<const ElfW(Ehdr)*>(info->dlpi_addr +
                                                                       phdr->p_vaddr);
          // Does it look like an ELF file?
          ASSERT_EQ(0, memcmp(ehdr, ELFMAG, SELFMAG));
          // Does the e_phnum match what dl_iterate_phdr told us?
          ASSERT_EQ(info->dlpi_phnum, ehdr->e_phnum);
          break;
        }
      }
    }
    size_t count;
  } f = {};
  ASSERT_EQ(0, dl_iterate_phdr(Functor::Callback, &f));
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
