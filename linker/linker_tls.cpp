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

#include "linker_tls.h"

#include "private/bionic_defs.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/linker_native_bridge.h"

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" void __linker_reserve_bionic_tls_in_static_tls() {
  __libc_shared_globals()->static_tls_layout.reserve_bionic_tls();
}

// Stub for linker static TLS layout.
void layout_linker_static_tls() {
  StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  layout.reserve_tcb();

  // The pthread key data is located at the very front of bionic_tls. As a
  // temporary workaround, allocate bionic_tls just after the thread pointer so
  // Golang can find its pthread key, as long as the executable's TLS segment is
  // small enough. Specifically, Golang scans forward 384 words from the TP on
  // ARM.
  //  - http://b/118381796
  //  - https://groups.google.com/d/msg/golang-dev/yVrkFnYrYPE/2G3aFzYqBgAJ
  __linker_reserve_bionic_tls_in_static_tls();

  layout.finish_layout();
}
