/*	$OpenBSD: stdio.c,v 1.9 2005/08/08 08:05:36 espie Exp $ */
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "local.h"

/*
 * Small standard I/O/seek/close functions.
 * These maintain the `known seek offset' for seek optimisation.
 */
int
__sread(void *cookie, char *buf, int n)
{
	FILE *fp = cookie;
	int ret;
	
	ret = TEMP_FAILURE_RETRY(read(fp->_file, buf, n));
	/* if the read succeeded, update the current offset */
	if (ret >= 0)
		fp->_offset += ret;
	else
		fp->_flags &= ~__SOFF;	/* paranoia */
	return (ret);
}

int
__swrite(void *cookie, const char *buf, int n)
{
	FILE *fp = cookie;

	if (fp->_flags & __SAPP)
		(void) TEMP_FAILURE_RETRY(lseek(fp->_file, (off_t)0, SEEK_END));
	fp->_flags &= ~__SOFF;	/* in case FAPPEND mode is set */
	return TEMP_FAILURE_RETRY(write(fp->_file, buf, n));
}

fpos_t
__sseek(void *cookie, fpos_t offset, int whence)
{
	FILE *fp = cookie;
	off_t ret;
	
	ret = TEMP_FAILURE_RETRY(lseek(fp->_file, (off_t)offset, whence));
	if (ret == (off_t)-1)
		fp->_flags &= ~__SOFF;
	else {
		fp->_flags |= __SOFF;
		fp->_offset = ret;
	}
	return (ret);
}

int
__sclose(void *cookie)
{
	return close(((FILE *)cookie)->_file);
}
