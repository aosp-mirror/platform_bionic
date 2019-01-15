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
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <android/fdsan.h>

#include <async_safe/log.h>

#include "local.h"
#include "glue.h"
#include "private/bionic_fortify.h"
#include "private/ErrnoRestorer.h"
#include "private/thread_private.h"

extern "C" int ___close(int fd);

#define ALIGNBYTES (sizeof(uintptr_t) - 1)
#define ALIGN(p) (((uintptr_t)(p) + ALIGNBYTES) &~ ALIGNBYTES)

#define	NDYNAMIC 10		/* add ten more whenever necessary */

#define PRINTF_IMPL(expr) \
    va_list ap; \
    va_start(ap, fmt); \
    int result = (expr); \
    va_end(ap); \
    return result;

#define MAKE_STD_STREAM(flags, fd)                                          \
  {                                                                         \
    ._flags = flags, ._file = fd, ._cookie = __sF + fd, ._close = __sclose, \
    ._read = __sread, ._write = __swrite, ._ext = {                         \
      ._base = reinterpret_cast<uint8_t*>(__sFext + fd)                     \
    }                                                                       \
  }

static struct __sfileext __sFext[3] = {
    {._lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
     ._caller_handles_locking = false,
     ._seek64 = __sseek64,
     ._popen_pid = 0},
    {._lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
     ._caller_handles_locking = false,
     ._seek64 = __sseek64,
     ._popen_pid = 0},
    {._lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
     ._caller_handles_locking = false,
     ._seek64 = __sseek64,
     ._popen_pid = 0},
};

// __sF is exported for backwards compatibility. Until M, we didn't have symbols
// for stdin/stdout/stderr; they were macros accessing __sF.
FILE __sF[3] = {
    MAKE_STD_STREAM(__SRD, STDIN_FILENO),
    MAKE_STD_STREAM(__SWR, STDOUT_FILENO),
    MAKE_STD_STREAM(__SWR|__SNBF, STDERR_FILENO),
};

FILE* stdin = &__sF[0];
FILE* stdout = &__sF[1];
FILE* stderr = &__sF[2];

static pthread_mutex_t __stdio_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

static uint64_t __get_file_tag(FILE* fp) {
  // Don't use a tag for the standard streams.
  // They don't really own their file descriptors, because the values are well-known, and you're
  // allowed to do things like `close(STDIN_FILENO); open("foo", O_RDONLY)` when single-threaded.
  if (fp == stdin || fp == stderr || fp == stdout) {
    return 0;
  }

  return android_fdsan_create_owner_tag(ANDROID_FDSAN_OWNER_TYPE_FILE,
                                        reinterpret_cast<uint64_t>(fp));
}

struct glue __sglue = { nullptr, 3, __sF };
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
  char* data = new char[sizeof(glue) + ALIGNBYTES + n * sizeof(FILE) + n * sizeof(__sfileext)];
  if (data == nullptr) return nullptr;

  glue* g = reinterpret_cast<glue*>(data);
  FILE* p = reinterpret_cast<FILE*>(ALIGN(data + sizeof(*g)));
  __sfileext* pext = reinterpret_cast<__sfileext*>(ALIGN(data + sizeof(*g)) + n * sizeof(FILE));
  g->next = nullptr;
  g->niobs = n;
  g->iobs = p;
  while (--n >= 0) {
    *p = {};
    _FILEEXT_SETUP(p, pext);
    p++;
    pext++;
  }
  return g;
}

static inline void free_fgetln_buffer(FILE* fp) {
  if (__predict_false(fp->_lb._base != nullptr)) {
    free(fp->_lb._base);
    fp->_lb._base = nullptr;
  }
}

/*
 * Find a free FILE for fopen et al.
 */
FILE* __sfp(void) {
	FILE *fp;
	int n;
	struct glue *g;

	pthread_mutex_lock(&__stdio_mutex);
	for (g = &__sglue; g != nullptr; g = g->next) {
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags == 0)
				goto found;
	}

	/* release lock while mallocing */
	pthread_mutex_unlock(&__stdio_mutex);
	if ((g = moreglue(NDYNAMIC)) == nullptr) return nullptr;
	pthread_mutex_lock(&__stdio_mutex);
	lastglue->next = g;
	lastglue = g;
	fp = g->iobs;
found:
	fp->_flags = 1;		/* reserve this slot; caller sets real flags */
	pthread_mutex_unlock(&__stdio_mutex);
	fp->_p = nullptr;		/* no current pointer */
	fp->_w = 0;		/* nothing to read or write */
	fp->_r = 0;
	fp->_bf._base = nullptr;	/* no buffer */
	fp->_bf._size = 0;
	fp->_lbfsize = 0;	/* not line buffered */
	fp->_file = -1;		/* no file */

	fp->_lb._base = nullptr;	/* no line buffer */
	fp->_lb._size = 0;

	memset(_EXT(fp), 0, sizeof(struct __sfileext));
	_FLOCK(fp) = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	_EXT(fp)->_caller_handles_locking = false;

	// Caller sets cookie, _read/_write etc.
	// We explicitly clear _seek and _seek64 to prevent subtle bugs.
	fp->_seek = nullptr;
	_EXT(fp)->_seek64 = nullptr;

	return fp;
}

int _fwalk(int (*callback)(FILE*)) {
  pthread_mutex_lock(&__stdio_mutex);
  int result = 0;
  for (glue* g = &__sglue; g != nullptr; g = g->next) {
    FILE* fp = g->iobs;
    for (int n = g->niobs; --n >= 0; ++fp) {
      ScopedFileLock sfl(fp);
      if (fp->_flags != 0 && (fp->_flags & __SIGN) == 0) {
        result |= (*callback)(fp);
      }
    }
  }
  pthread_mutex_unlock(&__stdio_mutex);
  return result;
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
    android_fdsan_exchange_owner_tag(fd, 0, __get_file_tag(fp));
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
  int mode_flags;
  int flags = __sflags(mode, &mode_flags);
  if (flags == 0) return nullptr;

  int fd = open(file, mode_flags, DEFFILEMODE);
  if (fd == -1) {
    return nullptr;
  }

  FILE* fp = __fopen(fd, flags);
  if (fp == nullptr) {
    ErrnoRestorer errno_restorer;
    close(fd);
    return nullptr;
  }

  // For append mode, even though we use O_APPEND, we need to seek to the end now.
  if ((mode_flags & O_APPEND) != 0) __sseek64(fp, 0, SEEK_END);
  return fp;
}
__strong_alias(fopen64, fopen);

FILE* fdopen(int fd, const char* mode) {
  int mode_flags;
  int flags = __sflags(mode, &mode_flags);
  if (flags == 0) return nullptr;

  // Make sure the mode the user wants is a subset of the actual mode.
  int fd_flags = fcntl(fd, F_GETFL, 0);
  if (fd_flags == -1) return nullptr;
  int tmp = fd_flags & O_ACCMODE;
  if (tmp != O_RDWR && (tmp != (mode_flags & O_ACCMODE))) {
    errno = EINVAL;
    return nullptr;
  }

  // Make sure O_APPEND is set on the underlying fd if our mode has 'a'.
  // POSIX says we just take the current offset of the underlying fd.
  if ((mode_flags & O_APPEND) && !(fd_flags & O_APPEND)) {
    if (fcntl(fd, F_SETFL, fd_flags | O_APPEND) == -1) return nullptr;
  }

  // Make sure O_CLOEXEC is set on the underlying fd if our mode has 'x'.
  if ((mode_flags & O_CLOEXEC) && !((tmp = fcntl(fd, F_GETFD)) & FD_CLOEXEC)) {
    fcntl(fd, F_SETFD, tmp | FD_CLOEXEC);
  }

  return __fopen(fd, flags);
}

// Re-direct an existing, open (probably) file to some other file.
// ANSI is written such that the original file gets closed if at
// all possible, no matter what.
// TODO: rewrite this mess completely.
FILE* freopen(const char* file, const char* mode, FILE* fp) {
  CHECK_FP(fp);
  int mode_flags;
  int flags = __sflags(mode, &mode_flags);
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

    // If close is null, closing is a no-op, hence pointless.
    isopen = (fp->_close != nullptr);
    if ((wantfd = fp->_file) < 0 && isopen) {
        (*fp->_close)(fp->_cookie);
        isopen = 0;
    }
  }

  // Get a new descriptor to refer to the new file.
  int fd = open(file, mode_flags, DEFFILEMODE);
  if (fd < 0 && isopen) {
    // If out of fd's close the old one and try again.
    if (errno == ENFILE || errno == EMFILE) {
      (*fp->_close)(fp->_cookie);
      isopen = 0;
      fd = open(file, mode_flags, DEFFILEMODE);
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
  fp->_p = nullptr;
  fp->_bf._base = nullptr;
  fp->_bf._size = 0;
  fp->_lbfsize = 0;
  if (HASUB(fp)) FREEUB(fp);
  _UB(fp)._size = 0;
  WCIO_FREE(fp);
  free_fgetln_buffer(fp);
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
    if (dup3(fd, wantfd, mode_flags & O_CLOEXEC) >= 0) {
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
  android_fdsan_exchange_owner_tag(fd, 0, __get_file_tag(fp));
  fp->_cookie = fp;
  fp->_read = __sread;
  fp->_write = __swrite;
  fp->_close = __sclose;
  _EXT(fp)->_seek64 = __sseek64;

  // For append mode, even though we use O_APPEND, we need to seek to the end now.
  if ((mode_flags & O_APPEND) != 0) __sseek64(fp, 0, SEEK_END);
  return fp;
}
__strong_alias(freopen64, freopen);

static int __FILE_close(FILE* fp) {
  if (fp->_flags == 0) {
    // Already freed!
    errno = EBADF;
    return EOF;
  }

  ScopedFileLock sfl(fp);
  WCIO_FREE(fp);
  int r = fp->_flags & __SWR ? __sflush(fp) : 0;
  if (fp->_close != nullptr && (*fp->_close)(fp->_cookie) < 0) {
    r = EOF;
  }
  if (fp->_flags & __SMBF) free(fp->_bf._base);
  if (HASUB(fp)) FREEUB(fp);
  free_fgetln_buffer(fp);

  // If we were created by popen(3), wait for the child.
  pid_t pid = _EXT(fp)->_popen_pid;
  if (pid > 0) {
    int status;
    if (TEMP_FAILURE_RETRY(wait4(pid, &status, 0, nullptr)) != -1) {
      r = status;
    }
  }
  _EXT(fp)->_popen_pid = 0;

  // Poison this FILE so accesses after fclose will be obvious.
  fp->_file = -1;
  fp->_r = fp->_w = 0;

  // Release this FILE for reuse.
  fp->_flags = 0;
  return r;
}

int fclose(FILE* fp) {
  CHECK_FP(fp);
  return __FILE_close(fp);
}

int fileno_unlocked(FILE* fp) {
  CHECK_FP(fp);
  int fd = fp->_file;
  if (fd == -1) {
    errno = EBADF;
    return -1;
  }
  return fd;
}

int fileno(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return fileno_unlocked(fp);
}

void clearerr_unlocked(FILE* fp) {
  CHECK_FP(fp);
  return __sclearerr(fp);
}

void clearerr(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  clearerr_unlocked(fp);
}

int feof_unlocked(FILE* fp) {
  CHECK_FP(fp);
  return ((fp->_flags & __SEOF) != 0);
}

int feof(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return feof_unlocked(fp);
}

int ferror_unlocked(FILE* fp) {
  CHECK_FP(fp);
  return __sferror(fp);
}

int ferror(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return ferror_unlocked(fp);
}

int __sflush(FILE* fp) {
  // Flushing a read-only file is a no-op.
  if ((fp->_flags & __SWR) == 0) return 0;

  // Flushing a file without a buffer is a no-op.
  unsigned char* p = fp->_bf._base;
  if (p == nullptr) return 0;

  // Set these immediately to avoid problems with longjmp and to allow
  // exchange buffering (via setvbuf) in user write function.
  int n = fp->_p - p;
  fp->_p = p;
  fp->_w = (fp->_flags & (__SLBF|__SNBF)) ? 0 : fp->_bf._size;

  while (n > 0) {
    int written = (*fp->_write)(fp->_cookie, reinterpret_cast<char*>(p), n);
    if (written <= 0) {
      fp->_flags |= __SERR;
      return EOF;
    }
    n -= written, p += written;
  }
  return 0;
}

int __sread(void* cookie, char* buf, int n) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
  return TEMP_FAILURE_RETRY(read(fp->_file, buf, n));
}

int __swrite(void* cookie, const char* buf, int n) {
  FILE* fp = reinterpret_cast<FILE*>(cookie);
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
  return android_fdsan_close_with_tag(fp->_file, __get_file_tag(fp));
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
  } else if (fp->_flags & __SWR && fp->_p != nullptr) {
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

  if (fp->_bf._base == nullptr) __smakebuf(fp);

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
  CHECK_FP(fp);
  static_assert(sizeof(off_t) == sizeof(long), "sizeof(off_t) != sizeof(long)");
  return __fseeko64(fp, offset, whence, 8*sizeof(off_t));
}
__strong_alias(fseek, fseeko);

int fseeko64(FILE* fp, off64_t offset, int whence) {
  CHECK_FP(fp);
  return __fseeko64(fp, offset, whence, 8*sizeof(off64_t));
}

int fsetpos(FILE* fp, const fpos_t* pos) {
  CHECK_FP(fp);
  return fseeko(fp, *pos, SEEK_SET);
}

int fsetpos64(FILE* fp, const fpos64_t* pos) {
  CHECK_FP(fp);
  return fseeko64(fp, *pos, SEEK_SET);
}

off_t ftello(FILE* fp) {
  CHECK_FP(fp);
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
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return __ftello64_unlocked(fp);
}

int fgetpos(FILE* fp, fpos_t* pos) {
  CHECK_FP(fp);
  *pos = ftello(fp);
  return (*pos == -1) ? -1 : 0;
}

int fgetpos64(FILE* fp, fpos64_t* pos) {
  CHECK_FP(fp);
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
  CHECK_FP(fp);
  PRINTF_IMPL(vfprintf(fp, fmt, ap));
}

int fgetc(FILE* fp) {
  CHECK_FP(fp);
  return getc(fp);
}

int fgetc_unlocked(FILE* fp) {
  CHECK_FP(fp);
  return getc_unlocked(fp);
}

char* fgets(char* buf, int n, FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return fgets_unlocked(buf, n, fp);
}

// Reads at most n-1 characters from the given file.
// Stops when a newline has been read, or the count runs out.
// Returns first argument, or nullptr if no characters were read.
// Does not return nullptr if n == 1.
char* fgets_unlocked(char* buf, int n, FILE* fp) {
  if (n <= 0) {
    errno = EINVAL;
    return nullptr;
  }

  _SET_ORIENTATION(fp, -1);

  char* s = buf;
  n--; // Leave space for NUL.
  while (n != 0) {
    // If the buffer is empty, refill it.
    if (fp->_r <= 0) {
      if (__srefill(fp)) {
        // EOF/error: stop with partial or no line.
        if (s == buf) return nullptr;
        break;
      }
    }
    size_t len = fp->_r;
    unsigned char* p = fp->_p;

    // Scan through at most n bytes of the current buffer,
    // looking for '\n'.  If found, copy up to and including
    // newline, and stop.  Otherwise, copy entire chunk and loop.
    if (len > static_cast<size_t>(n)) len = n;
    unsigned char* t = static_cast<unsigned char*>(memchr(p, '\n', len));
    if (t != nullptr) {
      len = ++t - p;
      fp->_r -= len;
      fp->_p = t;
      memcpy(s, p, len);
      s[len] = '\0';
      return buf;
    }
    fp->_r -= len;
    fp->_p += len;
    memcpy(s, p, len);
    s += len;
    n -= len;
  }
  *s = '\0';
  return buf;
}

int fputc(int c, FILE* fp) {
  CHECK_FP(fp);
  return putc(c, fp);
}

int fputc_unlocked(int c, FILE* fp) {
  CHECK_FP(fp);
  return putc_unlocked(c, fp);
}

int fputs(const char* s, FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return fputs_unlocked(s, fp);
}

int fputs_unlocked(const char* s, FILE* fp) {
  CHECK_FP(fp);
  size_t length = strlen(s);
  return (fwrite_unlocked(s, 1, length, fp) == length) ? 0 : EOF;
}

int fscanf(FILE* fp, const char* fmt, ...) {
  CHECK_FP(fp);
  PRINTF_IMPL(vfscanf(fp, fmt, ap));
}

int fwprintf(FILE* fp, const wchar_t* fmt, ...) {
  CHECK_FP(fp);
  PRINTF_IMPL(vfwprintf(fp, fmt, ap));
}

int fwscanf(FILE* fp, const wchar_t* fmt, ...) {
  CHECK_FP(fp);
  PRINTF_IMPL(vfwscanf(fp, fmt, ap));
}

int getc(FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return getc_unlocked(fp);
}

int getc_unlocked(FILE* fp) {
  CHECK_FP(fp);
  return __sgetc(fp);
}

int getchar_unlocked() {
  return getc_unlocked(stdin);
}

int getchar() {
  return getc(stdin);
}

ssize_t getline(char** buf, size_t* len, FILE* fp) {
  CHECK_FP(fp);
  return getdelim(buf, len, '\n', fp);
}

wint_t getwc(FILE* fp) {
  CHECK_FP(fp);
  return fgetwc(fp);
}

wint_t getwchar() {
  return fgetwc(stdin);
}

void perror(const char* msg) {
  if (msg == nullptr) msg = "";
  fprintf(stderr, "%s%s%s\n", msg, (*msg == '\0') ? "" : ": ", strerror(errno));
}

int printf(const char* fmt, ...) {
  PRINTF_IMPL(vfprintf(stdout, fmt, ap));
}

int putc(int c, FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return putc_unlocked(c, fp);
}

int putc_unlocked(int c, FILE* fp) {
  CHECK_FP(fp);
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

int puts(const char* s) {
  size_t length = strlen(s);
  ScopedFileLock sfl(stdout);
  return (fwrite_unlocked(s, 1, length, stdout) == length &&
          putc_unlocked('\n', stdout) != EOF) ? 0 : EOF;
}

wint_t putwc(wchar_t wc, FILE* fp) {
  CHECK_FP(fp);
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
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  fseek(fp, 0, SEEK_SET);
  clearerr_unlocked(fp);
}

int scanf(const char* fmt, ...) {
  PRINTF_IMPL(vfscanf(stdin, fmt, ap));
}

void setbuf(FILE* fp, char* buf) {
  CHECK_FP(fp);
  setbuffer(fp, buf, BUFSIZ);
}

void setbuffer(FILE* fp, char* buf, int size) {
  CHECK_FP(fp);
  setvbuf(fp, buf, buf ? _IOFBF : _IONBF, size);
}

int setlinebuf(FILE* fp) {
  CHECK_FP(fp);
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

int vfprintf(FILE* fp, const char* fmt, va_list ap) {
  ScopedFileLock sfl(fp);
  return __vfprintf(fp, fmt, ap);
}

int vfscanf(FILE* fp, const char* fmt, va_list ap) {
  ScopedFileLock sfl(fp);
  return __svfscanf(fp, fmt, ap);
}

int vfwprintf(FILE* fp, const wchar_t* fmt, va_list ap) {
  ScopedFileLock sfl(fp);
  return __vfwprintf(fp, fmt, ap);
}

int vfwscanf(FILE* fp, const wchar_t* fmt, va_list ap) {
  ScopedFileLock sfl(fp);
  return __vfwscanf(fp, fmt, ap);
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

static int fflush_all() {
  return _fwalk(__sflush);
}

int fflush(FILE* fp) {
  if (fp == nullptr) return fflush_all();
  ScopedFileLock sfl(fp);
  return fflush_unlocked(fp);
}

int fflush_unlocked(FILE* fp) {
  if (fp == nullptr) return fflush_all();
  if ((fp->_flags & (__SWR | __SRW)) == 0) {
    errno = EBADF;
    return EOF;
  }
  return __sflush(fp);
}

size_t fread(void* buf, size_t size, size_t count, FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return fread_unlocked(buf, size, count, fp);
}

size_t fread_unlocked(void* buf, size_t size, size_t count, FILE* fp) {
  CHECK_FP(fp);

  size_t desired_total;
  if (__builtin_mul_overflow(size, count, &desired_total)) {
    errno = EOVERFLOW;
    fp->_flags |= __SERR;
    return 0;
  }

  size_t total = desired_total;
  if (total == 0) return 0;

  _SET_ORIENTATION(fp, -1);

  // TODO: how can this ever happen?!
  if (fp->_r < 0) fp->_r = 0;

  // Ensure _bf._size is valid.
  if (fp->_bf._base == nullptr) __smakebuf(fp);

  char* dst = static_cast<char*>(buf);

  while (total > 0) {
    // Copy data out of the buffer.
    size_t buffered_bytes = MIN(static_cast<size_t>(fp->_r), total);
    memcpy(dst, fp->_p, buffered_bytes);
    fp->_p += buffered_bytes;
    fp->_r -= buffered_bytes;
    dst += buffered_bytes;
    total -= buffered_bytes;

    // Are we done?
    if (total == 0) goto out;

    // Do we have so much more to read that we should avoid copying it through the buffer?
    if (total > static_cast<size_t>(fp->_bf._size)) break;

    // Less than a buffer to go, so refill the buffer and go around the loop again.
    if (__srefill(fp)) goto out;
  }

  // Read directly into the caller's buffer.
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
  return ((desired_total - total) / size);
}

size_t fwrite(const void* buf, size_t size, size_t count, FILE* fp) {
  CHECK_FP(fp);
  ScopedFileLock sfl(fp);
  return fwrite_unlocked(buf, size, count, fp);
}

size_t fwrite_unlocked(const void* buf, size_t size, size_t count, FILE* fp) {
  CHECK_FP(fp);

  size_t n;
  if (__builtin_mul_overflow(size, count, &n)) {
    errno = EOVERFLOW;
    fp->_flags |= __SERR;
    return 0;
  }

  if (n == 0) return 0;

  __siov iov = { .iov_base = const_cast<void*>(buf), .iov_len = n };
  __suio uio = { .uio_iov = &iov, .uio_iovcnt = 1, .uio_resid = n };

  _SET_ORIENTATION(fp, -1);

  // The usual case is success (__sfvwrite returns 0); skip the divide if this happens,
  // since divides are generally slow.
  return (__sfvwrite(fp, &uio) == 0) ? count : ((n - uio.uio_resid) / size);
}

static int __close_if_popened(FILE* fp) {
  if (_EXT(fp)->_popen_pid > 0) close(fileno(fp));
  return 0;
}

static FILE* __popen_fail(int fds[2]) {
  ErrnoRestorer errno_restorer;
  close(fds[0]);
  close(fds[1]);
  return nullptr;
}

FILE* popen(const char* cmd, const char* mode) {
  // Was the request for a socketpair or just a pipe?
  int fds[2];
  bool bidirectional = false;
  if (strchr(mode, '+') != nullptr) {
    if (socketpair(AF_LOCAL, SOCK_CLOEXEC | SOCK_STREAM, 0, fds) == -1) return nullptr;
    bidirectional = true;
    mode = "r+";
  } else {
    if (pipe2(fds, O_CLOEXEC) == -1) return nullptr;
    mode = strrchr(mode, 'r') ? "r" : "w";
  }

  // If the parent wants to read, the child's fd needs to be stdout.
  int parent, child, desired_child_fd;
  if (*mode == 'r') {
    parent = 0;
    child = 1;
    desired_child_fd = STDOUT_FILENO;
  } else {
    parent = 1;
    child = 0;
    desired_child_fd = STDIN_FILENO;
  }

  // Ensure that the child fd isn't the desired child fd.
  if (fds[child] == desired_child_fd) {
    int new_fd = fcntl(fds[child], F_DUPFD_CLOEXEC, 0);
    if (new_fd == -1) return __popen_fail(fds);
    close(fds[child]);
    fds[child] = new_fd;
  }

  pid_t pid = vfork();
  if (pid == -1) return __popen_fail(fds);

  if (pid == 0) {
    close(fds[parent]);
    // POSIX says "The popen() function shall ensure that any streams from previous popen() calls
    // that remain open in the parent process are closed in the new child process."
    _fwalk(__close_if_popened);
    // dup2 so that the child fd isn't closed on exec.
    if (dup2(fds[child], desired_child_fd) == -1) _exit(127);
    close(fds[child]);
    if (bidirectional) dup2(STDOUT_FILENO, STDIN_FILENO);
    execl(_PATH_BSHELL, "sh", "-c", cmd, nullptr);
    _exit(127);
  }

  FILE* fp = fdopen(fds[parent], mode);
  if (fp == nullptr) return __popen_fail(fds);

  close(fds[child]);

  _EXT(fp)->_popen_pid = pid;
  return fp;
}

int pclose(FILE* fp) {
  CHECK_FP(fp);
  return __FILE_close(fp);
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
