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

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "linker.h"
#include "linker_globals.h"
#include "linker_note_gnu_property.h"
#include "platform/bionic/macros.h"

#define SONAME "test_so"

static char error_buffer[1024];

char* linker_get_error_buffer() {
  return error_buffer;
}

size_t linker_get_error_buffer_size() {
  return std::size(error_buffer);
}

static void reset_error_buffer() {
  error_buffer[0] = '\0';
}

platform_properties g_platform_properties {
#if defined(__aarch64__)
  // Assume "hardware" supports Armv8.5-A BTI.
  .bti_supported = true
#endif
};

// Helper macro to make the test cleaner.
#define PHDR_WITH_NOTE_GNU_PROPERTY(__prop)                                   \
  reset_error_buffer();                                                       \
  ElfW(Phdr) phdrs[] = {                                                      \
      {.p_type = PT_LOAD},                                                    \
      {                                                                       \
          .p_type = PT_GNU_PROPERTY,                                          \
          .p_vaddr = reinterpret_cast<ElfW(Addr)>(__prop),                    \
          .p_memsz = sizeof(ElfW(NhdrGNUProperty)) + (__prop)->nhdr.n_descsz, \
      },                                                                      \
      {.p_type = PT_NULL},                                                    \
  };                                                                          \
  auto note = GnuPropertySection(&phdrs[0], std::size(phdrs), 0, SONAME)

// Helper to check for no error message.
#define ASSERT_NO_ERROR_MSG() ASSERT_STREQ(error_buffer, "")

// Helper to check expected error message.
#define ASSERT_ERROR_MSG_EQ(__expected) ASSERT_STREQ(error_buffer, "\"" SONAME "\" " __expected)

static void test_bti_not_supported(GnuPropertySection& note __unused) {
#if defined(__aarch64__)
  ASSERT_FALSE(note.IsBTICompatible());
#endif
}

#if defined(__aarch64__)
static void test_bti_supported(GnuPropertySection& note __unused) {
  ASSERT_TRUE(note.IsBTICompatible());
}
#endif

// Helper class to build a well-formed .note.gnu.property section.
class GnuPropertySectionBuilder {
 public:
  GnuPropertySectionBuilder() {
    note = reinterpret_cast<ElfW(NhdrGNUProperty)*>(&section[0]);
    note->nhdr.n_namesz = 4;
    note->nhdr.n_descsz = 0;
    note->nhdr.n_type = NT_GNU_PROPERTY_TYPE_0;
    memcpy(note->n_name, "GNU", 4);
  }

  template <typename T>
  bool push(ElfW(Word) pr_type, ElfW(Word) pr_datasz, const T* pr_data) {
    // Must be aligned.
    const uintptr_t addition = align_up(pr_datasz, sizeof(ElfW(Addr)));
    if ((offset() + addition) > kMaxSectionSize) {
      return false;
    }
    ++entries;
    ElfW(Prop)* prop = reinterpret_cast<ElfW(Prop)*>(&section[offset()]);
    // Header
    prop->pr_type = pr_type;
    prop->pr_datasz = pr_datasz;
    step(2 * sizeof(ElfW(Word)));
    // Data
    memcpy(&section[offset()], reinterpret_cast<const void*>(pr_data), pr_datasz);
    step(pr_datasz);
    // Padding
    memset(&section[offset()], 0xAA, addition - pr_datasz);
    step(addition - pr_datasz);
    return true;
  }

  ElfW(NhdrGNUProperty)* data() const { return note; }

  void dump() const {
    std::cout << ".note.gnu.property\n";
    dump_member("n_namesz", note->nhdr.n_namesz);
    dump_member("n_descsz", note->nhdr.n_descsz);
    dump_member("n_type  ", note->nhdr.n_type);
    dump_member("n_name  ", note->n_name);
    dump_member("entries ", entries);
    if (entries > 0) {
      std::cout << "    raw data:";
      const uintptr_t end = note->nhdr.n_descsz + 16;
      for (uintptr_t offset = 16; offset < end; ++offset) {
        std::cout << std::hex;
        if ((offset % 8) == 0) {
          std::cout << "\n   ";
        }
        auto value = static_cast<unsigned>(section[offset]);
        std::cout << " ";
        if (value < 0x10) {
          std::cout << "0";
        }
        std::cout << static_cast<unsigned>(section[offset]);
      }
      std::cout << std::dec << "\n";
    }
  }

  void corrupt_n_descsz(ElfW(Word) n_descsz) { note->nhdr.n_descsz = n_descsz; }

 private:
  template <typename T>
  void dump_member(const char* name, T value) const {
    std::cout << "  " << name << " " << value << "\n";
  }

  ElfW(Word) offset() const { return note->nhdr.n_descsz + 16; }

  template <typename T>
  void step(T value) {
    note->nhdr.n_descsz += static_cast<ElfW(Word)>(value);
  }

  static const size_t kMaxSectionSize = 1024;

  alignas(8) uint8_t section[kMaxSectionSize];
  ElfW(NhdrGNUProperty)* note;
  size_t entries = 0;
};

// Tests that the default constructed instance does not report support
// for Armv8.5-A BTI.
TEST(note_gnu_property, default) {
  GnuPropertySection note;
  test_bti_not_supported(note);
  ASSERT_NO_ERROR_MSG();
}

// Tests that an instance without valid phdr pointer does not report
// support for Armv8.5-A BTI.
TEST(note_gnu_property, phdr_null) {
  auto note = GnuPropertySection(nullptr, 0, 0, SONAME);
  test_bti_not_supported(note);
  ASSERT_NO_ERROR_MSG();
}

// Tests that an instance without finding PT_GNU_PROPERTY does not
// report support for Armv8.5-A BTI.
TEST(note_gnu_property, no_pt_gnu_property) {
  ElfW(Phdr) phdrs[] = {
      {.p_type = PT_LOAD},
      {.p_type = PT_NULL},
  };

  reset_error_buffer();
  auto note = GnuPropertySection(&phdrs[0], std::size(phdrs), 0, SONAME);
  test_bti_not_supported(note);
  ASSERT_NO_ERROR_MSG();
}

// Tests the validity check for invalid PT_GNU_PROPERTY size.
TEST(note_gnu_property, pt_gnu_property_bad_size) {
  ElfW(Phdr) phdrs[] = {
      {.p_type = PT_LOAD},
      {
          .p_type = PT_GNU_PROPERTY,
          .p_vaddr = 0,
          .p_memsz = sizeof(ElfW(NhdrGNUProperty)) - 1,  // Invalid
      },
      {.p_type = PT_NULL},
  };

  reset_error_buffer();
  auto note = GnuPropertySection(&phdrs[0], std::size(phdrs), 0, SONAME);
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ("PT_GNU_PROPERTY segment is too small. Segment size is 15, minimum is 16.");
}

// Tests that advertised n_descsz should still fit into p_memsz.
TEST(note_gnu_property, pt_gnu_property_too_small) {
  ElfW(NhdrGNUProperty) prop = {
      .nhdr = {.n_namesz = PT_GNU_PROPERTY, .n_descsz = 1, .n_type = NT_GNU_PROPERTY_TYPE_0},
      .n_name = "GNU",
  };
  ElfW(Phdr) phdrs[] = {
      {
          .p_type = PT_GNU_PROPERTY,
          .p_vaddr = reinterpret_cast<ElfW(Addr)>(&prop),
          .p_memsz = sizeof(ElfW(NhdrGNUProperty)),  // Off by one
      },
  };

  reset_error_buffer();
  auto note = GnuPropertySection(&phdrs[0], std::size(phdrs), 0, SONAME);
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ("PT_GNU_PROPERTY segment p_memsz (16) is too small for note n_descsz (1).");
}

// Tests the validity check for invalid .note.gnu.property type.
TEST(note_gnu_property, pt_gnu_property_bad_type) {
  ElfW(NhdrGNUProperty) prop = {
      .nhdr =
          {
              .n_namesz = 4,
              .n_descsz = 0,
              .n_type = NT_GNU_PROPERTY_TYPE_0 - 1  // Invalid
          },
      .n_name = "GNU",
  };
  PHDR_WITH_NOTE_GNU_PROPERTY(&prop);
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ(".note.gnu.property: unexpected note type. Expected 5, got 4.");
}

// Tests the validity check for invalid .note.gnu.property name size.
TEST(note_gnu_property, pt_gnu_property_bad_namesz) {
  ElfW(NhdrGNUProperty) prop = {
      .nhdr = {.n_namesz = 3,  // Invalid
               .n_descsz = 0,
               .n_type = NT_GNU_PROPERTY_TYPE_0},
      .n_name = "GNU",
  };
  PHDR_WITH_NOTE_GNU_PROPERTY(&prop);
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ(".note.gnu.property: unexpected name size. Expected 4, got 3.");
}

// Tests the validity check for invalid .note.gnu.property name.
TEST(note_gnu_property, pt_gnu_property_bad_name) {
  ElfW(NhdrGNUProperty) prop = {
      .nhdr = {.n_namesz = 4, .n_descsz = 0, .n_type = NT_GNU_PROPERTY_TYPE_0},
      .n_name = "ABC",  // Invalid
  };
  PHDR_WITH_NOTE_GNU_PROPERTY(&prop);
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ(".note.gnu.property: unexpected name. Expected 'GNU', got 'ABC'.");
}

// Tests the validity check for not enough space for a Program Property header.
TEST(note_gnu_property, pt_gnu_property_pphdr_no_space) {
  ElfW(NhdrGNUProperty) prop = {
      .nhdr = {.n_namesz = 4,
               .n_descsz = 7,  // Invalid
               .n_type = NT_GNU_PROPERTY_TYPE_0},
      .n_name = "GNU",
  };
  PHDR_WITH_NOTE_GNU_PROPERTY(&prop);
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ(".note.gnu.property: no more space left for a Program Property Note header.");
}

// Tests an empty .note.gnu.property.
TEST(note_gnu_property, pt_gnu_property_no_data) {
  GnuPropertySectionBuilder prop;
  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  test_bti_not_supported(note);
  ASSERT_NO_ERROR_MSG();
}

// Tests a .note.gnu.property section with elements with pr_datasz = 0.
TEST(note_gnu_property, pt_gnu_property_no_prop) {
  GnuPropertySectionBuilder prop;
  ASSERT_TRUE(prop.push(1, 0, (void*)nullptr));
  ASSERT_TRUE(prop.push(2, 0, (void*)nullptr));
  ASSERT_TRUE(prop.push(3, 0, (void*)nullptr));
  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  test_bti_not_supported(note);
  ASSERT_NO_ERROR_MSG();
}

// Tests that GNU_PROPERTY_AARCH64_FEATURE_1_AND must have pr_datasz = 4.
TEST(note_gnu_property, pt_gnu_property_bad_pr_datasz) {
#if defined(__aarch64__)
  GnuPropertySectionBuilder prop;
  ElfW(Word) pr_data[] = {GNU_PROPERTY_AARCH64_FEATURE_1_BTI, 0, 0};
  ASSERT_TRUE(prop.push(GNU_PROPERTY_AARCH64_FEATURE_1_AND, 12, &pr_data));
  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  test_bti_not_supported(note);
  ASSERT_ERROR_MSG_EQ(
      ".note.gnu.property: property descriptor size is invalid. Expected 4 bytes for "
      "GNU_PROPERTY_AARCH64_FEATURE_1_AND, got 12.");
#else
  GTEST_SKIP() << "BTI is not supported on this architecture.";
#endif
}

// Tests a .note.gnu.property section with only GNU_PROPERTY_AARCH64_FEATURE_1_BTI property array.
TEST(note_gnu_property, pt_gnu_property_ok_1) {
#if defined(__aarch64__)
  GnuPropertySectionBuilder prop;
  ElfW(Word) pr_data[] = {GNU_PROPERTY_AARCH64_FEATURE_1_BTI};
  ASSERT_TRUE(prop.push(GNU_PROPERTY_AARCH64_FEATURE_1_AND, sizeof(pr_data), &pr_data));
  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  ASSERT_NO_ERROR_MSG();
  test_bti_supported(note);
#else
  GTEST_SKIP() << "BTI is not supported on this architecture.";
#endif
}

// Tests a .note.gnu.property section with only GNU_PROPERTY_AARCH64_FEATURE_1_BTI property array.
TEST(note_gnu_property, pt_gnu_property_ok_2) {
#if defined(__aarch64__)
  GnuPropertySectionBuilder prop;
  ElfW(Word) pr_data[] = {static_cast<ElfW(Word)>(~GNU_PROPERTY_AARCH64_FEATURE_1_BTI)};
  ASSERT_TRUE(prop.push(GNU_PROPERTY_AARCH64_FEATURE_1_AND, sizeof(pr_data), &pr_data));
  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  ASSERT_NO_ERROR_MSG();
  test_bti_not_supported(note);
#else
  GTEST_SKIP() << "BTI is not supported on this architecture.";
#endif
}

// Tests a .note.gnu.property section with more property arrays.
TEST(note_gnu_property, pt_gnu_property_ok_3) {
#if defined(__aarch64__)
  GnuPropertySectionBuilder prop;

  ElfW(Word) pr_data_0[8] = {0xCD};
  ASSERT_TRUE(prop.push(1, 4, &pr_data_0));
  ASSERT_TRUE(prop.push(2, 3, &pr_data_0));
  ASSERT_TRUE(prop.push(3, 8, &pr_data_0));

  ElfW(Word) pr_data[] = {GNU_PROPERTY_AARCH64_FEATURE_1_BTI};
  ASSERT_TRUE(prop.push(GNU_PROPERTY_AARCH64_FEATURE_1_AND, sizeof(pr_data), &pr_data));

  ASSERT_TRUE(prop.push(4, 1, &pr_data_0));

  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  ASSERT_NO_ERROR_MSG();
  test_bti_supported(note);
#else
  GTEST_SKIP() << "BTI is not supported on this architecture.";
#endif
}

// Tests a .note.gnu.property but with bad property descriptor size.
TEST(note_gnu_property, pt_gnu_property_bad_n_descsz) {
#if defined(__aarch64__)
  GnuPropertySectionBuilder prop;
  ElfW(Word) pr_data[] = {GNU_PROPERTY_AARCH64_FEATURE_1_BTI};
  ASSERT_TRUE(prop.push(GNU_PROPERTY_AARCH64_FEATURE_1_AND, sizeof(pr_data), &pr_data));

  ElfW(Word) n_descsz;
  if (sizeof(ElfW(Addr)) == 4) {
    n_descsz = 11;
  } else {
    n_descsz = 15;
  }

  prop.corrupt_n_descsz(n_descsz);

  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  if (sizeof(ElfW(Addr)) == 4) {
    ASSERT_ERROR_MSG_EQ(
        ".note.gnu.property: property descriptor size is invalid. Expected at least 12 bytes, got "
        "11.");
  } else {
    ASSERT_ERROR_MSG_EQ(
        ".note.gnu.property: property descriptor size is invalid. Expected at least 16 bytes, got "
        "15.");
  }
  test_bti_not_supported(note);
#else
  GTEST_SKIP() << "BTI is not supported on this architecture.";
#endif
}

// Tests if platform support is missing.
TEST(note_gnu_property, no_platform_support) {
#if defined(__aarch64__)
  auto bti_supported_orig = g_platform_properties.bti_supported;
  g_platform_properties.bti_supported = false;

  GnuPropertySectionBuilder prop;
  ElfW(Word) pr_data[] = {GNU_PROPERTY_AARCH64_FEATURE_1_BTI};
  ASSERT_TRUE(prop.push(GNU_PROPERTY_AARCH64_FEATURE_1_AND, sizeof(pr_data), &pr_data));
  PHDR_WITH_NOTE_GNU_PROPERTY(prop.data());
  ASSERT_NO_ERROR_MSG();
  test_bti_not_supported(note);

  g_platform_properties.bti_supported = bti_supported_orig;
#else
  GTEST_SKIP() << "BTI is not supported on this architecture.";
#endif
}
