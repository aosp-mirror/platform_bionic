/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <sys/auxv.h>
#if defined(__BIONIC__)
#include <sys/ifunc.h>
#endif

typedef int (*fn_ptr_t)();

int ret42() {
  return 42;
}

extern "C" fn_ptr_t resolver() {
  return ret42;
}

int ifunc() __attribute__((ifunc("resolver")));

TEST(ifunc, function) {
  ASSERT_EQ(42, ifunc());
}

#if defined(__BIONIC__)

#if defined(__aarch64__)

static uint64_t g_hwcap;
static __ifunc_arg_t g_arg;

extern "C" fn_ptr_t hwcap_resolver(uint64_t hwcap, __ifunc_arg_t* arg)
    __attribute__((no_sanitize("hwaddress"))) {
  g_hwcap = hwcap;
  g_arg = *arg;
  return ret42;
}

#elif defined(__arm__)

static unsigned long g_hwcap;

extern "C" fn_ptr_t hwcap_resolver(unsigned long hwcap) {
  g_hwcap = hwcap;
  return ret42;
}

#elif defined(__riscv)

#include <sys/hwprobe.h>

static uint64_t g_hwcap;
static __riscv_hwprobe_t g_hwprobe_ptr;
static void* g_null;

static riscv_hwprobe g_hwprobes[] = {{.key = RISCV_HWPROBE_KEY_IMA_EXT_0}};

extern "C" fn_ptr_t hwcap_resolver(uint64_t hwcap, __riscv_hwprobe_t hwprobe_ptr, void* null) {
  g_hwcap = hwcap;
  g_hwprobe_ptr = hwprobe_ptr;
  g_null = null;

  // Ensure that __riscv_hwprobe() can be called from an ifunc.
  if ((*hwprobe_ptr)(g_hwprobes, 1, 0, nullptr, 0) != 0) return nullptr;
  return ret42;
}

#else

extern "C" fn_ptr_t hwcap_resolver() {
  return ret42;
}

#endif

int hwcap() __attribute__((ifunc("hwcap_resolver")));

TEST(ifunc, hwcap) {
  ASSERT_EQ(42, hwcap());

#if defined(__aarch64__)
  EXPECT_EQ(getauxval(AT_HWCAP) | _IFUNC_ARG_HWCAP, g_hwcap);

  EXPECT_EQ(sizeof(__ifunc_arg_t), g_arg._size);
  EXPECT_EQ(getauxval(AT_HWCAP), g_arg._hwcap);
  EXPECT_EQ(getauxval(AT_HWCAP2), g_arg._hwcap2);
#elif defined(__arm__)
  EXPECT_EQ(getauxval(AT_HWCAP), g_hwcap);
#elif defined(__riscv)
  printf("hwcap=%lx hwprobe_ptr=%p (__riscv_hwprobe=%p) null=%p\n", g_hwcap, g_hwprobe_ptr,
         __riscv_hwprobe, g_null);

  EXPECT_EQ(getauxval(AT_HWCAP), g_hwcap);
  EXPECT_EQ(nullptr, g_null);

  riscv_hwprobe probes[] = {{.key = RISCV_HWPROBE_KEY_IMA_EXT_0}};
  ASSERT_EQ(0, __riscv_hwprobe(probes, 1, 0, nullptr, 0));
  EXPECT_EQ(probes[0].value, g_hwprobes[0].value);
#endif
}

#endif  // defined(__BIONIC__)
