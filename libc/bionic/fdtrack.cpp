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

#include <stdatomic.h>

#include <platform/bionic/fdtrack.h>
#include <platform/bionic/reserved_signals.h>

#include "private/bionic_fdtrack.h"
#include "private/bionic_tls.h"
#include "private/bionic_globals.h"

_Atomic(android_fdtrack_hook_t) __android_fdtrack_hook;

bool android_fdtrack_get_enabled() {
  return !__get_bionic_tls().fdtrack_disabled;
}

bool android_fdtrack_set_enabled(bool new_value) {
  auto& tls = __get_bionic_tls();
  bool prev = !tls.fdtrack_disabled;
  tls.fdtrack_disabled = !new_value;
  return prev;
}

bool android_fdtrack_compare_exchange_hook(android_fdtrack_hook_t* expected,
                                           android_fdtrack_hook_t value) {
  return atomic_compare_exchange_strong(&__android_fdtrack_hook, expected, value);
}

void __libc_init_fdtrack() {
  // Register a no-op signal handler.
  signal(BIONIC_SIGNAL_FDTRACK, [](int) {});
}
