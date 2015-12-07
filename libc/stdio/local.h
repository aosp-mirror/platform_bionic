/*	$OpenBSD: local.h,v 1.12 2005/10/10 17:37:44 espie Exp $	*/

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

#ifndef __BIONIC_STDIO_LOCAL_H__
#define __BIONIC_STDIO_LOCAL_H__

#include <pthread.h>
#include <stdbool.h>
#include <wchar.h>
#include "wcio.h"

/*
 * Information local to this implementation of stdio,
 * in particular, macros and private variables.
 */

__BEGIN_DECLS

struct __sbuf {
  unsigned char* _base;
#if defined(__LP64__)
  size_t _size;
#else
  int _size;
#endif
};

struct __sFILE {
	unsigned char *_p;	/* current position in (some) buffer */
	int	_r;		/* read space left for getc() */
	int	_w;		/* write space left for putc() */
#if defined(__LP64__)
	int	_flags;		/* flags, below; this FILE is free if 0 */
	int	_file;		/* fileno, if Unix descriptor, else -1 */
#else
	short	_flags;		/* flags, below; this FILE is free if 0 */
	short	_file;		/* fileno, if Unix descriptor, else -1 */
#endif
	struct	__sbuf _bf;	/* the buffer (at least 1 byte, if !NULL) */
	int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

	/* operations */
	void	*_cookie;	/* cookie passed to io functions */
	int	(*_close)(void *);
	int	(*_read)(void *, char *, int);
	fpos_t	(*_seek)(void *, fpos_t, int);
	int	(*_write)(void *, const char *, int);

	/* extension data, to avoid further ABI breakage */
	struct	__sbuf _ext;
	/* data for long sequences of ungetc() */
	unsigned char *_up;	/* saved _p when _p is doing ungetc data */
	int	_ur;		/* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
	unsigned char _nbuf[1];	/* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	struct	__sbuf _lb;	/* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
	fpos_t	_offset;	/* current lseek offset */
};

/*
 * file extension
 */
struct __sfileext {
  /* ungetc buffer */
  struct __sbuf _ub;

  /* wide char io status */
  struct wchar_io_data _wcio;

  /* file lock */
  pthread_mutex_t _lock;

  /* __fsetlocking support */
  bool _caller_handles_locking;
};

#if defined(__cplusplus)
#define _EXT(fp) reinterpret_cast<__sfileext*>((fp)->_ext._base)
#else
#define _EXT(fp) ((struct __sfileext *)((fp)->_ext._base))
#endif

#define _UB(fp) _EXT(fp)->_ub
#define _FLOCK(fp)  _EXT(fp)->_lock

#define _FILEEXT_INIT(fp) \
do { \
	_UB(fp)._base = NULL; \
	_UB(fp)._size = 0; \
	WCIO_INIT(fp); \
	pthread_mutexattr_t attr; \
	pthread_mutexattr_init(&attr); \
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
	pthread_mutex_init(&_FLOCK(fp), &attr); \
	pthread_mutexattr_destroy(&attr); \
	_EXT(fp)->_caller_handles_locking = false; \
} while (0)

#define _FILEEXT_SETUP(f, fext) \
do { \
	(f)->_ext._base = (unsigned char *)(fext); \
	_FILEEXT_INIT(f); \
} while (0)

/*
 * Android <= KitKat had getc/putc macros in <stdio.h> that referred
 * to __srget/__swbuf, so those symbols need to be public for LP32
 * but can be hidden for LP64.
 */
__LIBC32_LEGACY_PUBLIC__ int __srget(FILE*);
__LIBC32_LEGACY_PUBLIC__ int __swbuf(int, FILE*);
__LIBC32_LEGACY_PUBLIC__ int __srefill(FILE*);

/* This was referenced by the apportable middleware for LP32. */
__LIBC32_LEGACY_PUBLIC__ int __swsetup(FILE*);

/* These were referenced by a couple of different pieces of middleware and the Crystax NDK. */
__LIBC32_LEGACY_PUBLIC__ int __sflags(const char*, int*);
__LIBC32_LEGACY_PUBLIC__ FILE* __sfp(void);
__LIBC32_LEGACY_PUBLIC__ void __smakebuf(FILE*);

/* These are referenced by the Greed for Glory franchise. */
__LIBC32_LEGACY_PUBLIC__ int __sflush(FILE *);
__LIBC32_LEGACY_PUBLIC__ int __sread(void *, char *, int);
__LIBC32_LEGACY_PUBLIC__ int __swrite(void *, const char *, int);
__LIBC32_LEGACY_PUBLIC__ fpos_t __sseek(void *, fpos_t, int);
__LIBC32_LEGACY_PUBLIC__ int __sclose(void *);
__LIBC32_LEGACY_PUBLIC__ int _fwalk(int (*)(FILE *));

#pragma GCC visibility push(hidden)

int	__sflush_locked(FILE *);
int	__swhatbuf(FILE *, size_t *, int *);
wint_t __fgetwc_unlock(FILE *);
wint_t	__ungetwc(wint_t, FILE *);
int	__vfprintf(FILE *, const char *, __va_list);
int	__svfscanf(FILE * __restrict, const char * __restrict, __va_list);
int	__vfwprintf(FILE * __restrict, const wchar_t * __restrict, __va_list);
int	__vfwscanf(FILE * __restrict, const wchar_t * __restrict, __va_list);

/*
 * Return true if the given FILE cannot be written now.
 */
#define	cantwrite(fp) \
	((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && \
	 __swsetup(fp))

/*
 * Test whether the given stdio file has an active ungetc buffer;
 * release such a buffer, without restoring ordinary unread data.
 */
#define	HASUB(fp) (_UB(fp)._base != NULL)
#define	FREEUB(fp) { \
	if (_UB(fp)._base != (fp)->_ubuf) \
		free(_UB(fp)._base); \
	_UB(fp)._base = NULL; \
}

/*
 * test for an fgetln() buffer.
 */
#define	HASLB(fp) ((fp)->_lb._base != NULL)
#define	FREELB(fp) { \
	free((char *)(fp)->_lb._base); \
	(fp)->_lb._base = NULL; \
}

#define FLOCKFILE(fp)   if (!_EXT(fp)->_caller_handles_locking) flockfile(fp)
#define FUNLOCKFILE(fp) if (!_EXT(fp)->_caller_handles_locking) funlockfile(fp)

#define FLOATING_POINT
#define PRINTF_WIDE_CHAR
#define SCANF_WIDE_CHAR
#define NO_PRINTF_PERCENT_N

/* OpenBSD exposes these in <stdio.h>, but we only want them exposed to the implementation. */
#define __sfeof(p)     (((p)->_flags & __SEOF) != 0)
#define __sferror(p)   (((p)->_flags & __SERR) != 0)
#define __sclearerr(p) ((void)((p)->_flags &= ~(__SERR|__SEOF)))
#define __sfileno(p)   ((p)->_file)
#if !defined(__cplusplus)
#define __sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
static __inline int __sputc(int _c, FILE* _p) {
  if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n')) {
    return (*_p->_p++ = _c);
  } else {
    return (__swbuf(_c, _p));
  }
}
#endif

/* OpenBSD declares these in fvwrite.h but we want to ensure they're hidden. */
struct __suio;
extern int __sfvwrite(FILE *, struct __suio *);
wint_t __fputwc_unlock(wchar_t wc, FILE *fp);

/* Remove the if (!__sdidinit) __sinit() idiom from untouched upstream stdio code. */
extern void __sinit(void); // Not actually implemented.
#define __sdidinit 1

#pragma GCC visibility pop

__END_DECLS

#endif
