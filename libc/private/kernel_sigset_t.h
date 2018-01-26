/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBC_PRIVATE_KERNEL_SIGSET_T_H_
#define LIBC_PRIVATE_KERNEL_SIGSET_T_H_

#include <errno.h>
#include <signal.h>

#include <async_safe/log.h>

// Our sigset_t is wrong for ARM and x86. It's 32-bit but the kernel expects 64 bits.
// This means we can't support real-time signals correctly without breaking the ABI.
// In the meantime, we can use this union to pass an appropriately-sized block of memory
// to the kernel, at the cost of not being able to refer to real-time signals when
// initializing from a sigset_t on LP32.
union kernel_sigset_t {
 public:
  kernel_sigset_t() {
  }

  explicit kernel_sigset_t(int signal_number) {
    clear();
    if (!set(signal_number)) async_safe_fatal("kernel_sigset_t(%d)", signal_number);
  }

  explicit kernel_sigset_t(const sigset_t* value) {
    clear();
    set(value);
  }

  void clear() {
    __builtin_memset(this, 0, sizeof(*this));
  }

  bool clear(int signal_number) {
    int bit = bit_of(signal_number);
    if (bit == -1) return false;
    bits[bit / LONG_BIT] &= ~(1UL << (bit % LONG_BIT));
    return true;
  }

  void fill() {
    __builtin_memset(this, 0xff, sizeof(*this));
  }

  bool is_set(int signal_number) {
    int bit = bit_of(signal_number);
    if (bit == -1) return false;
    return ((bits[bit / LONG_BIT] >> (bit % LONG_BIT)) & 1) == 1;
  }

  bool set(int signal_number) {
    int bit = bit_of(signal_number);
    if (bit == -1) return false;
    bits[bit / LONG_BIT] |= 1UL << (bit % LONG_BIT);
    return true;
  }

  void set(const sigset_t* value) {
    clear();
    bionic = *value;
  }

  sigset_t* get() {
    return &bionic;
  }

  sigset_t bionic;
  unsigned long bits[_KERNEL__NSIG/LONG_BIT];

 private:
  int bit_of(int signal_number) {
    int bit = signal_number - 1; // Signal numbers start at 1, but bit positions start at 0.
    if (bit < 0 || bit >= static_cast<int>(8*sizeof(*this))) {
      errno = EINVAL;
      return -1;
    }
    return bit;
  }
};

extern "C" int __rt_sigpending(const kernel_sigset_t*, size_t);
extern "C" int __rt_sigprocmask(int, const kernel_sigset_t*, kernel_sigset_t*, size_t);
extern "C" int __rt_sigsuspend(const kernel_sigset_t*, size_t);

#endif
