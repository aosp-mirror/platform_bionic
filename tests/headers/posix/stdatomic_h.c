/*
 * Copyright (C) 2024 The Android Open Source Project
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

#include <stdatomic.h>

#include "header_checks.h"

static void stdatomic_h() {
  TYPE(atomic_flag);
  TYPE(atomic_bool);
  TYPE(atomic_char);
  TYPE(atomic_schar);
  TYPE(atomic_uchar);
  TYPE(atomic_short);
  TYPE(atomic_ushort);
  TYPE(atomic_int);
  TYPE(atomic_uint);
  TYPE(atomic_long);
  TYPE(atomic_ulong);
  TYPE(atomic_llong);
  TYPE(atomic_ullong);
#if !defined(__GLIBC__)
  TYPE(atomic_char16_t);
  TYPE(atomic_char32_t);
#endif
  TYPE(atomic_wchar_t);
  TYPE(atomic_int_least8_t);
  TYPE(atomic_uint_least8_t);
  TYPE(atomic_int_least16_t);
  TYPE(atomic_uint_least16_t);
  TYPE(atomic_int_least32_t);
  TYPE(atomic_uint_least32_t);
  TYPE(atomic_int_least64_t);
  TYPE(atomic_uint_least64_t);
  TYPE(atomic_int_fast8_t);
  TYPE(atomic_uint_fast8_t);
  TYPE(atomic_int_fast16_t);
  TYPE(atomic_uint_fast16_t);
  TYPE(atomic_int_fast32_t);
  TYPE(atomic_uint_fast32_t);
  TYPE(atomic_int_fast64_t);
  TYPE(atomic_uint_fast64_t);
  TYPE(atomic_intptr_t);
  TYPE(atomic_uintptr_t);
  TYPE(atomic_size_t);
  TYPE(atomic_ptrdiff_t);
  TYPE(atomic_intmax_t);
  TYPE(atomic_uintmax_t);

  memory_order m1 = memory_order_relaxed;
  memory_order m2 = memory_order_consume;
  memory_order m3 = memory_order_acquire;
  memory_order m4 = memory_order_release;
  memory_order m5 = memory_order_acq_rel;
  memory_order m6 = memory_order_seq_cst;

  MACRO(ATOMIC_BOOL_LOCK_FREE);
  MACRO(ATOMIC_CHAR_LOCK_FREE);
  MACRO(ATOMIC_CHAR16_T_LOCK_FREE);
  MACRO(ATOMIC_CHAR32_T_LOCK_FREE);
  MACRO(ATOMIC_WCHAR_T_LOCK_FREE);
  MACRO(ATOMIC_SHORT_LOCK_FREE);
  MACRO(ATOMIC_INT_LOCK_FREE);
  MACRO(ATOMIC_LONG_LOCK_FREE);
  MACRO(ATOMIC_LLONG_LOCK_FREE);
  MACRO(ATOMIC_POINTER_LOCK_FREE);

  atomic_flag f = ATOMIC_FLAG_INIT;
  atomic_int i = ATOMIC_VAR_INIT(123);

  // TODO: remove this #if after the next toolchain update (http://b/374104004).
#if !defined(__GLIBC__)
  i = kill_dependency(i);
#endif

#if !defined(atomic_compare_exchange_strong)
#error atomic_compare_exchange_strong
#endif
#if !defined(atomic_compare_exchange_strong_explicit)
#error atomic_compare_exchange_strong_explicit
#endif
#if !defined(atomic_compare_exchange_weak)
#error atomic_compare_exchange_weak
#endif
#if !defined(atomic_compare_exchange_weak_explicit)
#error atomic_compare_exchange_weak_explicit
#endif
#if !defined(atomic_exchange)
#error atomic_exchange
#endif
#if !defined(atomic_exchange_explicit)
#error atomic_exchange_explicit
#endif
#if !defined(atomic_fetch_add)
#error atomic_fetch_add
#endif
#if !defined(atomic_fetch_add_explicit)
#error atomic_fetch_add_explicit
#endif
#if !defined(atomic_fetch_and)
#error atomic_fetch_and
#endif
#if !defined(atomic_fetch_and_explicit)
#error atomic_fetch_and_explicit
#endif
#if !defined(atomic_fetch_or)
#error atomic_fetch_or
#endif
#if !defined(atomic_fetch_or_explicit)
#error atomic_fetch_or_explicit
#endif
#if !defined(atomic_fetch_sub)
#error atomic_fetch_sub
#endif
#if !defined(atomic_fetch_sub_explicit)
#error atomic_fetch_sub_explicit
#endif
#if !defined(atomic_fetch_xor)
#error atomic_fetch_xor
#endif
#if !defined(atomic_fetch_xor_explicit)
#error atomic_fetch_xor_explicit
#endif
#if !defined(atomic_init)
#error atomic_init
#endif
#if !defined(atomic_is_lock_free)
#error atomic_is_lock_free
#endif
#if !defined(atomic_load)
#error atomic_load
#endif
#if !defined(atomic_load_explicit)
#error atomic_load_explicit
#endif
#if !defined(atomic_store)
#error atomic_store
#endif
#if !defined(atomic_store_explicit)
#error atomic_store_explicit
#endif

  FUNCTION(atomic_flag_clear, void (*f)(volatile atomic_flag*));
  FUNCTION(atomic_flag_clear_explicit, void (*f)(volatile atomic_flag*, memory_order));
  FUNCTION(atomic_flag_test_and_set, bool (*f)(volatile atomic_flag*));
  FUNCTION(atomic_flag_test_and_set_explicit, bool (*f)(volatile atomic_flag*, memory_order));

  FUNCTION(atomic_signal_fence, void (*f)(memory_order));
  FUNCTION(atomic_thread_fence, void (*f)(memory_order));
}
