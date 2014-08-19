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

#include "private/ErrnoRestorer.h"
#include "private/libc_logging.h"

struct Pair {
  int code;
  const char* msg;
};

static const char* __code_string_lookup(const Pair* strings, int code) {
  for (size_t i = 0; strings[i].msg != NULL; ++i) {
    if (strings[i].code == code) {
      return strings[i].msg;
    }
  }
  return NULL;
}

static const Pair _sys_error_strings[] = {
#define  __BIONIC_ERRDEF(x,y,z)  { x, z },
#include <sys/_errdefs.h>
  { 0, NULL }
};

extern "C" __LIBC_HIDDEN__ const char* __strerror_lookup(int error_number) {
  return __code_string_lookup(_sys_error_strings, error_number);
}

static const Pair _sys_signal_strings[] = {
#define  __BIONIC_SIGDEF(signal_number, signal_description)  { signal_number, signal_description },
#include <sys/_sigdefs.h>
  { 0, NULL }
};

extern "C" __LIBC_HIDDEN__ const char* __strsignal_lookup(int signal_number) {
  return __code_string_lookup(_sys_signal_strings, signal_number);
}

int strerror_r(int error_number, char* buf, size_t buf_len) {
  ErrnoRestorer errno_restorer;
  size_t length;

  const char* error_name = __strerror_lookup(error_number);
  if (error_name != NULL) {
    length = strlcpy(buf, error_name, buf_len);
  } else {
    length = __libc_format_buffer(buf, buf_len, "Unknown error %d", error_number);
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
  if (signal_name != NULL) {
    return signal_name;
  }

  const char* prefix = "Unknown";
  if (signal_number >= SIGRTMIN && signal_number <= SIGRTMAX) {
    prefix = "Real-time";
    signal_number -= SIGRTMIN;
  }
  size_t length = snprintf(buf, buf_len, "%s signal %d", prefix, signal_number);
  if (length >= buf_len) {
    return NULL;
  }
  return buf;
}
