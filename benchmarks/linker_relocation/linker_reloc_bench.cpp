/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "spawn_benchmark.h"

#if defined(__LP64__)
static constexpr const char* kNativeTestDir = "nativetest64";
#else
static constexpr const char* kNativeTestDir = "nativetest";
#endif

static void BM_linker_relocation(benchmark::State& state) {
  std::string main = test_program("linker_reloc_bench_main");

  // Translate from:
  //    /data/benchmarktest[64]/linker-reloc-bench    [exe dir]
  // to:
  //    /data/nativetest[64]/linker-reloc-bench       [dir with test libs]
  std::string test_lib_dir = 
      android::base::Dirname(android::base::Dirname(android::base::GetExecutableDirectory())) +
      "/" + kNativeTestDir + "/linker-reloc-bench";

  setenv("LD_LIBRARY_PATH", test_lib_dir.c_str(), 1);

  BM_spawn_test(state, (const char*[]) { main.c_str(), nullptr });
}

BENCHMARK(BM_linker_relocation)->UseRealTime()->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
