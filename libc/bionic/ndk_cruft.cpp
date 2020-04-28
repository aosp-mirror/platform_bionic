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

// This file perpetuates the mistakes of the past.

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
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

#include "private/bionic_macros.h"

extern "C" {

// LP64 doesn't need to support any legacy cruft.
#if !defined(__LP64__)

// By the time any NDK-built code is running, there are plenty of threads.
int __isthreaded = 1;

// These were accidentally declared in <unistd.h> because we stupidly used to inline
// getpagesize() and __getpageshift(). Needed for backwards compatibility with old NDK apps.
unsigned int __page_size = PAGE_SIZE;
unsigned int __page_shift = 12;

// TODO: remove this backward compatibility hack (for jb-mr1 strace binaries).
pid_t __wait4(pid_t pid, int* status, int options, struct rusage* rusage) {
  return wait4(pid, status, options, rusage);
}

// TODO: does anything still need this?
int __open() {
  abort();
}

// TODO: does anything still need this?
void** __get_tls() {
#include "private/__get_tls.h"
  return __get_tls();
}

// This non-standard function was in our <string.h> for some reason.
void memswap(void* m1, void* m2, size_t n) {
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

int pthread_attr_setstackaddr(pthread_attr_t*, void*) {
  // This was removed from POSIX.1-2008, and is not implemented on bionic.
  // Needed for ABI compatibility with the NDK.
  return ENOSYS;
}

int pthread_attr_getstackaddr(const pthread_attr_t* attr, void** stack_addr) {
  // This was removed from POSIX.1-2008.
  // Needed for ABI compatibility with the NDK.
  *stack_addr = (char*)attr->stack_base + attr->stack_size;
  return 0;
}

// Non-standard cruft that should only ever have been in system/core/toolbox.
char* strtotimeval(const char* str, struct timeval* ts) {
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
uintmax_t strntoumax(const char *nptr, char **endptr, int base, size_t n) {
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
intmax_t strntoimax(const char* nptr, char** endptr, int base, size_t n) {
  return (intmax_t) strntoumax(nptr, endptr, base, n);
}

// POSIX calls this dprintf, but LP32 Android had fdprintf instead.
int fdprintf(int fd, const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int rc = vdprintf(fd, fmt, ap);
  va_end(ap);
  return rc;
}

// POSIX calls this vdprintf, but LP32 Android had fdprintf instead.
int vfdprintf(int fd, const char* fmt, va_list ap) {
  return vdprintf(fd, fmt, ap);
}

#define __futex_wake __real_futex_wake
#define __futex_wait __real_futex_wait
#include "private/bionic_futex.h"
#undef __futex_wake
#undef __futex_wait

// This used to be in <sys/atomics.h>.
int __futex_wake(volatile void* ftx, int count) {
  return __real_futex_wake(ftx, count);
}

// This used to be in <sys/atomics.h>.
int __futex_wait(volatile void* ftx, int value, const struct timespec* timeout) {
  return __real_futex_wait(ftx, value, timeout);
}

// Unity's libmono uses this.
int tkill(pid_t tid, int sig) {
  return syscall(__NR_tkill, tid, sig);
}

// This was removed from POSIX 2008.
wchar_t* wcswcs(wchar_t* haystack, wchar_t* needle) {
  return wcsstr(haystack, needle);
}

// This was removed from POSIX 2008.
sighandler_t bsd_signal(int signum, sighandler_t handler) {
  return signal(signum, handler);
}

// This was removed from POSIX 2008.
#undef bcopy
void bcopy(const void* src, void* dst, size_t n) {
  memmove(dst, src, n);
}

// This was removed from POSIX 2008.
#undef bzero
void bzero(void* dst, size_t n) {
  memset(dst, 0, n);
}

// sysv_signal() was never in POSIX.
extern "C++" sighandler_t _signal(int signum, sighandler_t handler, int flags);
sighandler_t sysv_signal(int signum, sighandler_t handler) {
  return _signal(signum, handler, SA_RESETHAND);
}

// This is a system call that was never in POSIX. Use readdir(3) instead.
int __getdents64(unsigned int, dirent*, unsigned int);
int getdents(unsigned int fd, dirent* dirp, unsigned int count) {
  return __getdents64(fd, dirp, count);
}

// This is a BSDism that we never implemented correctly. Used by Firefox.
int issetugid() {
  return 0;
}

// This was removed from POSIX 2004.
pid_t wait3(int* status, int options, struct rusage* rusage) {
  return wait4(-1, status, options, rusage);
}

// This was removed from POSIX 2004.
int getdtablesize() {
  struct rlimit r;

  if (getrlimit(RLIMIT_NOFILE, &r) < 0) {
    return sysconf(_SC_OPEN_MAX);
  }

  return r.rlim_cur;
}

// A leaked BSD stdio implementation detail that's now a no-op.
void __sinit() {}
int __sdidinit = 1;

// Only used by ftime, which was removed from POSIX 2008.
struct timeb {
  time_t          time;
  unsigned short  millitm;
  short           timezone;
  short           dstflag;
};

// This was removed from POSIX 2008.
int ftime(struct timeb* tb) {
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
char* index(const char* str, int ch) {
  return const_cast<char*>(strchr(str, ch));
}

// This was removed from BSD.
void arc4random_stir(void) {
  // The current implementation stirs itself as needed.
}

// This was removed from BSD.
void arc4random_addrandom(unsigned char*, int) {
  // The current implementation adds randomness as needed.
}

// Old versions of the NDK did not export malloc_usable_size, but did
// export dlmalloc_usable_size. We are moving away from dlmalloc in L
// so make this call malloc_usable_size.
size_t dlmalloc_usable_size(void* ptr) {
  return malloc_usable_size(ptr);
}

// In L we added a public pthread_gettid_np, but some apps were using the private API.
pid_t __pthread_gettid(pthread_t t) {
  return pthread_gettid_np(t);
}

// Older versions of apportable used dlmalloc directly instead of malloc,
// so export this compatibility shim that simply calls malloc.
void* dlmalloc(size_t size) {
  return malloc(size);
}

} // extern "C"

#define __get_thread __real_get_thread
#include "pthread_internal.h"
#undef __get_thread

extern "C" {

// Various third-party apps contain a backport of our pthread_rwlock implementation that uses this.
pthread_internal_t* __get_thread() {
  return __real_get_thread();
}

// This one exists only for the LP32 NDK and is not present anywhere else.
extern long __set_errno_internal(int);
long __set_errno(int n) {
  return __set_errno_internal(n);
}

// Since dlmalloc_inspect_all and dlmalloc_trim are exported for systems
// that use dlmalloc, be consistent and export them everywhere.
void dlmalloc_inspect_all(void (*)(void*, void*, size_t, void*), void*) {
}
int dlmalloc_trim(size_t) {
    return 0;
}

// LP32's <stdio.h> had putw (but not getw).
int putw(int value, FILE* fp) {
    return fwrite(&value, sizeof(value), 1, fp) == 1 ? 0 : EOF;
}

#endif // !defined (__LP64__)

} // extern "C"
