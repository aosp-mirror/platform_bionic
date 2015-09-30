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

#include "libc_init_common.h"

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/personality.h>
#include <sys/time.h>
#include <unistd.h>

#include "private/bionic_auxv.h"
#include "private/bionic_ssp.h"
#include "private/bionic_tls.h"
#include "private/KernelArgumentBlock.h"
#include "private/libc_logging.h"
#include "pthread_internal.h"

extern "C" abort_msg_t** __abort_message_ptr;
extern "C" void __bionic_setjmp_cookie_init(void);
extern "C" int __system_properties_init(void);
extern "C" int __set_tls(void* ptr);
extern "C" int __set_tid_address(int* tid_address);

__LIBC_HIDDEN__ void __libc_init_vdso();

// Not public, but well-known in the BSDs.
const char* __progname;

// Declared in <unistd.h>.
char** environ;

// Declared in "private/bionic_ssp.h".
uintptr_t __stack_chk_guard = 0;

// Setup for the main thread. For dynamic executables, this is called by the
// linker _before_ libc is mapped in memory. This means that all writes to
// globals from this function will apply to linker-private copies and will not
// be visible from libc later on.
//
// Note: this function creates a pthread_internal_t for the initial thread and
// stores the pointer in TLS, but does not add it to pthread's thread list. This
// has to be done later from libc itself (see __libc_init_common).
void __libc_init_main_thread(KernelArgumentBlock& args) {
  __libc_auxv = args.auxv;

  static pthread_internal_t main_thread;

  // Tell the kernel to clear our tid field when we exit, so we're like any other pthread.
  // As a side-effect, this tells us our pid (which is the same as the main thread's tid).
  main_thread.tid = __set_tid_address(&main_thread.tid);
  main_thread.set_cached_pid(main_thread.tid);

  // We don't want to free the main thread's stack even when the main thread exits
  // because things like environment variables with global scope live on it.
  // We also can't free the pthread_internal_t itself, since that lives on the main
  // thread's stack rather than on the heap.
  // The main thread has no mmap allocated space for stack or pthread_internal_t.
  main_thread.mmap_size = 0;
  pthread_attr_init(&main_thread.attr);
  main_thread.attr.guard_size = 0; // The main thread has no guard page.
  main_thread.attr.stack_size = 0; // User code should never see this; we'll compute it when asked.
  // TODO: the main thread's sched_policy and sched_priority need to be queried.

  __init_thread(&main_thread);
  __init_tls(&main_thread);
  __set_tls(main_thread.tls);

  // Store a pointer to the kernel argument block in a TLS slot to be
  // picked up by the libc constructor.
  main_thread.tls[TLS_SLOT_BIONIC_PREINIT] = &args;

  __init_alternate_signal_stack(&main_thread);
}

void __libc_init_common(KernelArgumentBlock& args) {
  // Initialize various globals.
  environ = args.envp;
  errno = 0;
  __libc_auxv = args.auxv;
  __progname = args.argv[0] ? args.argv[0] : "<unknown>";
  __abort_message_ptr = args.abort_message_ptr;

  // AT_RANDOM is a pointer to 16 bytes of randomness on the stack.
  __stack_chk_guard = *reinterpret_cast<uintptr_t*>(getauxval(AT_RANDOM));

  // Get the main thread from TLS and add it to the thread list.
  pthread_internal_t* main_thread = __get_thread();
  __pthread_internal_add(main_thread);

  __system_properties_init(); // Requires 'environ'.

  __bionic_setjmp_cookie_init();
  __libc_init_vdso();
}

__noreturn static void __early_abort(int line) {
  // We can't write to stdout or stderr because we're aborting before we've checked that
  // it's safe for us to use those file descriptors. We probably can't strace either, so
  // we rely on the fact that if we dereference a low address, either debuggerd or the
  // kernel's crash dump will show the fault address.
  *reinterpret_cast<int*>(line) = 0;
  _exit(EXIT_FAILURE);
}

// Force any of the closed stdin, stdout and stderr to be associated with /dev/null.
static void __nullify_closed_stdio() {
  int dev_null = TEMP_FAILURE_RETRY(open("/dev/null", O_RDWR));
  if (dev_null == -1) {
    // init won't have /dev/null available, but SELinux provides an equivalent.
    dev_null = TEMP_FAILURE_RETRY(open("/sys/fs/selinux/null", O_RDWR));
  }
  if (dev_null == -1) {
    __early_abort(__LINE__);
  }

  // If any of the stdio file descriptors is valid and not associated
  // with /dev/null, dup /dev/null to it.
  for (int i = 0; i < 3; i++) {
    // If it is /dev/null already, we are done.
    if (i == dev_null) {
      continue;
    }

    // Is this fd already open?
    int status = TEMP_FAILURE_RETRY(fcntl(i, F_GETFL));
    if (status != -1) {
      continue;
    }

    // The only error we allow is that the file descriptor does not
    // exist, in which case we dup /dev/null to it.
    if (errno == EBADF) {
      // Try dupping /dev/null to this stdio file descriptor and
      // repeat if there is a signal. Note that any errors in closing
      // the stdio descriptor are lost.
      status = TEMP_FAILURE_RETRY(dup2(dev_null, i));
      if (status == -1) {
        __early_abort(__LINE__);
      }
    } else {
      __early_abort(__LINE__);
    }
  }

  // If /dev/null is not one of the stdio file descriptors, close it.
  if (dev_null > 2) {
    if (close(dev_null) == -1) {
      __early_abort(__LINE__);
    }
  }
}

// Check if the environment variable definition at 'envstr'
// starts with '<name>=', and if so return the address of the
// first character after the equal sign. Otherwise return null.
static const char* env_match(const char* envstr, const char* name) {
  size_t i = 0;

  while (envstr[i] == name[i] && name[i] != '\0') {
    ++i;
  }

  if (name[i] == '\0' && envstr[i] == '=') {
    return envstr + i + 1;
  }

  return nullptr;
}

static bool __is_valid_environment_variable(const char* name) {
  // According to the kernel source, by default the kernel uses 32*PAGE_SIZE
  // as the maximum size for an environment variable definition.
  const int MAX_ENV_LEN = 32*4096;

  if (name == nullptr) {
    return false;
  }

  // Parse the string, looking for the first '=' there, and its size.
  int pos = 0;
  int first_equal_pos = -1;
  while (pos < MAX_ENV_LEN) {
    if (name[pos] == '\0') {
      break;
    }
    if (name[pos] == '=' && first_equal_pos < 0) {
      first_equal_pos = pos;
    }
    pos++;
  }

  // Check that it's smaller than MAX_ENV_LEN (to detect non-zero terminated strings).
  if (pos >= MAX_ENV_LEN) {
    return false;
  }

  // Check that it contains at least one equal sign that is not the first character
  if (first_equal_pos < 1) {
    return false;
  }

  return true;
}

static bool __is_unsafe_environment_variable(const char* name) {
  // None of these should be allowed in setuid programs.
  static const char* const UNSAFE_VARIABLE_NAMES[] = {
      "GCONV_PATH",
      "GETCONF_DIR",
      "HOSTALIASES",
      "JE_MALLOC_CONF",
      "LD_AOUT_LIBRARY_PATH",
      "LD_AOUT_PRELOAD",
      "LD_AUDIT",
      "LD_DEBUG",
      "LD_DEBUG_OUTPUT",
      "LD_DYNAMIC_WEAK",
      "LD_LIBRARY_PATH",
      "LD_ORIGIN_PATH",
      "LD_PRELOAD",
      "LD_PROFILE",
      "LD_SHOW_AUXV",
      "LD_USE_LOAD_BIAS",
      "LOCALDOMAIN",
      "LOCPATH",
      "MALLOC_CHECK_",
      "MALLOC_CONF",
      "MALLOC_TRACE",
      "NIS_PATH",
      "NLSPATH",
      "RESOLV_HOST_CONF",
      "RES_OPTIONS",
      "TMPDIR",
      "TZDIR",
      nullptr
  };
  for (size_t i = 0; UNSAFE_VARIABLE_NAMES[i] != nullptr; ++i) {
    if (env_match(name, UNSAFE_VARIABLE_NAMES[i]) != nullptr) {
      return true;
    }
  }
  return false;
}

static void __sanitize_environment_variables(char** env) {
  bool is_AT_SECURE = getauxval(AT_SECURE);
  char** src = env;
  char** dst = env;
  for (; src[0] != nullptr; ++src) {
    if (!__is_valid_environment_variable(src[0])) {
      continue;
    }
    // Remove various unsafe environment variables if we're loading a setuid program.
    if (is_AT_SECURE && __is_unsafe_environment_variable(src[0])) {
      continue;
    }
    dst[0] = src[0];
    ++dst;
  }
  dst[0] = nullptr;
}

static void __initialize_personality() {
#if !defined(__LP64__)
  int old_value = personality(0xffffffff);
  if (old_value == -1) {
    __libc_fatal("error getting old personality value: %s", strerror(errno));
  }

  if (personality((static_cast<unsigned int>(old_value) & ~PER_MASK) | PER_LINUX32) == -1) {
    __libc_fatal("error setting PER_LINUX32 personality: %s", strerror(errno));
  }
#endif
}

void __libc_init_AT_SECURE(KernelArgumentBlock& args) {
  __libc_auxv = args.auxv;

  // Check that the kernel provided a value for AT_SECURE.
  bool found_AT_SECURE = false;
  for (ElfW(auxv_t)* v = __libc_auxv; v->a_type != AT_NULL; ++v) {
    if (v->a_type == AT_SECURE) {
      found_AT_SECURE = true;
      break;
    }
  }
  if (!found_AT_SECURE) __early_abort(__LINE__);

  if (getauxval(AT_SECURE)) {
    // If this is a setuid/setgid program, close the security hole described in
    // ftp://ftp.freebsd.org/pub/FreeBSD/CERT/advisories/FreeBSD-SA-02:23.stdio.asc
    __nullify_closed_stdio();

    __sanitize_environment_variables(args.envp);
  }

  // Now the environment has been sanitized, make it available.
  environ = args.envp;

  __initialize_personality();
}

/* This function will be called during normal program termination
 * to run the destructors that are listed in the .fini_array section
 * of the executable, if any.
 *
 * 'fini_array' points to a list of function addresses. The first
 * entry in the list has value -1, the last one has value 0.
 */
void __libc_fini(void* array) {
  typedef void (*Dtor)();
  Dtor* fini_array = reinterpret_cast<Dtor*>(array);
  const Dtor minus1 = reinterpret_cast<Dtor>(static_cast<uintptr_t>(-1));

  // Sanity check - first entry must be -1.
  if (array == NULL || fini_array[0] != minus1) {
    return;
  }

  // Skip over it.
  fini_array += 1;

  // Count the number of destructors.
  int count = 0;
  while (fini_array[count] != NULL) {
    ++count;
  }

  // Now call each destructor in reverse order.
  while (count > 0) {
    Dtor dtor = fini_array[--count];

    // Sanity check, any -1 in the list is ignored.
    if (dtor == minus1) {
      continue;
    }

    dtor();
  }

#ifndef LIBC_STATIC
  {
    extern void __libc_postfini(void) __attribute__((weak));
    if (__libc_postfini) {
      __libc_postfini();
    }
  }
#endif
}
