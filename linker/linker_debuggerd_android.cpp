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

#include "linker_debuggerd.h"

#include "debuggerd/handler.h"
#include "private/bionic_globals.h"

#include "linker_gdb_support.h"

#if defined(__ANDROID_APEX__)
static debugger_process_info get_process_info() {
  return {
      .abort_msg = __libc_shared_globals()->abort_msg,
      .fdsan_table = &__libc_shared_globals()->fd_table,
      .gwp_asan_state = __libc_shared_globals()->gwp_asan_state,
      .gwp_asan_metadata = __libc_shared_globals()->gwp_asan_metadata,
      .scudo_stack_depot = __libc_shared_globals()->scudo_stack_depot,
      .scudo_region_info = __libc_shared_globals()->scudo_region_info,
      .scudo_ring_buffer = __libc_shared_globals()->scudo_ring_buffer,
      .scudo_ring_buffer_size = __libc_shared_globals()->scudo_ring_buffer_size,
  };
}
#endif

void linker_debuggerd_init() {
  // There may be a version mismatch between the bootstrap linker and the crash_dump in the APEX,
  // so don't pass in any process info from the bootstrap linker.
  debuggerd_callbacks_t callbacks = {
#if defined(__ANDROID_APEX__)
      .get_process_info = get_process_info,
#endif
      .post_dump = notify_gdb_of_libraries,
  };
  debuggerd_init(&callbacks);
}
