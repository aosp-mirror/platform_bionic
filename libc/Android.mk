LOCAL_PATH := $(call my-dir)

bionic_coverage ?= false

# Make everything depend on any changes to included makefiles.
libc_common_additional_dependencies := $(LOCAL_PATH)/Android.mk

# Load config for TARGET_ARCH
my_2nd_arch_prefix :=
include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/$(TARGET_ARCH).mk
libc_common_additional_dependencies += \
    $(LOCAL_PATH)/arch-$(TARGET_ARCH)/$(TARGET_ARCH).mk


ifdef TARGET_2ND_ARCH
# Load config for TARGET_2ND_ARCH
my_2nd_arch_prefix := $(TARGET_2ND_ARCH_VAR_PREFIX)
include $(LOCAL_PATH)/arch-$(TARGET_2ND_ARCH)/$(TARGET_2ND_ARCH).mk
my_2nd_arch_prefix :=
libc_common_additional_dependencies += \
    $(LOCAL_PATH)/arch-$(TARGET_2ND_ARCH)/$(TARGET_2ND_ARCH).mk
endif

# crt obj files
# ========================================================
# crtbrand.c needs <stdint.h> and a #define for the platform SDK version.
libc_crt_target_cflags := \
    -I$(LOCAL_PATH)/include \
    -DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION) \

my_2nd_arch_prefix :=
include $(LOCAL_PATH)/crt.mk
ifdef TARGET_2ND_ARCH
my_2nd_arch_prefix := $(TARGET_2ND_ARCH_VAR_PREFIX)
include $(LOCAL_PATH)/crt.mk
my_2nd_arch_prefix :=
endif

# Define the common source files for all the libc instances
# =========================================================
libc_common_src_files := \
    bionic/ether_aton.c \
    bionic/ether_ntoa.c \
    bionic/fts.c \
    bionic/getpriority.c \
    bionic/initgroups.c \
    bionic/isatty.c \
    bionic/memmem.c \
    bionic/pututline.c \
    bionic/sched_cpualloc.c \
    bionic/sched_cpucount.c \
    bionic/sigblock.c \
    bionic/siginterrupt.c \
    bionic/sigsetmask.c \
    bionic/system_properties_compat.c \
    stdio/fread.c \
    stdio/refill.c \
    stdio/snprintf.c\
    stdio/sprintf.c \
    stdio/stdio.cpp \
    stdio/stdio_ext.cpp \
    stdlib/atexit.c \
    stdlib/exit.c \

# Fortify implementations of libc functions.
libc_common_src_files += \
    bionic/__FD_chk.cpp \
    bionic/__fgets_chk.cpp \
    bionic/__fread_chk.cpp \
    bionic/__fwrite_chk.cpp \
    bionic/__getcwd_chk.cpp \
    bionic/__memchr_chk.cpp \
    bionic/__memmove_chk.cpp \
    bionic/__memrchr_chk.cpp \
    bionic/__poll_chk.cpp \
    bionic/__pread64_chk.cpp \
    bionic/__pread_chk.cpp \
    bionic/__pwrite64_chk.cpp \
    bionic/__pwrite_chk.cpp \
    bionic/__read_chk.cpp \
    bionic/__readlink_chk.cpp \
    bionic/__readlinkat_chk.cpp \
    bionic/__recvfrom_chk.cpp \
    bionic/__stpcpy_chk.cpp \
    bionic/__stpncpy_chk.cpp \
    bionic/__strchr_chk.cpp \
    bionic/__strlcat_chk.cpp \
    bionic/__strlcpy_chk.cpp \
    bionic/__strlen_chk.cpp \
    bionic/__strncat_chk.cpp \
    bionic/__strncpy_chk.cpp \
    bionic/__strrchr_chk.cpp \
    bionic/__umask_chk.cpp \
    bionic/__vsnprintf_chk.cpp \
    bionic/__vsprintf_chk.cpp \
    bionic/__write_chk.cpp

libc_bionic_ndk_src_files := \
    bionic/abort.cpp \
    bionic/accept.cpp \
    bionic/accept4.cpp \
    bionic/access.cpp \
    bionic/arpa_inet.cpp \
    bionic/assert.cpp \
    bionic/atof.cpp \
    bionic/bionic_netlink.cpp \
    bionic/bionic_systrace.cpp \
    bionic/bionic_time_conversions.cpp \
    bionic/brk.cpp \
    bionic/c16rtomb.cpp \
    bionic/c32rtomb.cpp \
    bionic/chmod.cpp \
    bionic/chown.cpp \
    bionic/clearenv.cpp \
    bionic/clock.cpp \
    bionic/clock_getcpuclockid.cpp \
    bionic/clock_nanosleep.cpp \
    bionic/clone.cpp \
    bionic/close.cpp \
    bionic/__cmsg_nxthdr.cpp \
    bionic/connect.cpp \
    bionic/ctype.cpp \
    bionic/dirent.cpp \
    bionic/dup2.cpp \
    bionic/epoll_create.cpp \
    bionic/epoll_pwait.cpp \
    bionic/epoll_wait.cpp \
    bionic/__errno.cpp \
    bionic/error.cpp \
    bionic/eventfd_read.cpp \
    bionic/eventfd_write.cpp \
    bionic/faccessat.cpp \
    bionic/fchmod.cpp \
    bionic/fchmodat.cpp \
    bionic/ffs.cpp \
    bionic/fgetxattr.cpp \
    bionic/flistxattr.cpp \
    bionic/flockfile.cpp \
    bionic/fpclassify.cpp \
    bionic/fsetxattr.cpp \
    bionic/ftruncate.cpp \
    bionic/futimens.cpp \
    bionic/getcwd.cpp \
    bionic/gethostname.cpp \
    bionic/getpgrp.cpp \
    bionic/getpid.cpp \
    bionic/gettid.cpp \
    bionic/__gnu_basename.cpp \
    bionic/ifaddrs.cpp \
    bionic/inotify_init.cpp \
    bionic/ioctl.cpp \
    bionic/lchown.cpp \
    bionic/lfs64_support.cpp \
    bionic/__libc_current_sigrtmax.cpp \
    bionic/__libc_current_sigrtmin.cpp \
    bionic/libc_init_common.cpp \
    bionic/libc_logging.cpp \
    bionic/libgen.cpp \
    bionic/link.cpp \
    bionic/locale.cpp \
    bionic/lockf.cpp \
    bionic/lstat.cpp \
    bionic/malloc_info.cpp \
    bionic/mbrtoc16.cpp \
    bionic/mbrtoc32.cpp \
    bionic/mbstate.cpp \
    bionic/mempcpy.cpp \
    bionic/mkdir.cpp \
    bionic/mkfifo.cpp \
    bionic/mknod.cpp \
    bionic/mntent.cpp \
    bionic/mremap.cpp \
    bionic/NetdClientDispatch.cpp \
    bionic/net_if.cpp \
    bionic/netinet_in.cpp \
    bionic/open.cpp \
    bionic/pathconf.cpp \
    bionic/pause.cpp \
    bionic/pipe.cpp \
    bionic/poll.cpp \
    bionic/posix_fadvise.cpp \
    bionic/posix_fallocate.cpp \
    bionic/posix_madvise.cpp \
    bionic/posix_timers.cpp \
    bionic/ptrace.cpp \
    bionic/pty.cpp \
    bionic/raise.cpp \
    bionic/rand.cpp \
    bionic/readlink.cpp \
    bionic/reboot.cpp \
    bionic/recv.cpp \
    bionic/rename.cpp \
    bionic/rmdir.cpp \
    bionic/scandir.cpp \
    bionic/sched_getaffinity.cpp \
    bionic/sched_getcpu.cpp \
    bionic/semaphore.cpp \
    bionic/send.cpp \
    bionic/setegid.cpp \
    bionic/__set_errno.cpp \
    bionic/seteuid.cpp \
    bionic/setpgrp.cpp \
    bionic/sigaction.cpp \
    bionic/sigaddset.cpp \
    bionic/sigdelset.cpp \
    bionic/sigemptyset.cpp \
    bionic/sigfillset.cpp \
    bionic/sigismember.cpp \
    bionic/signal.cpp \
    bionic/signalfd.cpp \
    bionic/sigpending.cpp \
    bionic/sigprocmask.cpp \
    bionic/sigqueue.cpp \
    bionic/sigsuspend.cpp \
    bionic/sigtimedwait.cpp \
    bionic/sigwait.cpp \
    bionic/sigwaitinfo.cpp \
    bionic/socket.cpp \
    bionic/stat.cpp \
    bionic/statvfs.cpp \
    bionic/strchrnul.cpp \
    bionic/strerror.cpp \
    bionic/strerror_r.cpp \
    bionic/strsignal.cpp \
    bionic/strtold.cpp \
    bionic/stubs.cpp \
    bionic/symlink.cpp \
    bionic/sysinfo.cpp \
    bionic/syslog.cpp \
    bionic/sys_siglist.c \
    bionic/sys_signame.c \
    bionic/system_properties.cpp \
    bionic/tdestroy.cpp \
    bionic/termios.cpp \
    bionic/thread_private.cpp \
    bionic/tmpfile.cpp \
    bionic/umount.cpp \
    bionic/unlink.cpp \
    bionic/utimes.cpp \
    bionic/wait.cpp \
    bionic/wchar.cpp \
    bionic/wctype.cpp \
    bionic/wmempcpy.cpp \

libc_bionic_src_files :=

# The following implementations depend on pthread data, so we can't include
# them in libc_ndk.a.
libc_bionic_src_files += \
    bionic/__cxa_thread_atexit_impl.cpp \
    bionic/fork.cpp \

# The data that backs getauxval is initialized in the libc init functions which
# are invoked by the linker. If this file is included in libc_ndk.a, only one of
# the copies of the global data will be initialized, resulting in nullptr
# dereferences.
libc_bionic_src_files += bionic/getauxval.cpp

# These four require getauxval, which isn't available on older platforms.
libc_bionic_src_files += bionic/getentropy_linux.c
libc_bionic_src_files += bionic/sysconf.cpp
libc_bionic_src_files += bionic/vdso.cpp
libc_bionic_src_files += bionic/setjmp_cookie.cpp

libc_bionic_src_files += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcat_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    bionic/strrchr.cpp \

libc_cxa_src_files := \
    bionic/__cxa_guard.cpp \
    bionic/__cxa_pure_virtual.cpp \
    bionic/new.cpp \

libc_upstream_freebsd_src_files := \
    upstream-freebsd/lib/libc/gen/ldexp.c \
    upstream-freebsd/lib/libc/gen/sleep.c \
    upstream-freebsd/lib/libc/gen/usleep.c \
    upstream-freebsd/lib/libc/stdlib/getopt_long.c \
    upstream-freebsd/lib/libc/stdlib/qsort.c \
    upstream-freebsd/lib/libc/stdlib/quick_exit.c \
    upstream-freebsd/lib/libc/stdlib/realpath.c \
    upstream-freebsd/lib/libc/string/wcpcpy.c \
    upstream-freebsd/lib/libc/string/wcpncpy.c \
    upstream-freebsd/lib/libc/string/wcscasecmp.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcscspn.c \
    upstream-freebsd/lib/libc/string/wcsdup.c \
    upstream-freebsd/lib/libc/string/wcslcat.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsncasecmp.c \
    upstream-freebsd/lib/libc/string/wcsncat.c \
    upstream-freebsd/lib/libc/string/wcsncmp.c \
    upstream-freebsd/lib/libc/string/wcsncpy.c \
    upstream-freebsd/lib/libc/string/wcsnlen.c \
    upstream-freebsd/lib/libc/string/wcspbrk.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wcsspn.c \
    upstream-freebsd/lib/libc/string/wcstok.c \
    upstream-freebsd/lib/libc/string/wmemchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-freebsd/lib/libc/string/wmemmove.c \
    upstream-freebsd/lib/libc/string/wmemset.c \

libc_upstream_netbsd_src_files := \
    upstream-netbsd/common/lib/libc/stdlib/random.c \
    upstream-netbsd/lib/libc/gen/ftw.c \
    upstream-netbsd/lib/libc/gen/nftw.c \
    upstream-netbsd/lib/libc/gen/nice.c \
    upstream-netbsd/lib/libc/gen/popen.c \
    upstream-netbsd/lib/libc/gen/psignal.c \
    upstream-netbsd/lib/libc/gen/utime.c \
    upstream-netbsd/lib/libc/gen/utmp.c \
    upstream-netbsd/lib/libc/inet/nsap_addr.c \
    upstream-netbsd/lib/libc/regex/regcomp.c \
    upstream-netbsd/lib/libc/regex/regerror.c \
    upstream-netbsd/lib/libc/regex/regexec.c \
    upstream-netbsd/lib/libc/regex/regfree.c \
    upstream-netbsd/lib/libc/stdlib/bsearch.c \
    upstream-netbsd/lib/libc/stdlib/div.c \
    upstream-netbsd/lib/libc/stdlib/drand48.c \
    upstream-netbsd/lib/libc/stdlib/erand48.c \
    upstream-netbsd/lib/libc/stdlib/jrand48.c \
    upstream-netbsd/lib/libc/stdlib/lcong48.c \
    upstream-netbsd/lib/libc/stdlib/ldiv.c \
    upstream-netbsd/lib/libc/stdlib/lldiv.c \
    upstream-netbsd/lib/libc/stdlib/lrand48.c \
    upstream-netbsd/lib/libc/stdlib/mrand48.c \
    upstream-netbsd/lib/libc/stdlib/nrand48.c \
    upstream-netbsd/lib/libc/stdlib/_rand48.c \
    upstream-netbsd/lib/libc/stdlib/rand_r.c \
    upstream-netbsd/lib/libc/stdlib/reallocarr.c \
    upstream-netbsd/lib/libc/stdlib/seed48.c \
    upstream-netbsd/lib/libc/stdlib/srand48.c \
    upstream-netbsd/lib/libc/string/memccpy.c \
    upstream-netbsd/lib/libc/string/strcasestr.c \
    upstream-netbsd/lib/libc/string/strcoll.c \
    upstream-netbsd/lib/libc/string/strxfrm.c \

libc_upstream_openbsd_gdtoa_src_files := \
    upstream-openbsd/android/gdtoa_support.cpp \
    upstream-openbsd/lib/libc/gdtoa/dmisc.c \
    upstream-openbsd/lib/libc/gdtoa/dtoa.c \
    upstream-openbsd/lib/libc/gdtoa/gdtoa.c \
    upstream-openbsd/lib/libc/gdtoa/gethex.c \
    upstream-openbsd/lib/libc/gdtoa/gmisc.c \
    upstream-openbsd/lib/libc/gdtoa/hd_init.c \
    upstream-openbsd/lib/libc/gdtoa/hdtoa.c \
    upstream-openbsd/lib/libc/gdtoa/hexnan.c \
    upstream-openbsd/lib/libc/gdtoa/ldtoa.c \
    upstream-openbsd/lib/libc/gdtoa/misc.c \
    upstream-openbsd/lib/libc/gdtoa/smisc.c \
    upstream-openbsd/lib/libc/gdtoa/strtod.c \
    upstream-openbsd/lib/libc/gdtoa/strtodg.c \
    upstream-openbsd/lib/libc/gdtoa/strtof.c \
    upstream-openbsd/lib/libc/gdtoa/strtord.c \
    upstream-openbsd/lib/libc/gdtoa/sum.c \
    upstream-openbsd/lib/libc/gdtoa/ulp.c \

libc_upstream_openbsd_gdtoa_src_files_32 := \
    $(libc_upstream_openbsd_gdtoa_src_files) \

libc_upstream_openbsd_gdtoa_src_files_64 := \
    $(libc_upstream_openbsd_gdtoa_src_files) \
    upstream-openbsd/lib/libc/gdtoa/strtorQ.c \

# These two depend on getentropy_linux.c, which isn't in libc_ndk.a.
libc_upstream_openbsd_src_files := \
    upstream-openbsd/lib/libc/crypt/arc4random.c \
    upstream-openbsd/lib/libc/crypt/arc4random_uniform.c \

libc_upstream_openbsd_src_files += \
    upstream-openbsd/lib/libc/string/memchr.c \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/memrchr.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/stpncpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c \
    upstream-openbsd/lib/libc/string/strncmp.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

libc_upstream_openbsd_ndk_src_files := \
    upstream-openbsd/lib/libc/compat-43/killpg.c \
    upstream-openbsd/lib/libc/gen/alarm.c \
    upstream-openbsd/lib/libc/gen/ctype_.c \
    upstream-openbsd/lib/libc/gen/daemon.c \
    upstream-openbsd/lib/libc/gen/err.c \
    upstream-openbsd/lib/libc/gen/errx.c \
    upstream-openbsd/lib/libc/gen/exec.c \
    upstream-openbsd/lib/libc/gen/fnmatch.c \
    upstream-openbsd/lib/libc/gen/ftok.c \
    upstream-openbsd/lib/libc/gen/getprogname.c \
    upstream-openbsd/lib/libc/gen/isctype.c \
    upstream-openbsd/lib/libc/gen/setprogname.c \
    upstream-openbsd/lib/libc/gen/time.c \
    upstream-openbsd/lib/libc/gen/tolower_.c \
    upstream-openbsd/lib/libc/gen/toupper_.c \
    upstream-openbsd/lib/libc/gen/verr.c \
    upstream-openbsd/lib/libc/gen/verrx.c \
    upstream-openbsd/lib/libc/gen/vwarn.c \
    upstream-openbsd/lib/libc/gen/vwarnx.c \
    upstream-openbsd/lib/libc/gen/warn.c \
    upstream-openbsd/lib/libc/gen/warnx.c \
    upstream-openbsd/lib/libc/locale/btowc.c \
    upstream-openbsd/lib/libc/locale/mbrlen.c \
    upstream-openbsd/lib/libc/locale/mbstowcs.c \
    upstream-openbsd/lib/libc/locale/mbtowc.c \
    upstream-openbsd/lib/libc/locale/wcscoll.c \
    upstream-openbsd/lib/libc/locale/wcstod.c \
    upstream-openbsd/lib/libc/locale/wcstof.c \
    upstream-openbsd/lib/libc/locale/wcstoimax.c \
    upstream-openbsd/lib/libc/locale/wcstol.c \
    upstream-openbsd/lib/libc/locale/wcstold.c \
    upstream-openbsd/lib/libc/locale/wcstoll.c \
    upstream-openbsd/lib/libc/locale/wcstombs.c \
    upstream-openbsd/lib/libc/locale/wcstoul.c \
    upstream-openbsd/lib/libc/locale/wcstoull.c \
    upstream-openbsd/lib/libc/locale/wcstoumax.c \
    upstream-openbsd/lib/libc/locale/wcsxfrm.c \
    upstream-openbsd/lib/libc/locale/wctob.c \
    upstream-openbsd/lib/libc/locale/wctomb.c \
    upstream-openbsd/lib/libc/net/htonl.c \
    upstream-openbsd/lib/libc/net/htons.c \
    upstream-openbsd/lib/libc/net/inet_lnaof.c \
    upstream-openbsd/lib/libc/net/inet_makeaddr.c \
    upstream-openbsd/lib/libc/net/inet_netof.c \
    upstream-openbsd/lib/libc/net/inet_ntoa.c \
    upstream-openbsd/lib/libc/net/inet_ntop.c \
    upstream-openbsd/lib/libc/net/inet_pton.c \
    upstream-openbsd/lib/libc/net/ntohl.c \
    upstream-openbsd/lib/libc/net/ntohs.c \
    upstream-openbsd/lib/libc/net/res_random.c \
    upstream-openbsd/lib/libc/stdio/asprintf.c \
    upstream-openbsd/lib/libc/stdio/clrerr.c \
    upstream-openbsd/lib/libc/stdio/dprintf.c \
    upstream-openbsd/lib/libc/stdio/feof.c \
    upstream-openbsd/lib/libc/stdio/ferror.c \
    upstream-openbsd/lib/libc/stdio/fflush.c \
    upstream-openbsd/lib/libc/stdio/fgetc.c \
    upstream-openbsd/lib/libc/stdio/fgetln.c \
    upstream-openbsd/lib/libc/stdio/fgets.c \
    upstream-openbsd/lib/libc/stdio/fgetwc.c \
    upstream-openbsd/lib/libc/stdio/fgetws.c \
    upstream-openbsd/lib/libc/stdio/flags.c \
    upstream-openbsd/lib/libc/stdio/fmemopen.c \
    upstream-openbsd/lib/libc/stdio/fprintf.c \
    upstream-openbsd/lib/libc/stdio/fpurge.c \
    upstream-openbsd/lib/libc/stdio/fputc.c \
    upstream-openbsd/lib/libc/stdio/fputs.c \
    upstream-openbsd/lib/libc/stdio/fputwc.c \
    upstream-openbsd/lib/libc/stdio/fputws.c \
    upstream-openbsd/lib/libc/stdio/fscanf.c \
    upstream-openbsd/lib/libc/stdio/fvwrite.c \
    upstream-openbsd/lib/libc/stdio/fwalk.c \
    upstream-openbsd/lib/libc/stdio/fwide.c \
    upstream-openbsd/lib/libc/stdio/fwprintf.c \
    upstream-openbsd/lib/libc/stdio/fwrite.c \
    upstream-openbsd/lib/libc/stdio/fwscanf.c \
    upstream-openbsd/lib/libc/stdio/getc.c \
    upstream-openbsd/lib/libc/stdio/getchar.c \
    upstream-openbsd/lib/libc/stdio/getdelim.c \
    upstream-openbsd/lib/libc/stdio/getline.c \
    upstream-openbsd/lib/libc/stdio/gets.c \
    upstream-openbsd/lib/libc/stdio/getwc.c \
    upstream-openbsd/lib/libc/stdio/getwchar.c \
    upstream-openbsd/lib/libc/stdio/makebuf.c \
    upstream-openbsd/lib/libc/stdio/mktemp.c \
    upstream-openbsd/lib/libc/stdio/open_memstream.c \
    upstream-openbsd/lib/libc/stdio/open_wmemstream.c \
    upstream-openbsd/lib/libc/stdio/perror.c \
    upstream-openbsd/lib/libc/stdio/printf.c \
    upstream-openbsd/lib/libc/stdio/putc.c \
    upstream-openbsd/lib/libc/stdio/putchar.c \
    upstream-openbsd/lib/libc/stdio/puts.c \
    upstream-openbsd/lib/libc/stdio/putwc.c \
    upstream-openbsd/lib/libc/stdio/putwchar.c \
    upstream-openbsd/lib/libc/stdio/remove.c \
    upstream-openbsd/lib/libc/stdio/rewind.c \
    upstream-openbsd/lib/libc/stdio/rget.c \
    upstream-openbsd/lib/libc/stdio/scanf.c \
    upstream-openbsd/lib/libc/stdio/setbuf.c \
    upstream-openbsd/lib/libc/stdio/setbuffer.c \
    upstream-openbsd/lib/libc/stdio/setvbuf.c \
    upstream-openbsd/lib/libc/stdio/sscanf.c \
    upstream-openbsd/lib/libc/stdio/swprintf.c \
    upstream-openbsd/lib/libc/stdio/swscanf.c \
    upstream-openbsd/lib/libc/stdio/tempnam.c \
    upstream-openbsd/lib/libc/stdio/tmpnam.c \
    upstream-openbsd/lib/libc/stdio/ungetc.c \
    upstream-openbsd/lib/libc/stdio/ungetwc.c \
    upstream-openbsd/lib/libc/stdio/vasprintf.c \
    upstream-openbsd/lib/libc/stdio/vdprintf.c \
    upstream-openbsd/lib/libc/stdio/vfprintf.c \
    upstream-openbsd/lib/libc/stdio/vfscanf.c \
    upstream-openbsd/lib/libc/stdio/vfwprintf.c \
    upstream-openbsd/lib/libc/stdio/vfwscanf.c \
    upstream-openbsd/lib/libc/stdio/vprintf.c \
    upstream-openbsd/lib/libc/stdio/vscanf.c \
    upstream-openbsd/lib/libc/stdio/vsnprintf.c \
    upstream-openbsd/lib/libc/stdio/vsprintf.c \
    upstream-openbsd/lib/libc/stdio/vsscanf.c \
    upstream-openbsd/lib/libc/stdio/vswprintf.c \
    upstream-openbsd/lib/libc/stdio/vswscanf.c \
    upstream-openbsd/lib/libc/stdio/vwprintf.c \
    upstream-openbsd/lib/libc/stdio/vwscanf.c \
    upstream-openbsd/lib/libc/stdio/wbuf.c \
    upstream-openbsd/lib/libc/stdio/wprintf.c \
    upstream-openbsd/lib/libc/stdio/wscanf.c \
    upstream-openbsd/lib/libc/stdio/wsetup.c \
    upstream-openbsd/lib/libc/stdlib/abs.c \
    upstream-openbsd/lib/libc/stdlib/atoi.c \
    upstream-openbsd/lib/libc/stdlib/atol.c \
    upstream-openbsd/lib/libc/stdlib/atoll.c \
    upstream-openbsd/lib/libc/stdlib/getenv.c \
    upstream-openbsd/lib/libc/stdlib/insque.c \
    upstream-openbsd/lib/libc/stdlib/imaxabs.c \
    upstream-openbsd/lib/libc/stdlib/imaxdiv.c \
    upstream-openbsd/lib/libc/stdlib/labs.c \
    upstream-openbsd/lib/libc/stdlib/llabs.c \
    upstream-openbsd/lib/libc/stdlib/lsearch.c \
    upstream-openbsd/lib/libc/stdlib/reallocarray.c \
    upstream-openbsd/lib/libc/stdlib/remque.c \
    upstream-openbsd/lib/libc/stdlib/setenv.c \
    upstream-openbsd/lib/libc/stdlib/strtoimax.c \
    upstream-openbsd/lib/libc/stdlib/strtol.c \
    upstream-openbsd/lib/libc/stdlib/strtoll.c \
    upstream-openbsd/lib/libc/stdlib/strtoul.c \
    upstream-openbsd/lib/libc/stdlib/strtoull.c \
    upstream-openbsd/lib/libc/stdlib/strtoumax.c \
    upstream-openbsd/lib/libc/stdlib/system.c \
    upstream-openbsd/lib/libc/stdlib/tfind.c \
    upstream-openbsd/lib/libc/stdlib/tsearch.c \
    upstream-openbsd/lib/libc/string/strcasecmp.c \
    upstream-openbsd/lib/libc/string/strcspn.c \
    upstream-openbsd/lib/libc/string/strdup.c \
    upstream-openbsd/lib/libc/string/strndup.c \
    upstream-openbsd/lib/libc/string/strpbrk.c \
    upstream-openbsd/lib/libc/string/strsep.c \
    upstream-openbsd/lib/libc/string/strspn.c \
    upstream-openbsd/lib/libc/string/strstr.c \
    upstream-openbsd/lib/libc/string/strtok.c \
    upstream-openbsd/lib/libc/string/wmemcpy.c \
    upstream-openbsd/lib/libc/string/wcslcpy.c \
    upstream-openbsd/lib/libc/string/wcsstr.c \
    upstream-openbsd/lib/libc/string/wcswidth.c \

libc_pthread_src_files := \
    bionic/pthread_atfork.cpp \
    bionic/pthread_attr.cpp \
    bionic/pthread_barrier.cpp \
    bionic/pthread_cond.cpp \
    bionic/pthread_create.cpp \
    bionic/pthread_detach.cpp \
    bionic/pthread_equal.cpp \
    bionic/pthread_exit.cpp \
    bionic/pthread_getcpuclockid.cpp \
    bionic/pthread_getschedparam.cpp \
    bionic/pthread_gettid_np.cpp \
    bionic/pthread_internal.cpp \
    bionic/pthread_join.cpp \
    bionic/pthread_key.cpp \
    bionic/pthread_kill.cpp \
    bionic/pthread_mutex.cpp \
    bionic/pthread_once.cpp \
    bionic/pthread_rwlock.cpp \
    bionic/pthread_self.cpp \
    bionic/pthread_setname_np.cpp \
    bionic/pthread_setschedparam.cpp \
    bionic/pthread_sigmask.cpp \
    bionic/pthread_spinlock.cpp \

libc_arch_static_src_files := \
    bionic/dl_iterate_phdr_static.cpp \

# Various kinds of cruft.
# ========================================================
libc_common_src_files += \
    bionic/ndk_cruft.cpp \

libc_bionic_ndk_src_files_32 += \
    bionic/mmap.cpp \

libc_common_src_files_32 += \
    bionic/legacy_32_bit_support.cpp \
    bionic/time64.c \

libc_netbsd_src_files_32 += \
    upstream-netbsd/common/lib/libc/hash/sha1/sha1.c \

libc_openbsd_src_files_32 += \
    upstream-openbsd/lib/libc/stdio/putw.c \


# Define some common cflags
# ========================================================
libc_common_cflags := \
    -D_LIBC=1 \
    -Wall -Wextra -Wunused \

use_clang := $(USE_CLANG_PLATFORM_BUILD)

# Clang/llvm has incompatible long double (fp128) for x86_64.
# https://llvm.org/bugs/show_bug.cgi?id=23897
ifeq ($(TARGET_ARCH),x86_64)
  use_clang := false
endif

# b/25291096, Clang/llvm compiled libc.so for mips/mips64 failed to boot.
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),mips mips64))
  use_clang := false
endif

ifeq ($(use_clang),)
  use_clang := true
endif

# Try to catch typical 32-bit assumptions that break with 64-bit pointers.
libc_common_cflags += \
    -Werror=pointer-to-int-cast \
    -Werror=int-to-pointer-cast \
    -Werror=type-limits \
    -Werror \

ifeq ($(strip $(DEBUG_BIONIC_LIBC)),true)
  libc_common_cflags += -DDEBUG
endif

libc_malloc_src := bionic/jemalloc_wrapper.cpp
libc_common_c_includes += external/jemalloc/include

# Define some common conlyflags
libc_common_conlyflags := \
    -std=gnu99

# Define some common cppflags
libc_common_cppflags := \

# Define some common includes
# ========================================================
libc_common_c_includes += \
    $(LOCAL_PATH)/stdio   \

# ========================================================
# Add in the arch or 32-bit specific flags
# Must be called with $(eval).
# $(1): the LOCAL_ variable name
# $(2): the bionic variable name to pull in
define patch-up-arch-specific-flags
$(1)_$(TARGET_ARCH) += $($(2)_$(TARGET_ARCH))
$(1)_32 += $($(2)_32)
ifdef TARGET_2ND_ARCH
$(1)_$(TARGET_2ND_ARCH) += $($(2)_$(TARGET_2ND_ARCH))
endif
endef


# ========================================================
# libc_stack_protector.a - stack protector code
# ========================================================
#
# Code that implements the stack protector (or that runs
# before TLS has been set up) needs to be compiled with
# -fno-stack-protector, since it accesses the stack canary
# TLS slot.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    bionic/__libc_init_main_thread.cpp \
    bionic/__stack_chk_fail.cpp \

LOCAL_SRC_FILES_arm64 := arch-arm64/bionic/__set_tls.c
LOCAL_SRC_FILES_x86 := arch-x86/bionic/__set_tls.c
LOCAL_SRC_FILES_x86_64 := arch-x86_64/bionic/__set_tls.c

LOCAL_CFLAGS := $(libc_common_cflags) -fno-stack-protector
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_stack_protector
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# libc_init_static.cpp also needs to be built without stack protector,
# because it's responsible for setting up TLS for static executables.
# This isn't the case for dynamic executables because the dynamic linker
# has already set up the main thread's TLS.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := bionic/libc_init_static.cpp
LOCAL_CFLAGS := $(libc_common_cflags) -fno-stack-protector
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_init_static
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_tzcode.a - upstream 'tzcode' code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-c-files-under,tzcode)
# tzcode doesn't include wcsftime, so we use the OpenBSD one.
LOCAL_SRC_FILES += upstream-openbsd/lib/libc/time/wcsftime.c

LOCAL_CFLAGS := $(libc_common_cflags) \
    -fvisibility=hidden \
    -Wno-unused-parameter \

# Don't use ridiculous amounts of stack.
LOCAL_CFLAGS += -DALL_STATE
# Include tzsetwall, timelocal, timegm, time2posix, and posix2time.
LOCAL_CFLAGS += -DSTD_INSPIRED
# Obviously, we want to be thread-safe.
LOCAL_CFLAGS += -DTHREAD_SAFE
# The name of the tm_gmtoff field in our struct tm.
LOCAL_CFLAGS += -DTM_GMTOFF=tm_gmtoff
# Where we store our tzdata.
LOCAL_CFLAGS += -DTZDIR=\"/system/usr/share/zoneinfo\"
# Include timezone and daylight globals.
LOCAL_CFLAGS += -DUSG_COMPAT=1
# Use the empty string (instead of "   ") as the timezone abbreviation fallback.
LOCAL_CFLAGS += -DWILDABBR=\"\"
LOCAL_CFLAGS += -DNO_RUN_TIME_WARNINGS_ABOUT_YEAR_2000_PROBLEMS_THANK_YOU
LOCAL_CFLAGS += -Dlint

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) $(LOCAL_PATH)/tzcode/
LOCAL_MODULE := libc_tzcode
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_dns.a - modified NetBSD DNS code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(call all-c-files-under,dns) \
    upstream-netbsd/lib/libc/isc/ev_streams.c \
    upstream-netbsd/lib/libc/isc/ev_timers.c \
    upstream-netbsd/lib/libc/resolv/mtctxres.c \

LOCAL_CFLAGS += \
    $(libc_common_cflags) \
    -DANDROID_CHANGES \
    -DINET6 \
    -fvisibility=hidden \
    -Wno-unused-parameter \
    -include netbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) \
    $(LOCAL_PATH)/dns/include \
    $(LOCAL_PATH)/private \
    $(LOCAL_PATH)/upstream-netbsd/lib/libc/include \
    $(LOCAL_PATH)/upstream-netbsd/android/include \

LOCAL_MODULE := libc_dns
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_freebsd.a - upstream FreeBSD C library code
# ========================================================
#
# These files are built with the freebsd-compat.h header file
# automatically included.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_upstream_freebsd_src_files)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare -Wno-uninitialized \
    -include freebsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) \
    $(LOCAL_PATH)/upstream-freebsd/android/include \
    $(LOCAL_PATH)/upstream-freebsd/lib/libc/include \

LOCAL_MODULE := libc_freebsd
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES_EXCLUDE,libc_freebsd_src_files_exclude))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_netbsd.a - upstream NetBSD C library code
# ========================================================
#
# These files are built with the netbsd-compat.h header file
# automatically included.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_upstream_netbsd_src_files)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare \
    -Wno-uninitialized \
    -Wno-unused-parameter \
    -DPOSIX_MISTAKE \
    -include netbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) \
    $(LOCAL_PATH)/upstream-netbsd/android/include \
    $(LOCAL_PATH)/upstream-netbsd/lib/libc/include \

LOCAL_MODULE := libc_netbsd
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_netbsd_src_files))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_openbsd_ndk.a - upstream OpenBSD C library code
# that can be safely included in the libc_ndk.a (doesn't
# contain any troublesome global data or constructors).
# ========================================================
#
# These files are built with the openbsd-compat.h header file
# automatically included.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_upstream_openbsd_ndk_src_files)
LOCAL_CLANG := $(use_clang)

LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare \
    -Wno-uninitialized \
    -Wno-unused-parameter \
    -include openbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) \
    $(LOCAL_PATH)/private \
    $(LOCAL_PATH)/upstream-openbsd/android/include \
    $(LOCAL_PATH)/upstream-openbsd/lib/libc/include \
    $(LOCAL_PATH)/upstream-openbsd/lib/libc/gdtoa/ \

LOCAL_MODULE := libc_openbsd_ndk
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_openbsd.a - upstream OpenBSD C library code
# ========================================================
#
# These files are built with the openbsd-compat.h header file
# automatically included.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_upstream_openbsd_src_files)
LOCAL_CLANG := $(use_clang)

LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare \
    -Wno-uninitialized \
    -Wno-unused-parameter \
    -include openbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) \
    $(LOCAL_PATH)/private \
    $(LOCAL_PATH)/upstream-openbsd/android/include \
    $(LOCAL_PATH)/upstream-openbsd/lib/libc/include \
    $(LOCAL_PATH)/upstream-openbsd/lib/libc/gdtoa/ \

LOCAL_MODULE := libc_openbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_openbsd_src_files))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES_EXCLUDE,libc_openbsd_src_files_exclude))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_gdtoa.a - upstream OpenBSD C library gdtoa code
# ========================================================
#
# These files are built with the openbsd-compat.h header file
# automatically included.

include $(CLEAR_VARS)

LOCAL_SRC_FILES_32 := $(libc_upstream_openbsd_gdtoa_src_files_32)
LOCAL_SRC_FILES_64 := $(libc_upstream_openbsd_gdtoa_src_files_64)
LOCAL_CLANG := $(use_clang)

LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare -Wno-uninitialized \
    -fvisibility=hidden \
    -include openbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) \
    $(LOCAL_PATH)/private \
    $(LOCAL_PATH)/upstream-openbsd/android/include \
    $(LOCAL_PATH)/upstream-openbsd/lib/libc/include \

LOCAL_MODULE := libc_gdtoa
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_bionic.a - home-grown C library code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_bionic_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -Wframe-larger-than=2048 \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags) -Wold-style-cast
LOCAL_C_INCLUDES := $(libc_common_c_includes) bionic/libstdc++/include
LOCAL_MODULE := libc_bionic
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_bionic_src_files))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES_EXCLUDE,libc_bionic_src_files_exclude))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_bionic_ndk.a - The portions of libc_bionic that can
# be safely used in libc_ndk.a (no troublesome global data
# or constructors).
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_bionic_ndk_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -Wframe-larger-than=2048 \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags) -Wold-style-cast
LOCAL_C_INCLUDES := $(libc_common_c_includes) bionic/libstdc++/include
LOCAL_MODULE := libc_bionic_ndk
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_bionic_ndk_src_files))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_pthread.a - pthreads parts that previously lived in
# libc_bionic.a. Relocated to their own library because
# they can't be included in libc_ndk.a (as they layout of
# pthread_t has changed over the years and has ABI
# compatibility issues).
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_pthread_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -Wframe-larger-than=2048 \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags) -Wold-style-cast
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_pthread
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_cxa.a - Things traditionally in libstdc++
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_cxa_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -fvisibility=hidden \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) bionic/libstdc++/include
LOCAL_MODULE := libc_cxa
LOCAL_CLANG := true # GCC refuses to hide new/delete
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
# b/17574078: Need to disable coverage until we have a prebuilt libprofile_rt.
# Since this is a static library built with clang, it needs to link
# libprofile_rt when it is linked into the final binary. Since the final binary
# is built with GCC, it won't link libprofile_rt. We can't very easily just add
# libprofile_rt to all link lines the way we've done for libgcov because
# libprofile_rt isn't prebuilt, and it would be tricky to write a rule that
# would make sure libprofile_rt is built.
LOCAL_NATIVE_COVERAGE := false

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_syscalls.a
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES_$(TARGET_ARCH) := $(call all-S-files-under,arch-$(TARGET_ARCH)/syscalls)
ifdef TARGET_2ND_ARCH
LOCAL_SRC_FILES_$(TARGET_2ND_ARCH) := $(call all-S-files-under,arch-$(TARGET_2ND_ARCH)/syscalls)
endif
LOCAL_MODULE := libc_syscalls
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_aeabi.a
# This is an LP32 ARM-only library that needs to be built with -fno-builtin
# to avoid infinite recursion. For the other architectures we just build an
# empty library to keep this makefile simple.
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES_arm := arch-arm/bionic/__aeabi.c
LOCAL_MODULE := libc_aeabi
LOCAL_CLANG := $(use_clang)
LOCAL_CFLAGS := $(libc_common_cflags) -fno-builtin
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

include $(BUILD_STATIC_LIBRARY)

# ========================================================
# libc_ndk.a
# Compatibility library for the NDK. This library contains
# all the parts of libc that are safe to statically link.
# We can't safely statically link things that can only run
# on a certain version of the OS. Examples include
# anything that talks to netd (a large portion of the DNS
# code) and anything that is dependent on the layout of a
# data structure that has changed across releases (such as
# pthread_t).
# ========================================================

include $(CLEAR_VARS)

LOCAL_MODULE := libc_ndk
LOCAL_CLANG := $(use_clang)
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CFLAGS := $(libc_common_cflags) -fvisibility=hidden -O0
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

LOCAL_SRC_FILES := \
    $(libc_common_src_files) \
    $(libc_arch_dynamic_src_files) \
    $(libc_ndk_stub_src_files) \
    bionic/malloc_common.cpp \

LOCAL_SRC_FILES_arm += \
    arch-common/bionic/crtbegin_so.c \
    arch-arm/bionic/atexit_legacy.c \
    arch-common/bionic/crtend_so.S \

LOCAL_CFLAGS := $(libc_common_cflags) \
    -DLIBC_STATIC \

LOCAL_WHOLE_STATIC_LIBRARIES := \
    libc_bionic_ndk \
    libc_cxa \
    libc_freebsd \
    libc_gdtoa \
    libc_malloc \
    libc_netbsd \
    libc_openbsd_ndk \
    libc_stack_protector \
    libc_syscalls \
    libc_tzcode \
    libm \
    libjemalloc \

LOCAL_WHOLE_STATIC_LIBRARIES_arm := libc_aeabi
LOCAL_CXX_STL := none

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_common_src_files))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_dynamic_src_files))
$(eval $(call patch-up-arch-specific-flags,LOCAL_ASFLAGS,LOCAL_CFLAGS))

LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
include $(BUILD_STATIC_LIBRARY)

# ========================================================
# libc_common.a
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_common_src_files)
LOCAL_CFLAGS := $(libc_common_cflags)
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_common
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := \
    libc_bionic \
    libc_bionic_ndk \
    libc_cxa \
    libc_dns \
    libc_freebsd \
    libc_gdtoa \
    libc_malloc \
    libc_netbsd \
    libc_openbsd \
    libc_openbsd_ndk \
    libc_pthread \
    libc_stack_protector \
    libc_syscalls \
    libc_tzcode \

LOCAL_WHOLE_STATIC_LIBRARIES_arm := libc_aeabi

LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=

# TODO: split out the asflags.
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_common_src_files))
$(eval $(call patch-up-arch-specific-flags,LOCAL_ASFLAGS,LOCAL_CFLAGS))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_nomalloc.a
# ========================================================
#
# This is a version of the static C library that does not
# include malloc. It's useful in situations when the user wants
# to provide their own malloc implementation, or wants to
# explicitly disallow the use of malloc, such as in the
# dynamic linker.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(libc_arch_static_src_files) \

LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -DLIBC_STATIC \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_MODULE := libc_nomalloc
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common libc_init_static
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_static_src_files))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_malloc.a: the _prefixed_ malloc functions (like dlcalloc).
# ========================================================

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(libc_malloc_src)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -fvisibility=hidden \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_malloc
LOCAL_CLANG := $(use_clang)
LOCAL_CXX_STL := none
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(libc_arch_static_src_files) \
    bionic/malloc_common.cpp \
    bionic/libc_init_static.cpp \

LOCAL_CFLAGS := $(libc_common_cflags) \
    -DLIBC_STATIC \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc
LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common libc_init_static libjemalloc

LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_static_src_files))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc.so
# ========================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(libc_common_cflags)
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_SRC_FILES := \
    arch-common/bionic/crtbegin_so.c \
    arch-common/bionic/crtbrand.S \
    $(libc_arch_dynamic_src_files) \
    bionic/malloc_common.cpp \
    bionic/libc_init_dynamic.cpp \
    bionic/NetdClient.cpp \
    arch-common/bionic/crtend_so.S \

LOCAL_MODULE := libc
LOCAL_CLANG := $(use_clang)
LOCAL_REQUIRED_MODULES := tzdata
LOCAL_ADDITIONAL_DEPENDENCIES := \
    $(libc_common_additional_dependencies) \
    $(LOCAL_PATH)/libc.arm.map \
    $(LOCAL_PATH)/libc.arm64.map \
    $(LOCAL_PATH)/libc.mips.map \
    $(LOCAL_PATH)/libc.mips64.map \
    $(LOCAL_PATH)/libc.x86.map \
    $(LOCAL_PATH)/libc.x86_64.map \
    $(LOCAL_PATH)/libc.arm.brillo.map \
    $(LOCAL_PATH)/libc.mips.brillo.map \
    $(LOCAL_PATH)/libc.x86.brillo.map \

# Leave the symbols in the shared library so that stack unwinders can produce
# meaningful name resolution.
LOCAL_STRIP_MODULE := keep_symbols

# Do not pack libc.so relocations; see http://b/20645321 for details.
LOCAL_PACK_MODULE_RELOCATIONS := false

# WARNING: The only library libc.so should depend on is libdl.so!  If you add other libraries,
# make sure to add -Wl,--exclude-libs=libgcc.a to the LOCAL_LDFLAGS for those libraries.  This
# ensures that symbols that are pulled into those new libraries from libgcc.a are not declared
# external; if that were the case, then libc would not pull those symbols from libgcc.a as it
# should, instead relying on the external symbols from the dependent libraries.  That would
# create a "cloaked" dependency on libgcc.a in libc though the libraries, which is not what
# you wanted!

LOCAL_SHARED_LIBRARIES := libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common libjemalloc

LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES :=

# TODO: This is to work around b/24465209. Remove after root cause is fixed
LOCAL_LDFLAGS_arm := -Wl,--hash-style=both
LOCAL_LDFLAGS_x86 := -Wl,--hash-style=both

# Don't re-export new/delete and friends, even if the compiler really wants to.
ifdef BRILLO
LOCAL_LDFLAGS_arm    += -Wl,--version-script,$(LOCAL_PATH)/libc.arm.brillo.map
LOCAL_LDFLAGS_mips   += -Wl,--version-script,$(LOCAL_PATH)/libc.mips.brillo.map
LOCAL_LDFLAGS_x86    += -Wl,--version-script,$(LOCAL_PATH)/libc.x86.brillo.map
else
LOCAL_LDFLAGS_arm    += -Wl,--version-script,$(LOCAL_PATH)/libc.arm.map
LOCAL_LDFLAGS_mips   += -Wl,--version-script,$(LOCAL_PATH)/libc.mips.map
LOCAL_LDFLAGS_x86    += -Wl,--version-script,$(LOCAL_PATH)/libc.x86.map
endif

LOCAL_LDFLAGS_arm64  += -Wl,--version-script,$(LOCAL_PATH)/libc.arm64.map
LOCAL_LDFLAGS_mips64 += -Wl,--version-script,$(LOCAL_PATH)/libc.mips64.map
LOCAL_LDFLAGS_x86_64 += -Wl,--version-script,$(LOCAL_PATH)/libc.x86_64.map

# We'd really like to do this for all architectures, but since this wasn't done
# before, these symbols must continue to be exported on LP32 for binary
# compatibility.
LOCAL_LDFLAGS_64 := -Wl,--exclude-libs,libgcc.a

# Unfortunately --exclude-libs clobbers our version script, so we have to
# prevent the build system from using this flag.
LOCAL_NO_EXCLUDE_LIBS := true

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_dynamic_src_files))

LOCAL_NO_CRT := true
LOCAL_ASFLAGS += $(libc_crt_target_cflags)

# special for arm
LOCAL_CFLAGS_arm += -DCRT_LEGACY_WORKAROUND
LOCAL_SRC_FILES_arm += \
    arch-arm/bionic/atexit_legacy.c

LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

include $(BUILD_SHARED_LIBRARY)

# ========================================================
# libc_logging.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(libc_common_cflags)
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
    bionic/libc_logging.cpp \

LOCAL_MODULE := libc_logging

LOCAL_CLANG := $(use_clang)
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)

# ========================================================
# libstdc++.so
# ========================================================
libstdcxx_common_src_files := \
    bionic/__cxa_guard.cpp \
    bionic/__cxa_pure_virtual.cpp \
    bionic/new.cpp \

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(libc_common_c_includes) bionic/libstdc++/include
LOCAL_CFLAGS := $(libc_common_cflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

# TODO: This is to work around b/24465209. Remove after root cause is fixed
LOCAL_LDFLAGS_arm := -Wl,--hash-style=both
LOCAL_LDFLAGS_x86 := -Wl,--hash-style=both

LOCAL_SRC_FILES := $(libstdcxx_common_src_files)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
LOCAL_STATIC_LIBRARIES := libc_logging
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)
include $(BUILD_SHARED_LIBRARY)

# ========================================================
# libstdc++.a
# ========================================================
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(libc_common_c_includes) bionic/libstdc++/include
LOCAL_CFLAGS := $(libc_common_cflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_SRC_FILES := $(libstdcxx_common_src_files)
LOCAL_MODULE:= libstdc++
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_CXX_STL := none
LOCAL_SYSTEM_SHARED_LIBRARIES := libc
LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := $(bionic_coverage)
include $(BUILD_STATIC_LIBRARY)
