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

/*
 * The MIPS64 getdents64() system call is only present in 3.10+ kernels.
 * If the getdents64() system call is not available fall back to using
 * getdents() and modify the result to be compatible with getdents64().
 */

#include <dirent.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


/* The mips_getdents type is 64bit clean */
struct mips_dirent {
        uint64_t d_ino;     /* Inode number */
        uint64_t d_off;     /* Offset to next mips_dirent */
        uint16_t d_reclen;  /* Length of this mips_dirent */
        char     d_name[];  /* Filename (null-terminated) */
                            /* length is actually (d_reclen - 2 -
                               offsetof(struct mips_dirent, d_name) */
        // char  pad;       /* Zero padding byte */
        // char  d_type;    /* File type (only since Linux 2.6.4; offset is (d_reclen - 1)) */
};

extern "C" int __getdents64(unsigned int fd, struct dirent *dirp, unsigned int count);
extern "C" int __getdents(unsigned int fd, struct mips_dirent *dirp, unsigned int count);
int getdents(unsigned int fd, struct dirent *dirp, unsigned int count)
{
        int r;
        int oerrno = errno;

        /* Use getdents64() if it is available */
        r = __getdents64(fd, dirp, count);
        if (r >= 0 || errno != ENOSYS)
                return r;

        /* Fallback to getdents() */
        errno = oerrno;
        r = __getdents(fd, (struct mips_dirent *)dirp, count);
        if (r > 0) {
                char *p;
                char type;
                union dirents {
                        struct mips_dirent m;
                        struct dirent d;
                } *u;

                p = (char *)dirp;
                do {
                        u = (union dirents *)p;

                        /* This should not happen, but just in case... */
                        if (p + u->m.d_reclen > (char *)dirp + r)
                                break;

                        /* shuffle the dirent */
                        type = *(p + u->m.d_reclen - 1);
                        memmove(u->d.d_name, u->m.d_name,
                                u->m.d_reclen - 2 - offsetof(struct mips_dirent, d_name) + 1);
                        u->d.d_type = type;

                        p += u->m.d_reclen;
                } while (p < (char *)dirp + r);
        }
        return r;
}
