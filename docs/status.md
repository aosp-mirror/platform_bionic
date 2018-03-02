# Android bionic status

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

Missing functionality:
  * `<aio.h>`
  * `<wordexp.h>`
  * Thread cancellation
  * Robust mutexes

Run `./libc/tools/check-symbols-glibc.py` in bionic/ for the current
list of POSIX functions implemented by glibc but not by bionic.

### libc

Current libc symbols: https://android.googlesource.com/platform/bionic/+/master/libc/libc.map.txt

New libc functions in P:
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

New libc behavior in P:
  * `%C` and `%S` support in the printf family (previously only the wprintf family supported these)
  * `%mc`/`%ms`/`%m[` support in the scanf family
  * `%s` support in strptime (strftime already supported it)

New libc functions in O:
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

New libc functions in N:
  * more FORTIFY support functions (`fread`/`fwrite`/`getcwd`/`pwrite`/`write`)
  * all remaining `_FILE_OFFSET_BITS=64` functions, completing `_FILE_OFFSET_BITS=64` support in bionic (8)
  * all 7 `pthread_barrier*` functions
  * all 5 `pthread_spin*` functions
  * `lockf`/`preadv`/`pwritev`/`scandirat` and `off64_t` variants
  * `adjtimex`/`clock_adjtime`
  * `getifaddrs`/`freeifaddrs`/`if_freenameindex`/`if_nameindex`
  * `getgrgid_r`/`getgrnam_r`
  * GNU extensions `fileno_unlocked`/`strchrnul`
  * 32-bit `prlimit`

libc function count over time:
  G 803, H 825, I 826, J 846, J-MR1 873, J-MR2 881, K 896, L 1116, M 1181, N 1226, O 1278

```
ndk-r17$ for i in `ls -1v platforms/android-*/arch-arm/usr/lib/libc.so` ; do \
  echo $i; nm $i | grep -vw [AbdNnt] | grep -vw B | wc -l ; done
```

### libm

Current libm symbols: https://android.googlesource.com/platform/bionic/+/master/libm/libm.map.txt

0 remaining missing POSIX libm functions.

19 new libm functions in O: complex trig/exp/log functions.

libm function count over time:
  G 158, J-MR2 164, L 220, M 265, O 284



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
