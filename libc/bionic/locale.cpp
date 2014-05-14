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

#include <locale.h>
#include <pthread.h>
#include <stdlib.h>

// We currently support a single locale, the "C" locale (also known as "POSIX").

struct __locale_t {
  // Because we only support one locale, these are just tokens with no data.
};

static pthread_once_t g_locale_once = PTHREAD_ONCE_INIT;
static lconv g_locale;

// We don't use pthread_once for this so that we know when the resource (a TLS slot) will be taken.
static pthread_key_t g_uselocale_key;
__attribute__((constructor)) static void __bionic_tls_uselocale_key_init() {
  pthread_key_create(&g_uselocale_key, NULL);
}

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

static bool __bionic_current_locale_is_utf8 = false;

static bool __is_supported_locale(const char* locale) {
  return (strcmp(locale, "") == 0 ||
          strcmp(locale, "C") == 0 || strcmp(locale, "C.UTF-8") == 0 ||
          strcmp(locale, "POSIX") == 0);
}

static locale_t __new_locale() {
  return reinterpret_cast<locale_t>(malloc(sizeof(__locale_t)));
}

lconv* localeconv() {
  pthread_once(&g_locale_once, __locale_init);
  return &g_locale;
}

locale_t duplocale(locale_t l) {
  locale_t clone = __new_locale();
  if (clone != NULL && l != LC_GLOBAL_LOCALE) {
    *clone = *l;
  }
  return clone;
}

void freelocale(locale_t l) {
  free(l);
}

locale_t newlocale(int category_mask, const char* locale_name, locale_t /*base*/) {
  // Is 'category_mask' valid?
  if ((category_mask & ~LC_ALL_MASK) != 0) {
    errno = EINVAL;
    return NULL;
  }

  if (!__is_supported_locale(locale_name)) {
    errno = ENOENT;
    return NULL;
  }

  return __new_locale();
}

char* setlocale(int category, const char* locale_name) {
  // Is 'category' valid?
  if (category < LC_CTYPE || category > LC_IDENTIFICATION) {
    errno = EINVAL;
    return NULL;
  }

  // Caller wants to set the locale rather than just query?
  if (locale_name != NULL) {
    if (!__is_supported_locale(locale_name)) {
      // We don't support this locale.
      errno = ENOENT;
      return NULL;
    }
    __bionic_current_locale_is_utf8 = (strstr(locale_name, "UTF-8") != NULL);
  }

  return const_cast<char*>(__bionic_current_locale_is_utf8 ? "C.UTF-8" : "C");
}

locale_t uselocale(locale_t new_locale) {
  locale_t old_locale = static_cast<locale_t>(pthread_getspecific(g_uselocale_key));

  // If this is the first call to uselocale(3) on this thread, we return LC_GLOBAL_LOCALE.
  if (old_locale == NULL) {
    old_locale = LC_GLOBAL_LOCALE;
  }

  if (new_locale != NULL) {
    pthread_setspecific(g_uselocale_key, new_locale);
  }

  return old_locale;
}
