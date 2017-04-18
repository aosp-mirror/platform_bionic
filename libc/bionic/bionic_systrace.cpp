/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "private/bionic_lock.h"
#include "private/bionic_systrace.h"
#include "private/CachedProperty.h"
#include "private/libc_logging.h"

#include <cutils/trace.h> // For ATRACE_TAG_BIONIC.

#define WRITE_OFFSET   32

static Lock g_lock;
static int g_trace_marker_fd = -1;

static bool should_trace() {
  static CachedProperty g_debug_atrace_tags_enableflags("debug.atrace.tags.enableflags");
  static uint64_t g_tags;

  g_lock.lock();
  if (g_debug_atrace_tags_enableflags.DidChange()) {
    g_tags = strtoull(g_debug_atrace_tags_enableflags.Get(), nullptr, 0);
  }
  g_lock.unlock();
  return ((g_tags & ATRACE_TAG_BIONIC) != 0);
}

static int get_trace_marker_fd() {
  g_lock.lock();
  if (g_trace_marker_fd == -1) {
    g_trace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_CLOEXEC | O_WRONLY);
  }
  g_lock.unlock();
  return g_trace_marker_fd;
}

void bionic_trace_begin(const char* message) {
  if (!should_trace()) {
    return;
  }

  int trace_marker_fd = get_trace_marker_fd();
  if (trace_marker_fd == -1) {
    return;
  }

  // If bionic tracing has been enabled, then write the message to the
  // kernel trace_marker.
  int length = strlen(message);
  char buf[length + WRITE_OFFSET];
  size_t len = snprintf(buf, length + WRITE_OFFSET, "B|%d|%s", getpid(), message);

  // Tracing may stop just after checking property and before writing the message.
  // So the write is acceptable to fail. See b/20666100.
  TEMP_FAILURE_RETRY(write(trace_marker_fd, buf, len));
}

void bionic_trace_end() {
  if (!should_trace()) {
    return;
  }

  int trace_marker_fd = get_trace_marker_fd();
  if (trace_marker_fd == -1) {
    return;
  }

  TEMP_FAILURE_RETRY(write(trace_marker_fd, "E", 1));
}

ScopedTrace::ScopedTrace(const char* message) : called_end_(false) {
  bionic_trace_begin(message);
}

ScopedTrace::~ScopedTrace() {
  End();
}

void ScopedTrace::End() {
  if (!called_end_) {
    bionic_trace_end();
    called_end_ = true;
  }
}
