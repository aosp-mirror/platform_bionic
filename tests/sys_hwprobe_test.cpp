/*
 * Copyright (C) 2023 The Android Open Source Project
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

#if __has_include(<sys/hwprobe.h>)
#include <sys/hwprobe.h>
#include <sys/syscall.h>
#endif


#if defined(__riscv)
#include <riscv_vector.h>

__attribute__((noinline))
uint64_t scalar_cast(uint8_t const* p) {
  return *(uint64_t const*)p;
}

__attribute__((noinline))
uint64_t scalar_memcpy(uint8_t const* p) {
  uint64_t r;
  __builtin_memcpy(&r, p, sizeof(r));
  return r;
}

__attribute__((noinline))
uint64_t vector_memcpy(uint8_t* d, uint8_t const* p) {
  __builtin_memcpy(d, p, 16);
  return *(uint64_t const*)d;
}

__attribute__((noinline))
uint64_t vector_ldst(uint8_t* d, uint8_t const* p) {
  __riscv_vse8(d, __riscv_vle8_v_u8m1(p, 16), 16);
  return *(uint64_t const*)d;
}

__attribute__((noinline))
uint64_t vector_ldst64(uint8_t* d, uint8_t const* p) {
  __riscv_vse64((unsigned long *)d, __riscv_vle64_v_u64m1((const unsigned long *)p, 16), 16);
  return *(uint64_t const*)d;
}

// For testing scalar and vector unaligned accesses.
uint64_t tmp[3] = {1,1,1};
uint64_t dst[3] = {1,1,1};
#endif

TEST(sys_hwprobe, __riscv_hwprobe_misaligned_scalar) {
#if defined(__riscv)
  uint8_t* p = (uint8_t*)tmp + 1;
  ASSERT_NE(0U, scalar_cast(p));
  ASSERT_NE(0U, scalar_memcpy(p));
#else
  GTEST_SKIP() << "__riscv_hwprobe requires riscv64";
#endif
}

TEST(sys_hwprobe, __riscv_hwprobe_misaligned_vector) {
#if defined(__riscv)
  uint8_t* p = (uint8_t*)tmp + 1;
  uint8_t* d = (uint8_t*)dst + 1;

  ASSERT_NE(0U, vector_ldst(d, p));
  ASSERT_NE(0U, vector_memcpy(d, p));
  ASSERT_NE(0U, vector_ldst64(d, p));
#else
  GTEST_SKIP() << "__riscv_hwprobe requires riscv64";
#endif
}

TEST(sys_hwprobe, __riscv_hwprobe) {
#if defined(__riscv) && __has_include(<sys/hwprobe.h>)
  riscv_hwprobe probes[] = {{.key = RISCV_HWPROBE_KEY_IMA_EXT_0},
                            {.key = RISCV_HWPROBE_KEY_CPUPERF_0}};
  ASSERT_EQ(0, __riscv_hwprobe(probes, 2, 0, nullptr, 0));
  EXPECT_EQ(RISCV_HWPROBE_KEY_IMA_EXT_0, probes[0].key);
  EXPECT_TRUE((probes[0].value & RISCV_HWPROBE_IMA_FD) != 0);
  EXPECT_TRUE((probes[0].value & RISCV_HWPROBE_IMA_C) != 0);
  EXPECT_TRUE((probes[0].value & RISCV_HWPROBE_IMA_V) != 0);
  EXPECT_TRUE((probes[0].value & RISCV_HWPROBE_EXT_ZBA) != 0);
  EXPECT_TRUE((probes[0].value & RISCV_HWPROBE_EXT_ZBB) != 0);
  EXPECT_TRUE((probes[0].value & RISCV_HWPROBE_EXT_ZBS) != 0);

  EXPECT_EQ(RISCV_HWPROBE_KEY_CPUPERF_0, probes[1].key);
  EXPECT_TRUE((probes[1].value & RISCV_HWPROBE_MISALIGNED_MASK) == RISCV_HWPROBE_MISALIGNED_FAST);
#else
  GTEST_SKIP() << "__riscv_hwprobe requires riscv64";
#endif
}

TEST(sys_hwprobe, __riscv_hwprobe_syscall_vdso) {
#if defined(__riscv) && __has_include(<sys/hwprobe.h>)
  riscv_hwprobe probes_vdso[] = {{.key = RISCV_HWPROBE_KEY_IMA_EXT_0},
                                 {.key = RISCV_HWPROBE_KEY_CPUPERF_0}};
  ASSERT_EQ(0, __riscv_hwprobe(probes_vdso, 2, 0, nullptr, 0));

  riscv_hwprobe probes_syscall[] = {{.key = RISCV_HWPROBE_KEY_IMA_EXT_0},
                                    {.key = RISCV_HWPROBE_KEY_CPUPERF_0}};
  ASSERT_EQ(0, syscall(SYS_riscv_hwprobe, probes_syscall, 2, 0, nullptr, 0));

  // Check we got the same answers from the vdso and the syscall.
  EXPECT_EQ(RISCV_HWPROBE_KEY_IMA_EXT_0, probes_syscall[0].key);
  EXPECT_EQ(probes_vdso[0].key, probes_syscall[0].key);
  EXPECT_EQ(probes_vdso[0].value, probes_syscall[0].value);
  EXPECT_EQ(RISCV_HWPROBE_KEY_CPUPERF_0, probes_syscall[1].key);
  EXPECT_EQ(probes_vdso[1].key, probes_syscall[1].key);
  EXPECT_EQ(probes_vdso[1].value, probes_syscall[1].value);
#else
  GTEST_SKIP() << "__riscv_hwprobe requires riscv64";
#endif
}

TEST(sys_hwprobe, __riscv_hwprobe_fail) {
#if defined(__riscv) && __has_include(<sys/hwprobe.h>)
  riscv_hwprobe probes[] = {};
  ASSERT_EQ(EINVAL, __riscv_hwprobe(probes, 0, 0, nullptr, ~0));
#else
  GTEST_SKIP() << "__riscv_hwprobe requires riscv64";
#endif
}