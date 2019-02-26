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

#if defined(LIBC_STATIC)
#error This file should not be compiled for static targets.
#endif

#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <private/bionic_config.h>
#include <private/bionic_malloc.h>
#include <private/bionic_malloc_dispatch.h>
#include <sys/system_properties.h>

#include "malloc_common.h"
#include "malloc_common_dynamic.h"
#include "malloc_heapprofd.h"

static constexpr char kHeapprofdSharedLib[] = "heapprofd_client.so";
static constexpr char kHeapprofdPrefix[] = "heapprofd";
static constexpr char kHeapprofdPropertyEnable[] = "heapprofd.enable";
static constexpr int kHeapprofdSignal = __SIGRTMIN + 4;

// The logic for triggering heapprofd (at runtime) is as follows:
// 1. HEAPPROFD_SIGNAL is received by the process, entering the
//    MaybeInstallInitHeapprofdHook signal handler.
// 2. If the initialization is not already in flight
//    (gHeapprofdInitInProgress is false), the malloc hook is set to
//    point at InitHeapprofdHook, and gHeapprofdInitInProgress is set to
//    true.
// 3. The next malloc call enters InitHeapprofdHook, which removes the malloc
//    hook, and spawns a detached pthread to run the InitHeapprofd task.
//    (gHeapprofdInitHook_installed atomic is used to perform this once.)
// 4. InitHeapprofd, on a dedicated pthread, loads the heapprofd client library,
//    installs the full set of heapprofd hooks, and invokes the client's
//    initializer. The dedicated pthread then terminates.
// 5. gHeapprofdInitInProgress and gHeapprofdInitHookInstalled are
//    reset to false such that heapprofd can be reinitialized. Reinitialization
//    means that a new profiling session is started, and any still active is
//    torn down.
//
// The incremental hooking and a dedicated task thread are used since we cannot
// do heavy work within a signal handler, or when blocking a malloc invocation.

// The handle returned by dlopen when previously loading the heapprofd
// hooks. nullptr if shared library has not been already been loaded.
static _Atomic (void*) gHeapprofdHandle = nullptr;

static _Atomic bool gHeapprofdInitInProgress = false;
static _Atomic bool gHeapprofdInitHookInstalled = false;

// In a Zygote child process, this is set to true if profiling of this process
// is allowed. Note that this is set at a later time than the global
// gMallocLeakZygoteChild. The latter is set during the fork (while still in
// zygote's SELinux domain). While this bit is set after the child is
// specialized (and has transferred SELinux domains if applicable).
static _Atomic bool gMallocZygoteChildProfileable = false;

extern "C" void* MallocInitHeapprofdHook(size_t);

static constexpr MallocDispatch __heapprofd_init_dispatch
  __attribute__((unused)) = {
    Malloc(calloc),
    Malloc(free),
    Malloc(mallinfo),
    MallocInitHeapprofdHook,
    Malloc(malloc_usable_size),
    Malloc(memalign),
    Malloc(posix_memalign),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
    Malloc(pvalloc),
#endif
    Malloc(realloc),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
    Malloc(valloc),
#endif
    Malloc(iterate),
    Malloc(malloc_disable),
    Malloc(malloc_enable),
    Malloc(mallopt),
    Malloc(aligned_alloc),
  };

static void MaybeInstallInitHeapprofdHook(int) {
  // Zygote child processes must be marked profileable.
  if (gMallocLeakZygoteChild &&
      !atomic_load_explicit(&gMallocZygoteChildProfileable, memory_order_acquire)) {
    return;
  }

  if (!atomic_exchange(&gHeapprofdInitInProgress, true)) {
    __libc_globals.mutate([](libc_globals* globals) {
      atomic_store(&globals->current_dispatch_table, &__heapprofd_init_dispatch);
    });
  }
}

static bool GetHeapprofdProgramProperty(char* data, size_t size) {
  constexpr char prefix[] = "heapprofd.enable.";
  // - 1 to skip nullbyte, which we will write later.
  constexpr size_t prefix_size = sizeof(prefix) - 1;
  if (size < prefix_size) {
    error_log("%s: Overflow constructing heapprofd property", getprogname());
    return false;
  }
  memcpy(data, prefix, prefix_size);

  int fd = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
  if (fd == -1) {
    error_log("%s: Failed to open /proc/self/cmdline", getprogname());
    return false;
  }
  char cmdline[128];
  ssize_t rd = read(fd, cmdline, sizeof(cmdline) - 1);
  close(fd);
  if (rd == -1) {
    error_log("%s: Failed to read /proc/self/cmdline", getprogname());
    return false;
  }
  cmdline[rd] = '\0';
  char* first_arg = static_cast<char*>(memchr(cmdline, '\0', rd));
  if (first_arg == nullptr || first_arg == cmdline + size - 1) {
    error_log("%s: Overflow reading cmdline", getprogname());
    return false;
  }
  // For consistency with what we do with Java app cmdlines, trim everything
  // after the @ sign of the first arg.
  char* first_at = static_cast<char*>(memchr(cmdline, '@', rd));
  if (first_at != nullptr && first_at < first_arg) {
    *first_at = '\0';
    first_arg = first_at;
  }

  char* start = static_cast<char*>(memrchr(cmdline, '/', first_arg - cmdline));
  if (start == first_arg) {
    // The first argument ended in a slash.
    error_log("%s: cmdline ends in /", getprogname());
    return false;
  } else if (start == nullptr) {
    start = cmdline;
  } else {
    // Skip the /.
    start++;
  }

  size_t name_size = static_cast<size_t>(first_arg - start);
  if (name_size >= size - prefix_size) {
    error_log("%s: overflow constructing heapprofd property.", getprogname());
    return false;
  }
  // + 1 to also copy the trailing null byte.
  memcpy(data + prefix_size, start, name_size + 1);
  return true;
}

bool HeapprofdShouldLoad() {
  // First check for heapprofd.enable. If it is set to "all", enable
  // heapprofd for all processes. Otherwise, check heapprofd.enable.${prog},
  // if it is set and not 0, enable heap profiling for this process.
  char property_value[PROP_VALUE_MAX];
  if (__system_property_get(kHeapprofdPropertyEnable, property_value) == 0) {
    return false;
  }
  if (strcmp(property_value, "all") == 0) {
    return true;
  }

  char program_property[128];
  if (!GetHeapprofdProgramProperty(program_property,
                                   sizeof(program_property))) {
    return false;
  }
  if (__system_property_get(program_property, property_value) == 0) {
    return false;
  }
  return property_value[0] != '\0';
}

void HeapprofdInstallSignalHandler() {
  struct sigaction action = {};
  action.sa_handler = MaybeInstallInitHeapprofdHook;
  sigaction(kHeapprofdSignal, &action, nullptr);
}

static void DisplayError(int) {
  error_log("Cannot install heapprofd while malloc debug/malloc hooks are enabled.");
}

void HeapprofdInstallErrorSignalHandler() {
  struct sigaction action = {};
  action.sa_handler = DisplayError;
  sigaction(kHeapprofdSignal, &action, nullptr);
}

static void CommonInstallHooks(libc_globals* globals) {
  void* impl_handle = atomic_load(&gHeapprofdHandle);
  bool reusing_handle = impl_handle != nullptr;
  if (!reusing_handle) {
    impl_handle = LoadSharedLibrary(kHeapprofdSharedLib, kHeapprofdPrefix, &globals->malloc_dispatch_table);
    if (impl_handle == nullptr) {
      return;
    }
  } else if (!InitSharedLibrary(impl_handle, kHeapprofdSharedLib, kHeapprofdPrefix, &globals->malloc_dispatch_table)) {
    return;
  }

  if (FinishInstallHooks(globals, nullptr, kHeapprofdPrefix)) {
    atomic_store(&gHeapprofdHandle, impl_handle);
  } else if (!reusing_handle) {
    dlclose(impl_handle);
  }

  atomic_store(&gHeapprofdInitInProgress, false);
}

void HeapprofdInstallHooksAtInit(libc_globals* globals) {
  if (atomic_exchange(&gHeapprofdInitInProgress, true)) {
    return;
  }
  CommonInstallHooks(globals);
}

static void* InitHeapprofd(void*) {
  __libc_globals.mutate([](libc_globals* globals) {
    CommonInstallHooks(globals);
  });

  // Allow to install hook again to re-initialize heap profiling after the
  // current session finished.
  atomic_store(&gHeapprofdInitHookInstalled, false);
  return nullptr;
}

extern "C" void* MallocInitHeapprofdHook(size_t bytes) {
  if (!atomic_exchange(&gHeapprofdInitHookInstalled, true)) {
    __libc_globals.mutate([](libc_globals* globals) {
      atomic_store(&globals->current_dispatch_table, nullptr);
    });

    pthread_t thread_id;
    if (pthread_create(&thread_id, nullptr, InitHeapprofd, nullptr) != 0) {
      error_log("%s: heapprofd: failed to pthread_create.", getprogname());
    } else if (pthread_detach(thread_id) != 0) {
      error_log("%s: heapprofd: failed to pthread_detach", getprogname());
    }
    if (pthread_setname_np(thread_id, "heapprofdinit") != 0) {
      error_log("%s: heapprod: failed to pthread_setname_np", getprogname());
    }
  }
  return Malloc(malloc)(bytes);
}

// Marks this process as a profileable zygote child.
static bool HandleInitZygoteChildProfiling() {
  atomic_store_explicit(&gMallocZygoteChildProfileable, true, memory_order_release);

  // Conditionally start "from startup" profiling.
  if (HeapprofdShouldLoad()) {
    // Directly call the signal handler (will correctly guard against
    // concurrent signal delivery).
    MaybeInstallInitHeapprofdHook(kHeapprofdSignal);
  }
  return true;
}

static bool DispatchReset() {
  if (!atomic_exchange(&gHeapprofdInitInProgress, true)) {
    __libc_globals.mutate([](libc_globals* globals) {
      atomic_store(&globals->current_dispatch_table, nullptr);
    });
    atomic_store(&gHeapprofdInitInProgress, false);
    return true;
  }
  errno = EAGAIN;
  return false;
}

bool HeapprofdMallopt(int opcode, void* arg, size_t arg_size) {
  if (opcode == M_INIT_ZYGOTE_CHILD_PROFILING) {
    if (arg != nullptr || arg_size != 0) {
      errno = EINVAL;
      return false;
    }
    return HandleInitZygoteChildProfiling();
  }
  if (opcode == M_RESET_HOOKS) {
    if (arg != nullptr || arg_size != 0) {
      errno = EINVAL;
      return false;
    }
    return DispatchReset();
  }
  errno = ENOTSUP;
  return false;
}
