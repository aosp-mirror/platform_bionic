/*
 * Copyright (C) 2012 The Android Open Source Project
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

#ifndef _BIONIC_THREAD_LOCAL_BUFFER_H_included
#define _BIONIC_THREAD_LOCAL_BUFFER_H_included

#include <malloc.h>
#include <pthread.h>

// libstdc++ currently contains __cxa_guard_acquire and __cxa_guard_release,
// so we make do with macros instead of a C++ class.
// TODO: move __cxa_guard_acquire and __cxa_guard_release into libc.

#define GLOBAL_INIT_THREAD_LOCAL_BUFFER(name) \
  static pthread_once_t name ## _once; \
  static pthread_key_t name ## _key; \
  static void name ## _key_destroy(void* buffer) { \
    free(buffer); \
  } \
  static void name ## _key_init() { \
    pthread_key_create(&name ## _key, name ## _key_destroy); \
  }

// Leaves "name_buffer" and "name_byte_count" defined and initialized.
#define LOCAL_INIT_THREAD_LOCAL_BUFFER(type, name, byte_count) \
  pthread_once(&name ## _once, name ## _key_init); \
  type name ## _buffer = reinterpret_cast<type>(pthread_getspecific(name ## _key)); \
  if (name ## _buffer == NULL) { \
    name ## _buffer = reinterpret_cast<type>(malloc(byte_count)); \
    pthread_setspecific(name ## _key, name ## _buffer); \
  } \
  const size_t name ## _buffer_size = byte_count

#endif // _BIONIC_THREAD_LOCAL_BUFFER_H_included
