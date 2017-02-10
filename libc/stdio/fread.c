/*	$OpenBSD: fread.c,v 1.12 2014/05/01 16:40:36 deraadt Exp $ */
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

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/param.h>
#include "local.h"

#define MUL_NO_OVERFLOW	(1UL << (sizeof(size_t) * 4))

size_t
fread(void *buf, size_t size, size_t count, FILE *fp) __overloadable
{
	/*
	 * Extension:  Catch integer overflow.
	 */
	if ((size >= MUL_NO_OVERFLOW || count >= MUL_NO_OVERFLOW) &&
	    size > 0 && SIZE_MAX / size < count) {
		errno = EOVERFLOW;
		fp->_flags |= __SERR;
		return (0);
	}

	const size_t desired_total = count * size;
	size_t total = desired_total;

	/*
	 * ANSI and SUSv2 require a return value of 0 if size or count are 0.
	 */
	if (total == 0) {
		return (0);
	}

	FLOCKFILE(fp);
	_SET_ORIENTATION(fp, -1);

	// TODO: how can this ever happen?!
	if (fp->_r < 0)
		fp->_r = 0;

	/*
	 * Ensure _bf._size is valid.
	 */
	if (fp->_bf._base == NULL) {
		__smakebuf(fp);
	}

	char* dst = buf;

	while (total > 0) {
		/*
		 * Copy data out of the buffer.
		 */
		size_t buffered_bytes = MIN((size_t) fp->_r, total);
		memcpy(dst, fp->_p, buffered_bytes);
		fp->_p += buffered_bytes;
		fp->_r -= buffered_bytes;
		dst += buffered_bytes;
		total -= buffered_bytes;

		/*
		 * Are we done?
		 */
		if (total == 0) {
			goto out;
		}

		/*
		 * Do we have so much more to read that we should
		 * avoid copying it through the buffer?
		 */
		if (total > (size_t) fp->_bf._size) {
			/*
			 * Make sure that fseek doesn't think it can
			 * reuse the buffer since we are going to read
			 * directly from the file descriptor.
			 */
			fp->_flags |= __SMOD;
			break;
		}

		/*
		 * Less than a buffer to go, so refill the buffer and
		 * go around the loop again.
		 */
		if (__srefill(fp)) {
			goto out;
		}
	}

	/*
	 * Read directly into the caller's buffer.
	 */
	while (total > 0) {
		ssize_t bytes_read = (*fp->_read)(fp->_cookie, dst, total);
		if (bytes_read <= 0) {
			fp->_flags |= (bytes_read == 0) ? __SEOF : __SERR;
			break;
		}
		dst += bytes_read;
		total -= bytes_read;
	}

out:
	FUNLOCKFILE(fp);
	return ((desired_total - total) / size);
}
