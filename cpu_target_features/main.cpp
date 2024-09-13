/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <stdio.h>

#include "print_target_features.inc"

int main() {
#if defined(__aarch64__)
  printAarch64TargetFeatures();
  return 0;
#elif defined(__arm__)
  printArm32TargetFeatures();
  return 0;
#elif defined(__x86_64__) || defined(__i386__)
  printX86TargetFeatures();
  return 0;
#elif defined(__riscv)
  printRiscvTargetFeatures();
  return 0;
#else
#error Unsupported arch. This binary only supports aarch64, arm, x86, x86-64, and risc-v
#endif
}
