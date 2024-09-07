## fdtrack

[TOC]

fdtrack is a file descriptor leak checker added to Android in API level 30.

fdtrack consists of two parts: a set of hooks in bionic to register a callback
that's invoked on file descriptor operations, and a library that implements a
hook to perform and store backtraces for file descriptor creation.

### bionic hooks
bionic provides a header in the `bionic_libc_platform_headers` header_lib at <[bionic/fdtrack.h](https://android.googlesource.com/platform/bionic/+/refs/heads/main/libc/platform/bionic/fdtrack.h)>.
Register a callback with `android_fdtrack_compare_exchange_hook` to receive
callbacks upon file descriptor creation and destruction. This function can be
called at any point in order to start capturing events, but be sure to properly
handle unbalanced closes. This callback may be called from an async signal safe
context, but not vfork (bionic tracks whether a thread is vforked, and chooses
not to call callbacks when this is the case).

### libfdtrack
[libfdtrack](https://android.googlesource.com/platform/bionic/+/refs/heads/main/libfdtrack)
implements a library that uses libunwindstack to unwind and store fd creation backtraces.


#### Using libfdtrack
libfdtrack registers its hook upon being loaded, so to start capturing
backtraces, `dlopen("libfdtrack.so", RTLD_GLOBAL)` is all that's needed. To dump
its output to logcat, either use `fdtrack_dump`, or send the signal
`BIONIC_SIGNAL_FDTRACK` (available from `<bionic/reserved_signals.h>`) to the
process. If you wish to iterate through the results programmatically,
`fdtrack_iterate` can be used (warning: this interface is currently unstable,
don't use it in code that can be used on multiple platform versions.)

libfdtrack adds a significant amount of overhead, so for processes that are
latency-critical like system_server, it's not feasible to always capture
backtraces. Instead, if you can detect that an fd leak is ongoing, turning on
backtraces for a while and then triggering a dump can be sufficient.
system_server [implements this approach](https://android.googlesource.com/platform/frameworks/base/+/679f3e4242b8e018eb7df90ef433f81088a64fff%5E%21/),
spawning a thread that regularly checks the count of fds in the process, turns
on fdtrack when it hits a threshold, and then aborts after another threshold.
This dumps the output to logcat, which will be available in both the tombstone
and logcat from bugreports.

#### Implementation details
There are multiple methods to unwind in Android:

 * libunwindstack
   * Pros
     * Primary method on the platform
     * Able to unwind through ART
   * Cons
     * Uses malloc internally: unsafe unless a separate allocator is
       statically-linked and steps are taken to prevent the unwind from being
       interrupted by a signal
     * Slow - infeasible to be used always in latency-sensitive processes
 * `android_unsafe_frame_pointer_chase`
   * Pros
     * Definitely async signal safe
     * Fast
   * Cons
     * Unable to unwind through ART because it doesn't maintain the frame pointer
     * Requires -fno-omit-frame-pointer to be used on all code being unwound
       through, which currently isn't the case on Android
     * Frame layout is a mess on 32-bit ARM: the ARM standard, clang, and GCC
       [all disagree](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92172)
     * Chasing the frame pointer will often result in multiple frames inside the
       same function

libfdtrack chooses to use libunwindstack for now, since unwinding through ART
is critical to being useful for the initial user, system_server.
