/* $OpenBSD: strerror_r.c,v 1.6 2005/08/08 08:05:37 espie Exp $ */
/* Public Domain <marc@snafu.org> */

// G++ automatically defines _GNU_SOURCE, which then means that <string.h>
// gives us the GNU variant.
#undef _GNU_SOURCE

#include <string.h>

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>

#include <async_safe/log.h>

#include "private/ErrnoRestorer.h"

struct Pair {
  int code;
  const char* msg;
};

static const char* __code_string_lookup(const Pair* strings, int code) {
  for (size_t i = 0; strings[i].msg != nullptr; ++i) {
    if (strings[i].code == code) {
      return strings[i].msg;
    }
  }
  return nullptr;
}

static const Pair _sys_error_strings[] = {
    {0, "Success"},
    {EPERM, "Operation not permitted"},
    {ENOENT, "No such file or directory"},
    {ESRCH, "No such process"},
    {EINTR, "Interrupted system call"},
    {EIO, "I/O error"},
    {ENXIO, "No such device or address"},
    {E2BIG, "Argument list too long"},
    {ENOEXEC, "Exec format error"},
    {EBADF, "Bad file descriptor"},
    {ECHILD, "No child processes"},
    {EAGAIN, "Try again"},
    {ENOMEM, "Out of memory"},
    {EACCES, "Permission denied"},
    {EFAULT, "Bad address"},
    {ENOTBLK, "Block device required"},
    {EBUSY, "Device or resource busy"},
    {EEXIST, "File exists"},
    {EXDEV, "Cross-device link"},
    {ENODEV, "No such device"},
    {ENOTDIR, "Not a directory"},
    {EISDIR, "Is a directory"},
    {EINVAL, "Invalid argument"},
    {ENFILE, "File table overflow"},
    {EMFILE, "Too many open files"},
    {ENOTTY, "Not a typewriter"},
    {ETXTBSY, "Text file busy"},
    {EFBIG, "File too large"},
    {ENOSPC, "No space left on device"},
    {ESPIPE, "Illegal seek"},
    {EROFS, "Read-only file system"},
    {EMLINK, "Too many links"},
    {EPIPE, "Broken pipe"},
    {EDOM, "Math argument out of domain of func"},
    {ERANGE, "Math result not representable"},
    {EDEADLK, "Resource deadlock would occur"},
    {ENAMETOOLONG, "File name too long"},
    {ENOLCK, "No record locks available"},
    {ENOSYS, "Function not implemented"},
    {ENOTEMPTY, "Directory not empty"},
    {ELOOP, "Too many symbolic links encountered"},
    {ENOMSG, "No message of desired type"},
    {EIDRM, "Identifier removed"},
    {ECHRNG, "Channel number out of range"},
    {EL2NSYNC, "Level 2 not synchronized"},
    {EL3HLT, "Level 3 halted"},
    {EL3RST, "Level 3 reset"},
    {ELNRNG, "Link number out of range"},
    {EUNATCH, "Protocol driver not attached"},
    {ENOCSI, "No CSI structure available"},
    {EL2HLT, "Level 2 halted"},
    {EBADE, "Invalid exchange"},
    {EBADR, "Invalid request descriptor"},
    {EXFULL, "Exchange full"},
    {ENOANO, "No anode"},
    {EBADRQC, "Invalid request code"},
    {EBADSLT, "Invalid slot"},
    {EBFONT, "Bad font file format"},
    {ENOSTR, "Device not a stream"},
    {ENODATA, "No data available"},
    {ETIME, "Timer expired"},
    {ENOSR, "Out of streams resources"},
    {ENONET, "Machine is not on the network"},
    {ENOPKG, "Package not installed"},
    {EREMOTE, "Object is remote"},
    {ENOLINK, "Link has been severed"},
    {EADV, "Advertise error"},
    {ESRMNT, "Srmount error"},
    {ECOMM, "Communication error on send"},
    {EPROTO, "Protocol error"},
    {EMULTIHOP, "Multihop attempted"},
    {EDOTDOT, "RFS specific error"},
    {EBADMSG, "Not a data message"},
    {EOVERFLOW, "Value too large for defined data type"},
    {ENOTUNIQ, "Name not unique on network"},
    {EBADFD, "File descriptor in bad state"},
    {EREMCHG, "Remote address changed"},
    {ELIBACC, "Can not access a needed shared library"},
    {ELIBBAD, "Accessing a corrupted shared library"},
    {ELIBSCN, ".lib section in a.out corrupted"},
    {ELIBMAX, "Attempting to link in too many shared libraries"},
    {ELIBEXEC, "Cannot exec a shared library directly"},
    {EILSEQ, "Illegal byte sequence"},
    {ERESTART, "Interrupted system call should be restarted"},
    {ESTRPIPE, "Streams pipe error"},
    {EUSERS, "Too many users"},
    {ENOTSOCK, "Socket operation on non-socket"},
    {EDESTADDRREQ, "Destination address required"},
    {EMSGSIZE, "Message too long"},
    {EPROTOTYPE, "Protocol wrong type for socket"},
    {ENOPROTOOPT, "Protocol not available"},
    {EPROTONOSUPPORT, "Protocol not supported"},
    {ESOCKTNOSUPPORT, "Socket type not supported"},
    {EOPNOTSUPP, "Operation not supported on transport endpoint"},
    {EPFNOSUPPORT, "Protocol family not supported"},
    {EAFNOSUPPORT, "Address family not supported by protocol"},
    {EADDRINUSE, "Address already in use"},
    {EADDRNOTAVAIL, "Cannot assign requested address"},
    {ENETDOWN, "Network is down"},
    {ENETUNREACH, "Network is unreachable"},
    {ENETRESET, "Network dropped connection because of reset"},
    {ECONNABORTED, "Software caused connection abort"},
    {ECONNRESET, "Connection reset by peer"},
    {ENOBUFS, "No buffer space available"},
    {EISCONN, "Transport endpoint is already connected"},
    {ENOTCONN, "Transport endpoint is not connected"},
    {ESHUTDOWN, "Cannot send after transport endpoint shutdown"},
    {ETOOMANYREFS, "Too many references: cannot splice"},
    {ETIMEDOUT, "Connection timed out"},
    {ECONNREFUSED, "Connection refused"},
    {EHOSTDOWN, "Host is down"},
    {EHOSTUNREACH, "No route to host"},
    {EALREADY, "Operation already in progress"},
    {EINPROGRESS, "Operation now in progress"},
    {ESTALE, "Stale NFS file handle"},
    {EUCLEAN, "Structure needs cleaning"},
    {ENOTNAM, "Not a XENIX named type file"},
    {ENAVAIL, "No XENIX semaphores available"},
    {EISNAM, "Is a named type file"},
    {EREMOTEIO, "Remote I/O error"},
    {EDQUOT, "Quota exceeded"},
    {ENOMEDIUM, "No medium found"},
    {EMEDIUMTYPE, "Wrong medium type"},
    {ECANCELED, "Operation Canceled"},
    {ENOKEY, "Required key not available"},
    {EKEYEXPIRED, "Key has expired"},
    {EKEYREVOKED, "Key has been revoked"},
    {EKEYREJECTED, "Key was rejected by service"},
    {EOWNERDEAD, "Owner died"},
    {ENOTRECOVERABLE, "State not recoverable"},
    {ERFKILL, "Operation not possible due to RF-kill"},
    {EHWPOISON, "Memory page has hardware error"},
    {0, nullptr}
};

extern "C" __LIBC_HIDDEN__ const char* __strerror_lookup(int error_number) {
  return __code_string_lookup(_sys_error_strings, error_number);
}

static const Pair _sys_signal_strings[] = {
#define  __BIONIC_SIGDEF(signal_number, signal_description)  { signal_number, signal_description },
#include "private/bionic_sigdefs.h"
  { 0, nullptr }
};

extern "C" __LIBC_HIDDEN__ const char* __strsignal_lookup(int signal_number) {
  return __code_string_lookup(_sys_signal_strings, signal_number);
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
    errno_restorer.override(ERANGE);
    return -1;
  }

  return 0;
}

extern "C" char* __gnu_strerror_r(int error_number, char* buf, size_t buf_len) {
  ErrnoRestorer errno_restorer; // The glibc strerror_r doesn't set errno if it truncates...
  strerror_r(error_number, buf, buf_len);
  return buf; // ...and just returns whatever fit.
}

extern "C" __LIBC_HIDDEN__ const char* __strsignal(int signal_number, char* buf, size_t buf_len) {
  const char* signal_name = __strsignal_lookup(signal_number);
  if (signal_name != nullptr) {
    return signal_name;
  }

  const char* prefix = "Unknown";
  if (signal_number >= SIGRTMIN && signal_number <= SIGRTMAX) {
    prefix = "Real-time";
    signal_number -= SIGRTMIN;
  }
  size_t length = snprintf(buf, buf_len, "%s signal %d", prefix, signal_number);
  if (length >= buf_len) {
    return nullptr;
  }
  return buf;
}
