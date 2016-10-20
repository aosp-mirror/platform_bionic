/*	$OpenBSD: findfp.c,v 1.15 2013/12/17 16:33:27 deraadt Exp $ */
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

#define __BIONIC_NO_STDIO_FORTIFY
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <paths.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include "local.h"
#include "glue.h"
#include "private/bionic_fortify.h"
#include "private/ErrnoRestorer.h"
#include "private/thread_private.h"

#define ALIGNBYTES (sizeof(uintptr_t) - 1)
#define ALIGN(p) (((uintptr_t)(p) + ALIGNBYTES) &~ ALIGNBYTES)

#define	NDYNAMIC 10		/* add ten more whenever necessary */

#define PRINTF_IMPL(expr) \
    va_list ap; \
    va_start(ap, fmt); \
    int result = (expr); \
    va_end(ap); \
    return result;

#define std(flags, file) \
    {0,0,0,flags,file,{0,0},0,__sF+file,__sclose,__sread,nullptr,__swrite, \
    {(unsigned char *)(__sFext+file), 0},nullptr,0,{0},{0},{0,0},0,0}

_THREAD_PRIVATE_MUTEX(__sfp_mutex);

// TODO: when we no longer have to support both clang and GCC, we can simplify all this.
#define SBUF_INIT {0,0}
#if defined(__LP64__)
#define MBSTATE_T_INIT {{0},{0}}
#else
#define MBSTATE_T_INIT {{0}}
#endif
#define WCHAR_IO_DATA_INIT {MBSTATE_T_INIT,MBSTATE_T_INIT,{0},0,0}

static struct __sfileext __sFext[3] = {
  { SBUF_INIT, WCHAR_IO_DATA_INIT, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP, false, __sseek64 },
  { SBUF_INIT, WCHAR_IO_DATA_INIT, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP, false, __sseek64 },
  { SBUF_INIT, WCHAR_IO_DATA_INIT, PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP, false, __sseek64 },
};

// __sF is exported for backwards compatibility. Until M, we didn't have symbols
// for stdin/stdout/stderr; they were macros accessing __sF.
FILE __sF[3] = {
  std(__SRD, STDIN_FILENO),
  std(__SWR, STDOUT_FILENO),
  std(__SWR|__SNBF, STDERR_FILENO),
};

FILE* stdin = &__sF[0];
FILE* stdout = &__sF[1];
FILE* stderr = &__sF[2];

struct glue __sglue = { NULL, 3, __sF };
static struct glue* lastglue = &__sglue;

class ScopedFileLock {
 public:
  explicit ScopedFileLock(FILE* fp) : fp_(fp) {
    FLOCKFILE(fp_);
  }
  ~ScopedFileLock() {
    FUNLOCKFILE(fp_);
  }

 private:
  FILE* fp_;
};

static glue* moreglue(int n) {
  static FILE empty;

  char* data = new char[sizeof(glue) + ALIGNBYTES + n * sizeof(FILE) + n * sizeof(__sfileext)];
  if (data == nullptr) return nullptr;

  glue* g = reinterpret_cast<glue*>(data);
  FILE* p = reinterpret_cast<FILE*>(ALIGN(data + sizeof(*g)));
  __sfileext* pext = reinterpret_cast<__sfileext*>(ALIGN(data + sizeof(*g)) + n * sizeof(FILE));
  g->next = NULL;
  g->niobs = n;
  g->iobs = p;
  while (--n >= 0) {
    *p = empty;
    _FILEEXT_SETUP(p, pext);
    p++;
    pext++;
  }
  return g;
}

/*
 * Find a free FILE for fopen et al.
 */
FILE* __sfp(void) {
	FILE *fp;
	int n;
	struct glue *g;

	_THREAD_PRIVATE_MUTEX_LOCK(__sfp_mutex);
	for (g = &__sglue; g != NULL; g = g->next) {
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags == 0)
				goto found;
	}

	/* release lock while mallocing */
	_THREAD_PRIVATE_MUTEX_UNLOCK(__sfp_mutex);
	if ((g = moreglue(NDYNAMIC)) == NULL)
		return (NULL);
	_THREAD_PRIVATE_MUTEX_LOCK(__sfp_mutex);
	lastglue->next = g;
	lastglue = g;
	fp = g->iobs;
found:
	fp->_flags = 1;		/* reserve this slot; caller sets real flags */
	_THREAD_PRIVATE_MUTEX_UNLOCK(__sfp_mutex);
	fp->_p = NULL;		/* no current pointer */
	fp->_w = 0;		/* nothing to read or write */
	fp->_r = 0;
	fp->_bf._base = NULL;	/* no buffer */
	fp->_bf._size = 0;
	fp->_lbfsize = 0;	/* not line buffered */
	fp->_file = -1;		/* no file */

	fp->_lb._base = NULL;	/* no line buffer */
	fp->_lb._size = 0;
	_FILEEXT_INIT(fp);

	// Caller sets cookie, _read/_write etc.
	// We explicitly clear _seek and _seek64 to prevent subtle bugs.
	fp->_seek = nullptr;
	_EXT(fp)->_seek64 = nullptr;

	return fp;
}

extern "C" __LIBC_HIDDEN__ void __libc_stdio_cleanup(void) {
  // Equivalent to fflush(nullptr), but without all the locking since we're shutting down anyway.
  _fwalk(__sflush);
}

static FILE* __fopen(int fd, int flags) {
#if !defined(__LP64__)
  if (fd > SHRT_MAX) {
    errno = EMFILE;
    return nullptr;
  }
#endif

  FILE* fp = __sfp();
  if (fp != nullptr) {
    fp->_file = fd;
    fp->_flags = flags;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_close = __sclose;
    _EXT(fp)->_seek64 = __sseek64;
  }
  return fp;
}

FILE* fopen(const char* file, const char* mode) {
  int oflags;
  int flags = __sflags(mode, &oflags);
  if (flags == 0) return nullptr;

  int fd = open(file, oflags, DEFFILEMODE);
  if (fd == -1) {
    return nullptr;
  }

  FILE* fp = __fopen(fd, flags);
  if (fp == nullptr) {
    ErrnoRestorer errno_restorer;
    close(fd);
    return nullptr;
  }

  // When opening in append mode, even though we use O_APPEND,
  // we need to seek to the end so that ftell() gets the right
  // answer.  If the user then alters the seek pointer, or
  // the file extends, this will fail, but there is not much
  // we can do about this.  (We could set __SAPP and check in
  // fseek and ftell.)
  // TODO: check in __sseek instead.
  if (oflags & O_APPEND) __sseek64(fp, 0, SEEK_END);

  return fp;
}
__strong_alias(fopen64, fopen);

FILE* fdopen(int fd, const char* mode) {
  int oflags;
  int flags = __sflags(mode, &oflags);
  if (flags == 0) return nullptr;

  // Make sure the mode the user wants is a subset of the actual mode.
  int fdflags = fcntl(fd, F_GETFL, 0);
  if (fdflags < 0) return nullptr;
  int tmp = fdflags & O_ACCMODE;
  if (tmp != O_RDWR && (tmp != (oflags & O_ACCMODE))) {
    errno = EINVAL;
    return nullptr;
  }

  // If opened for appending, but underlying descriptor does not have
  // O_APPEND bit set, assert __SAPP so that __swrite() will lseek to
  // end before each write.
  // TODO: use fcntl(2) to set O_APPEND instead.
  if ((oflags & O_APPEND) && !(fdflags & O_APPEND)) flags |= __SAPP;

  // If close-on-exec was requested, then turn it on if not already.
  if ((oflags & O_CLOEXEC) && !((tmp = fcntl(fd, F_GETFD)) & FD_CLOEXEC)) {
    fcntl(fd, F_SETFD, tmp | FD_CLOEXEC);
  }

  return __fopen(fd, flags);
}

// Re-direct an existing, open (probably) file to some other file.
// ANSI is written such that the original file gets closed if at
// all possible, no matter what.
// TODO: rewrite this mess completely.
FILE* freopen(const char* file, const char* mode, FILE* fp) {
  int oflags;
  int flags = __sflags(mode, &oflags);
  if (flags == 0) {
    fclose(fp);
    return nullptr;
  }

  ScopedFileLock sfl(fp);

  // There are actually programs that depend on being able to "freopen"
  // descriptors that weren't originally open.  Keep this from breaking.
  // Remember whether the stream was open to begin with, and which file
  // descriptor (if any) was associated with it.  If it was attached to
  // a descriptor, defer closing it; freopen("/dev/stdin", "r", stdin)
  // should work.  This is unnecessary if it was not a Unix file.
  int isopen, wantfd;
  if (fp->_flags == 0) {
    fp->_flags = __SEOF; // Hold on to it.
    isopen = 0;
    wantfd = -1;
  } else {
    // Flush the stream; ANSI doesn't require this.
    if (fp->_flags & __SWR) __sflush(fp);

    // If close is NULL, closing is a no-op, hence pointless.
    isopen = fp->_close != NULL;
    if ((wantfd = fp->_file) < 0 && isopen) {
        (*fp->_close)(fp->_cookie);
        isopen = 0;
    }
  }

  // Get a new descriptor to refer to the new file.
  int fd = open(file, oflags, DEFFILEMODE);
  if (fd < 0 && isopen) {
    // If out of fd's close the old one and try again.
    if (errno == ENFILE || errno == EMFILE) {
      (*fp->_close)(fp->_cookie);
      isopen = 0;
      fd = open(file, oflags, DEFFILEMODE);
    }
  }

  int sverrno = errno;

  // Finish closing fp.  Even if the open succeeded above, we cannot
  // keep fp->_base: it may be the wrong size.  This loses the effect
  // of any setbuffer calls, but stdio has always done this before.
  if (isopen && fd != wantfd) (*fp->_close)(fp->_cookie);
  if (fp->_flags & __SMBF) free(fp->_bf._base);
  fp->_w = 0;
  fp->_r = 0;
  fp->_p = NULL;
  fp->_bf._base = NULL;
  fp->_bf._size = 0;
  fp->_lbfsize = 0;
  if (HASUB(fp)) FREEUB(fp);
  _UB(fp)._size = 0;
  WCIO_FREE(fp);
  if (HASLB(fp)) FREELB(fp);
  fp->_lb._size = 0;

  if (fd < 0) { // Did not get it after all.
    fp->_flags = 0; // Release.
    errno = sverrno; // Restore errno in case _close clobbered it.
    return nullptr;
  }

  // If reopening something that was open before on a real file, try
  // to maintain the descriptor.  Various C library routines (perror)
  // assume stderr is always fd STDERR_FILENO, even if being freopen'd.
  if (wantfd >= 0 && fd != wantfd) {
    if (dup3(fd, wantfd, oflags & O_CLOEXEC) >= 0) {
      close(fd);
      fd = wantfd;
    }
  }

  // _file is only a short.
  if (fd > SHRT_MAX) {
      fp->_flags = 0; // Release.
      errno = EMFILE;
      return nullptr;
  }

  fp->_flags = flags;
  fp->_file = fd;
  fp->_cookie = fp;
  fp->_read = __sread;
  fp->_write = __swrite;
  fp->_close = __sclose;
  _EXT(fp)->_seek64 = __sseek64;

  // When opening in append mode, even though we use O_APPEND,
  // we need to seek to the end so that ftell() gets the right
  // answer.  If the user then alters the seek pointer, or
  // the file extends, this will fail, but there is not much
  // we can do about this.  (We could set __SAPP and check in
  // fseek and ftell.)
  if (oflags & O_APPEND) __sseek64(fp, 0, SEEK_END);
  return fp;
}
__strong_alias(freopen64, freopen);

int fclose(FILE* fp) {
  if (fp->_flags == 0) {
    // Already freed!
    errno = EBADF;
    return EOF;
  }

  ScopedFileLock sfl(fp);
  WCIO_FREE(fp);
  int r = fp->_flags & __SWR ? __sflush(fp) : 0;
  if (fp->_close != NULL && (*fp->_close)(fp->_cookie) < 0) {
    r = EOF;
  }
  if (fp->_flags & __SMBF) free(fp->_bf._base);
  if (HASUB(fp)) FREEUB(fp);
  if (HASLB(fp)) FREELB(fp);

  // Poison this FILE so accesses after fclose will be obvious.
  fp->_file = -1;
  fp->_r = fp->_w = 0;

  // Release this FILE for reuse.
  fp->_flags = 0;
  return r;
}

int fileno_unlocked(FILE* fp) {
  int fd = fp->_file;
  if (fd == -1) {
    errno = EBADF;
    return -1;
  }
  return fd;
}

int fileno(FILE* fp) {
  ScopedFileLock sfl(fp);
  return fileno_unlocked(fp);
}

void clearerr_unlocked(FILE* fp) {
  return __sclearerr(fp);
}

void clearerr(FILE* fp) {
  ScopedFileLock sfl(fp);
  clearerr_unlocked(fp);
}

int feof_unlocked(FILE* fp) {
  return ((fp->_flags & __SEOF) != 0);
}

int feof(FILE* fp) {
  ScopedFileLock sfl(fp);
  return feof_unlocked(fp);
}

int ferror_unlocked(FILE* fp) {
  return __sferror(fp);
}

int ferror(FILE* fp) {
  ScopedFileLock sfl(fp);
  return ferror_unlocked(fp);
}

int __sread(void* cookie, char* buf, int n) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
  return TEMP_FAILURE_RETRY(read(fp->_file, buf, n));
}

int __swrite(void* cookie, const char* buf, int n) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
  if (fp->_flags & __SAPP) {
    // The FILE* is in append mode, but the underlying fd doesn't have O_APPEND set.
    // We need to seek manually.
    // TODO: use fcntl(2) to set O_APPEND in fdopen(3) instead?
    TEMP_FAILURE_RETRY(lseek64(fp->_file, 0, SEEK_END));
  }
  return TEMP_FAILURE_RETRY(write(fp->_file, buf, n));
}

fpos_t __sseek(void* cookie, fpos_t offset, int whence) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
  return TEMP_FAILURE_RETRY(lseek(fp->_file, offset, whence));
}

off64_t __sseek64(void* cookie, off64_t offset, int whence) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
  return TEMP_FAILURE_RETRY(lseek64(fp->_file, offset, whence));
}

int __sclose(void* cookie) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
  return close(fp->_file);
}

static off64_t __seek_unlocked(FILE* fp, off64_t offset, int whence) {
  // Use `_seek64` if set, but fall back to `_seek`.
  if (_EXT(fp)->_seek64 != nullptr) {
    return (*_EXT(fp)->_seek64)(fp->_cookie, offset, whence);
  } else if (fp->_seek != nullptr) {
    off64_t result = (*fp->_seek)(fp->_cookie, offset, whence);
#if !defined(__LP64__)
    // Avoid sign extension if off64_t is larger than off_t.
    if (result != -1) result &= 0xffffffff;
#endif
    return result;
  } else {
    errno = ESPIPE;
    return -1;
  }
}

static off64_t __ftello64_unlocked(FILE* fp) {
  // Find offset of underlying I/O object, then adjust for buffered bytes.
  __sflush(fp);  // May adjust seek offset on append stream.
  off64_t result = __seek_unlocked(fp, 0, SEEK_CUR);
  if (result == -1) {
    return -1;
  }

  if (fp->_flags & __SRD) {
    // Reading.  Any unread characters (including
    // those from ungetc) cause the position to be
    // smaller than that in the underlying object.
    result -= fp->_r;
    if (HASUB(fp)) result -= fp->_ur;
  } else if (fp->_flags & __SWR && fp->_p != NULL) {
    // Writing.  Any buffered characters cause the
    // position to be greater than that in the
    // underlying object.
    result += fp->_p - fp->_bf._base;
  }
  return result;
}

int __fseeko64(FILE* fp, off64_t offset, int whence, int off_t_bits) {
  ScopedFileLock sfl(fp);

  // Change any SEEK_CUR to SEEK_SET, and check `whence` argument.
  // After this, whence is either SEEK_SET or SEEK_END.
  if (whence == SEEK_CUR) {
    fpos64_t current_offset = __ftello64_unlocked(fp);
    if (current_offset == -1) {
      return -1;
    }
    offset += current_offset;
    whence = SEEK_SET;
  } else if (whence != SEEK_SET && whence != SEEK_END) {
    errno = EINVAL;
    return -1;
  }

  // If our caller has a 32-bit interface, refuse to go past a 32-bit file offset.
  if (off_t_bits == 32 && offset > LONG_MAX) {
    errno = EOVERFLOW;
    return -1;
  }

  if (fp->_bf._base == NULL) __smakebuf(fp);

  // Flush unwritten data and attempt the seek.
  if (__sflush(fp) || __seek_unlocked(fp, offset, whence) == -1) {
    return -1;
  }

  // Success: clear EOF indicator and discard ungetc() data.
  if (HASUB(fp)) FREEUB(fp);
  fp->_p = fp->_bf._base;
  fp->_r = 0;
  /* fp->_w = 0; */	/* unnecessary (I think...) */
  fp->_flags &= ~__SEOF;
  return 0;
}

int fseeko(FILE* fp, off_t offset, int whence) {
  static_assert(sizeof(off_t) == sizeof(long), "sizeof(off_t) != sizeof(long)");
  return __fseeko64(fp, offset, whence, 8*sizeof(off_t));
}
__strong_alias(fseek, fseeko);

int fseeko64(FILE* fp, off64_t offset, int whence) {
  return __fseeko64(fp, offset, whence, 8*sizeof(off_t));
}

int fsetpos(FILE* fp, const fpos_t* pos) {
  return fseeko(fp, *pos, SEEK_SET);
}

int fsetpos64(FILE* fp, const fpos64_t* pos) {
  return fseeko64(fp, *pos, SEEK_SET);
}

off_t ftello(FILE* fp) {
  static_assert(sizeof(off_t) == sizeof(long), "sizeof(off_t) != sizeof(long)");
  off64_t result = ftello64(fp);
  if (result > LONG_MAX) {
    errno = EOVERFLOW;
    return -1;
  }
  return result;
}
__strong_alias(ftell, ftello);

off64_t ftello64(FILE* fp) {
  ScopedFileLock sfl(fp);
  return __ftello64_unlocked(fp);
}

int fgetpos(FILE* fp, fpos_t* pos) {
  *pos = ftello(fp);
  return (*pos == -1) ? -1 : 0;
}

int fgetpos64(FILE* fp, fpos64_t* pos) {
  *pos = ftello64(fp);
  return (*pos == -1) ? -1 : 0;
}

static FILE* __funopen(const void* cookie,
                       int (*read_fn)(void*, char*, int),
                       int (*write_fn)(void*, const char*, int),
                       int (*close_fn)(void*)) {
  if (read_fn == nullptr && write_fn == nullptr) {
    errno = EINVAL;
    return nullptr;
  }

  FILE* fp = __sfp();
  if (fp == nullptr) return nullptr;

  if (read_fn != nullptr && write_fn != nullptr) {
    fp->_flags = __SRW;
  } else if (read_fn != nullptr) {
    fp->_flags = __SRD;
  } else if (write_fn != nullptr) {
    fp->_flags = __SWR;
  }

  fp->_file = -1;
  fp->_cookie = const_cast<void*>(cookie); // The funopen(3) API is incoherent.
  fp->_read = read_fn;
  fp->_write = write_fn;
  fp->_close = close_fn;

  return fp;
}

FILE* funopen(const void* cookie,
              int (*read_fn)(void*, char*, int),
              int (*write_fn)(void*, const char*, int),
              fpos_t (*seek_fn)(void*, fpos_t, int),
              int (*close_fn)(void*)) {
  FILE* fp = __funopen(cookie, read_fn, write_fn, close_fn);
  if (fp != nullptr) {
    fp->_seek = seek_fn;
  }
  return fp;
}

FILE* funopen64(const void* cookie,
                int (*read_fn)(void*, char*, int),
                int (*write_fn)(void*, const char*, int),
                fpos64_t (*seek_fn)(void*, fpos64_t, int),
                int (*close_fn)(void*)) {
  FILE* fp = __funopen(cookie, read_fn, write_fn, close_fn);
  if (fp != nullptr) {
    _EXT(fp)->_seek64 = seek_fn;
  }
  return fp;
}

int asprintf(char** s, const char* fmt, ...) {
  PRINTF_IMPL(vasprintf(s, fmt, ap));
}

char* ctermid(char* s) {
  return s ? strcpy(s, _PATH_TTY) : const_cast<char*>(_PATH_TTY);
}

int dprintf(int fd, const char* fmt, ...) {
  PRINTF_IMPL(vdprintf(fd, fmt, ap));
}

int fprintf(FILE* fp, const char* fmt, ...) {
  PRINTF_IMPL(vfprintf(fp, fmt, ap));
}

int fgetc(FILE* fp) {
  return getc(fp);
}

int fputc(int c, FILE* fp) {
  return putc(c, fp);
}

int fscanf(FILE* fp, const char* fmt, ...) {
  PRINTF_IMPL(vfscanf(fp, fmt, ap));
}

int fwprintf(FILE* fp, const wchar_t* fmt, ...) {
  PRINTF_IMPL(vfwprintf(fp, fmt, ap));
}

int fwscanf(FILE* fp, const wchar_t* fmt, ...) {
  PRINTF_IMPL(vfwscanf(fp, fmt, ap));
}

int getc(FILE* fp) {
  ScopedFileLock sfl(fp);
  return getc_unlocked(fp);
}

int getc_unlocked(FILE* fp) {
  return __sgetc(fp);
}

int getchar_unlocked() {
  return getc_unlocked(stdin);
}

int getchar() {
  return getc(stdin);
}

ssize_t getline(char** buf, size_t* len, FILE* fp) {
  return getdelim(buf, len, '\n', fp);
}

wint_t getwc(FILE* fp) {
  return fgetwc(fp);
}

wint_t getwchar() {
  return fgetwc(stdin);
}

int printf(const char* fmt, ...) {
  PRINTF_IMPL(vfprintf(stdout, fmt, ap));
}

int putc(int c, FILE* fp) {
  ScopedFileLock sfl(fp);
  return putc_unlocked(c, fp);
}

int putc_unlocked(int c, FILE* fp) {
  if (cantwrite(fp)) {
    errno = EBADF;
    return EOF;
  }
  _SET_ORIENTATION(fp, -1);
  if (--fp->_w >= 0 || (fp->_w >= fp->_lbfsize && c != '\n')) {
    return (*fp->_p++ = c);
  }
  return (__swbuf(c, fp));
}

int putchar(int c) {
  return putc(c, stdout);
}

int putchar_unlocked(int c) {
  return putc_unlocked(c, stdout);
}

wint_t putwc(wchar_t wc, FILE* fp) {
  return fputwc(wc, fp);
}

wint_t putwchar(wchar_t wc) {
  return fputwc(wc, stdout);
}

int remove(const char* path) {
  if (unlink(path) != -1) return 0;
  if (errno != EISDIR) return -1;
  return rmdir(path);
}

void rewind(FILE* fp) {
  ScopedFileLock sfl(fp);
  fseek(fp, 0, SEEK_SET);
  clearerr_unlocked(fp);
}

int scanf(const char* fmt, ...) {
  PRINTF_IMPL(vfscanf(stdin, fmt, ap));
}

void setbuf(FILE* fp, char* buf) {
  setbuffer(fp, buf, BUFSIZ);
}

void setbuffer(FILE* fp, char* buf, int size) {
  setvbuf(fp, buf, buf ? _IOFBF : _IONBF, size);
}

int setlinebuf(FILE* fp) {
  return setvbuf(fp, nullptr, _IOLBF, 0);
}

int snprintf(char* s, size_t n, const char* fmt, ...) {
  PRINTF_IMPL(vsnprintf(s, n, fmt, ap));
}

int sprintf(char* s, const char* fmt, ...) {
  PRINTF_IMPL(vsprintf(s, fmt, ap));
}

int sscanf(const char* s, const char* fmt, ...) {
  PRINTF_IMPL(vsscanf(s, fmt, ap));
}

int swprintf(wchar_t* s, size_t n, const wchar_t* fmt, ...) {
  PRINTF_IMPL(vswprintf(s, n, fmt, ap));
}

int swscanf(const wchar_t* s, const wchar_t* fmt, ...) {
  PRINTF_IMPL(vswscanf(s, fmt, ap));
}

int vprintf(const char* fmt, va_list ap) {
  return vfprintf(stdout, fmt, ap);
}

int vscanf(const char* fmt, va_list ap) {
  return vfscanf(stdin, fmt, ap);
}

int vsnprintf(char* s, size_t n, const char* fmt, va_list ap) {
  // stdio internals use int rather than size_t.
  static_assert(INT_MAX <= SSIZE_MAX, "SSIZE_MAX too large to fit in int");

  __check_count("vsnprintf", "size", n);

  // Stdio internals do not deal correctly with zero length buffer.
  char dummy;
  if (n == 0) {
    s = &dummy;
    n = 1;
  }

  FILE f;
  __sfileext fext;
  _FILEEXT_SETUP(&f, &fext);
  f._file = -1;
  f._flags = __SWR | __SSTR;
  f._bf._base = f._p = reinterpret_cast<unsigned char*>(s);
  f._bf._size = f._w = n - 1;

  int result = __vfprintf(&f, fmt, ap);
  *f._p = '\0';
  return result;
}

int vsprintf(char* s, const char* fmt, va_list ap) {
  return vsnprintf(s, SSIZE_MAX, fmt, ap);
}

int vwprintf(const wchar_t* fmt, va_list ap) {
  return vfwprintf(stdout, fmt, ap);
}

int vwscanf(const wchar_t* fmt, va_list ap) {
  return vfwscanf(stdin, fmt, ap);
}

int wprintf(const wchar_t* fmt, ...) {
  PRINTF_IMPL(vfwprintf(stdout, fmt, ap));
}

int wscanf(const wchar_t* fmt, ...) {
  PRINTF_IMPL(vfwscanf(stdin, fmt, ap));
}

namespace {

namespace phony {
#include <bits/struct_file.h>
}

static_assert(sizeof(::__sFILE) == sizeof(phony::__sFILE),
              "size mismatch between `struct __sFILE` implementation and public stub");
static_assert(alignof(::__sFILE) == alignof(phony::__sFILE),
              "alignment mismatch between `struct __sFILE` implementation and public stub");

}
