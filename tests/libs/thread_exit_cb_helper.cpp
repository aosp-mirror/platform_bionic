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

// Prevent tests from being compiled with glibc because thread_properties.h
// only exists in Bionic.
#if defined(__BIONIC__)

#include <stdio.h>
#include <sys/thread_properties.h>

// Helper binary for testing thread_exit_cb registration.

void exit_cb_1() {
  printf("exit_cb_1 called ");
}

void exit_cb_2() {
  printf("exit_cb_2 called ");
}

void exit_cb_3() {
  printf("exit_cb_3 called");
}

void test_register_thread_exit_cb() {
  // Register the exit-cb in reverse order (3,2,1)
  // so that they'd be called in 1,2,3 order.
  __libc_register_thread_exit_callback(&exit_cb_3);
  __libc_register_thread_exit_callback(&exit_cb_2);
  __libc_register_thread_exit_callback(&exit_cb_1);
}

int main() {
  test_register_thread_exit_cb();
  return 0;
}
#else
int main() {
  return 0;
}
#endif  // __BIONIC__
