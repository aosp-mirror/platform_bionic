/*
 * Copyright (C) 2024 The Android Open Source Project
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

#define STATIC_TLS_LAYOUT_TEST

#include "private/bionic_elf_tls.h"

#include <string>
#include <tuple>

#include <gtest/gtest.h>

#include <android-base/silent_death_test.h>

#include "private/bionic_tls.h"

using namespace std::string_literals;

struct AlignedSizeFlat {
  size_t size = 0;
  size_t align = 1;
  size_t skew = 0;
};

static TlsAlignedSize unflatten_size(AlignedSizeFlat flat) {
  return TlsAlignedSize{.size = flat.size,
                        .align = TlsAlign{
                            .value = flat.align,
                            .skew = flat.skew,
                        }};
}

TEST(static_tls_layout, reserve_tp_pair) {
  auto reserve_tp = [](const AlignedSizeFlat& before, const AlignedSizeFlat& after,
                       StaticTlsLayout layout = {}) {
    auto allocs = layout.reserve_tp_pair(unflatten_size(before), unflatten_size(after));
    return std::make_tuple(layout, allocs);
  };

  StaticTlsLayout layout;
  StaticTlsLayout::TpAllocations allocs;

  // Simple case.
  std::tie(layout, allocs) = reserve_tp({.size = 8, .align = 2}, {.size = 16, .align = 2});
  EXPECT_EQ(0u, allocs.before);
  EXPECT_EQ(8u, allocs.tp);
  EXPECT_EQ(8u, allocs.after);
  EXPECT_EQ(24u, layout.size());
  EXPECT_EQ(2u, layout.align_);

  // Zero-sized `before`
  std::tie(layout, allocs) = reserve_tp({.size = 0}, {.size = 64, .align = 8});
  EXPECT_EQ(0u, allocs.before);
  EXPECT_EQ(0u, allocs.tp);
  EXPECT_EQ(0u, allocs.after);

  // Zero-sized `after`
  std::tie(layout, allocs) = reserve_tp({.size = 64, .align = 8}, {.size = 0});
  EXPECT_EQ(0u, allocs.before);
  EXPECT_EQ(64u, allocs.tp);
  EXPECT_EQ(64u, allocs.after);

  // The `before` allocation is shifted forward to the TP.
  std::tie(layout, allocs) = reserve_tp({.size = 1}, {.size = 64, .align = 8});
  EXPECT_EQ(7u, allocs.before);
  EXPECT_EQ(8u, allocs.tp);
  EXPECT_EQ(8u, allocs.after);

  // Alignment gap between `before` and TP.
  std::tie(layout, allocs) = reserve_tp({.size = 9, .align = 4}, {.size = 1});
  EXPECT_EQ(0u, allocs.before);
  EXPECT_EQ(12u, allocs.tp);
  EXPECT_EQ(12u, allocs.after);
  EXPECT_EQ(13u, layout.size());
  EXPECT_EQ(4u, layout.align_);

  // Alignment gap between `before` and TP.
  std::tie(layout, allocs) = reserve_tp({.size = 9, .align = 4}, {.size = 128, .align = 64});
  EXPECT_EQ(52u, allocs.before);
  EXPECT_EQ(64u, allocs.tp);
  EXPECT_EQ(64u, allocs.after);
  EXPECT_EQ(192u, layout.size());
  EXPECT_EQ(64u, layout.align_);

  // Skew-aligned `before` with low alignment.
  std::tie(layout, allocs) =
      reserve_tp({.size = 1, .align = 4, .skew = 1}, {.size = 64, .align = 8});
  EXPECT_EQ(5u, allocs.before);
  EXPECT_EQ(8u, allocs.tp);

  // Skew-aligned `before` with high alignment.
  std::tie(layout, allocs) = reserve_tp({.size = 48, .align = 64, .skew = 17}, {.size = 1});
  EXPECT_EQ(17u, allocs.before);
  EXPECT_EQ(128u, allocs.tp);

  // An unrelated byte precedes the pair in the layout. Make sure `before` is
  // still aligned.
  layout = {};
  layout.reserve_type<char>();
  std::tie(layout, allocs) = reserve_tp({.size = 12, .align = 16}, {.size = 1}, layout);
  EXPECT_EQ(16u, allocs.before);
  EXPECT_EQ(32u, allocs.tp);

  // Skew-aligned `after`.
  std::tie(layout, allocs) =
      reserve_tp({.size = 32, .align = 8}, {.size = 16, .align = 4, .skew = 3});
  EXPECT_EQ(0u, allocs.before);
  EXPECT_EQ(32u, allocs.tp);
  EXPECT_EQ(35u, allocs.after);
  EXPECT_EQ(51u, layout.size());
}

// A "NUM_words" literal is the size in bytes of NUM words of memory.
static size_t operator""_words(unsigned long long i) {
  return i * sizeof(void*);
}

using static_tls_layout_DeathTest = SilentDeathTest;

TEST_F(static_tls_layout_DeathTest, arm) {
#if !defined(__arm__) && !defined(__aarch64__)
  GTEST_SKIP() << "test only applies to arm32/arm64 targets";
#endif

  auto reserve_exe = [](const AlignedSizeFlat& config) {
    StaticTlsLayout layout;
    TlsSegment seg = {.aligned_size = unflatten_size(config)};
    layout.reserve_exe_segment_and_tcb(&seg, "prog");
    return layout;
  };

  auto underalign_error = [](size_t align, size_t offset) {
    return R"(error: "prog": executable's TLS segment is underaligned: )"s
           R"(alignment is )"s +
           std::to_string(align) + R"( \(skew )" + std::to_string(offset) +
           R"(\), needs to be at least (32 for ARM|64 for ARM64) Bionic)"s;
  };

  // Amount of memory needed for negative TLS slots, given a segment p_align of
  // 8 or 16 words.
  const size_t base8 = __BIONIC_ALIGN(-MIN_TLS_SLOT, 8) * sizeof(void*);
  const size_t base16 = __BIONIC_ALIGN(-MIN_TLS_SLOT, 16) * sizeof(void*);

  StaticTlsLayout layout;

  // An executable with a single word.
  layout = reserve_exe({.size = 1_words, .align = 8_words});
  EXPECT_EQ(base8 + MIN_TLS_SLOT * sizeof(void*), layout.offset_bionic_tcb());
  EXPECT_EQ(base8, layout.offset_thread_pointer());
  EXPECT_EQ(base8 + 8_words, layout.offset_exe());
  EXPECT_EQ(base8 + 9_words, layout.size());
  EXPECT_EQ(8_words, layout.align_);

  // Simple underalignment case.
  EXPECT_DEATH(reserve_exe({.size = 1_words, .align = 1_words}), underalign_error(1_words, 0));

  // Skewed by 1 word is OK.
  layout = reserve_exe({.size = 1_words, .align = 8_words, .skew = 1_words});
  EXPECT_EQ(base8, layout.offset_thread_pointer());
  EXPECT_EQ(base8 + 9_words, layout.offset_exe());
  EXPECT_EQ(base8 + 10_words, layout.size());
  EXPECT_EQ(8_words, layout.align_);

  // Skewed by 2 words would overlap Bionic slots, regardless of the p_align
  // value.
  EXPECT_DEATH(reserve_exe({.size = 1_words, .align = 8_words, .skew = 2_words}),
               underalign_error(8_words, 2_words));
  EXPECT_DEATH(reserve_exe({.size = 1_words, .align = 0x1000, .skew = 2_words}),
               underalign_error(0x1000, 2_words));

  // Skewed by 8 words is OK again.
  layout = reserve_exe({.size = 1_words, .align = 16_words, .skew = 8_words});
  EXPECT_EQ(base16, layout.offset_thread_pointer());
  EXPECT_EQ(base16 + 8_words, layout.offset_exe());
  EXPECT_EQ(base16 + 9_words, layout.size());
  EXPECT_EQ(16_words, layout.align_);

  // Skewed by 9 words is also OK. (The amount of skew doesn't need to be a
  // multiple of anything.)
  layout = reserve_exe({.size = 1_words, .align = 16_words, .skew = 9_words});
  EXPECT_EQ(base16, layout.offset_thread_pointer());
  EXPECT_EQ(base16 + 9_words, layout.offset_exe());
  EXPECT_EQ(base16 + 10_words, layout.size());
  EXPECT_EQ(16_words, layout.align_);

  // Skew with large alignment.
  layout = reserve_exe({.size = 1_words, .align = 256_words, .skew = 8_words});
  EXPECT_EQ(256_words, layout.offset_thread_pointer());
  EXPECT_EQ(264_words, layout.offset_exe());
  EXPECT_EQ(265_words, layout.size());
  EXPECT_EQ(256_words, layout.align_);
}
