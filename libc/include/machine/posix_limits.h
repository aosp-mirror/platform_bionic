/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef _POSIX_LIMITS_H_
#define _POSIX_LIMITS_H_

/* TODO: complete and check these. */
#define _POSIX2_LINE_MAX            2048
#define _POSIX2_RE_DUP_MAX          _POSIX_RE_DUP_MAX
#define _POSIX_ARG_MAX              4096
#define _POSIX_CHILD_MAX            25
#define _POSIX_CHOWN_RESTRICTED     1  /* yes, chown requires appropriate privileges */
#define _POSIX_FSYNC                1  /* fdatasync() supported */
#define _POSIX_JOB_CONTROL          1  /* job control is a Linux feature */
#define _POSIX_LINK_MAX             8
#define _POSIX_LOGIN_NAME_MAX       9  /* includes trailing NUL */
#define _POSIX_MAPPED_FILES         1  /* mmap-ed files supported */
#define _POSIX_MAX_CANON            255
#define _POSIX_MAX_INPUT            255
#define _POSIX_MONOTONIC_CLOCK      0  /* the monotonic clock may be available; ask sysconf */
#define _POSIX_NAME_MAX             14
#define _POSIX_NGROUPS_MAX          0
#define _POSIX_NO_TRUNC             1  /* very long pathnames generate an error */
#define _POSIX_OPEN_MAX             16
#define _POSIX_PATH_MAX             256
#define _POSIX_PIPE_BUF             512
#define _POSIX_PRIORITY_SCHEDULING  1  /* priority scheduling is a Linux feature */
#define _POSIX_REALTIME_SIGNALS     -1 /* for now, this is not supported */
#define _POSIX_RE_DUP_MAX           255
#define _POSIX_SAVED_IDS            1  /* saved user ids is a Linux feature */
#define _POSIX_SEMAPHORES           200809L
#define _POSIX_SEM_VALUE_MAX        32767
#define _POSIX_SSIZE_MAX            32767
#define _POSIX_STREAM_MAX           8
#define _POSIX_SYMLINK_MAX          255
#define _POSIX_SYMLOOP_MAX          8
#define _POSIX_SYNCHRONIZED_IO      1  /* synchronized i/o supported */
#define _POSIX_THREAD_PRIO_INHERIT  200112L  /* linux feature */
#define _POSIX_THREAD_PRIO_PROTECT  200112L  /* linux feature */
#define _POSIX_THREADS              1  /* we support threads */
#define _POSIX_THREAD_STACKADDR     1  /* we support thread stack address */
#define _POSIX_THREAD_STACKSIZE     1  /* we support thread stack size */
#define _POSIX_TIMERS               1  /* Posix timers are supported */
#define _POSIX_TTY_NAME_MAX         9  /* includes trailing NUL */
#define _POSIX_TZNAME_MAX           3
#define _POSIX_VDISABLE             '\0'

#endif /* _POSIX_LIMITS_H_ */
