/*
 * Copyright (C) 2013 The Android Open Source Project
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

// This file perpetuates the mistakes of the past, but only for 32-bit targets.
#if !defined(__LP64__)

#include <ctype.h>
#include <dirent.h>
#include <inttypes.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wchar.h>

// These were accidentally declared in <unistd.h> because we stupidly used to inline
// getpagesize() and __getpageshift(). Needed for backwards compatibility with old NDK apps.
extern "C" {
  unsigned int __page_size = PAGE_SIZE;
  unsigned int __page_shift = 12;
}

// TODO: remove this backward compatibility hack (for jb-mr1 strace binaries).
extern "C" pid_t __wait4(pid_t pid, int* status, int options, struct rusage* rusage) {
  return wait4(pid, status, options, rusage);
}

// TODO: does anything still need this?
extern "C" int __open() {
  abort();
}

// TODO: does anything still need this?
extern "C" void** __get_tls() {
#include "private/__get_tls.h"
  return __get_tls();
}

// This non-standard function was in our <string.h> for some reason.
extern "C" void memswap(void* m1, void* m2, size_t n) {
  char* p = reinterpret_cast<char*>(m1);
  char* p_end = p + n;
  char* q = reinterpret_cast<char*>(m2);
  while (p < p_end) {
    char tmp = *p;
    *p = *q;
    *q = tmp;
    p++;
    q++;
  }
}

extern "C" int pthread_attr_setstackaddr(pthread_attr_t*, void*) {
  // This was removed from POSIX.1-2008, and is not implemented on bionic.
  // Needed for ABI compatibility with the NDK.
  return ENOSYS;
}

extern "C" int pthread_attr_getstackaddr(const pthread_attr_t* attr, void** stack_addr) {
  // This was removed from POSIX.1-2008.
  // Needed for ABI compatibility with the NDK.
  *stack_addr = (char*)attr->stack_base + attr->stack_size;
  return 0;
}

// Non-standard cruft that should only ever have been in system/core/toolbox.
extern "C" char* strtotimeval(const char* str, struct timeval* ts) {
  char* s;
  ts->tv_sec = strtoumax(str, &s, 10);

  long fractional_seconds = 0;
  if (*s == '.') {
    s++;
    int count = 0;

    // Read up to 6 digits (microseconds).
    while (*s && isdigit(*s)) {
      if (++count < 7) {
        fractional_seconds = fractional_seconds*10 + (*s - '0');
      }
      s++;
    }

    for (; count < 6; count++) {
      fractional_seconds *= 10;
    }
  }

  ts->tv_usec = fractional_seconds;
  return s;
}

static inline int digitval(int ch) {
  unsigned d;

  d = (unsigned)(ch - '0');
  if (d < 10) return (int)d;

  d = (unsigned)(ch - 'a');
  if (d < 6) return (int)(d+10);

  d = (unsigned)(ch - 'A');
  if (d < 6) return (int)(d+10);

  return -1;
}

// This non-standard function was in our <inttypes.h> for some reason.
extern "C" uintmax_t strntoumax(const char *nptr, char **endptr, int base, size_t n) {
  const unsigned char*  p   = (const unsigned char *)nptr;
  const unsigned char*  end = p + n;
  int                   minus = 0;
  uintmax_t             v = 0;
  int                   d;

  while (p < end && isspace(*p)) {
    p++;
  }

  if (p < end) {
    char c = p[0];
    if (c == '-' || c == '+') {
      minus = (c == '-');
      p++;
    }
  }

  if (base == 0) {
    if (p+2 < end && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      p += 2;
      base = 16;
    } else if (p+1 < end && p[0] == '0') {
      p   += 1;
      base = 8;
    } else {
      base = 10;
    }
  } else if (base == 16) {
    if (p+2 < end && p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      p += 2;
    }
  }

  while (p < end && (d = digitval(*p)) >= 0 && d < base) {
    v = v*base + d;
    p += 1;
  }

  if (endptr) {
    *endptr = (char*) p;
  }

  return minus ? -v : v;
}

// This non-standard function was in our <inttypes.h> for some reason.
extern "C" intmax_t strntoimax(const char* nptr, char** endptr, int base, size_t n) {
  return (intmax_t) strntoumax(nptr, endptr, base, n);
}

// POSIX calls this dprintf, but LP32 Android had fdprintf instead.
extern "C" int fdprintf(int fd, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int rc = vdprintf(fd, fmt, ap);
  va_end(ap);
  return rc;
}

// POSIX calls this vdprintf, but LP32 Android had fdprintf instead.
extern "C" int vfdprintf(int fd, const char* fmt, va_list ap) {
  return vdprintf(fd, fmt, ap);
}

#define __futex_wake __real_futex_wake
#define __futex_wait __real_futex_wait
#include "private/bionic_futex.h"
#undef __futex_wake
#undef __futex_wait

// This used to be in <sys/atomics.h>.
extern "C" int __futex_wake(volatile void* ftx, int count) {
  return __real_futex_wake(ftx, count);
}

// This used to be in <sys/atomics.h>.
extern "C" int __futex_wait(volatile void* ftx, int value, const struct timespec* timeout) {
  return __real_futex_wait(ftx, value, timeout);
}

// Unity's libmono uses this.
extern "C" int tkill(pid_t tid, int sig) {
  return syscall(__NR_tkill, tid, sig);
}

// This was removed from POSIX 2008.
extern "C" wchar_t* wcswcs(wchar_t* haystack, wchar_t* needle) {
  return wcsstr(haystack, needle);
}

// This was removed from POSIX 2008.
extern "C" sighandler_t bsd_signal(int signum, sighandler_t handler) {
  return signal(signum, handler);
}

// sysv_signal() was never in POSIX.
extern sighandler_t _signal(int signum, sighandler_t handler, int flags);
extern "C" sighandler_t sysv_signal(int signum, sighandler_t handler) {
  return _signal(signum, handler, SA_RESETHAND);
}

// This is a system call that was never in POSIX. Use readdir(3) instead.
extern "C" int __getdents64(unsigned int, dirent*, unsigned int);
extern "C" int getdents(unsigned int fd, dirent* dirp, unsigned int count) {
  return __getdents64(fd, dirp, count);
}

// This is a BSDism that we never implemented correctly. Used by Firefox.
extern "C" int issetugid() {
  return 0;
}

// This was removed from POSIX 2004.
extern "C" pid_t wait3(int* status, int options, struct rusage* rusage) {
  return wait4(-1, status, options, rusage);
}

// This was removed from POSIX 2004.
extern "C" int getdtablesize() {
  struct rlimit r;

  if (getrlimit(RLIMIT_NOFILE, &r) < 0) {
    return sysconf(_SC_OPEN_MAX);
  }

  return r.rlim_cur;
}

// Only used by ftime, which was removed from POSIX 2008.
struct timeb {
  time_t          time;
  unsigned short  millitm;
  short           timezone;
  short           dstflag;
};

// This was removed from POSIX 2008.
extern "C" int ftime(struct timeb* tb) {
  struct timeval  tv;
  struct timezone tz;

  if (gettimeofday(&tv, &tz) < 0)
    return -1;

  tb->time    = tv.tv_sec;
  tb->millitm = (tv.tv_usec + 500) / 1000;

  if (tb->millitm == 1000) {
    ++tb->time;
    tb->millitm = 0;
  }

  tb->timezone = tz.tz_minuteswest;
  tb->dstflag  = tz.tz_dsttime;

  return 0;
}

// This was removed from POSIX 2008.
extern "C" char* index(const char* str, int ch) {
  return strchr(str, ch);
}

// This was removed from BSD.
extern "C" void arc4random_stir(void) {
  // The current implementation stirs itself as needed.
}

// This was removed from BSD.
extern "C" void arc4random_addrandom(unsigned char*, int) {
  // The current implementation adds randomness as needed.
}

// Old versions of the NDK did not export malloc_usable_size, but did
// export dlmalloc_usable_size. We are moving away from dlmalloc in L
// so make this call malloc_usable_size.
extern "C" size_t dlmalloc_usable_size(void* ptr) {
  return malloc_usable_size(ptr);
}

// Older versions of appportable used dlmalloc directly instead of malloc,
// so export this compatibility shim that simply calls malloc.
extern "C" void* dlmalloc(size_t size) {
  return malloc(size);
}

#endif
