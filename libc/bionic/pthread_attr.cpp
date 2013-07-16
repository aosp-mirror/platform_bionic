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

#include "pthread_internal.h"

// Traditionally we give threads a 1MiB stack. When we started allocating per-thread
// alternate signal stacks to ease debugging of stack overflows, we subtracted the
// same amount we were using there from the default thread stack size. This should
// keep memory usage roughly constant.
#define DEFAULT_THREAD_STACK_SIZE ((1 * 1024 * 1024) - SIGSTKSZ)

int pthread_attr_init(pthread_attr_t* attr) {
  attr->flags = 0;
  attr->stack_base = NULL;
  attr->stack_size = DEFAULT_THREAD_STACK_SIZE;
  attr->guard_size = PAGE_SIZE;
  attr->sched_policy = SCHED_NORMAL;
  attr->sched_priority = 0;
  return 0;
}

int pthread_attr_destroy(pthread_attr_t* attr) {
  memset(attr, 0x42, sizeof(pthread_attr_t));
  return 0;
}

int pthread_attr_setdetachstate(pthread_attr_t* attr, int state) {
  if (state == PTHREAD_CREATE_DETACHED) {
    attr->flags |= PTHREAD_ATTR_FLAG_DETACHED;
  } else if (state == PTHREAD_CREATE_JOINABLE) {
    attr->flags &= ~PTHREAD_ATTR_FLAG_DETACHED;
  } else {
    return EINVAL;
  }
  return 0;
}

int pthread_attr_getdetachstate(pthread_attr_t const* attr, int* state) {
  *state = (attr->flags & PTHREAD_ATTR_FLAG_DETACHED) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE;
  return 0;
}

int pthread_attr_setschedpolicy(pthread_attr_t* attr, int policy) {
  attr->sched_policy = policy;
  return 0;
}

int pthread_attr_getschedpolicy(pthread_attr_t const* attr, int* policy) {
  *policy = attr->sched_policy;
  return 0;
}

int pthread_attr_setschedparam(pthread_attr_t * attr, struct sched_param const* param) {
  attr->sched_priority = param->sched_priority;
  return 0;
}

int pthread_attr_getschedparam(pthread_attr_t const* attr, struct sched_param* param) {
  param->sched_priority = attr->sched_priority;
  return 0;
}

int pthread_attr_setstacksize(pthread_attr_t* attr, size_t stack_size) {
  if (stack_size < PTHREAD_STACK_MIN) {
    return EINVAL;
  }
  attr->stack_size = stack_size;
  return 0;
}

int pthread_attr_getstacksize(pthread_attr_t const* attr, size_t* stack_size) {
  *stack_size = attr->stack_size;
  return 0;
}

int pthread_attr_setstackaddr(pthread_attr_t*, void*) {
  // This was removed from POSIX.1-2008, and is not implemented on bionic.
  // Needed for ABI compatibility with the NDK.
  return ENOSYS;
}

int pthread_attr_getstackaddr(pthread_attr_t const* attr, void** stack_addr) {
  // This was removed from POSIX.1-2008.
  // Needed for ABI compatibility with the NDK.
  *stack_addr = (char*)attr->stack_base + attr->stack_size;
  return 0;
}

int pthread_attr_setstack(pthread_attr_t* attr, void* stack_base, size_t stack_size) {
  if ((stack_size & (PAGE_SIZE - 1) || stack_size < PTHREAD_STACK_MIN)) {
    return EINVAL;
  }
  if ((uint32_t)stack_base & (PAGE_SIZE - 1)) {
    return EINVAL;
  }
  attr->stack_base = stack_base;
  attr->stack_size = stack_size;
  return 0;
}

int pthread_attr_getstack(pthread_attr_t const* attr, void** stack_base, size_t* stack_size) {
  *stack_base = attr->stack_base;
  *stack_size = attr->stack_size;
  return 0;
}

int pthread_attr_setguardsize(pthread_attr_t* attr, size_t guard_size) {
  attr->guard_size = guard_size;
  return 0;
}

int pthread_attr_getguardsize(pthread_attr_t const* attr, size_t* guard_size) {
  *guard_size = attr->guard_size;
  return 0;
}

int pthread_getattr_np(pthread_t thid, pthread_attr_t* attr) {
  pthread_internal_t* thread = (pthread_internal_t*) thid;
  *attr = thread->attr;
  return 0;
}

int pthread_attr_setscope(pthread_attr_t* , int scope) {
  if (scope == PTHREAD_SCOPE_SYSTEM) {
    return 0;
  }
  if (scope == PTHREAD_SCOPE_PROCESS) {
    return ENOTSUP;
  }
  return EINVAL;
}

int pthread_attr_getscope(pthread_attr_t const*) {
  return PTHREAD_SCOPE_SYSTEM;
}
