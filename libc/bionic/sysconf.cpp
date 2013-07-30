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

#include <asm/page.h>
#include <bionic_tls.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>  // For FOPEN_MAX.
#include <string.h>
#include <sys/sysconf.h>
#include <time.h>
#include <unistd.h>

#include "private/ScopedReaddir.h"

/* seems to be the default on Linux, per the GLibc sources and my own digging */

#define  SYSTEM_CLK_TCK         100
#define  SYSTEM_IOV_MAX         1024
#define  SYSTEM_DELAYTIMER_MAX  2147483647
#define  SYSTEM_MQ_OPEN_MAX     8
#define  SYSTEM_MQ_PRIO_MAX     32768
#define  SYSTEM_SEM_NSEMS_MAX   256
#define  SYSTEM_SEM_VALUE_MAX   0x3fffffff  /* see bionic/semaphore.c */
#define  SYSTEM_SIGQUEUE_MAX    32
#define  SYSTEM_TIMER_MAX       32
#define  SYSTEM_LOGIN_NAME_MAX  256
#define  SYSTEM_TTY_NAME_MAX    32

/* the following depends on our implementation */
#define  SYSTEM_ATEXIT_MAX          65536    /* our implementation is unlimited */
#define  SYSTEM_THREAD_THREADS_MAX  2048     /* really unlimited */

#define  SYSTEM_2_C_BIND     _POSIX_VERSION  /* Posix C binding version */
#define  SYSTEM_2_C_VER      _POSIX2_C_VERSION
#define  SYSTEM_2_C_DEV      -1       /* Posix C development tools unsupported on the device */
#define  SYSTEM_2_FORT_DEV   -1       /* Fortran development unsupported */
#define  SYSTEM_2_FORT_RUN   -1       /* Fortran runtime unsupported */
#define  SYSTEM_2_SW_DEV     -1       /* posix software dev utilities unsupported */
#define  SYSTEM_2_LOCALEDEF  -1       /* localedef() unimplemented */
#define  SYSTEM_2_UPE        -1       /* No UPE for you ! (User Portability Utilities) */
#define  SYSTEM_2_VERSION    -1       /* No posix command-line tools */

static bool __matches_cpuN(const char* s) {
  // The %c trick is to ensure that we have the anchored match "^cpu[0-9]+$".
  unsigned cpu;
  char dummy;
  return (sscanf(s, "cpu%u%c", &cpu, &dummy) == 1);
}

static int __sysconf_nprocessors_conf() {
  // On x86 kernels you can use /proc/cpuinfo for this, but on ARM kernels offline CPUs disappear
  // from there. This method works on both.
  ScopedReaddir reader("/sys/devices/system/cpu");
  if (reader.IsBad()) {
    return 1;
  }

  int result = 0;
  dirent* entry;
  while ((entry = reader.ReadEntry()) != NULL) {
    if (entry->d_type == DT_DIR && __matches_cpuN(entry->d_name)) {
      ++result;
    }
  }
  return result;
}

static int __sysconf_nprocessors_onln() {
  FILE* fp = fopen("/proc/stat", "r");
  if (fp == NULL) {
    return 1;
  }

  int result = 0;
  char buf[256];
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    // Extract just the first word from the line.
    // 'cpu0 7976751 1364388 3116842 469770388 8629405 0 49047 0 0 0'
    char* p = strchr(buf, ' ');
    if (p != NULL) {
      *p = 0;
    }
    if (__matches_cpuN(buf)) {
      ++result;
    }
  }
  fclose(fp);
  return result;
}

static int __get_meminfo(const char* pattern) {
  FILE* fp = fopen("/proc/meminfo", "r");
  if (fp == NULL) {
    return -1;
  }

  int result = -1;
  char buf[256];
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    long total;
    if (sscanf(buf, pattern, &total) == 1) {
      result = (int) (total / (PAGE_SIZE/1024));
      break;
    }
  }
  fclose(fp);
  return result;
}

static int __sysconf_phys_pages() {
  return __get_meminfo("MemTotal: %ld kB");
}

static int __sysconf_avphys_pages() {
  return __get_meminfo("MemFree: %ld kB");
}

static int __sysconf_monotonic_clock() {
  timespec t;
  int rc = clock_getres(CLOCK_MONOTONIC, &t);
  return (rc == -1) ? -1 : _POSIX_VERSION;
}

int sysconf(int name) {
    switch (name) {
#ifdef _POSIX_ARG_MAX
    case _SC_ARG_MAX:           return _POSIX_ARG_MAX;
#endif
#ifdef _POSIX2_BC_BASE_MAX
    case _SC_BC_BASE_MAX:       return _POSIX2_BC_BASE_MAX;
#endif
#ifdef _POSIX2_BC_DIM_MAX
    case _SC_BC_DIM_MAX:        return _POSIX2_BC_DIM_MAX;
#endif
#ifdef _POSIX2_BC_SCALE_MAX
    case _SC_BC_SCALE_MAX:      return _POSIX2_BC_SCALE_MAX;
#endif
#ifdef _POSIX2_BC_STRING_MAX
    case _SC_BC_STRING_MAX:     return _POSIX2_BC_STRING_MAX;
#endif
    case _SC_CHILD_MAX:         return CHILD_MAX;
    case _SC_CLK_TCK:           return SYSTEM_CLK_TCK;
#ifdef _POSIX2_COLL_WEIGHTS_MASK
    case _SC_COLL_WEIGHTS_MAX:  return _POSIX2_COLL_WEIGHTS_MASK;
#endif
#ifdef _POSIX2_EXPR_NEST_MAX
    case _SC_EXPR_NEST_MAX:    return _POSIX2_EXPR_NEST_MAX;
#endif
#ifdef _POSIX2_LINE_MAX
    case _SC_LINE_MAX:          return _POSIX2_LINE_MAX;
#endif
    case _SC_NGROUPS_MAX:       return NGROUPS_MAX;
    case _SC_OPEN_MAX:          return OPEN_MAX;
    //case _SC_PASS_MAX:          return PASS_MAX;
    case _SC_2_C_BIND:          return SYSTEM_2_C_BIND;
    case _SC_2_C_DEV:           return SYSTEM_2_C_DEV;
    case _SC_2_C_VERSION:       return SYSTEM_2_C_VER;
    //case _SC_2_CHAR_TERM:         return ;
    case _SC_2_FORT_DEV:        return SYSTEM_2_FORT_DEV;
    case _SC_2_FORT_RUN:        return SYSTEM_2_FORT_RUN;
    case _SC_2_LOCALEDEF:       return SYSTEM_2_LOCALEDEF;
    case _SC_2_SW_DEV:          return SYSTEM_2_SW_DEV;
    case _SC_2_UPE:             return SYSTEM_2_UPE;
    case _SC_2_VERSION:         return SYSTEM_2_VERSION;
#ifdef _POSIX_JOB_CONTROL
    case _SC_JOB_CONTROL:       return _POSIX_JOB_CONTROL;
#endif
#ifdef _POSIX_SAVED_IDS
    case _SC_SAVED_IDS:         return _POSIX_SAVED_IDS;
#endif
#ifdef _POSIX_VERSION
    case _SC_VERSION:           return _POSIX_VERSION;
#endif
    //case _SC_RE_DUP_<AX:        return ;
    case _SC_STREAM_MAX:        return FOPEN_MAX;
    //case _SC_TZNAME_MAX:        return ;
#if _XOPEN_CRYPT
    case _SC_XOPEN_CRYPT:       return _XOPEN_CRYPT;
#endif
#ifdef _XOPEN_ENH_I18N
    case _SC_XOPEN_ENH_I18N:    return _XOPEN_ENH_I18N;
#endif
#ifdef _XOPEN_SHM
    case _SC_XOPEN_SHM:         return _XOPEN_SHM;
#endif
#ifdef _XOPEN_VERSION
    case _SC_XOPEN_VERSION:     return _XOPEN_VERSION;
#endif
#ifdef _XOPEN_XCU_VERSION
    case _SC_XOPEN_XCU_VERSION: return _XOPEN_XCU_VERSION;
#endif
#ifdef _XOPEN_REALTIME
    case _SC_XOPEN_REALTIME:    return _XOPEN_REALTIME;
#endif
#ifdef _XOPEN_REALTIME_THREADS
    case _SC_XOPEN_REALTIME_THREADS: return _XOPEN_REALTIME_THREADS;
#endif
#ifdef _XOPEN_LEGACY
    case _SC_XOPEN_LEGACY:      return _XOPEN_LEGACY;
#endif
    case _SC_ATEXIT_MAX:        return SYSTEM_ATEXIT_MAX;
    case _SC_IOV_MAX:           return SYSTEM_IOV_MAX;
    case _SC_PAGESIZE:          return PAGE_SIZE;
    case _SC_PAGE_SIZE:         return PAGE_SIZE;
#ifdef _XOPEN_UNIX
    case _SC_XOPEN_UNIX:        return _XOPEN_UNIX;
#endif

    // XXX: TODO: XBS5 nonsense

#ifdef AIO_LISTIO_MAX
    case _SC_AIO_LISTIO_MAX:       return AIO_LISTIO_MAX;
#endif
#ifdef AIO_MAX
    case _SC_AIO_MAX:              return AIO_MAX;
#endif
#ifdef AIO_PRIO_DELTA_MAX
    case _SC_AIO_PRIO_DELTA_MAX:   return AIO_PRIO_DELTA_MAX;
#endif
    case _SC_DELAYTIMER_MAX:    return SYSTEM_DELAYTIMER_MAX;
    case _SC_MQ_OPEN_MAX:       return SYSTEM_MQ_OPEN_MAX;
    case _SC_MQ_PRIO_MAX:       return SYSTEM_MQ_PRIO_MAX;
    case _SC_RTSIG_MAX:         return RTSIG_MAX;
    case _SC_SEM_NSEMS_MAX:     return SYSTEM_SEM_NSEMS_MAX;
    case _SC_SEM_VALUE_MAX:     return SYSTEM_SEM_VALUE_MAX;
    case _SC_SIGQUEUE_MAX:      return SYSTEM_SIGQUEUE_MAX;
    case _SC_TIMER_MAX:         return SYSTEM_TIMER_MAX;
#ifdef _POSIX_ASYNCHRONOUS_IO
    case _SC_ASYNCHRONOUS_IO:   return _POSIX_ASYNCHRONOUS_IO;
#endif
#ifdef _POSIX_FSYNC
    case _SC_FSYNC:             return _POSIX_FSYNC;
#endif
#ifdef _POSIX_MAPPED_FILES
    case _SC_MAPPED_FILES:      return _POSIX_MAPPED_FILES;
#endif
#ifdef _POSIX_MEMLOCK
    case _SC_MEMLOCK:           return _POSIX_MEMLOCK;
#endif
#ifdef _POSIX_MEMLOCK_RANGE
    case _SC_MEMLOCK_RANGE:     return _POSIX_MEMLOCK_RANGE
#endif
#ifdef _POSIX_MEMORY_PROTECTION
    case _SC_MEMORY_PROTECTION: return _POSIX_MEMORY_PROTECTION;
#endif
#ifdef _POSIX_MESSAGE_PASSING
    case _SC_MESSAGE_PASSING:   return _POSIX_MESSAGE_PASSING;
#endif
#ifdef _POSIX_PRIORITIZED_IO
    case _SC_PRIORITIZED_IO:    return _POSIX_PRIORITIZED_IO;
#endif
#ifdef _POSIX_PRIORITY_SCHEDULING
    case _SC_PRIORITY_SCHEDULING:  return _POSIX_PRIORITY_SCHEDULING;
#endif
#ifdef _POSIX_REALTIME_SIGNALS
    case _SC_REALTIME_SIGNALS:  return _POSIX_REALTIME_SIGNALS;
#endif
#ifdef _POSIX_SEMAPHORES
    case _SC_SEMAPHORES:        return _POSIX_SEMAPHORES;
#endif
#ifdef _POSIX_SHARED_MEMORY_OBJECTS
    case _SC_SHARED_MEMORY_OBJECTS:  return _POSIX_SHARED_MEMORY_OBJECTS;
#endif
#ifdef _POSIX_SYNCHRONIZED_IO
    case _SC_SYNCHRONIZED_IO:   return _POSIX_SYNCHRONIZED_IO;
#endif
#ifdef _POSIX_TIMERS
    case _SC_TIMERS:            return _POSIX_TIMERS;
#endif

    case _SC_GETGR_R_SIZE_MAX: return 1024;
    case _SC_GETPW_R_SIZE_MAX: return 1024;

    case _SC_LOGIN_NAME_MAX:    return SYSTEM_LOGIN_NAME_MAX;

    case _SC_THREAD_DESTRUCTOR_ITERATIONS:
      return _POSIX_THREAD_DESTRUCTOR_ITERATIONS;

    case _SC_THREAD_KEYS_MAX:
      return (BIONIC_TLS_SLOTS - TLS_SLOT_FIRST_USER_SLOT - GLOBAL_INIT_THREAD_LOCAL_BUFFER_COUNT);

    case _SC_THREAD_STACK_MIN:    return PTHREAD_STACK_MIN;
    case _SC_THREAD_THREADS_MAX:  return SYSTEM_THREAD_THREADS_MAX;
    case _SC_TTY_NAME_MAX:        return SYSTEM_TTY_NAME_MAX;
#ifdef _POSIX_THREADS
    case _SC_THREADS:             return _POSIX_THREADS;
#endif
#ifdef _POSIX_THREAD_ATTR_STACKADDR
    case _SC_THREAD_ATTR_STACKADDR:   return _POSIX_THREAD_ATTR_STACKADDR;
#endif
#ifdef _POSIX_THREAD_ATTR_STACKSIZE
    case _SC_THREAD_ATTR_STACKSIZE:   return _POSIX_THREAD_ATTR_STACKSIZE;
#endif
#ifdef _POSIX_THREAD_PRIORITY_SCHEDULING
    case _SC_THREAD_PRIORITY_SCHEDULING:  return _POSIX_THREAD_PRIORITY_SCHEDULING;
#endif
#ifdef _POSIX_THREAD_PRIO_INHERIT
    case _SC_THREAD_PRIO_INHERIT:         return _POSIX_THREAD_PRIO_INHERIT;
#endif
#ifdef _POSIX_THREAD_PRIO_PROTECT
    case _SC_THREAD_PRIO_PROTECT: return _POSIX_THREAD_PRIO_PROTECT;
#endif
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
    case _SC_THREAD_SAFE_FUNCTIONS:  return _POSIX_THREAD_SAFE_FUNCTIONS
#endif

    case _SC_MONOTONIC_CLOCK:   return __sysconf_monotonic_clock();
    case _SC_NPROCESSORS_CONF:  return __sysconf_nprocessors_conf();
    case _SC_NPROCESSORS_ONLN:  return __sysconf_nprocessors_onln();
    case _SC_PHYS_PAGES:        return __sysconf_phys_pages();
    case _SC_AVPHYS_PAGES:      return __sysconf_avphys_pages();

    default:
       /* Posix says EINVAL is the only error that shall be returned,
        * but GLibc uses ENOSYS */
        errno = ENOSYS;
        return -1;
    }
}
