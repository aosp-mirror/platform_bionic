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
#include <sys/mman.h>
#include <unistd.h>

#include "pthread_internal.h"

#include "private/bionic_macros.h"
#include "private/bionic_prctl.h"
#include "private/bionic_ssp.h"
#include "private/bionic_tls.h"
#include "private/libc_logging.h"
#include "private/ErrnoRestorer.h"

// x86 uses segment descriptors rather than a direct pointer to TLS.
#if defined(__i386__)
#include <asm/ldt.h>
void __init_user_desc(struct user_desc*, bool, void*);
#endif

extern "C" int __isthreaded;

// This code is used both by each new pthread and the code that initializes the main thread.
void __init_tls(pthread_internal_t* thread) {
  // Slot 0 must point to itself. The x86 Linux kernel reads the TLS from %fs:0.
  thread->tls[TLS_SLOT_SELF] = thread->tls;
  thread->tls[TLS_SLOT_THREAD_ID] = thread;

  // Add a guard page before and after.
  size_t allocation_size = BIONIC_TLS_SIZE + 2 * PAGE_SIZE;
  void* allocation = mmap(nullptr, allocation_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (allocation == MAP_FAILED) {
    __libc_fatal("failed to allocate TLS");
  }

  thread->bionic_tls = reinterpret_cast<bionic_tls*>(static_cast<char*>(allocation) + PAGE_SIZE);
  if (mprotect(thread->bionic_tls, BIONIC_TLS_SIZE, PROT_READ | PROT_WRITE) != 0) {
    __libc_fatal("failed to mprotect TLS");
  }
}

void __init_thread_stack_guard(pthread_internal_t* thread) {
  // GCC looks in the TLS for the stack guard on x86, so copy it there from our global.
  thread->tls[TLS_SLOT_STACK_GUARD] = reinterpret_cast<void*>(__stack_chk_guard);
}

void __init_alternate_signal_stack(pthread_internal_t* thread) {
  // Create and set an alternate signal stack.
  void* stack_base = mmap(NULL, SIGNAL_STACK_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (stack_base != MAP_FAILED) {

    // Create a guard page to catch stack overflows in signal handlers.
    if (mprotect(stack_base, PAGE_SIZE, PROT_NONE) == -1) {
      munmap(stack_base, SIGNAL_STACK_SIZE);
      return;
    }
    stack_t ss;
    ss.ss_sp = reinterpret_cast<uint8_t*>(stack_base) + PAGE_SIZE;
    ss.ss_size = SIGNAL_STACK_SIZE - PAGE_SIZE;
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);
    thread->alternate_signal_stack = stack_base;

    // We can only use const static allocated string for mapped region name, as Android kernel
    // uses the string pointer directly when dumping /proc/pid/maps.
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, ss.ss_sp, ss.ss_size, "thread signal stack");
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, stack_base, PAGE_SIZE, "thread signal stack guard page");
  }
}

int __init_thread(pthread_internal_t* thread) {
  int error = 0;

  if (__predict_true((thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) == 0)) {
    atomic_init(&thread->join_state, THREAD_NOT_JOINED);
  } else {
    atomic_init(&thread->join_state, THREAD_DETACHED);
  }

  // Set the scheduling policy/priority of the thread.
  if (thread->attr.sched_policy != SCHED_NORMAL) {
    sched_param param;
    param.sched_priority = thread->attr.sched_priority;
    if (sched_setscheduler(thread->tid, thread->attr.sched_policy, &param) == -1) {
#if defined(__LP64__)
      // For backwards compatibility reasons, we only report failures on 64-bit devices.
      error = errno;
#endif
      __libc_format_log(ANDROID_LOG_WARN, "libc",
                        "pthread_create sched_setscheduler call failed: %s", strerror(errno));
    }
  }

  thread->cleanup_stack = NULL;

  return error;
}

static void* __create_thread_mapped_space(size_t mmap_size, size_t stack_guard_size) {
  // Create a new private anonymous map.
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  void* space = mmap(NULL, mmap_size, prot, flags, -1, 0);
  if (space == MAP_FAILED) {
    __libc_format_log(ANDROID_LOG_WARN,
                      "libc",
                      "pthread_create failed: couldn't allocate %zu-bytes mapped space: %s",
                      mmap_size, strerror(errno));
    return NULL;
  }

  // Stack is at the lower end of mapped space, stack guard region is at the lower end of stack.
  // Set the stack guard region to PROT_NONE, so we can detect thread stack overflow.
  if (mprotect(space, stack_guard_size, PROT_NONE) == -1) {
    __libc_format_log(ANDROID_LOG_WARN, "libc",
                      "pthread_create failed: couldn't mprotect PROT_NONE %zu-byte stack guard region: %s",
                      stack_guard_size, strerror(errno));
    munmap(space, mmap_size);
    return NULL;
  }
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, space, stack_guard_size, "thread stack guard page");

  return space;
}

static int __allocate_thread(pthread_attr_t* attr, pthread_internal_t** threadp, void** child_stack) {
  size_t mmap_size;
  uint8_t* stack_top;

  if (attr->stack_base == NULL) {
    // The caller didn't provide a stack, so allocate one.
    // Make sure the stack size and guard size are multiples of PAGE_SIZE.
    mmap_size = BIONIC_ALIGN(attr->stack_size + sizeof(pthread_internal_t), PAGE_SIZE);
    attr->guard_size = BIONIC_ALIGN(attr->guard_size, PAGE_SIZE);
    attr->stack_base = __create_thread_mapped_space(mmap_size, attr->guard_size);
    if (attr->stack_base == NULL) {
      return EAGAIN;
    }
    stack_top = reinterpret_cast<uint8_t*>(attr->stack_base) + mmap_size;
  } else {
    // Remember the mmap size is zero and we don't need to free it.
    mmap_size = 0;
    stack_top = reinterpret_cast<uint8_t*>(attr->stack_base) + attr->stack_size;
  }

  // Mapped space(or user allocated stack) is used for:
  //   pthread_internal_t
  //   thread stack (including guard page)

  // To safely access the pthread_internal_t and thread stack, we need to find a 16-byte aligned boundary.
  stack_top = reinterpret_cast<uint8_t*>(
                (reinterpret_cast<uintptr_t>(stack_top) - sizeof(pthread_internal_t)) & ~0xf);

  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(stack_top);
  if (mmap_size == 0) {
    // If thread was not allocated by mmap(), it may not have been cleared to zero.
    // So assume the worst and zero it.
    memset(thread, 0, sizeof(pthread_internal_t));
  }
  attr->stack_size = stack_top - reinterpret_cast<uint8_t*>(attr->stack_base);

  thread->mmap_size = mmap_size;
  thread->attr = *attr;
  __init_tls(thread);
  __init_thread_stack_guard(thread);

  *threadp = thread;
  *child_stack = stack_top;
  return 0;
}

static int __pthread_start(void* arg) {
  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(arg);

  // Wait for our creating thread to release us. This lets it have time to
  // notify gdb about this thread before we start doing anything.
  // This also provides the memory barrier needed to ensure that all memory
  // accesses previously made by the creating thread are visible to us.
  thread->startup_handshake_lock.lock();

  __init_alternate_signal_stack(thread);

  void* result = thread->start_routine(thread->start_routine_arg);
  pthread_exit(result);

  return 0;
}

// A dummy start routine for pthread_create failures where we've created a thread but aren't
// going to run user code on it. We swap out the user's start routine for this and take advantage
// of the regular thread teardown to free up resources.
static void* __do_nothing(void*) {
  return NULL;
}

int pthread_create(pthread_t* thread_out, pthread_attr_t const* attr,
                   void* (*start_routine)(void*), void* arg) {
  ErrnoRestorer errno_restorer;

  // Inform the rest of the C library that at least one thread was created.
  __isthreaded = 1;

  pthread_attr_t thread_attr;
  if (attr == NULL) {
    pthread_attr_init(&thread_attr);
  } else {
    thread_attr = *attr;
    attr = NULL; // Prevent misuse below.
  }

  pthread_internal_t* thread = NULL;
  void* child_stack = NULL;
  int result = __allocate_thread(&thread_attr, &thread, &child_stack);
  if (result != 0) {
    return result;
  }

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
  void* tls = reinterpret_cast<void*>(thread->tls);
#if defined(__i386__)
  // On x86 (but not x86-64), CLONE_SETTLS takes a pointer to a struct user_desc rather than
  // a pointer to the TLS itself.
  user_desc tls_descriptor;
  __init_user_desc(&tls_descriptor, false, tls);
  tls = &tls_descriptor;
#endif
  int rc = clone(__pthread_start, child_stack, flags, thread, &(thread->tid), tls, &(thread->tid));
  if (rc == -1) {
    int clone_errno = errno;
    // We don't have to unlock the mutex at all because clone(2) failed so there's no child waiting to
    // be unblocked, but we're about to unmap the memory the mutex is stored in, so this serves as a
    // reminder that you can't rewrite this function to use a ScopedPthreadMutexLocker.
    thread->startup_handshake_lock.unlock();
    if (thread->mmap_size != 0) {
      munmap(thread->attr.stack_base, thread->mmap_size);
    }
    __libc_format_log(ANDROID_LOG_WARN, "libc", "pthread_create failed: clone failed: %s", strerror(errno));
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
