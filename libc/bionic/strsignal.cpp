/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <signal.h>
#include <string.h>

#include "bionic/pthread_internal.h"

const char* const sys_siglist[NSIG] = {
#define __BIONIC_SIGDEF(signal_number, signal_description) [signal_number] = signal_description,
#include "private/bionic_sigdefs.h"
};

const char* const sys_signame[NSIG] = {
#define __BIONIC_SIGDEF(signal_number, unused) [signal_number] = &(#signal_number)[3],
#include "private/bionic_sigdefs.h"
};

extern "C" __LIBC_HIDDEN__ const char* __strsignal(int signal_number, char* buf, size_t buf_len) {
  const char* signal_name = nullptr;
  if (signal_number >= 0 && signal_number < NSIG) {
    signal_name = sys_siglist[signal_number];
  }
  if (signal_name != nullptr) {
    return signal_name;
  }

  const char* prefix = "Unknown";
  if (signal_number >= SIGRTMIN && signal_number <= SIGRTMAX) {
    prefix = "Real-time";
    signal_number -= SIGRTMIN;
  }
  size_t length = snprintf(buf, buf_len, "%s signal %d", prefix, signal_number);
  if (length >= buf_len) {
    return nullptr;
  }
  return buf;
}

char* strsignal(int signal_number) {
  bionic_tls& tls = __get_bionic_tls();
  return const_cast<char*>(__strsignal(signal_number, tls.strsignal_buf, sizeof(tls.strsignal_buf)));
}
