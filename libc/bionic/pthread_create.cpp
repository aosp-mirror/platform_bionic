/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <pthread.h>

#include <errno.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/random.h>
#include <unistd.h>

#include "pthread_internal.h"

#include <async_safe/log.h>

#include "platform/bionic/macros.h"
#include "platform/bionic/mte.h"
#include "platform/bionic/page.h"
#include "private/ErrnoRestorer.h"
#include "private/ScopedRWLock.h"
#include "private/bionic_constants.h"
#include "private/bionic_defs.h"
#include "private/bionic_globals.h"
#include "private/bionic_ssp.h"
#include "private/bionic_systrace.h"
#include "private/bionic_tls.h"

// x86 uses segment descriptors rather than a direct pointer to TLS.
#if defined(__i386__)
#include <asm/ldt.h>
void __init_user_desc(struct user_desc*, bool, void*);
#endif

__attribute__((no_stack_protector))
void __init_tcb_stack_guard(bionic_tcb* tcb) {
  // GCC looks in the TLS for the stack guard on x86, so copy it there from our global.
  tcb->tls_slot(TLS_SLOT_STACK_GUARD) = reinterpret_cast<void*>(__stack_chk_guard);
}

void __init_bionic_tls_ptrs(bionic_tcb* tcb, bionic_tls* tls) {
  tcb->thread()->bionic_tls = tls;
  tcb->tls_slot(TLS_SLOT_BIONIC_TLS) = tls;
}

// Allocate a temporary bionic_tls that the dynamic linker's main thread can
// use while it's loading the initial set of ELF modules.
bionic_tls* __allocate_temp_bionic_tls() {
  size_t allocation_size = __BIONIC_ALIGN(sizeof(bionic_tls), page_size());
  void* allocation = mmap(nullptr, allocation_size,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1, 0);
  if (allocation == MAP_FAILED) {
    async_safe_fatal("failed to allocate bionic_tls: %m");
  }
  return static_cast<bionic_tls*>(allocation);
}

void __free_temp_bionic_tls(bionic_tls* tls) {
  munmap(tls, __BIONIC_ALIGN(sizeof(bionic_tls), page_size()));
}

static void __init_alternate_signal_stack(pthread_internal_t* thread) {
  // Create and set an alternate signal stack.
  int prot = PROT_READ | PROT_WRITE;
#ifdef __aarch64__
  if (atomic_load(&__libc_globals->memtag_stack)) {
    prot |= PROT_MTE;
  }
#endif
  void* stack_base = mmap(nullptr, SIGNAL_STACK_SIZE, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (stack_base != MAP_FAILED) {
    // Create a guard to catch stack overflows in signal handlers.
    if (mprotect(stack_base, PTHREAD_GUARD_SIZE, PROT_NONE) == -1) {
      munmap(stack_base, SIGNAL_STACK_SIZE);
      return;
    }
    stack_t ss;
    ss.ss_sp = reinterpret_cast<uint8_t*>(stack_base) + PTHREAD_GUARD_SIZE;
    ss.ss_size = SIGNAL_STACK_SIZE - PTHREAD_GUARD_SIZE;
    ss.ss_flags = 0;
    sigaltstack(&ss, nullptr);
    thread->alternate_signal_stack = stack_base;

    // We can only use const static allocated string for mapped region name, as Android kernel
    // uses the string pointer directly when dumping /proc/pid/maps.
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, ss.ss_sp, ss.ss_size, "thread signal stack");
  }
}

static void __init_shadow_call_stack(pthread_internal_t* thread __unused) {
#if defined(__aarch64__) || defined(__riscv)
  // Allocate the stack and the guard region.
  char* scs_guard_region = reinterpret_cast<char*>(
      mmap(nullptr, SCS_GUARD_REGION_SIZE, 0, MAP_PRIVATE | MAP_ANON, -1, 0));
  thread->shadow_call_stack_guard_region = scs_guard_region;

  // The address is aligned to SCS_SIZE so that we only need to store the lower log2(SCS_SIZE) bits
  // in jmp_buf. See the SCS commentary in pthread_internal.h for more detail.
  char* scs_aligned_guard_region =
      reinterpret_cast<char*>(align_up(reinterpret_cast<uintptr_t>(scs_guard_region), SCS_SIZE));

  // We need to ensure that [scs_offset,scs_offset+SCS_SIZE) is in the guard region and that there
  // is at least one unmapped page after the shadow call stack (to catch stack overflows). We can't
  // use arc4random_uniform in init because /dev/urandom might not have been created yet.
  size_t scs_offset =
      (getpid() == 1) ? 0 : (arc4random_uniform(SCS_GUARD_REGION_SIZE / SCS_SIZE - 1) * SCS_SIZE);

  // Make the stack read-write, and store its address in the register we're using as the shadow
  // stack pointer. This is deliberately the only place where the address is stored.
  char* scs = scs_aligned_guard_region + scs_offset;
  if (mprotect(scs, SCS_SIZE, PROT_READ | PROT_WRITE) == -1) {
    async_safe_fatal("shadow stack read-write mprotect(%p, %d) failed: %m", scs, SCS_SIZE);
  }
#if defined(__aarch64__)
  __asm__ __volatile__("mov x18, %0" ::"r"(scs));
#elif defined(__riscv)
  __asm__ __volatile__("mv x3, %0" ::"r"(scs));
#endif
#endif
}

void __init_additional_stacks(pthread_internal_t* thread) {
  __init_alternate_signal_stack(thread);
  __init_shadow_call_stack(thread);
}

int __init_thread(pthread_internal_t* thread) {
  thread->cleanup_stack = nullptr;

  if (__predict_true((thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) == 0)) {
    atomic_init(&thread->join_state, THREAD_NOT_JOINED);
  } else {
    atomic_init(&thread->join_state, THREAD_DETACHED);
  }

  // Set the scheduling policy/priority of the thread if necessary.
  bool need_set = true;
  int policy;
  sched_param param;
  if ((thread->attr.flags & PTHREAD_ATTR_FLAG_INHERIT) != 0) {
    // Unless the parent has SCHED_RESET_ON_FORK set, we've already inherited from the parent.
    policy = sched_getscheduler(0);
    need_set = ((policy & SCHED_RESET_ON_FORK) != 0);
    if (need_set) {
      if (policy == -1) {
        async_safe_format_log(ANDROID_LOG_WARN, "libc",
                              "pthread_create sched_getscheduler failed: %m");
        return errno;
      }
      if (sched_getparam(0, &param) == -1) {
        async_safe_format_log(ANDROID_LOG_WARN, "libc", "pthread_create sched_getparam failed: %m");
        return errno;
      }
    }
  } else {
    policy = thread->attr.sched_policy;
    param.sched_priority = thread->attr.sched_priority;
  }
  // Backwards compatibility: before P, Android didn't have pthread_attr_setinheritsched,
  // and our behavior was neither of the POSIX behaviors.
  if ((thread->attr.flags & (PTHREAD_ATTR_FLAG_INHERIT|PTHREAD_ATTR_FLAG_EXPLICIT)) == 0) {
    need_set = (thread->attr.sched_policy != SCHED_NORMAL);
  }
  if (need_set) {
    if (sched_setscheduler(thread->tid, policy, &param) == -1) {
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "pthread_create sched_setscheduler(%d, {%d}) call failed: %m", policy,
                            param.sched_priority);
#if defined(__LP64__)
      // For backwards compatibility reasons, we only report failures on 64-bit devices.
      return errno;
#endif
    }
  }

  return 0;
}

// Allocate a thread's primary mapping. This mapping includes static TLS and
// optionally a stack. Static TLS includes ELF TLS segments and the bionic_tls
// struct.
//
// The stack_guard_size must be a multiple of the page_size().
ThreadMapping __allocate_thread_mapping(size_t stack_size, size_t stack_guard_size) {
  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;

  // Allocate in order: stack guard, stack, static TLS, guard page.
  size_t mmap_size;
  if (__builtin_add_overflow(stack_size, stack_guard_size, &mmap_size)) return {};
  if (__builtin_add_overflow(mmap_size, layout.size(), &mmap_size)) return {};
  if (__builtin_add_overflow(mmap_size, PTHREAD_GUARD_SIZE, &mmap_size)) return {};

  // Align the result to a page size.
  const size_t unaligned_size = mmap_size;
  mmap_size = __BIONIC_ALIGN(mmap_size, page_size());
  if (mmap_size < unaligned_size) return {};

  // Create a new private anonymous map. Make the entire mapping PROT_NONE, then carve out a
  // read+write area in the middle.
  const int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  char* const space = static_cast<char*>(mmap(nullptr, mmap_size, PROT_NONE, flags, -1, 0));
  if (space == MAP_FAILED) {
    async_safe_format_log(ANDROID_LOG_WARN, "libc",
                          "pthread_create failed: couldn't allocate %zu-bytes mapped space: %m",
                          mmap_size);
    return {};
  }
  const size_t writable_size = mmap_size - stack_guard_size - PTHREAD_GUARD_SIZE;
  int prot = PROT_READ | PROT_WRITE;
  const char* prot_str = "R+W";
#ifdef __aarch64__
  if (atomic_load(&__libc_globals->memtag_stack)) {
    prot |= PROT_MTE;
    prot_str = "R+W+MTE";
  }
#endif
  if (mprotect(space + stack_guard_size, writable_size, prot) != 0) {
    async_safe_format_log(
        ANDROID_LOG_WARN, "libc",
        "pthread_create failed: couldn't mprotect %s %zu-byte thread mapping region: %m", prot_str,
        writable_size);
    munmap(space, mmap_size);
    return {};
  }

  ThreadMapping result = {};
  result.mmap_base = space;
  result.mmap_size = mmap_size;
  result.mmap_base_unguarded = space + stack_guard_size;
  result.mmap_size_unguarded = mmap_size - stack_guard_size - PTHREAD_GUARD_SIZE;
  result.static_tls = space + mmap_size - PTHREAD_GUARD_SIZE - layout.size();
  result.stack_base = space;
  result.stack_top = result.static_tls;
  return result;
}

static int __allocate_thread(pthread_attr_t* attr, bionic_tcb** tcbp, void** child_stack) {
  ThreadMapping mapping;
  char* stack_top;
  bool stack_clean = false;

  if (attr->stack_base == nullptr) {
    // The caller didn't provide a stack, so allocate one.

    // Make sure the guard size is a multiple of page_size().
    const size_t unaligned_guard_size = attr->guard_size;
    attr->guard_size = __BIONIC_ALIGN(attr->guard_size, page_size());
    if (attr->guard_size < unaligned_guard_size) return EAGAIN;

    mapping = __allocate_thread_mapping(attr->stack_size, attr->guard_size);
    if (mapping.mmap_base == nullptr) return EAGAIN;

    stack_top = mapping.stack_top;
    attr->stack_base = mapping.stack_base;
    stack_clean = true;
  } else {
    mapping = __allocate_thread_mapping(0, PTHREAD_GUARD_SIZE);
    if (mapping.mmap_base == nullptr) return EAGAIN;

    stack_top = static_cast<char*>(attr->stack_base) + attr->stack_size;
  }

  // Carve out space from the stack for the thread's pthread_internal_t. This
  // memory isn't counted in pthread_attr_getstacksize.

  // To safely access the pthread_internal_t and thread stack, we need to find a 16-byte aligned boundary.
  stack_top = align_down(stack_top - sizeof(pthread_internal_t), 16);

  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(stack_top);
  if (!stack_clean) {
    // If thread was not allocated by mmap(), it may not have been cleared to zero.
    // So assume the worst and zero it.
    memset(thread, 0, sizeof(pthread_internal_t));
  }

  // Locate static TLS structures within the mapped region.
  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  auto tcb = reinterpret_cast<bionic_tcb*>(mapping.static_tls + layout.offset_bionic_tcb());
  auto tls = reinterpret_cast<bionic_tls*>(mapping.static_tls + layout.offset_bionic_tls());

  // Initialize TLS memory.
  __init_static_tls(mapping.static_tls);
  __init_tcb(tcb, thread);
  __init_tcb_dtv(tcb);
  __init_tcb_stack_guard(tcb);
  __init_bionic_tls_ptrs(tcb, tls);

  attr->stack_size = stack_top - static_cast<char*>(attr->stack_base);
  thread->attr = *attr;
  thread->mmap_base = mapping.mmap_base;
  thread->mmap_size = mapping.mmap_size;
  thread->mmap_base_unguarded = mapping.mmap_base_unguarded;
  thread->mmap_size_unguarded = mapping.mmap_size_unguarded;
  thread->stack_top = reinterpret_cast<uintptr_t>(stack_top);

  *tcbp = tcb;
  *child_stack = stack_top;
  return 0;
}

void __set_stack_and_tls_vma_name(bool is_main_thread) {
  // Name the thread's stack-and-tls area to help with debugging. This mapped area also includes
  // static TLS data, which is typically a few pages (e.g. bionic_tls).
  pthread_internal_t* thread = __get_thread();
  const char* name;
  if (is_main_thread) {
    name = "stack_and_tls:main";
  } else {
    // The kernel doesn't copy the name string, but this variable will last at least as long as the
    // mapped area. The mapped area's VMAs are unmapped with a single call to munmap.
    auto& name_buffer = thread->vma_name_buffer;
    static_assert(arraysize(name_buffer) >= arraysize("stack_and_tls:") + 11 + 1);
    async_safe_format_buffer(name_buffer, arraysize(name_buffer), "stack_and_tls:%d", thread->tid);
    name = name_buffer;
  }
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, thread->mmap_base_unguarded, thread->mmap_size_unguarded,
        name);
}

extern "C" int __rt_sigprocmask(int, const sigset64_t*, sigset64_t*, size_t);

__attribute__((no_sanitize("hwaddress")))
#ifdef __aarch64__
// This function doesn't return, but it does appear in stack traces. Avoid using return PAC in this
// function because we may end up resetting IA, which may confuse unwinders due to mismatching keys.
__attribute__((target("branch-protection=bti")))
#endif
static int __pthread_start(void* arg) {
  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(arg);

  __hwasan_thread_enter();

  // Wait for our creating thread to release us. This lets it have time to
  // notify gdb about this thread before we start doing anything.
  // This also provides the memory barrier needed to ensure that all memory
  // accesses previously made by the creating thread are visible to us.
  thread->startup_handshake_lock.lock();

  __set_stack_and_tls_vma_name(false);
  __init_additional_stacks(thread);
  __rt_sigprocmask(SIG_SETMASK, &thread->start_mask, nullptr, sizeof(thread->start_mask));
#ifdef __aarch64__
  // Chrome's sandbox prevents this prctl, so only reset IA if the target SDK level is high enough.
  // Furthermore, processes loaded from vendor partitions may have their own sandboxes that would
  // reject the prctl. Because no devices launched with PAC enabled before S, we can avoid issues on
  // upgrading devices by checking for PAC support before issuing the prctl.
  static const bool pac_supported = getauxval(AT_HWCAP) & HWCAP_PACA;
  if (pac_supported && android_get_application_target_sdk_version() >= __ANDROID_API_S__) {
    prctl(PR_PAC_RESET_KEYS, PR_PAC_APIAKEY, 0, 0, 0);
  }
#endif

  void* result = thread->start_routine(thread->start_routine_arg);
  pthread_exit(result);

  return 0;
}

// A no-op start routine for pthread_create failures where we've created a thread but aren't
// going to run user code on it. We swap out the user's start routine for this and take advantage
// of the regular thread teardown to free up resources.
static void* __do_nothing(void*) {
  return nullptr;
}

pthread_rwlock_t g_thread_creation_lock = PTHREAD_RWLOCK_INITIALIZER;

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_create(pthread_t* thread_out, pthread_attr_t const* attr,
                   void* (*start_routine)(void*), void* arg) {
  ErrnoRestorer errno_restorer;

  pthread_attr_t thread_attr;
  ScopedTrace trace("pthread_create");
  if (attr == nullptr) {
    pthread_attr_init(&thread_attr);
  } else {
    thread_attr = *attr;
    attr = nullptr; // Prevent misuse below.
  }

  bionic_tcb* tcb = nullptr;
  void* child_stack = nullptr;
  int result = __allocate_thread(&thread_attr, &tcb, &child_stack);
  if (result != 0) {
    return result;
  }

  pthread_internal_t* thread = tcb->thread();

  // Create a lock for the thread to wait on once it starts so we can keep
  // it from doing anything until after we notify the debugger about it
  //
  // This also provides the memory barrier we need to ensure that all
  // memory accesses previously performed by this thread are visible to
  // the new thread.
  thread->startup_handshake_lock.init(false);
  thread->startup_handshake_lock.lock();

  thread->start_routine = start_routine;
  thread->start_routine_arg = arg;

  thread->set_cached_pid(getpid());

  int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM |
      CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID;
  void* tls = &tcb->tls_slot(0);
#if defined(__i386__)
  // On x86 (but not x86-64), CLONE_SETTLS takes a pointer to a struct user_desc rather than
  // a pointer to the TLS itself.
  user_desc tls_descriptor;
  __init_user_desc(&tls_descriptor, false, tls);
  tls = &tls_descriptor;
#endif

  ScopedReadLock locker(&g_thread_creation_lock);

  sigset64_t block_all_mask;
  sigfillset64(&block_all_mask);
  __rt_sigprocmask(SIG_SETMASK, &block_all_mask, &thread->start_mask, sizeof(thread->start_mask));
  int rc = clone(__pthread_start, child_stack, flags, thread, &(thread->tid), tls, &(thread->tid));
  __rt_sigprocmask(SIG_SETMASK, &thread->start_mask, nullptr, sizeof(thread->start_mask));
  if (rc == -1) {
    int clone_errno = errno;
    // We don't have to unlock the mutex at all because clone(2) failed so there's no child waiting to
    // be unblocked, but we're about to unmap the memory the mutex is stored in, so this serves as a
    // reminder that you can't rewrite this function to use a ScopedPthreadMutexLocker.
    thread->startup_handshake_lock.unlock();
    if (thread->mmap_size != 0) {
      munmap(thread->mmap_base, thread->mmap_size);
    }
    async_safe_format_log(ANDROID_LOG_WARN, "libc", "pthread_create failed: clone failed: %m");
    return clone_errno;
  }

  int init_errno = __init_thread(thread);
  if (init_errno != 0) {
    // Mark the thread detached and replace its start_routine with a no-op.
    // Letting the thread run is the easiest way to clean up its resources.
    atomic_store(&thread->join_state, THREAD_DETACHED);
    __pthread_internal_add(thread);
    thread->start_routine = __do_nothing;
    thread->startup_handshake_lock.unlock();
    return init_errno;
  }

  // Publish the pthread_t and unlock the mutex to let the new thread start running.
  *thread_out = __pthread_internal_add(thread);
  thread->startup_handshake_lock.unlock();

  return 0;
}
