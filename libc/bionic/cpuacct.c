/*
 * Copyright (C) 2010 The Android Open Source Project
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
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "cpuacct.h"
#include <fcntl.h>

int cpuacct_add(uid_t uid)
{
    int count;
    int fd;
    char buf[80];
    ssize_t n;
    int ret = 0;

    count = snprintf(buf, sizeof(buf), "/acct/uid/%d/tasks", uid);
    fd = open(buf, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        /* Note: sizeof("tasks") returns 6, which includes the NULL char */
        buf[count - sizeof("tasks")] = 0;
        if (mkdir(buf, 0775) < 0)
            return -errno;

        /* Note: sizeof("tasks") returns 6, which includes the NULL char */
        buf[count - sizeof("tasks")] = '/';
        fd = open(buf, O_RDWR | O_CREAT, 0666);
    }
    if (fd == -1)
        return -errno;

    n = TEMP_FAILURE_RETRY(write(fd, "0", 1));
    if (n < 0)
        ret = -errno;
    else if (n == 0)
        ret = -EIO;

    if (TEMP_FAILURE_RETRY(close(fd)) == -1)
        return -errno;

    return ret;
}
