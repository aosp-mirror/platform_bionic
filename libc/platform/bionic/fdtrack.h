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

#pragma once

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdint.h>

__BEGIN_DECLS

// Types of an android_fdtrack_event.
enum android_fdtrack_event_type {
  // File descriptor creation: create is the active member of android_fdtrack_event::data.
  ANDROID_FDTRACK_EVENT_TYPE_CREATE,

  // File descriptor closed.
  ANDROID_FDTRACK_EVENT_TYPE_CLOSE,
};

struct android_fdtrack_event {
  // File descriptor for which this event occurred.
  int fd;

  // Type of event: this is one of the enumerators of android_fdtrack_event_type.
  uint8_t type;

  // Data for the event.
  union {
    struct {
      const char* function_name;
    } create;
  } data;
};

// Callback invoked upon file descriptor creation/closure.
typedef void (*android_fdtrack_hook_t)(struct android_fdtrack_event*);

// Register a hook which is called to track fd lifecycle events.
bool android_fdtrack_compare_exchange_hook(android_fdtrack_hook_t* expected, android_fdtrack_hook_t value) __INTRODUCED_IN(30);

// Enable/disable fdtrack *on the current thread*.
// This is primarily useful when performing operations which you don't want to track
// (e.g. when emitting already-recorded information).
bool android_fdtrack_get_enabled() __INTRODUCED_IN(30);
bool android_fdtrack_set_enabled(bool new_value) __INTRODUCED_IN(30);

__END_DECLS
