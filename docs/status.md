Android bionic status
=====================

libc
----

Current libc symbols: https://android.googlesource.com/platform/bionic/+/master/libc/libc.map.txt

New libc functions in P:
  * `__freading`/`__fwriting` (completing <stdio_ext.h>)
  * `getlogin_r`
  * `iconv`/`iconv_close`/`iconv_open` (adding <iconv.h>)
  * `syncfs`

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
ndk-r17$ for i in `ls -1v platforms/android-*/arch-arm/usr/lib/libc.so` ; do echo $i; nm $i | grep -vw [AbdNnt] | grep -vw B | wc -l ; done
```

Run `./libc/tools/check-symbols-glibc.py` in bionic/ for the current
list of POSIX functions implemented by glibc but not by bionic. Currently
(2017-09):
```
aio_cancel
aio_error
aio_fsync
aio_read
aio_return
aio_suspend
aio_write
endhostent
endnetent
endprotoent
endutxent
fexecve
fmtmsg
getdate
getdate_err
getnetent
getprotoent
glob
globfree
hcreate
hdestroy
hsearch
lio_listio
posix_spawn
posix_spawn_file_actions_addclose
posix_spawn_file_actions_adddup2
posix_spawn_file_actions_addopen
posix_spawn_file_actions_destroy
posix_spawn_file_actions_init
posix_spawnattr_destroy
posix_spawnattr_getflags
posix_spawnattr_getpgroup
posix_spawnattr_getschedparam
posix_spawnattr_getschedpolicy
posix_spawnattr_getsigdefault
posix_spawnattr_getsigmask
posix_spawnattr_init
posix_spawnattr_setflags
posix_spawnattr_setpgroup
posix_spawnattr_setschedparam
posix_spawnattr_setschedpolicy
posix_spawnattr_setsigdefault
posix_spawnattr_setsigmask
posix_spawnp
pthread_attr_getinheritsched
pthread_attr_setinheritsched
pthread_cancel
pthread_getconcurrency
pthread_mutex_consistent
pthread_mutex_getprioceiling
pthread_mutex_setprioceiling
pthread_mutexattr_getprioceiling
pthread_mutexattr_getprotocol
pthread_mutexattr_getrobust
pthread_mutexattr_setprioceiling
pthread_mutexattr_setprotocol
pthread_mutexattr_setrobust
pthread_setcancelstate
pthread_setcanceltype
pthread_setconcurrency
pthread_setschedprio
pthread_testcancel
sethostent
setnetent
setprotoent
sockatmark
swab
wordexp
wordfree
```

libm
----

Current libm symbols: https://android.googlesource.com/platform/bionic/+/master/libm/libm.map.txt

0 remaining missing POSIX libm functions.

19 new libm functions in O: complex trig/exp/log functions.

libm function count over time:
  G 158, J-MR2 164, L 220, M 265, O 284
