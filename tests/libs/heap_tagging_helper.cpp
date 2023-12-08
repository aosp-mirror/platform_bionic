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
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <unistd.h>
#include <memory>

#include <android-base/stringprintf.h>

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

void action2(int signo, siginfo_t* info __unused, void*) {
  fprintf(stderr, "unexpected signal %d\n", signo);
  _exit(0);
}

__attribute__((optnone)) int main() {
  struct sigaction sa = {};
  sa.sa_sigaction = action;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, nullptr);
  // suppress HWASan crash in logcat / tombstone.
  struct sigaction dfl_sa = {};
  dfl_sa.sa_handler = SIG_DFL;
  sigaction(SIGABRT, &dfl_sa, nullptr);

  std::unique_ptr<int[]> p = std::make_unique<int[]>(4);
  volatile int oob = p[-1];
  (void)oob;

#if defined(__BIONIC__) && defined(__aarch64__)
  // If we get here, bad access on system heap memory did not trigger a fault.
  // This suggests that MTE is disabled. Make sure that explicitly tagged PROT_MTE memory does not
  // trigger a fault either.
  if (getauxval(AT_HWCAP2) & HWCAP2_MTE) {
    sa.sa_sigaction = action2;
    sigaction(SIGSEGV, &sa, nullptr);

    size_t page_size = static_cast<size_t>(sysconf(_SC_PAGESIZE));
    void* p = mmap(nullptr, page_size, PROT_READ | PROT_WRITE | PROT_MTE,
                   MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (!p) {
      fprintf(stderr, "mmap failed\n");
      return 1;
    }

    void* q = p;
    __asm__ __volatile__(
        ".arch_extension memtag\n"
        "irg %[Ptr], %[Ptr], xzr\n"
        "stg %[Ptr], [%[Ptr]]\n"
        "addg %[Ptr], %[Ptr], 0, 1\n"
        "str xzr, [%[Ptr]]\n"
        : [Ptr] "+&r"(q)
        :
        : "memory");

    munmap(p, page_size);
  }
#endif  // __aarch64__

  // In fact, make sure that there are no tagged mappings at all.
  auto cmd = android::base::StringPrintf("cat /proc/%d/smaps | grep -E 'VmFlags:.* mt'", getpid());
  if (system(cmd.c_str()) == 0) {
    fprintf(stderr, "unexpected PROT_MTE mappings found\n");
    return 1;
  }

  fprintf(stderr, "normal exit\n");
  return 0;
}
