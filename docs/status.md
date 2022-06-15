# Android bionic status

This document details libc/libm/libdl additions and behavior changes.

See also
[Android linker changes for NDK developers](../android-changes-for-ndk-developers.md)
for changes related to native code loading in various Android releases.

## Bionic function availability

### POSIX

You can see the current status with respect to POSIX in the form of tests:
https://android.googlesource.com/platform/bionic/+/master/tests/headers/posix/

Some POSIX functionality is not supported by the Linux kernel, and
is guarded with tests for `__linux__`. Other functionality is not
supported by bionic or glibc, and guarded with tests for `__BIONIC__`
and `__GLIBC__`. In other cases historical accidents mean 32-bit
bionic diverged but 64-bit bionic matches POSIX; these are guarded with
`__LP64__`.

Most bionic-only diversions should be accompanied by an explanatory comment.

Missing functions are either obsolete or explicitly disallowed by SELinux:
  * `a64l`/`l64a`
  * `confstr`
  * `crypt`/`encrypt`/`setkey`
  * `gethostid`
  * `shm_open`/`shm_unlink`
  * `sockatmark`
  * `ualarm`

Missing functionality:
  * `<aio.h>`
  * `<monetary.h>`. See
    [discussion](https://github.com/android/ndk/issues/1182).
  * `<wordexp.h>`
  * Thread cancellation (`pthread_cancel`). Unlikely to ever be implemented
    because of the difficulty and cost of implementing it, and the difficulty
    of using it correctly. See
    [This is why we can't have safe cancellation points](https://lwn.net/Articles/683118/)
    for more about thread cancellation.
  * Robust mutexes. See
    [discussion](https://github.com/android/ndk/issues/1181).

Run `./libc/tools/check-symbols-glibc.py` in bionic/ for the current
list of POSIX functions implemented by glibc but not by bionic.

### libc

Current libc symbols: https://android.googlesource.com/platform/bionic/+/master/libc/libc.map.txt

New libc functions in T (API level 33):
  * `backtrace`, `backtrace_symbols`, `backtrace_symbols_fd` (`<execinfo.h>`).
  * New system call wrappers: `preadv2`, `preadv64v2`, `pwritev2`,
    `pwritev64v2`.

New libc functions in S (API level 31):
  * New hooks for sanitizers for TLS access: `__libc_get_static_tls_bounds`,
    `__libc_register_thread_exit_callback`, `__libc_iterate_dynamic_tls`,
    `__libc_register_dynamic_tls_listeners`.
  * New helper to allow the zygote to give each zygote child its own stack
    cookie (currently unused): `android_reset_stack_guards`.
  * Non-inline symbols for `ffsl`, `ffsll`.
  * New system call wrappers: `pidfd_getfd`, `pidfd_open`, `pidfd_send_signal`,
    `process_madvise`.

New libc functions in R (API level 30):
  * Full C11 `<threads.h>` (available as inlines for older API levels).
  * `memfd_create` and `mlock2` (Linux-specific GNU extensions).
  * `renameat2` and `statx` (Linux-specific GNU extensions).
  * `pthread_cond_clockwait`/`pthread_mutex_clocklock`/`pthread_rwlock_clockrdlock`/`pthread_rwlock_clockwrlock`/`sem_clockwait`

New libc behavior in R (API level 30):
  * [fdsan](fdsan.md) now aborts when it detects common file descriptor errors,
    rather than just logging.

New libc functions in Q (API level 29):
  * `timespec_get` (C11 `<time.h>` addition)
  * `reallocarray` (BSD/GNU extension in `<malloc.h>` and `<stdlib.h>`)
  * `res_randomid` (in `<resolv.h>`)
  * `pthread_sigqueue` (GNU extension)
  * `getloadavg` (BSD/GNU extension in <stdlib.h>)

New libc behavior in Q (API level 29):
  * Whole printf family now supports the GNU `%m` extension, rather than a
    special-case hack in `syslog`.
  * `popen` now always uses `O_CLOEXEC`, not just with the `e` extension.
  * Bug fixes to handling of UTF-8 U+fffe/U+ffff and code points above U+10ffff.
  * `aligned_alloc` correctly verifies that `size` is a multiple of `alignment`.
  * Using `%n` with the printf family is now reported as a FORTIFY failure.
    Previous versions of Android would ignore the `%n` but not consume the
    corresponding pointer argument, leading to obscure errors. The scanf family
    is unchanged.
  * Support in strptime for `%F`, `%G`, `%g`, `%P`, `%u`, `%V`, and `%v`.
    (strftime already supported them all.)
  * [fdsan](fdsan.md) detects and logs common file descriptor errors at runtime.

New libc functions in P (API level 28):
  * `aligned_alloc`
  * `__freading`/`__fwriting` (completing <stdio_ext.h>)
  * `endhostent`/`endnetent`/`endprotoent`/`getnetent`/`getprotoent`/`sethostent`/`setnetent`/`setprotoent` (completing <netdb.h>)
  * `fexecve`
  * `fflush_unlocked`/`fgetc_unlocked`/`fgets_unlocked`/`fputc_unlocked`/`fputs_unlocked`/`fread_unlocked`/`fwrite_unlocked`
  * `getentropy`/`getrandom` (adding <sys/random.h>)
  * `getlogin_r`
  * `glob`/`globfree` (adding <glob.h>)
  * `hcreate`/`hcreate_r`/`hdestroy`/`hdestroy_r`/`hsearch`/`hsearch_r` (completing <search.h>)
  * `iconv`/`iconv_close`/`iconv_open` (adding <iconv.h>)
  * `pthread_attr_getinheritsched`/`pthread_attr_setinheritsched`/`pthread_setschedprio`
  * `pthread_mutexattr_getprotocol`/`pthread_mutexattr_setprotocol` (mutex priority inheritance)
  * <signal.h> support for `sigaction64_t` and `sigset64_t` allowing LP32 access to real-time signals
  * <spawn.h>
  * `swab`
  * `syncfs`

New libc behavior in P (API level 28):
  * `%C` and `%S` support in the printf family (previously only the wprintf family supported these).
  * `%mc`/`%ms`/`%m[` support in the scanf family.
  * `%s` support in strptime (strftime already supported it).
  * Using a `pthread_mutex_t` after it's been destroyed will be detected at
    runtime and reported as a FORTIFY failure.
  * Passing a null `FILE*` or `DIR*` to libc is now detected at runtime and
    reported as a FORTIFY failure.

New libc functions in O (API level 26):
  * `sendto` FORTIFY support
  * `__system_property_read_callback`/`__system_property_wait`
  * legacy `bsd_signal`
  * `catclose`/`catgets`/`catopen` (adding <nl_types.h>)
  * `ctermid`
  * all 6 <grp.h>/<pwd.h> (get|set|end)(gr|pw)ent functions
  * `futimes`/`futimesat`/`lutimes`
  * `getdomainname`/`setdomainname`
  * `getsubopt`
  * `hasmntopt`
  * `mallopt`
  * `mblen`
  * 4 <sys/msg.h> `msg*` functions
  * <langinfo.h> `nl_langinfo`/`nl_langinfo_l`
  * `pthread_getname_np`
  * 2 new Linux system calls `quotactl` and `sync_file_range`
  * 4 <sys/sem.h> `sem*` functions
  * 4 <sys/shm.h> `shm*` functions
  * 5 legacy <signal.h> functions: `sighold`/`sigignore`/`sigpause`/`sigrelse`/`sigset`
  * `strtod_l`/`strtof_l`/`strtol_l`/`strtoul_l`
  * <wctype.h> `towctrans`/`towctrans_l`/`wctrans`/`wctrans_l`

New libc behavior in O (API level 26):
  * Passing an invalid `pthread_t` to libc is now detected at runtime and
    reported as a FORTIFY failure. Most commonly this is a result of confusing
    `pthread_t` and `pid_t`.

New libc functions in N (API level 24):
  * more FORTIFY support functions (`fread`/`fwrite`/`getcwd`/`pwrite`/`write`)
  * all remaining `_FILE_OFFSET_BITS=64` functions, completing `_FILE_OFFSET_BITS=64` support in bionic (8)
  * all 7 `pthread_barrier*` functions
  * all 5 `pthread_spin*` functions
  * `lockf`/`preadv`/`pwritev`/`scandirat` and `off64_t` variants
  * `adjtimex`/`clock_adjtime`
  * <ifaddrs.h> `getifaddrs`/`freeifaddrs`/`if_freenameindex`/`if_nameindex`
  * `getgrgid_r`/`getgrnam_r`
  * GNU extensions `fileno_unlocked`/`strchrnul`
  * 32-bit `prlimit`

New libc behavior in N (API level 24):
  * `sem_wait` now returns EINTR when interrupted by a signal.

New libc functions in M (API level 23):
  * <dirent.h> `telldir`, `seekdir`.
  * <malloc.h> `malloc_info`.
  * <netdb.h> `gethostbyaddr_r`, `gethostbyname2_r`.
  * <pthread.h> `pthread_rwlockattr_getkind_np`/`pthread_rwlockattr_setkind_np`.
  * <pty.h> `forkpty`, `openpty`.
  * <signal.h> `sigqueue`, `sigtimedwait`, `sigwaitinfo`.
  * <stdio.h> `fmemopen`, `open_memstream`, `feof_unlocked`, `ferror_unlocked`, `clearerr_unlocked`.
  * <stdio_ext.h> `__flbf`, `__freadable`, `__fsetlocking`, `__fwritable`, `__fbufsize`, `__fpending`, `_flushlbf`, `__fpurge`.
  * <stdlib.h> `mkostemp`/`mkostemps`, `lcong48`.
  * <string.h> `basename`, `strerror_l`, `strerror_r`, `mempcpy`.
  * <sys/sysinfo.h> `get_nprocs_conf`/`get_nprocs`, `get_phys_pages`, `get_avphys_pages`.
  * <sys/uio.h> `process_vm_readv`/`process_vm_writev`.
  * `clock_getcpuclockid`, `login_tty`, `mkfifoat`, `posix_madvise`, `sethostname`, `strcasecmp_l`/`strncasecmp_l`.
  * <wchar.h> `open_wmemstream`, `wcscasecmp_l`/`wcsncasecmp_l`, `wmempcpy`.
  * all of <error.h>.
  * re-introduced various <resolv.h> functions: `ns_format_ttl`, `ns_get16`, `ns_get32`, `ns_initparse`, `ns_makecanon`, `ns_msg_getflag`, `ns_name_compress`, `ns_name_ntol`, `ns_name_ntop`, `ns_name_pack`, `ns_name_pton`, `ns_name_rollback`, `ns_name_skip`, `ns_name_uncompress`, `ns_name_unpack`, `ns_parserr`, `ns_put16`, `ns_put32`, `ns_samename`, `ns_skiprr`, `ns_sprintrr`, and `ns_sprintrrf`.

New libc functions in L (API level 21):
  * <android/dlext.h>.
  * <android/set_abort_message.h>.
  * <arpa/inet.h> `inet_lnaof`, `inet_netof`, `inet_network`, `inet_makeaddr`.
  * <wctype.h> `iswblank`.
  * <ctype.h> `isalnum_l`, `isalpha_l`, `isblank_l`, `icntrl_l`, `isdigit_l`, `isgraph_l`, `islower_l`, `isprint_l`, `ispunct_l`, `isspace_l`, `isupper_l`, `isxdigit_l`, `_tolower`, `tolower_l`, `_toupper`, `toupper_l`.
  * <fcntl.h> `fallocate`, `posix_fadvise`, `posix_fallocate`, `splice`, `tee`, `vmsplice`.
  * <inttypes.h> `wcstoimax`, `wcstoumax`.
  * <link.h> `dl_iterate_phdr`.
  * <mntent.h> `setmntent`, `endmntent`, `getmntent_r`.
  * <poll.h> `ppoll`.
  * <pthread.h> `pthread_condattr_getclock`, `pthread_condattr_setclock`, `pthread_mutex_timedlock`, `pthread_gettid_np`.
  * <sched.h> `setns`.
  * <search.h> `insque`, `remque`, `lfind`, `lsearch`, `twalk`.
  * <stdio.h> `dprintf`, `vdprintf`.
  * <stdlib.h> `initstate`, `setstate`, `getprogname`/`setprogname`, `atof`/`strtof`, `at_quick_exit`/`_Exit`/`quick_exit`, `grantpt`, `mbtowc`/`wctomb`, `posix_openpt`, `rand_r`/`rand`/`random`/`srand`/`srandom`, `strtold_l`/`strtoll_l`/`strtoull_l`.
  * <string.h> `strcoll_l`/`strxfrm_l`, `stpcpy`/`stpncpy`.
  * <sys/resource.h> `prlimit`.
  * <sys/socket.h> `accept4`, `sendmmsg`.
  * <sys/stat.h> `mkfifo`/`mknodat`.
  * <time.h> `strftime_l`.
  * <unistd.h> `dup3`, `execvpe`, `getpagesize`, `linkat`/`symlinkat`/`readlinkat`, `truncate`.
  * <wchar.h> `wcstof`, `vfwscanf`/`vswscanf`/`vwscanf`, `wcstold_l`/`wcstoll`/`wcstoll_l`/`wcstoull`/`wcstoull_l`, `mbsnrtowcs`/`wcsnrtombs`, `wcscoll_l`/`wcsxfrm_l`.
  * <wctype.h> `iswalnum_l`/`iswalpha_l`/`iswblank_l`/`iswcntrl_l`/`iswctype_l`/`iswdigit_l`/`iswgraph_l`/`iswlower_l`/`iswprint_l`/`iswpunct_l`/`iswspace_l`/`iswupper_l`/`iswxdigit_l`, `wctype_l`, `towlower_l`/`towupper_l`.
  * all of <fts.h>.
  * all of <locale.h>.
  * all of <sys/epoll.h>.
  * all of <sys/fsuid.h>.
  * all of <sys/inotify.h>.
  * all of <uchar.h>.

New libc functions in K (API level 19):
  * <inttypes.h> `imaxabs`, `imaxdiv`.
  * <stdlib.h> `abs`, `labs`, `llabs`.
  * <sys/stat.h> `futimens`.
  * all of <sys/statvfs.h>.
  * all of <sys/swap.h>.
  * all of <sys/timerfd.h>.

New libc functions in J-MR2 (API level 18):
  * <stdio.h> `getdelim` and `getline`.
  * <sys/auxv.h> `getauxval`.
  * <sys/signalfd.h> `signalfd`.

New libc functions in J-MR1 (API level 17):
  * <ftw.h>.
  * <signal.h> `psiginfo` and `psignal`.
  * `getsid`, `malloc_usable_size`, `mlockall`/`munlockall`, `posix_memalign`, `unshare`.

New libc functions in J (API level 16):
  * the <search.h> tree functions `tdelete`, `tdestroy`, `tfind`, and `tsearch`.
  * `faccessat`, `readahead`, `tgkill`.
  * all of <sys/xattr.h>.

libc function count over time:

| OS    | API level | Function count |
|-------|-----------|----------------|
| J     | 16        | 842            |
| J MR1 | 17        | 870            |
| J MR2 | 18        | 878            |
| K     | 19        | 893            |
| L     | 21        | 1118           |
| M     | 23        | 1183           |
| N     | 24        | 1228           |
| O     | 26        | 1280           |
| P     | 28        | 1378           |
| Q     | 29        | 1394           |

Data collected by:
```
ndk-r21$ for i in `ls -1v platforms/android-*/arch-arm/usr/lib/libc.so` ; do \
  echo $i; nm $i | grep -w T | wc -l ; done
```

### libm

Current libm symbols: https://android.googlesource.com/platform/bionic/+/master/libm/libm.map.txt

0 remaining missing C11/POSIX libm functions.

New libm functions in O (API level 26):
  * <complex.h> `clog`/`clogf`, `cpow`/`cpowf` functions.

New libm functions in M (API level 23):
  * <complex.h> `cabs`, `carg`, `cimag`, `cacos`, `cacosh`, `casin`, `casinh`, `catan`, `catanh`, `ccos`, `ccosh`, `cexp`, `conj`, `cproj`, `csin`, `csinh`, `csqrt`, `ctan`, `ctanh`, `creal`, `cabsf`, `cargf`, `cimagf`, `cacosf`, `cacoshf`, `casinf`, `casinhf`, `catanf`, `catanhf`, `ccosf`, `ccoshf`, `cexpf`, `conjf`, `cprojf`, `csinf`, `csinhf`, `csqrtf`, `ctanf`, `ctanhf`, `crealf`, `cabsl`, `cprojl`, `csqrtl`.
  * <math.h> `lgammal_r`.

New libm functions in L (API level 21):
  * <complex.h> `cabsl`, `cprojl`, `csqrtl`.
  * <math.h> `isinf`, `significandl`.

New libm functions in J-MR2 (API level 18):
  * <math.h> `log2`, `log2f`.


## Target API level behavioral differences

Most bionic bug fixes and improvements have been made without checks for
the app's `targetSdkVersion`. As of O there were exactly two exceptions,
but there are likely to be more in future because of Project Treble.

### Invalid `pthread_t` handling (targetSdkVersion >= O)

As part of a long-term goal to remove the global thread list,
and in an attempt to flush out racy code, we changed how an invalid
`pthread_t` is handled. For `pthread_detach`, `pthread_getcpuclockid`,
`pthread_getschedparam`/`pthread_setschedparam`, `pthread_join`, and
`pthread_kill`, instead of returning ESRCH when passed an invalid
`pthread_t`, if you're targeting O or above, they'll abort with the
message "attempt to use invalid pthread\_t".

Note that this doesn't change behavior as much as you might think: the
old lookup only held the global thread list lock for the duration of
the lookup, so there was still a race between that and the dereference
in the caller, given that callers actually need the tid to pass to some
syscall or other, and sometimes update fields in the `pthread_internal_t`
struct too.

We can't check a thread's tid against 0 to see whether a `pthread_t`
is still valid because a dead thread gets its thread struct unmapped
along with its stack, so the dereference isn't safe.

To fix your code, taking the affected functions one by one:

  * `pthread_getcpuclockid` and `pthread_getschedparam`/`pthread_setschedparam`
    should be fine. Unsafe calls to those seem highly unlikely.

  * Unsafe `pthread_detach` callers probably want to switch to
    `pthread_attr_setdetachstate` instead, or use
    `pthread_detach(pthread_self());` from the new thread's start routine
    rather than calling detach in the parent.

  * `pthread_join` calls should be safe anyway, because a joinable thread
    won't actually exit and unmap until it's joined. If you're joining an
    unjoinable thread, the fix is to stop marking it detached. If you're
    joining an already-joined thread, you need to rethink your design!

  * Unsafe `pthread_kill` calls aren't portably fixable. (And are obviously
    inherently non-portable as-is.) The best alternative on Android is to
    use `pthread_gettid_np` at some point that you know the thread to be
    alive, and then call `kill`/`tgkill` with signal 0 (which checks
    whether a process exists rather than actually sending a
    signal). That's still not completely safe because if you're too late
    the tid may have been reused, but your code is inherently unsafe without
    a redesign anyway.

### Interruptable `sem_wait` (targetSdkVersion >= N)

POSIX says that `sem_wait` can be interrupted by delivery of a
signal. This wasn't historically true in Android, and when we fixed this
bug we found that existing code relied on the old behavior. To preserve
compatibility, `sem_wait` can only return EINTR on Android if the app
targets N or later.


## FORTIFY

The `_FORTIFY_SOURCE` macro can be used to enable extra
automatic bounds checking for common libc functions. If a buffer
overrun is detected, the program is safely aborted as in this
[example](https://source.android.com/devices/tech/debug/native-crash#fortify).

Note that in recent releases Android's FORTIFY has been extended to
cover other issues. It can now detect, for example, passing `O_CREAT`
to open(2) without specifying a mode. It also performs some checking
regardless of whether the caller was built with FORTIFY enabled. In P,
for example, calling a `pthread_mutex_` function on a destroyed mutex,
calling a `<dirent.h>` function on a null pointer, using `%n` with the
printf(3) family, or using the scanf(3) `m` modifier incorrectly will
all result in FORTIFY failures even for code not built with FORTIFY.

More background information is available in our
[FORTIFY in Android](https://android-developers.googleblog.com/2017/04/fortify-in-android.html)
blog post.

The Android platform is built with `-D_FORTIFY_SOURCE=2`, but NDK users
need to manually enable FORTIFY by setting that themselves in whatever
build system they're using. The exact subset of FORTIFY available to
NDK users will depend on their target ABI level, because when a FORTIFY
check can't be guaranteed at compile-time, a call to a run-time `_chk`
function is added.
