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

#include <errno.h>
#include <locale.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <wchar.h>

#include "private/bionic_macros.h"

#if defined(__BIONIC_BUILD_FOR_ANDROID_SUPPORT)
#define USE_TLS_SLOT 0
#else
#define USE_TLS_SLOT 1
#endif

#if USE_TLS_SLOT
#include "bionic/pthread_internal.h"
#endif

// We only support two locales, the "C" locale (also known as "POSIX"),
// and the "C.UTF-8" locale (also known as "en_US.UTF-8").

static bool __bionic_current_locale_is_utf8 = true;

struct __locale_t {
  size_t mb_cur_max;

  explicit __locale_t(size_t mb_cur_max) : mb_cur_max(mb_cur_max) {
  }

  explicit __locale_t(const __locale_t* other) {
    if (other == LC_GLOBAL_LOCALE) {
      mb_cur_max = __bionic_current_locale_is_utf8 ? 4 : 1;
    } else {
      mb_cur_max = other->mb_cur_max;
    }
  }

  BIONIC_DISALLOW_IMPLICIT_CONSTRUCTORS(__locale_t);
};

size_t __ctype_get_mb_cur_max() {
  locale_t l = uselocale(nullptr);
  if (l == LC_GLOBAL_LOCALE) {
    return __bionic_current_locale_is_utf8 ? 4 : 1;
  } else {
    return l->mb_cur_max;
  }
}

#if !USE_TLS_SLOT
static thread_local locale_t g_current_locale;
#endif

static pthread_once_t g_locale_once = PTHREAD_ONCE_INIT;
static lconv g_locale;

static void __locale_init() {
  g_locale.decimal_point = const_cast<char*>(".");

  char* not_available = const_cast<char*>("");
  g_locale.thousands_sep = not_available;
  g_locale.grouping = not_available;
  g_locale.int_curr_symbol = not_available;
  g_locale.currency_symbol = not_available;
  g_locale.mon_decimal_point = not_available;
  g_locale.mon_thousands_sep = not_available;
  g_locale.mon_grouping = not_available;
  g_locale.positive_sign = not_available;
  g_locale.negative_sign = not_available;

  g_locale.int_frac_digits = CHAR_MAX;
  g_locale.frac_digits = CHAR_MAX;
  g_locale.p_cs_precedes = CHAR_MAX;
  g_locale.p_sep_by_space = CHAR_MAX;
  g_locale.n_cs_precedes = CHAR_MAX;
  g_locale.n_sep_by_space = CHAR_MAX;
  g_locale.p_sign_posn = CHAR_MAX;
  g_locale.n_sign_posn = CHAR_MAX;
  g_locale.int_p_cs_precedes = CHAR_MAX;
  g_locale.int_p_sep_by_space = CHAR_MAX;
  g_locale.int_n_cs_precedes = CHAR_MAX;
  g_locale.int_n_sep_by_space = CHAR_MAX;
  g_locale.int_p_sign_posn = CHAR_MAX;
  g_locale.int_n_sign_posn = CHAR_MAX;
}

static bool __is_supported_locale(const char* locale_name) {
  return (strcmp(locale_name, "") == 0 ||
          strcmp(locale_name, "C") == 0 ||
          strcmp(locale_name, "C.UTF-8") == 0 ||
          strcmp(locale_name, "en_US.UTF-8") == 0 ||
          strcmp(locale_name, "POSIX") == 0);
}

static bool __is_utf8_locale(const char* locale_name) {
  return (*locale_name == '\0' || strstr(locale_name, "UTF-8"));
}

lconv* localeconv() {
  pthread_once(&g_locale_once, __locale_init);
  return &g_locale;
}

locale_t duplocale(locale_t l) {
  return new __locale_t(l);
}

void freelocale(locale_t l) {
  delete l;
}

locale_t newlocale(int category_mask, const char* locale_name, locale_t /*base*/) {
  // Are 'category_mask' and 'locale_name' valid?
  if ((category_mask & ~LC_ALL_MASK) != 0 || locale_name == nullptr) {
    errno = EINVAL;
    return nullptr;
  }

  if (!__is_supported_locale(locale_name)) {
    errno = ENOENT;
    return nullptr;
  }

  return new __locale_t(__is_utf8_locale(locale_name) ? 4 : 1);
}

char* setlocale(int category, const char* locale_name) {
  // Is 'category' valid?
  if (category < LC_CTYPE || category > LC_IDENTIFICATION) {
    errno = EINVAL;
    return nullptr;
  }

  // Caller wants to set the locale rather than just query?
  if (locale_name != nullptr) {
    if (!__is_supported_locale(locale_name)) {
      // We don't support this locale.
      errno = ENOENT;
      return nullptr;
    }
    __bionic_current_locale_is_utf8 = __is_utf8_locale(locale_name);
  }

  return const_cast<char*>(__bionic_current_locale_is_utf8 ? "C.UTF-8" : "C");
}

static locale_t* get_current_locale_ptr() {
#if USE_TLS_SLOT
  return &__get_bionic_tls().locale;
#else
  return &g_current_locale;
#endif
}

locale_t uselocale(locale_t new_locale) {
  locale_t old_locale = *get_current_locale_ptr();

  // If this is the first call to uselocale(3) on this thread, we return LC_GLOBAL_LOCALE.
  if (old_locale == nullptr) {
    old_locale = LC_GLOBAL_LOCALE;
  }

  if (new_locale != nullptr) {
    *get_current_locale_ptr() = new_locale;
  }

  return old_locale;
}
