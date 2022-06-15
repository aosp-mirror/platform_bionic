/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <signal.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include <unistd.h>
#include <memory>

void action(int signo, siginfo_t* info __unused, void*) {
#ifdef __ANDROID__
  if (signo == 11 && info->si_code == SEGV_MTEAERR) {
    fprintf(stderr, "SEGV_MTEAERR\n");
    _exit(0);
  }

  if (signo == 11 && info->si_code == SEGV_MTESERR) {
    fprintf(stderr, "SEGV_MTESERR\n");
    _exit(0);
  }
#endif

  fprintf(stderr, "signo %d\n", signo);
  _exit(0);
}

__attribute__((optnone)) int main() {
  struct sigaction sa = {};
  sa.sa_sigaction = action;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, nullptr);

  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  volatile int oob = p[-1];
  (void)oob;

  fprintf(stderr, "normal exit\n");
  return 0;
}
