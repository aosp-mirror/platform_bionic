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
#include <sys/mman.h>

#include "pthread_internal.h"

#include "private/bionic_ssp.h"
#include "private/bionic_tls.h"
#include "private/libc_logging.h"
#include "private/thread_private.h"
#include "private/ErrnoRestorer.h"
#include "private/ScopedPthreadMutexLocker.h"

extern "C" int __pthread_clone(void* (*fn)(void*), void* child_stack, int flags, void* arg);

#ifdef __i386__
#define ATTRIBUTES __attribute__((noinline)) __attribute__((fastcall))
#else
#define ATTRIBUTES __attribute__((noinline))
#endif

extern "C" void ATTRIBUTES _thread_created_hook(pid_t thread_id);

static const int kPthreadInitFailed = 1;

static pthread_mutex_t gPthreadStackCreationLock = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t gDebuggerNotificationLock = PTHREAD_MUTEX_INITIALIZER;

void  __init_tls(pthread_internal_t* thread) {
  // Zero-initialize all the slots.
  for (size_t i = 0; i < BIONIC_TLS_SLOTS; ++i) {
    thread->tls[i] = NULL;
  }

  // Slot 0 must point to itself. The x86 Linux kernel reads the TLS from %fs:0.
  thread->tls[TLS_SLOT_SELF] = thread->tls;
  thread->tls[TLS_SLOT_THREAD_ID] = thread;
  // GCC looks in the TLS for the stack guard on x86, so copy it there from our global.
  thread->tls[TLS_SLOT_STACK_GUARD] = (void*) __stack_chk_guard;

  __set_tls(thread->tls);

  // Create and set an alternate signal stack.
  // This must happen after __set_tls, in case a system call fails and tries to set errno.
  stack_t ss;
  ss.ss_sp = mmap(NULL, SIGSTKSZ, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0);
  if (ss.ss_sp != MAP_FAILED) {
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);
    thread->alternate_signal_stack = ss.ss_sp;
  }
}

// This trampoline is called from the assembly _pthread_clone function.
// Our 'tls' and __pthread_clone's 'child_stack' are one and the same, just growing in
// opposite directions.
extern "C" void __thread_entry(void* (*func)(void*), void* arg, void** tls) {
  // Wait for our creating thread to release us. This lets it have time to
  // notify gdb about this thread before we start doing anything.
  // This also provides the memory barrier needed to ensure that all memory
  // accesses previously made by the creating thread are visible to us.
  pthread_mutex_t* start_mutex = (pthread_mutex_t*) &tls[TLS_SLOT_SELF];
  pthread_mutex_lock(start_mutex);
  pthread_mutex_destroy(start_mutex);

  pthread_internal_t* thread = (pthread_internal_t*) tls[TLS_SLOT_THREAD_ID];
  thread->tls = tls;
  __init_tls(thread);

  if ((thread->internal_flags & kPthreadInitFailed) != 0) {
    pthread_exit(NULL);
  }

  void* result = func(arg);
  pthread_exit(result);
}

__LIBC_ABI_PRIVATE__
int _init_thread(pthread_internal_t* thread, bool add_to_thread_list) {
  int error = 0;

  // Set the scheduling policy/priority of the thread.
  if (thread->attr.sched_policy != SCHED_NORMAL) {
    struct sched_param param;
    param.sched_priority = thread->attr.sched_priority;
    if (sched_setscheduler(thread->tid, thread->attr.sched_policy, &param) == -1) {
      // For backwards compatibility reasons, we just warn about failures here.
      // error = errno;
      __libc_format_log(ANDROID_LOG_WARN, "libc",
                        "pthread_create sched_setscheduler call failed: %s", strerror(errno));
    }
  }

  pthread_cond_init(&thread->join_cond, NULL);
  thread->cleanup_stack = NULL;

  if (add_to_thread_list) {
    _pthread_internal_add(thread);
  }

  return error;
}

static void* __create_thread_stack(pthread_internal_t* thread) {
  ScopedPthreadMutexLocker lock(&gPthreadStackCreationLock);

  // Create a new private anonymous map.
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  void* stack = mmap(NULL, thread->attr.stack_size, prot, flags, -1, 0);
  if (stack == MAP_FAILED) {
    __libc_format_log(ANDROID_LOG_WARN,
                      "libc",
                      "pthread_create failed: couldn't allocate %zd-byte stack: %s",
                      thread->attr.stack_size, strerror(errno));
    return NULL;
  }

  // Set the guard region at the end of the stack to PROT_NONE.
  if (mprotect(stack, thread->attr.guard_size, PROT_NONE) == -1) {
    __libc_format_log(ANDROID_LOG_WARN, "libc",
                      "pthread_create failed: couldn't mprotect PROT_NONE %zd-byte stack guard region: %s",
                      thread->attr.guard_size, strerror(errno));
    munmap(stack, thread->attr.stack_size);
    return NULL;
  }

  return stack;
}

int pthread_create(pthread_t* thread_out, pthread_attr_t const* attr,
                   void* (*start_routine)(void*), void* arg) {
  ErrnoRestorer errno_restorer;

  // Inform the rest of the C library that at least one thread
  // was created. This will enforce certain functions to acquire/release
  // locks (e.g. atexit()) to protect shared global structures.
  // This works because pthread_create() is not called by the C library
  // initialization routine that sets up the main thread's data structures.
  __isthreaded = 1;

  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(calloc(sizeof(*thread), 1));
  if (thread == NULL) {
    __libc_format_log(ANDROID_LOG_WARN, "libc", "pthread_create failed: couldn't allocate thread");
    return EAGAIN;
  }
  thread->allocated_on_heap = true;

  if (attr == NULL) {
    pthread_attr_init(&thread->attr);
  } else {
    thread->attr = *attr;
    attr = NULL; // Prevent misuse below.
  }

  // Make sure the stack size and guard size are multiples of PAGE_SIZE.
  thread->attr.stack_size = (thread->attr.stack_size + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1);
  thread->attr.guard_size = (thread->attr.guard_size + (PAGE_SIZE-1)) & ~(PAGE_SIZE-1);

  if (thread->attr.stack_base == NULL) {
    // The caller didn't provide a stack, so allocate one.
    thread->attr.stack_base = __create_thread_stack(thread);
    if (thread->attr.stack_base == NULL) {
      free(thread);
      return EAGAIN;
    }
  } else {
    // The caller did provide a stack, so remember we're not supposed to free it.
    thread->attr.flags |= PTHREAD_ATTR_FLAG_USER_STACK;
  }

  // Make room for the TLS area.
  // The child stack is the same address, just growing in the opposite direction.
  // At offsets >= 0, we have the TLS slots.
  // At offsets < 0, we have the child stack.
  void** tls = (void**)((uint8_t*)(thread->attr.stack_base) + thread->attr.stack_size - BIONIC_TLS_SLOTS * sizeof(void*));
  void* child_stack = tls;

  // Create a mutex for the thread in TLS_SLOT_SELF to wait on once it starts so we can keep
  // it from doing anything until after we notify the debugger about it
  //
  // This also provides the memory barrier we need to ensure that all
  // memory accesses previously performed by this thread are visible to
  // the new thread.
  pthread_mutex_t* start_mutex = (pthread_mutex_t*) &tls[TLS_SLOT_SELF];
  pthread_mutex_init(start_mutex, NULL);
  ScopedPthreadMutexLocker start_locker(start_mutex);

  tls[TLS_SLOT_THREAD_ID] = thread;

  int flags = CLONE_FILES | CLONE_FS | CLONE_VM | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM;

  int tid = __pthread_clone(start_routine, child_stack, flags, arg);
  if (tid < 0) {
    int clone_errno = errno;
    if ((thread->attr.flags & PTHREAD_ATTR_FLAG_USER_STACK) == 0) {
      munmap(thread->attr.stack_base, thread->attr.stack_size);
    }
    free(thread);
    __libc_format_log(ANDROID_LOG_WARN, "libc", "pthread_create failed: clone failed: %s", strerror(errno));
    return clone_errno;
  }

  thread->tid = tid;

  int init_errno = _init_thread(thread, true);
  if (init_errno != 0) {
    // Mark the thread detached and let its __thread_entry run to
    // completion. (It'll just exit immediately, cleaning up its resources.)
    thread->internal_flags |= kPthreadInitFailed;
    thread->attr.flags |= PTHREAD_ATTR_FLAG_DETACHED;
    return init_errno;
  }

  // Notify any debuggers about the new thread.
  {
    ScopedPthreadMutexLocker debugger_locker(&gDebuggerNotificationLock);
    _thread_created_hook(thread->tid);
  }

  // Publish the pthread_t and let the thread run.
  *thread_out = (pthread_t) thread;

  return 0;
}
