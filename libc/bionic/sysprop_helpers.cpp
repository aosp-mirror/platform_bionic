/*
 * Copyright (C) 2021 The Android Open Source Project
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

#include "sysprop_helpers.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sys/system_properties.h"

static bool get_property_value(const char* property_name, char* dest, size_t dest_size) {
  assert(property_name && dest && dest_size != 0);
  const prop_info* prop = __system_property_find(property_name);
  if (!prop) return false;

  struct PropCbCookie {
    char* dest;
    size_t size;
  };
  *dest = '\0';
  PropCbCookie cb_cookie = {dest, dest_size};

  __system_property_read_callback(
      prop,
      [](void* cookie, const char* /* name */, const char* value, uint32_t /* serial */) {
        auto* cb_cookie = reinterpret_cast<PropCbCookie*>(cookie);
        strncpy(cb_cookie->dest, value, cb_cookie->size);
      },
      &cb_cookie);
  if (*dest != '\0') return true;

  return false;
}

bool get_config_from_env_or_sysprops(const char* env_var_name, const char* const* sys_prop_names,
                                     size_t sys_prop_names_size, char* options,
                                     size_t options_size) {
  const char* env = getenv(env_var_name);
  if (env && *env != '\0') {
    strncpy(options, env, options_size);
    options[options_size - 1] = '\0';  // Ensure null-termination.
    return true;
  }

  for (size_t i = 0; i < sys_prop_names_size; ++i) {
    if (sys_prop_names[i] == nullptr) continue;
    if (get_property_value(sys_prop_names[i], options, options_size)) return true;
  }
  return false;
}
