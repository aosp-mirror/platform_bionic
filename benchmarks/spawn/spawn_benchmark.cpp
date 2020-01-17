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

#include "spawn_benchmark.h"

#include <errno.h>
#include <spawn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <android-base/stringprintf.h>

extern char** environ;

void BM_spawn_test(benchmark::State& state, const char* const* argv) {
  for (auto _ : state) {
    pid_t child = 0;
    if (int spawn_err = posix_spawn(&child, argv[0], nullptr, nullptr, const_cast<char**>(argv),
                                    environ)) {
      state.SkipWithError(android::base::StringPrintf(
          "posix_spawn of %s failed: %s", argv[0], strerror(spawn_err)).c_str());
      break;
    }

    int wstatus = 0;
    const pid_t wait_result = TEMP_FAILURE_RETRY(waitpid(child, &wstatus, 0));
    if (wait_result != child) {
      state.SkipWithError(android::base::StringPrintf(
          "waitpid on pid %d for %s failed: %s",
          static_cast<int>(child), argv[0], strerror(errno)).c_str());
      break;
    }
    if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 127) {
      state.SkipWithError(android::base::StringPrintf("could not exec %s", argv[0]).c_str());
      break;
    }
  }
}
