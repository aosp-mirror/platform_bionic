/*
 * Copyright (C) 2022 The Android Open Source Project
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

#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>

#include <bionic/malloc.h>

#include "libs_utils.h"

#if defined(__aarch64__)

template <typename T>
static inline void mte_set_tag(T* p) {
  __asm__ __volatile__(
      ".arch_extension memtag\n"
      "stg %[Ptr], [%[Ptr]]\n"
      :
      : [Ptr] "r"(p)
      : "memory");
}

template <typename T>
static inline T* mte_get_tag(T* p) {
  __asm__ __volatile__(
      ".arch_extension memtag\n"
      "ldg %[Ptr], [%[Ptr]]\n"
      : [Ptr] "+r"(p)
      :
      : "memory");
  return p;
}

template <typename T>
static inline T* mte_increment_tag(T* p) {
  T* res;
  __asm__ __volatile__(
      ".arch_extension memtag\n"
      "addg %[Res], %[Ptr], #0, #1\n"
      : [Res] "=r"(res)
      : [Ptr] "r"(p)
      : "memory");
  return res;
}

constexpr size_t kStackAllocationSize = 128 * 1024;

// Prevent optimizations.
volatile void* sink;

enum struct ChildAction { Exit, Execve, Execl };

// Either execve or _exit, transferring control back to parent.
__attribute__((no_sanitize("memtag"), optnone, noinline)) void vfork_child2(ChildAction action,
                                                                            void* fp_parent) {
  // Make sure that the buffer in the caller has not been optimized out.
  void* fp = __builtin_frame_address(0);
  CHECK(reinterpret_cast<uintptr_t>(fp_parent) - reinterpret_cast<uintptr_t>(fp) >=
        kStackAllocationSize);
  if (action == ChildAction::Execve) {
    const char* argv[] = {"/system/bin/true", nullptr};
    const char* envp[] = {nullptr};
    execve("/system/bin/true", const_cast<char**>(argv), const_cast<char**>(envp));
    fprintf(stderr, "execve failed: %m\n");
    _exit(1);
  } else if (action == ChildAction::Execl) {
    execl("/system/bin/true", "/system/bin/true", "unusedA", "unusedB", nullptr);
    fprintf(stderr, "execl failed: %m\n");
    _exit(1);
  } else if (action == ChildAction::Exit) {
    _exit(0);
  }
  CHECK(0);
}

// Place a tagged buffer on the stack. Do not tag the top half so that the parent does not crash too
// early even if things go wrong.
__attribute__((no_sanitize("memtag"), optnone, noinline)) void vfork_child(ChildAction action) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = mte_increment_tag(p);
    mte_set_tag(q);
    CHECK(mte_get_tag(p) == q);
  }
  vfork_child2(action, __builtin_frame_address(0));
}

// Parent. Check that the stack has correct allocation tags.
__attribute__((no_sanitize("memtag"), optnone, noinline)) void vfork_parent(pid_t pid) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  fprintf(stderr, "vfork_parent %p\n", &buf);
  bool success = true;
  for (char* p = buf; p < buf + sizeof(buf); p += 16) {
    char* q = mte_get_tag(p);
    if (p != q) {
      fprintf(stderr, "tag mismatch at offset %zx: %p != %p\n", p - buf, p, q);
      success = false;
      break;
    }
  }

  int wstatus;
  do {
    int res = waitpid(pid, &wstatus, 0);
    CHECK(res == pid);
  } while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

  CHECK(WIFEXITED(wstatus));
  CHECK(WEXITSTATUS(wstatus) == 0);

  if (!success) exit(1);
}

void test_vfork(ChildAction action) {
  pid_t pid = vfork();
  if (pid == 0) {
    vfork_child(action);
  } else {
    vfork_parent(pid);
  }
}

__attribute__((no_sanitize("memtag"), optnone, noinline)) static void settag_and_longjmp(
    jmp_buf cont) {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  sink = &buf;

  for (char* p = buf; p < buf + sizeof(buf) / 2; p += 16) {
    char* q = mte_increment_tag(p);
    mte_set_tag(q);
    if (mte_get_tag(p) != q) {
      fprintf(stderr, "failed to set allocation tags on stack: %p != %p\n", mte_get_tag(p), q);
      exit(1);
    }
  }
  longjmp(cont, 42);
}

// Check that the stack has correct allocation tags.
__attribute__((no_sanitize("memtag"), optnone, noinline)) static void check_stack_tags() {
  alignas(16) char buf[kStackAllocationSize] __attribute__((uninitialized));
  for (char* p = buf; p < buf + sizeof(buf); p += 16) {
    void* q = mte_get_tag(p);
    if (p != q) {
      fprintf(stderr, "stack tags mismatch: expected %p, got %p", p, q);
      exit(1);
    }
  }
}

void check_longjmp_restores_tags() {
  int value;
  jmp_buf jb;
  if ((value = setjmp(jb)) == 0) {
    settag_and_longjmp(jb);
    exit(2);  // Unreachable.
  } else {
    CHECK(value == 42);
    check_stack_tags();
  }
}

class SigAltStackScoped {
  stack_t old_ss;
  void* altstack_start;
  size_t altstack_size;

 public:
  SigAltStackScoped(size_t sz) : altstack_size(sz) {
    altstack_start = mmap(nullptr, altstack_size, PROT_READ | PROT_WRITE | PROT_MTE,
                          MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (altstack_start == MAP_FAILED) {
      fprintf(stderr, "sigaltstack mmap failed: %m\n");
      exit(1);
    }
    stack_t ss = {};
    ss.ss_sp = altstack_start;
    ss.ss_size = altstack_size;
    int res = sigaltstack(&ss, &old_ss);
    CHECK(res == 0);
  }

  ~SigAltStackScoped() {
    int res = sigaltstack(&old_ss, nullptr);
    CHECK(res == 0);
    munmap(altstack_start, altstack_size);
  }
};

class SigActionScoped {
  int signo;
  struct sigaction oldsa;

 public:
  using handler_t = void (*)(int, siginfo_t* siginfo, void*);

  SigActionScoped(int signo, handler_t handler) : signo(signo) {
    struct sigaction sa = {};
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    int res = sigaction(signo, &sa, &oldsa);
    CHECK(res == 0);
  }

  ~SigActionScoped() {
    int res = sigaction(signo, &oldsa, nullptr);
    CHECK(res == 0);
  }
};

void test_longjmp() {
  check_longjmp_restores_tags();

  std::thread t([]() { check_longjmp_restores_tags(); });
  t.join();
}

void test_longjmp_sigaltstack() {
  const size_t kAltStackSize = kStackAllocationSize + getpagesize() * 16;
  SigAltStackScoped sigAltStackScoped(kAltStackSize);
  SigActionScoped sigActionScoped(
      SIGUSR1, [](int, siginfo_t*, void*) { check_longjmp_restores_tags(); });
  raise(SIGUSR1);

  // same for a secondary thread
  std::thread t([&]() {
    SigAltStackScoped sigAltStackScoped(kAltStackSize);
    raise(SIGUSR1);
  });
  t.join();
}

void test_android_mallopt() {
  bool memtag_stack;
  CHECK(android_mallopt(M_MEMTAG_STACK_IS_ON, &memtag_stack, sizeof(memtag_stack)));
  CHECK(memtag_stack);
}

static uintptr_t GetTag(void* addr) {
  return reinterpret_cast<uintptr_t>(addr) & (0xFULL << 56);
}

static uintptr_t GetTag(volatile void* addr) {
  return GetTag(const_cast<void*>(addr));
}

static volatile char* throw_frame;
static volatile char* skip_frame3_frame;
volatile char *x;

__attribute__((noinline)) void throws() {
  // Prevent optimization.
  if (getpid() == 0) return;
  throw_frame = reinterpret_cast<char*>(__builtin_frame_address(0));
  throw "error";
}

__attribute__((noinline)) void maybe_throws() {
  // These are all unique sizes so in case of a failure, we can see which ones
  // are not untagged from the tag dump.
  volatile char y[5 * 16]= {};
  x = y;
  // Make sure y is tagged.
  CHECK(GetTag(&y) != GetTag(__builtin_frame_address(0)));
  throws();
}

__attribute__((noinline, no_sanitize("memtag"))) void skip_frame() {
  volatile char y[6*16] = {};
  x = y;
  // Make sure y is not tagged.
  CHECK(GetTag(&y) == GetTag(__builtin_frame_address(0)));
  maybe_throws();
}

__attribute__((noinline)) void skip_frame2() {
  volatile char y[7*16] = {};
  x = y;
  // Make sure y is tagged.
  CHECK(GetTag(&y) != GetTag(__builtin_frame_address(0)));
  skip_frame();
}

__attribute__((noinline, no_sanitize("memtag"))) void skip_frame3() {
  volatile char y[8*16] = {};
  x = y;
  skip_frame3_frame = reinterpret_cast<char*>(__builtin_frame_address(0));
  // Make sure y is not tagged.
  CHECK(GetTag(&y) == GetTag(__builtin_frame_address(0)));
  skip_frame2();
}

void test_exception_cleanup() {
  // This is here for debugging purposes, if something goes wrong we can
  // verify that this placeholder did not get untagged.
  volatile char placeholder[16*16] = {};
  x = placeholder;
  try {
    skip_frame3();
  } catch (const char* e) {
  }
  if (throw_frame >= skip_frame3_frame) {
    fprintf(stderr, "invalid throw frame");
    exit(1);
  }
  for (char* b = const_cast<char*>(throw_frame); b < skip_frame3_frame; ++b) {
    if (mte_get_tag(b) != b) {
      fprintf(stderr, "invalid tag at %p", b);
      exit(1);
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("nothing to do\n");
    return 1;
  }

  if (strcmp(argv[1], "vfork_execve") == 0) {
    test_vfork(ChildAction::Execve);
    return 0;
  }

  if (strcmp(argv[1], "vfork_execl") == 0) {
    test_vfork(ChildAction::Execl);
    return 0;
  }

  if (strcmp(argv[1], "vfork_exit") == 0) {
    test_vfork(ChildAction::Exit);
    return 0;
  }

  if (strcmp(argv[1], "longjmp") == 0) {
    test_longjmp();
    return 0;
  }

  if (strcmp(argv[1], "longjmp_sigaltstack") == 0) {
    test_longjmp_sigaltstack();
    return 0;
  }

  if (strcmp(argv[1], "android_mallopt") == 0) {
    test_android_mallopt();
    return 0;
  }

  if (strcmp(argv[1], "exception_cleanup") == 0) {
    test_exception_cleanup();
    return 0;
  }

  printf("unrecognized command: %s\n", argv[1]);
  return 1;
}
#else
int main(int, char**) {
  printf("aarch64 only\n");
  return 1;
}
#endif  // defined(__aarch64__)
