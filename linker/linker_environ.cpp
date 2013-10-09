/*
 * Copyright (C) 2010 The Android Open Source Project
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

#include "linker_environ.h"

#include <linux/auxvec.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

#include "private/KernelArgumentBlock.h"

static char** _envp;
static bool _AT_SECURE_value = true;

bool get_AT_SECURE() {
  return _AT_SECURE_value;
}

static void __init_AT_SECURE(KernelArgumentBlock& args) {
  // Check auxv for AT_SECURE first to see if program is setuid, setgid,
  // has file caps, or caused a SELinux/AppArmor domain transition.
  bool kernel_supplied_AT_SECURE;
  _AT_SECURE_value = args.getauxval(AT_SECURE, &kernel_supplied_AT_SECURE);

  // We don't support ancient kernels.
  if (!kernel_supplied_AT_SECURE) {
    const char* msg = "FATAL: kernel did not supply AT_SECURE\n";
    write(2, msg, strlen(msg));
    exit(EXIT_FAILURE);
  }
}

// Check if the environment variable definition at 'envstr'
// starts with '<name>=', and if so return the address of the
// first character after the equal sign. Otherwise return NULL.
static const char* env_match(const char* envstr, const char* name) {
  size_t i = 0;

  while (envstr[i] == name[i] && name[i] != '\0') {
    ++i;
  }

  if (name[i] == '\0' && envstr[i] == '=') {
    return envstr + i + 1;
  }

  return NULL;
}

static bool __is_valid_environment_variable(const char* name) {
  // According to its sources, the kernel uses 32*PAGE_SIZE by default
  // as the maximum size for an env. variable definition.
  const int MAX_ENV_LEN = 32*4096;

  if (name == NULL) {
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
      "MALLOC_TRACE",
      "NIS_PATH",
      "NLSPATH",
      "RESOLV_HOST_CONF",
      "RES_OPTIONS",
      "TMPDIR",
      "TZDIR",
      NULL
  };
  for (size_t i = 0; UNSAFE_VARIABLE_NAMES[i] != NULL; ++i) {
    if (env_match(name, UNSAFE_VARIABLE_NAMES[i]) != NULL) {
      return true;
    }
  }
  return false;
}

static void __sanitize_environment_variables() {
  char** src  = _envp;
  char** dst = _envp;
  for (; src[0] != NULL; ++src) {
    if (!__is_valid_environment_variable(src[0])) {
      continue;
    }
    // Remove various unsafe environment variables if we're loading a setuid program.
    if (get_AT_SECURE() && __is_unsafe_environment_variable(src[0])) {
        continue;
    }
    dst[0] = src[0];
    ++dst;
  }
  dst[0] = NULL;
}

void linker_env_init(KernelArgumentBlock& args) {
  // Store environment pointer - can't be NULL.
  _envp = args.envp;

  __init_AT_SECURE(args);
  __sanitize_environment_variables();
}

const char* linker_env_get(const char* name) {
  if (name == NULL || name[0] == '\0') {
    return NULL;
  }

  for (char** p = _envp; p[0] != NULL; ++p) {
    const char* val = env_match(p[0], name);
    if (val != NULL) {
      if (val[0] == '\0') {
        return NULL; // Return NULL for empty strings.
      }
      return val;
    }
  }
  return NULL;
}
