/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <unistd.h>

#include "header_checks.h"

static void unistd_h() {
  MACRO(_POSIX_VERSION);
  MACRO(_POSIX2_VERSION);
  MACRO(_XOPEN_VERSION);

  MACRO(_POSIX_ADVISORY_INFO);
  MACRO(_POSIX_ASYNCHRONOUS_IO);
  MACRO(_POSIX_BARRIERS);
  MACRO(_POSIX_CHOWN_RESTRICTED);
  MACRO(_POSIX_CLOCK_SELECTION);
  MACRO(_POSIX_CPUTIME);
  MACRO(_POSIX_FSYNC);
  MACRO(_POSIX_IPV6);
  MACRO(_POSIX_JOB_CONTROL);
  MACRO(_POSIX_MAPPED_FILES);
  MACRO(_POSIX_MEMLOCK);
  MACRO(_POSIX_MEMLOCK_RANGE);
  MACRO(_POSIX_MEMORY_PROTECTION);
  MACRO(_POSIX_MESSAGE_PASSING);
  MACRO(_POSIX_MONOTONIC_CLOCK);
  MACRO(_POSIX_NO_TRUNC);
  MACRO(_POSIX_PRIORITIZED_IO);
  MACRO(_POSIX_PRIORITY_SCHEDULING);
  MACRO(_POSIX_RAW_SOCKETS);
  MACRO(_POSIX_READER_WRITER_LOCKS);
  MACRO(_POSIX_REALTIME_SIGNALS);
  MACRO(_POSIX_REGEXP);
  MACRO(_POSIX_SAVED_IDS);
  MACRO(_POSIX_SEMAPHORES);
  MACRO(_POSIX_SHARED_MEMORY_OBJECTS);
  MACRO(_POSIX_SHELL);
  MACRO(_POSIX_SPAWN);
  MACRO(_POSIX_SPIN_LOCKS);
  MACRO(_POSIX_SPORADIC_SERVER);
  MACRO(_POSIX_SYNCHRONIZED_IO);
  MACRO(_POSIX_THREAD_ATTR_STACKADDR);
  MACRO(_POSIX_THREAD_ATTR_STACKSIZE);
  MACRO(_POSIX_THREAD_CPUTIME);
  MACRO(_POSIX_THREAD_PRIO_INHERIT);
  MACRO(_POSIX_THREAD_PRIO_PROTECT);
  MACRO(_POSIX_THREAD_PRIORITY_SCHEDULING);
  MACRO(_POSIX_THREAD_PROCESS_SHARED);
  MACRO(_POSIX_THREAD_ROBUST_PRIO_INHERIT);
  MACRO(_POSIX_THREAD_ROBUST_PRIO_PROTECT);
  MACRO(_POSIX_THREAD_SAFE_FUNCTIONS);
  MACRO(_POSIX_THREAD_SPORADIC_SERVER);
  MACRO(_POSIX_THREADS);
  MACRO(_POSIX_TIMEOUTS);
  MACRO(_POSIX_TIMERS);
  MACRO(_POSIX_TYPED_MEMORY_OBJECTS);
  MACRO(_POSIX2_C_BIND);
  MACRO(_POSIX2_CHAR_TERM);
  MACRO(_POSIX2_LOCALEDEF);
  MACRO(_POSIX2_SW_DEV);
#if 0 // No libc I can find actually has this.
  MACRO(_POSIX2_UPE);
#endif
  MACRO(_XOPEN_CRYPT);
  MACRO(_XOPEN_ENH_I18N);
  MACRO(_XOPEN_REALTIME);
  MACRO(_XOPEN_REALTIME_THREADS);
  MACRO(_XOPEN_SHM);
  MACRO(_XOPEN_UNIX);
#if defined(_XOPEN_UUCP)
#if _XOPEN_UUCP != -1 && _XOPEN_UUCP != 0 && _XOPEN_UUCP != 200809L
#error _XOPEN_UUCP
#endif
#endif

  MACRO(NULL);

  MACRO(F_OK);
  MACRO(R_OK);
  MACRO(W_OK);
  MACRO(X_OK);

#if !defined(__BIONIC__) // No confstr on Android.
  MACRO(_CS_PATH);
  MACRO(_CS_POSIX_V7_ILP32_OFF32_CFLAGS);
  MACRO(_CS_POSIX_V7_ILP32_OFF32_LDFLAGS);
  MACRO(_CS_POSIX_V7_ILP32_OFF32_LIBS);
  MACRO(_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS);
  MACRO(_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS);
  MACRO(_CS_POSIX_V7_ILP32_OFFBIG_LIBS);
  MACRO(_CS_POSIX_V7_LP64_OFF64_CFLAGS);
  MACRO(_CS_POSIX_V7_LP64_OFF64_LDFLAGS);
  MACRO(_CS_POSIX_V7_LP64_OFF64_LIBS);
  MACRO(_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS);
  MACRO(_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS);
  MACRO(_CS_POSIX_V7_LPBIG_OFFBIG_LIBS);
  MACRO(_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS);
  MACRO(_CS_V7_ENV);
#endif

  MACRO(SEEK_CUR);
  MACRO(SEEK_END);
  MACRO(SEEK_SET);

  MACRO(F_LOCK);
  MACRO(F_TEST);
  MACRO(F_TLOCK);
  MACRO(F_ULOCK);

  MACRO(_PC_2_SYMLINKS);
  MACRO(_PC_ALLOC_SIZE_MIN);
  MACRO(_PC_ASYNC_IO);
  MACRO(_PC_CHOWN_RESTRICTED);
  MACRO(_PC_FILESIZEBITS);
  MACRO(_PC_LINK_MAX);
  MACRO(_PC_MAX_CANON);
  MACRO(_PC_MAX_INPUT);
  MACRO(_PC_NAME_MAX);
  MACRO(_PC_NO_TRUNC);
  MACRO(_PC_PATH_MAX);
  MACRO(_PC_PIPE_BUF);
  MACRO(_PC_PRIO_IO);
  MACRO(_PC_REC_INCR_XFER_SIZE);
  MACRO(_PC_REC_MAX_XFER_SIZE);
  MACRO(_PC_REC_MIN_XFER_SIZE);
  MACRO(_PC_REC_XFER_ALIGN);
  MACRO(_PC_SYMLINK_MAX);
  MACRO(_PC_SYNC_IO);
#if 0 // No libc I can find actually has this.
  MACRO(_PC_TIMESTAMP_RESOLUTION);
#endif
  MACRO(_PC_VDISABLE);

  MACRO(_SC_2_C_BIND);
  MACRO(_SC_2_C_DEV);
  MACRO(_SC_2_CHAR_TERM);
  MACRO(_SC_2_FORT_DEV);
  MACRO(_SC_2_FORT_RUN);
  MACRO(_SC_2_LOCALEDEF);
  MACRO(_SC_2_SW_DEV);
  MACRO(_SC_2_UPE);
  MACRO(_SC_2_VERSION);
  MACRO(_SC_ADVISORY_INFO);
  MACRO(_SC_AIO_LISTIO_MAX);
  MACRO(_SC_AIO_MAX);
  MACRO(_SC_AIO_PRIO_DELTA_MAX);
  MACRO(_SC_ARG_MAX);
  MACRO(_SC_ASYNCHRONOUS_IO);
  MACRO(_SC_ATEXIT_MAX);
  MACRO(_SC_BARRIERS);
  MACRO(_SC_BC_BASE_MAX);
  MACRO(_SC_BC_DIM_MAX);
  MACRO(_SC_BC_SCALE_MAX);
  MACRO(_SC_BC_STRING_MAX);
  MACRO(_SC_CHILD_MAX);
  MACRO(_SC_CLK_TCK);
  MACRO(_SC_CLOCK_SELECTION);
  MACRO(_SC_COLL_WEIGHTS_MAX);
  MACRO(_SC_CPUTIME);
  MACRO(_SC_DELAYTIMER_MAX);
  MACRO(_SC_EXPR_NEST_MAX);
  MACRO(_SC_FSYNC);
  MACRO(_SC_GETGR_R_SIZE_MAX);
  MACRO(_SC_GETPW_R_SIZE_MAX);
  MACRO(_SC_HOST_NAME_MAX);
  MACRO(_SC_IOV_MAX);
  MACRO(_SC_IPV6);
  MACRO(_SC_JOB_CONTROL);
  MACRO(_SC_LINE_MAX);
  MACRO(_SC_LOGIN_NAME_MAX);
  MACRO(_SC_MAPPED_FILES);
  MACRO(_SC_MEMLOCK);
  MACRO(_SC_MEMLOCK_RANGE);
  MACRO(_SC_MEMORY_PROTECTION);
  MACRO(_SC_MESSAGE_PASSING);
  MACRO(_SC_MONOTONIC_CLOCK);
  MACRO(_SC_MQ_OPEN_MAX);
  MACRO(_SC_MQ_PRIO_MAX);
  MACRO(_SC_NGROUPS_MAX);
  MACRO(_SC_OPEN_MAX);
  MACRO(_SC_PAGE_SIZE);
  MACRO(_SC_PAGESIZE);
  MACRO(_SC_PRIORITIZED_IO);
  MACRO(_SC_PRIORITY_SCHEDULING);
  MACRO(_SC_RAW_SOCKETS);
  MACRO(_SC_RE_DUP_MAX);
  MACRO(_SC_READER_WRITER_LOCKS);
  MACRO(_SC_REALTIME_SIGNALS);
  MACRO(_SC_REGEXP);
  MACRO(_SC_RTSIG_MAX);
  MACRO(_SC_SAVED_IDS);
  MACRO(_SC_SEM_NSEMS_MAX);
  MACRO(_SC_SEM_VALUE_MAX);
  MACRO(_SC_SEMAPHORES);
  MACRO(_SC_SHARED_MEMORY_OBJECTS);
  MACRO(_SC_SHELL);
  MACRO(_SC_SIGQUEUE_MAX);
  MACRO(_SC_SPAWN);
  MACRO(_SC_SPIN_LOCKS);
  MACRO(_SC_SPORADIC_SERVER);
  MACRO(_SC_SS_REPL_MAX);
  MACRO(_SC_STREAM_MAX);
  MACRO(_SC_SYMLOOP_MAX);
  MACRO(_SC_SYNCHRONIZED_IO);
  MACRO(_SC_THREAD_ATTR_STACKADDR);
  MACRO(_SC_THREAD_ATTR_STACKSIZE);
  MACRO(_SC_THREAD_CPUTIME);
  MACRO(_SC_THREAD_DESTRUCTOR_ITERATIONS);
  MACRO(_SC_THREAD_KEYS_MAX);
  MACRO(_SC_THREAD_PRIO_INHERIT);
  MACRO(_SC_THREAD_PRIO_PROTECT);
  MACRO(_SC_THREAD_PRIORITY_SCHEDULING);
  MACRO(_SC_THREAD_PROCESS_SHARED);
  MACRO(_SC_THREAD_ROBUST_PRIO_INHERIT);
  MACRO(_SC_THREAD_ROBUST_PRIO_PROTECT);
  MACRO(_SC_THREAD_SAFE_FUNCTIONS);
  MACRO(_SC_THREAD_SPORADIC_SERVER);
  MACRO(_SC_THREAD_STACK_MIN);
  MACRO(_SC_THREAD_THREADS_MAX);
  MACRO(_SC_THREADS);
  MACRO(_SC_TIMEOUTS);
  MACRO(_SC_TIMER_MAX);
  MACRO(_SC_TIMERS);
  MACRO(_SC_TRACE);
  MACRO(_SC_TRACE_EVENT_FILTER);
  MACRO(_SC_TRACE_EVENT_NAME_MAX);
  MACRO(_SC_TRACE_INHERIT);
  MACRO(_SC_TRACE_LOG);
  MACRO(_SC_TRACE_NAME_MAX);
  MACRO(_SC_TRACE_SYS_MAX);
  MACRO(_SC_TRACE_USER_EVENT_MAX);
  MACRO(_SC_TYPED_MEMORY_OBJECTS);
  MACRO(_SC_TZNAME_MAX);
  MACRO(_SC_V7_ILP32_OFF32);
  MACRO(_SC_VERSION);
  MACRO(_SC_XOPEN_CRYPT);
  MACRO(_SC_XOPEN_ENH_I18N);
  MACRO(_SC_XOPEN_REALTIME);
  MACRO(_SC_XOPEN_REALTIME_THREADS);
  MACRO(_SC_XOPEN_SHM);
  MACRO(_SC_XOPEN_STREAMS);
  MACRO(_SC_XOPEN_UNIX);
#if 0 // No libc I can find actually has this.
  MACRO(_SC_XOPEN_UUCP);
#endif
  MACRO(_SC_XOPEN_VERSION);

  MACRO_VALUE(STDERR_FILENO, 2);
  MACRO_VALUE(STDIN_FILENO, 0);
  MACRO_VALUE(STDOUT_FILENO, 1);

  MACRO(_POSIX_VDISABLE);

  TYPE(size_t);
  TYPE(ssize_t);
  TYPE(uid_t);
  TYPE(gid_t);
  TYPE(off_t);
  TYPE(pid_t);
  TYPE(intptr_t);

  FUNCTION(access, int (*f)(const char*, int));
  FUNCTION(alarm, unsigned (*f)(unsigned));
  FUNCTION(chdir, int (*f)(const char*));
  FUNCTION(chown, int (*f)(const char*, uid_t, gid_t));
  FUNCTION(close, int (*f)(int));
#if !defined(__BIONIC__)
  FUNCTION(confstr, size_t (*f)(int, char*, size_t));
  FUNCTION(crypt, char* (*f)(const char*, const char*));
#endif
  FUNCTION(dup, int (*f)(int));
  FUNCTION(dup2, int (*f)(int, int));
  FUNCTION(_exit, void (*f)(int));
#if !defined(__BIONIC__)
  FUNCTION(encrypt, void (*f)(char[64], int));
#endif
  FUNCTION(execl, int (*f)(const char*, const char*, ...));
  FUNCTION(execle, int (*f)(const char*, const char*, ...));
  FUNCTION(execlp, int (*f)(const char*, const char*, ...));
  FUNCTION(execv, int (*f)(const char*, char* const[]));
  FUNCTION(execve, int (*f)(const char*, char* const[], char* const[]));
  FUNCTION(execvp, int (*f)(const char*, char* const[]));
  FUNCTION(faccessat, int (*f)(int, const char*, int, int));
  FUNCTION(fchdir, int (*f)(int));
  FUNCTION(fchown, int (*f)(int, uid_t, gid_t));
  FUNCTION(fchownat, int (*f)(int, const char*, uid_t, gid_t, int));
  FUNCTION(fdatasync, int (*f)(int));
  FUNCTION(fexecve, int (*f)(int, char* const[], char* const[]));
  FUNCTION(fork, pid_t (*f)(void));
  FUNCTION(fpathconf, long (*f)(int, int));
  FUNCTION(fsync, int (*f)(int));
  FUNCTION(ftruncate, int (*f)(int, off_t));
  FUNCTION(getcwd, char* (*f)(char*, size_t));
  FUNCTION(getegid, gid_t (*f)(void));
  FUNCTION(geteuid, uid_t (*f)(void));
  FUNCTION(getgid, gid_t (*f)(void));
  FUNCTION(getgroups, int (*f)(int, gid_t[]));
#if !defined(__BIONIC__)
  FUNCTION(gethostid, long (*f)(void));
#endif
  FUNCTION(gethostname, int (*f)(char*, size_t));
  FUNCTION(getlogin, char* (*f)(void));
  FUNCTION(getlogin_r, int (*f)(char*, size_t));
  FUNCTION(getopt, int (*f)(int, char* const[], const char*));
  FUNCTION(getpgid, pid_t (*f)(pid_t));
  FUNCTION(getpgrp, pid_t (*f)(void));
  FUNCTION(getpid, pid_t (*f)(void));
  FUNCTION(getppid, pid_t (*f)(void));
  FUNCTION(getsid, pid_t (*f)(pid_t));
  FUNCTION(getuid, uid_t (*f)(void));
  FUNCTION(isatty, int (*f)(int));
  FUNCTION(lchown, int (*f)(const char*, uid_t, gid_t));
  FUNCTION(link, int (*f)(const char*, const char*));
  FUNCTION(linkat, int (*f)(int, const char*, int, const char*, int));
  FUNCTION(lockf, int (*f)(int, int, off_t));
  FUNCTION(lseek, off_t (*f)(int, off_t, int));
  FUNCTION(nice, int (*f)(int));
  FUNCTION(pathconf, long (*f)(const char*, int));
  FUNCTION(pause, int (*f)(void));
  FUNCTION(pipe, int (*f)(int[2]));
  FUNCTION(pread, ssize_t (*f)(int, void*, size_t, off_t));
  FUNCTION(pwrite, ssize_t (*f)(int, const void*, size_t, off_t));
  FUNCTION(read, ssize_t (*f)(int, void*, size_t));
  FUNCTION(readlink, ssize_t (*f)(const char*, char*, size_t));
  FUNCTION(readlinkat, ssize_t (*f)(int, const char*, char*, size_t));
  FUNCTION(rmdir, int (*f)(const char*));
  FUNCTION(setegid, int (*f)(gid_t));
  FUNCTION(seteuid, int (*f)(uid_t));
  FUNCTION(setgid, int (*f)(gid_t));
  FUNCTION(setpgid, int (*f)(pid_t, pid_t));
  FUNCTION(setpgrp, pid_t (*f)(void));
  FUNCTION(setregid, int (*f)(gid_t, gid_t));
  FUNCTION(setreuid, int (*f)(uid_t, uid_t));
  FUNCTION(setsid, pid_t (*f)(void));
  FUNCTION(setuid, int (*f)(uid_t));
  FUNCTION(sleep, unsigned (*f)(unsigned));
  FUNCTION(swab, void (*f)(const void*, void*, ssize_t));
  FUNCTION(symlink, int (*f)(const char*, const char*));
  FUNCTION(symlinkat, int (*f)(const char*, int, const char*));
  FUNCTION(sync, void (*f)(void));
  FUNCTION(sysconf, long (*f)(int));
  FUNCTION(tcgetpgrp, pid_t (*f)(int));
  FUNCTION(tcsetpgrp, int (*f)(int, pid_t));
  FUNCTION(truncate, int (*f)(const char*, off_t));
  FUNCTION(ttyname, char* (*f)(int));
  FUNCTION(ttyname_r, int (*f)(int, char*, size_t));
  FUNCTION(unlink, int (*f)(const char*));
  FUNCTION(unlinkat, int (*f)(int, const char*, int));
  FUNCTION(write, ssize_t (*f)(int, const void*, size_t));

  char* cp;
  cp = optarg;
  int i;
  i = opterr;
  i = optind;
  i = optopt;
}
