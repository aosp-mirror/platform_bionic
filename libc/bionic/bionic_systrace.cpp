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

#include <cutils/trace.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "private/bionic_systrace.h"
#include "private/libc_logging.h"

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#define WRITE_OFFSET   32

static const prop_info* g_pinfo = NULL;
static uint32_t g_serial = -1;
static uint64_t g_tags = 0;
static int g_trace_marker_fd = -1;

static bool should_trace() {
  // If g_pinfo is null, this means that systrace hasn't been run and it's safe to
  // assume that no trace writing will need to take place.  However, to avoid running
  // this costly find check each time, we set it to a non-tracing value so that next
  // time, it will just check the serial to see if the value has been changed.
  // this function also deals with the bootup case, during which the call to property
  // set will fail if the property server hasn't yet started.
  if (g_pinfo == NULL) {
    g_pinfo = __system_property_find("debug.atrace.tags.enableflags");
    if (g_pinfo == NULL) {
      __system_property_set("debug.atrace.tags.enableflags", "0");
      g_pinfo = __system_property_find("debug.atrace.tags.enableflags");
      if (g_pinfo == NULL) {
        return false;
      }
    }
  }

  // Find out which tags have been enabled on the command line and set
  // the value of tags accordingly.  If the value of the property changes,
  // the serial will also change, so the costly system_property_read function
  // can be avoided by calling the much cheaper system_property_serial
  // first.  The values within pinfo may change, but its location is guaranteed
  // not to move.
  const uint32_t cur_serial = __system_property_serial(g_pinfo);
  if (cur_serial != g_serial) {
    g_serial = cur_serial;
    char value[PROP_VALUE_MAX];
    __system_property_read(g_pinfo, 0, value);
    g_tags = strtoull(value, NULL, 0);
  }

  // Finally, verify that this tag value enables bionic tracing.
  return ((g_tags & ATRACE_TAG_BIONIC) != 0);
}

ScopedTrace::ScopedTrace(const char* message) {
  if (!should_trace()) {
    return;
  }

  if (g_trace_marker_fd == -1) {
    g_trace_marker_fd = open("/sys/kernel/debug/tracing/trace_marker", O_WRONLY | O_CLOEXEC);
    if (g_trace_marker_fd == -1) {
      __libc_fatal("Could not open kernel trace file: %s\n", strerror(errno));
    }
  }

  // If bionic tracing has been enabled, then write the message to the
  // kernel trace_marker.
  int length = strlen(message);
  char buf[length + WRITE_OFFSET];
  size_t len = snprintf(buf, length + WRITE_OFFSET, "B|%d|%s", getpid(), message);
  ssize_t wbytes = TEMP_FAILURE_RETRY(write(g_trace_marker_fd, buf, len));

  // Error while writing
  if (static_cast<size_t>(wbytes) != len) {
    __libc_fatal("Could not write to kernel trace file: %s\n", strerror(errno));
  }
}

ScopedTrace::~ScopedTrace() {
  if (!should_trace()) {
    return;
  }

  ssize_t wbytes = TEMP_FAILURE_RETRY(write(g_trace_marker_fd, "E", 1));

  // Error while writing
  if (static_cast<size_t>(wbytes) != 1) {
    __libc_fatal("Could not write to kernel trace file: %s\n", strerror(errno));
  }
}
