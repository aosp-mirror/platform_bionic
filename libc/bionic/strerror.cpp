/*
 * Copyright (C) 2012 The Android Open Source Project
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

// G++ automatically defines _GNU_SOURCE, which then means that <string.h>
// gives us the GNU variant.
#undef _GNU_SOURCE

#include <string.h>

#include <errno.h>
#include <limits.h>

#include <async_safe/log.h>

#include "private/ErrnoRestorer.h"

#include <string.h>

#include "bionic/pthread_internal.h"

#define ERRNO_STRING_MAP_ENTRY(errno, string) [errno] = string " (" #errno ")"

static const char* __sys_error_strings[] = {
    [0] = "Success",
    ERRNO_STRING_MAP_ENTRY(EPERM, "Operation not permitted"),
    ERRNO_STRING_MAP_ENTRY(ENOENT, "No such file or directory"),
    ERRNO_STRING_MAP_ENTRY(ESRCH, "No such process"),
    ERRNO_STRING_MAP_ENTRY(EINTR, "Interrupted system call"),
    ERRNO_STRING_MAP_ENTRY(EIO, "I/O error"),
    ERRNO_STRING_MAP_ENTRY(ENXIO, "No such device or address"),
    ERRNO_STRING_MAP_ENTRY(E2BIG, "Argument list too long"),
    ERRNO_STRING_MAP_ENTRY(ENOEXEC, "Exec format error"),
    ERRNO_STRING_MAP_ENTRY(EBADF, "Bad file descriptor"),
    ERRNO_STRING_MAP_ENTRY(ECHILD, "No child processes"),
    ERRNO_STRING_MAP_ENTRY(EAGAIN, "Try again"),
    ERRNO_STRING_MAP_ENTRY(ENOMEM, "Out of memory"),
    ERRNO_STRING_MAP_ENTRY(EACCES, "Permission denied"),
    ERRNO_STRING_MAP_ENTRY(EFAULT, "Bad address"),
    ERRNO_STRING_MAP_ENTRY(ENOTBLK, "Block device required"),
    ERRNO_STRING_MAP_ENTRY(EBUSY, "Device or resource busy"),
    ERRNO_STRING_MAP_ENTRY(EEXIST, "File exists"),
    ERRNO_STRING_MAP_ENTRY(EXDEV, "Cross-device link"),
    ERRNO_STRING_MAP_ENTRY(ENODEV, "No such device"),
    ERRNO_STRING_MAP_ENTRY(ENOTDIR, "Not a directory"),
    ERRNO_STRING_MAP_ENTRY(EISDIR, "Is a directory"),
    ERRNO_STRING_MAP_ENTRY(EINVAL, "Invalid argument"),
    ERRNO_STRING_MAP_ENTRY(ENFILE, "File table overflow"),
    ERRNO_STRING_MAP_ENTRY(EMFILE, "Too many open files"),
    ERRNO_STRING_MAP_ENTRY(ENOTTY, "Inappropriate ioctl for device"),
    ERRNO_STRING_MAP_ENTRY(ETXTBSY, "Text file busy"),
    ERRNO_STRING_MAP_ENTRY(EFBIG, "File too large"),
    ERRNO_STRING_MAP_ENTRY(ENOSPC, "No space left on device"),
    ERRNO_STRING_MAP_ENTRY(ESPIPE, "Illegal seek"),
    ERRNO_STRING_MAP_ENTRY(EROFS, "Read-only file system"),
    ERRNO_STRING_MAP_ENTRY(EMLINK, "Too many links"),
    ERRNO_STRING_MAP_ENTRY(EPIPE, "Broken pipe"),
    ERRNO_STRING_MAP_ENTRY(EDOM, "Math argument out of domain of func"),
    ERRNO_STRING_MAP_ENTRY(ERANGE, "Math result not representable"),
    ERRNO_STRING_MAP_ENTRY(EDEADLK, "Resource deadlock would occur"),
    ERRNO_STRING_MAP_ENTRY(ENAMETOOLONG, "File name too long"),
    ERRNO_STRING_MAP_ENTRY(ENOLCK, "No record locks available"),
    ERRNO_STRING_MAP_ENTRY(ENOSYS, "Function not implemented"),
    ERRNO_STRING_MAP_ENTRY(ENOTEMPTY, "Directory not empty"),
    ERRNO_STRING_MAP_ENTRY(ELOOP, "Too many symbolic links encountered"),
    ERRNO_STRING_MAP_ENTRY(ENOMSG, "No message of desired type"),
    ERRNO_STRING_MAP_ENTRY(EIDRM, "Identifier removed"),
    ERRNO_STRING_MAP_ENTRY(ECHRNG, "Channel number out of range"),
    ERRNO_STRING_MAP_ENTRY(EL2NSYNC, "Level 2 not synchronized"),
    ERRNO_STRING_MAP_ENTRY(EL3HLT, "Level 3 halted"),
    ERRNO_STRING_MAP_ENTRY(EL3RST, "Level 3 reset"),
    ERRNO_STRING_MAP_ENTRY(ELNRNG, "Link number out of range"),
    ERRNO_STRING_MAP_ENTRY(EUNATCH, "Protocol driver not attached"),
    ERRNO_STRING_MAP_ENTRY(ENOCSI, "No CSI structure available"),
    ERRNO_STRING_MAP_ENTRY(EL2HLT, "Level 2 halted"),
    ERRNO_STRING_MAP_ENTRY(EBADE, "Invalid exchange"),
    ERRNO_STRING_MAP_ENTRY(EBADR, "Invalid request descriptor"),
    ERRNO_STRING_MAP_ENTRY(EXFULL, "Exchange full"),
    ERRNO_STRING_MAP_ENTRY(ENOANO, "No anode"),
    ERRNO_STRING_MAP_ENTRY(EBADRQC, "Invalid request code"),
    ERRNO_STRING_MAP_ENTRY(EBADSLT, "Invalid slot"),
    ERRNO_STRING_MAP_ENTRY(EBFONT, "Bad font file format"),
    ERRNO_STRING_MAP_ENTRY(ENOSTR, "Device not a stream"),
    ERRNO_STRING_MAP_ENTRY(ENODATA, "No data available"),
    ERRNO_STRING_MAP_ENTRY(ETIME, "Timer expired"),
    ERRNO_STRING_MAP_ENTRY(ENOSR, "Out of streams resources"),
    ERRNO_STRING_MAP_ENTRY(ENONET, "Machine is not on the network"),
    ERRNO_STRING_MAP_ENTRY(ENOPKG, "Package not installed"),
    ERRNO_STRING_MAP_ENTRY(EREMOTE, "Object is remote"),
    ERRNO_STRING_MAP_ENTRY(ENOLINK, "Link has been severed"),
    ERRNO_STRING_MAP_ENTRY(EADV, "Advertise error"),
    ERRNO_STRING_MAP_ENTRY(ESRMNT, "Srmount error"),
    ERRNO_STRING_MAP_ENTRY(ECOMM, "Communication error on send"),
    ERRNO_STRING_MAP_ENTRY(EPROTO, "Protocol error"),
    ERRNO_STRING_MAP_ENTRY(EMULTIHOP, "Multihop attempted"),
    ERRNO_STRING_MAP_ENTRY(EDOTDOT, "RFS specific error"),
    ERRNO_STRING_MAP_ENTRY(EBADMSG, "Not a data message"),
    ERRNO_STRING_MAP_ENTRY(EOVERFLOW, "Value too large for defined data type"),
    ERRNO_STRING_MAP_ENTRY(ENOTUNIQ, "Name not unique on network"),
    ERRNO_STRING_MAP_ENTRY(EBADFD, "File descriptor in bad state"),
    ERRNO_STRING_MAP_ENTRY(EREMCHG, "Remote address changed"),
    ERRNO_STRING_MAP_ENTRY(ELIBACC, "Can not access a needed shared library"),
    ERRNO_STRING_MAP_ENTRY(ELIBBAD, "Accessing a corrupted shared library"),
    ERRNO_STRING_MAP_ENTRY(ELIBSCN, ".lib section in a.out corrupted"),
    ERRNO_STRING_MAP_ENTRY(ELIBMAX, "Attempting to link in too many shared libraries"),
    ERRNO_STRING_MAP_ENTRY(ELIBEXEC, "Cannot exec a shared library directly"),
    ERRNO_STRING_MAP_ENTRY(EILSEQ, "Illegal byte sequence"),
    ERRNO_STRING_MAP_ENTRY(ERESTART, "Interrupted system call should be restarted"),
    ERRNO_STRING_MAP_ENTRY(ESTRPIPE, "Streams pipe error"),
    ERRNO_STRING_MAP_ENTRY(EUSERS, "Too many users"),
    ERRNO_STRING_MAP_ENTRY(ENOTSOCK, "Socket operation on non-socket"),
    ERRNO_STRING_MAP_ENTRY(EDESTADDRREQ, "Destination address required"),
    ERRNO_STRING_MAP_ENTRY(EMSGSIZE, "Message too long"),
    ERRNO_STRING_MAP_ENTRY(EPROTOTYPE, "Protocol wrong type for socket"),
    ERRNO_STRING_MAP_ENTRY(ENOPROTOOPT, "Protocol not available"),
    ERRNO_STRING_MAP_ENTRY(EPROTONOSUPPORT, "Protocol not supported"),
    ERRNO_STRING_MAP_ENTRY(ESOCKTNOSUPPORT, "Socket type not supported"),
    ERRNO_STRING_MAP_ENTRY(EOPNOTSUPP, "Operation not supported on transport endpoint"),
    ERRNO_STRING_MAP_ENTRY(EPFNOSUPPORT, "Protocol family not supported"),
    ERRNO_STRING_MAP_ENTRY(EAFNOSUPPORT, "Address family not supported by protocol"),
    ERRNO_STRING_MAP_ENTRY(EADDRINUSE, "Address already in use"),
    ERRNO_STRING_MAP_ENTRY(EADDRNOTAVAIL, "Cannot assign requested address"),
    ERRNO_STRING_MAP_ENTRY(ENETDOWN, "Network is down"),
    ERRNO_STRING_MAP_ENTRY(ENETUNREACH, "Network is unreachable"),
    ERRNO_STRING_MAP_ENTRY(ENETRESET, "Network dropped connection because of reset"),
    ERRNO_STRING_MAP_ENTRY(ECONNABORTED, "Software caused connection abort"),
    ERRNO_STRING_MAP_ENTRY(ECONNRESET, "Connection reset by peer"),
    ERRNO_STRING_MAP_ENTRY(ENOBUFS, "No buffer space available"),
    ERRNO_STRING_MAP_ENTRY(EISCONN, "Transport endpoint is already connected"),
    ERRNO_STRING_MAP_ENTRY(ENOTCONN, "Transport endpoint is not connected"),
    ERRNO_STRING_MAP_ENTRY(ESHUTDOWN, "Cannot send after transport endpoint shutdown"),
    ERRNO_STRING_MAP_ENTRY(ETOOMANYREFS, "Too many references: cannot splice"),
    ERRNO_STRING_MAP_ENTRY(ETIMEDOUT, "Connection timed out"),
    ERRNO_STRING_MAP_ENTRY(ECONNREFUSED, "Connection refused"),
    ERRNO_STRING_MAP_ENTRY(EHOSTDOWN, "Host is down"),
    ERRNO_STRING_MAP_ENTRY(EHOSTUNREACH, "No route to host"),
    ERRNO_STRING_MAP_ENTRY(EALREADY, "Operation already in progress"),
    ERRNO_STRING_MAP_ENTRY(EINPROGRESS, "Operation now in progress"),
    ERRNO_STRING_MAP_ENTRY(ESTALE, "Stale NFS file handle"),
    ERRNO_STRING_MAP_ENTRY(EUCLEAN, "Structure needs cleaning"),
    ERRNO_STRING_MAP_ENTRY(ENOTNAM, "Not a XENIX named type file"),
    ERRNO_STRING_MAP_ENTRY(ENAVAIL, "No XENIX semaphores available"),
    ERRNO_STRING_MAP_ENTRY(EISNAM, "Is a named type file"),
    ERRNO_STRING_MAP_ENTRY(EREMOTEIO, "Remote I/O error"),
    ERRNO_STRING_MAP_ENTRY(EDQUOT, "Quota exceeded"),
    ERRNO_STRING_MAP_ENTRY(ENOMEDIUM, "No medium found"),
    ERRNO_STRING_MAP_ENTRY(EMEDIUMTYPE, "Wrong medium type"),
    ERRNO_STRING_MAP_ENTRY(ECANCELED, "Operation Canceled"),
    ERRNO_STRING_MAP_ENTRY(ENOKEY, "Required key not available"),
    ERRNO_STRING_MAP_ENTRY(EKEYEXPIRED, "Key has expired"),
    ERRNO_STRING_MAP_ENTRY(EKEYREVOKED, "Key has been revoked"),
    ERRNO_STRING_MAP_ENTRY(EKEYREJECTED, "Key was rejected by service"),
    ERRNO_STRING_MAP_ENTRY(EOWNERDEAD, "Owner died"),
    ERRNO_STRING_MAP_ENTRY(ENOTRECOVERABLE, "State not recoverable"),
    ERRNO_STRING_MAP_ENTRY(ERFKILL, "Operation not possible due to RF-kill"),
    ERRNO_STRING_MAP_ENTRY(EHWPOISON, "Memory page has hardware error"),
};

static inline const char* __strerror_lookup(int error_number) {
  if (error_number < 0 || error_number >= static_cast<int>(arraysize(__sys_error_strings))) {
    return nullptr;
  }
  return __sys_error_strings[error_number];
}

int strerror_r(int error_number, char* buf, size_t buf_len) {
  ErrnoRestorer errno_restorer;
  size_t length;

  const char* error_name = __strerror_lookup(error_number);
  if (error_name != nullptr) {
    length = strlcpy(buf, error_name, buf_len);
  } else {
    length = async_safe_format_buffer(buf, buf_len, "Unknown error %d", error_number);
  }
  if (length >= buf_len) {
    return ERANGE;
  }

  return 0;
}

extern "C" char* __gnu_strerror_r(int error_number, char* buf, size_t buf_len) {
  ErrnoRestorer errno_restorer; // The glibc strerror_r doesn't set errno if it truncates...
  strerror_r(error_number, buf, buf_len);
  return buf; // ...and just returns whatever fit.
}

char* strerror(int error_number) {
  // Just return the original constant in the easy cases.
  char* result = const_cast<char*>(__strerror_lookup(error_number));
  if (result != nullptr) {
    return result;
  }

  bionic_tls& tls = __get_bionic_tls();
  result = tls.strerror_buf;
  strerror_r(error_number, result, sizeof(tls.strerror_buf));
  return result;
}
