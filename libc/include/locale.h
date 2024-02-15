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

#ifndef _LOCALE_H_
#define _LOCALE_H_

#include <sys/cdefs.h>
#include <xlocale.h>

#define __need_NULL
#include <stddef.h>

__BEGIN_DECLS

#define LC_CTYPE           0
#define LC_NUMERIC         1
#define LC_TIME            2
#define LC_COLLATE         3
#define LC_MONETARY        4
#define LC_MESSAGES        5
#define LC_ALL             6
#define LC_PAPER           7
#define LC_NAME            8
#define LC_ADDRESS         9
#define LC_TELEPHONE      10
#define LC_MEASUREMENT    11
#define LC_IDENTIFICATION 12

#define LC_CTYPE_MASK          (1 << LC_CTYPE)
#define LC_NUMERIC_MASK        (1 << LC_NUMERIC)
#define LC_TIME_MASK           (1 << LC_TIME)
#define LC_COLLATE_MASK        (1 << LC_COLLATE)
#define LC_MONETARY_MASK       (1 << LC_MONETARY)
#define LC_MESSAGES_MASK       (1 << LC_MESSAGES)
#define LC_PAPER_MASK          (1 << LC_PAPER)
#define LC_NAME_MASK           (1 << LC_NAME)
#define LC_ADDRESS_MASK        (1 << LC_ADDRESS)
#define LC_TELEPHONE_MASK      (1 << LC_TELEPHONE)
#define LC_MEASUREMENT_MASK    (1 << LC_MEASUREMENT)
#define LC_IDENTIFICATION_MASK (1 << LC_IDENTIFICATION)

#define LC_ALL_MASK (LC_CTYPE_MASK | LC_NUMERIC_MASK | LC_TIME_MASK | LC_COLLATE_MASK | \
                     LC_MONETARY_MASK | LC_MESSAGES_MASK | LC_PAPER_MASK | LC_NAME_MASK | \
                     LC_ADDRESS_MASK | LC_TELEPHONE_MASK | LC_MEASUREMENT_MASK | \
                     LC_IDENTIFICATION_MASK)

struct lconv {
  char* _Nonnull decimal_point;
  char* _Nonnull thousands_sep;
  char* _Nonnull grouping;
  char* _Nonnull int_curr_symbol;
  char* _Nonnull currency_symbol;
  char* _Nonnull mon_decimal_point;
  char* _Nonnull mon_thousands_sep;
  char* _Nonnull mon_grouping;
  char* _Nonnull positive_sign;
  char* _Nonnull negative_sign;
  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;
  char int_p_cs_precedes;
  char int_p_sep_by_space;
  char int_n_cs_precedes;
  char int_n_sep_by_space;
  char int_p_sign_posn;
  char int_n_sign_posn;
};

struct lconv* _Nonnull localeconv(void);

locale_t _Nullable duplocale(locale_t _Nonnull __l);
void freelocale(locale_t _Nonnull __l);
locale_t _Nullable newlocale(int __category_mask, const char* _Nonnull __locale_name, locale_t _Nullable __base);
char* _Nullable setlocale(int __category, const char* _Nullable __locale_name);
locale_t _Nullable uselocale(locale_t _Nullable __l);

#define LC_GLOBAL_LOCALE __BIONIC_CAST(reinterpret_cast, locale_t, -1L)

__END_DECLS

#endif
