/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <sys/procfs.h>

TEST(sys_procfs, smoke) {
  elf_greg_t reg;
  memset(&reg, 0, sizeof(reg));

  elf_gregset_t regs;
  memset(&regs, 0, sizeof(regs));

  elf_fpregset_t fp_regs;
  memset(&fp_regs, 0, sizeof(fp_regs));

  prgregset_t pr_g_regs;
  memset(&pr_g_regs, 0, sizeof(pr_g_regs));

  prfpregset_t pr_fp_regs;
  memset(&pr_fp_regs, 0, sizeof(pr_fp_regs));

  static_assert(sizeof(prgregset_t) == sizeof(elf_gregset_t), "");
  static_assert(sizeof(prfpregset_t) == sizeof(elf_fpregset_t), "");
}
