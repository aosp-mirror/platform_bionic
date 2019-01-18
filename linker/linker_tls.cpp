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

#include <vector>

#include "private/ScopedRWLock.h"
#include "private/bionic_defs.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/linker_native_bridge.h"
#include "linker_main.h"
#include "linker_soinfo.h"

static bool g_static_tls_finished;
static std::vector<TlsModule> g_tls_modules;

static inline size_t module_id_to_idx(size_t id) { return id - 1; }
static inline size_t module_idx_to_id(size_t idx) { return idx + 1; }

static size_t get_unused_module_index() {
  for (size_t i = 0; i < g_tls_modules.size(); ++i) {
    if (g_tls_modules[i].soinfo_ptr == nullptr) {
      return i;
    }
  }
  g_tls_modules.push_back({});
  __libc_shared_globals()->tls_modules.module_count = g_tls_modules.size();
  __libc_shared_globals()->tls_modules.module_table = g_tls_modules.data();
  return g_tls_modules.size() - 1;
}

static void register_tls_module(soinfo* si, size_t static_offset) {
  // The global TLS module table points at the std::vector of modules declared
  // in this file, so acquire a write lock before modifying the std::vector.
  ScopedWriteLock locker(&__libc_shared_globals()->tls_modules.rwlock);

  size_t module_idx = get_unused_module_index();

  soinfo_tls* si_tls = si->get_tls();
  si_tls->module_id = module_idx_to_id(module_idx);

  g_tls_modules[module_idx] = {
    .segment = si_tls->segment,
    .static_offset = static_offset,
    .first_generation = ++__libc_shared_globals()->tls_modules.generation,
    .soinfo_ptr = si,
  };
}

static void unregister_tls_module(soinfo* si) {
  ScopedWriteLock locker(&__libc_shared_globals()->tls_modules.rwlock);

  soinfo_tls* si_tls = si->get_tls();
  TlsModule& mod = g_tls_modules[module_id_to_idx(si_tls->module_id)];
  CHECK(mod.static_offset == SIZE_MAX);
  CHECK(mod.soinfo_ptr == si);
  mod = {};
  si_tls->module_id = kUninitializedModuleId;
}

// The reference is valid until a TLS module is registered or unregistered.
const TlsModule& get_tls_module(size_t module_id) {
  size_t module_idx = module_id_to_idx(module_id);
  CHECK(module_idx < g_tls_modules.size());
  return g_tls_modules[module_idx];
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
extern "C" void __linker_reserve_bionic_tls_in_static_tls() {
  __libc_shared_globals()->static_tls_layout.reserve_bionic_tls();
}

void linker_setup_exe_static_tls(const char* progname) {
  soinfo* somain = solist_get_somain();
  StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  if (somain->get_tls() == nullptr) {
    layout.reserve_exe_segment_and_tcb(nullptr, progname);
  } else {
    register_tls_module(somain, layout.reserve_exe_segment_and_tcb(&somain->get_tls()->segment, progname));
  }

  // The pthread key data is located at the very front of bionic_tls. As a
  // temporary workaround, allocate bionic_tls just after the thread pointer so
  // Golang can find its pthread key, as long as the executable's TLS segment is
  // small enough. Specifically, Golang scans forward 384 words from the TP on
  // ARM.
  //  - http://b/118381796
  //  - https://github.com/golang/go/issues/29674
  __linker_reserve_bionic_tls_in_static_tls();
}

void linker_finalize_static_tls() {
  g_static_tls_finished = true;
  __libc_shared_globals()->static_tls_layout.finish_layout();
}

void register_soinfo_tls(soinfo* si) {
  soinfo_tls* si_tls = si->get_tls();
  if (si_tls == nullptr || si_tls->module_id != kUninitializedModuleId) {
    return;
  }
  size_t static_offset = SIZE_MAX;
  if (!g_static_tls_finished) {
    StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
    static_offset = layout.reserve_solib_segment(si_tls->segment);
  }
  register_tls_module(si, static_offset);
}

void unregister_soinfo_tls(soinfo* si) {
  soinfo_tls* si_tls = si->get_tls();
  if (si_tls == nullptr || si_tls->module_id == kUninitializedModuleId) {
    return;
  }
  return unregister_tls_module(si);
}
