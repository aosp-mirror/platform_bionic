LOCAL_PATH := $(call my-dir)

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
    bionic/arc4random.c \
    bionic/bindresvport.c \
    bionic/daemon.c \
    bionic/err.c \
    bionic/ether_aton.c \
    bionic/ether_ntoa.c \
    bionic/fdprintf.c \
    bionic/ftime.c \
    bionic/fts.c \
    bionic/getdtablesize.c \
    bionic/gethostname.c \
    bionic/getpriority.c \
    bionic/getpt.c \
    bionic/if_indextoname.c \
    bionic/if_nametoindex.c \
    bionic/initgroups.c \
    bionic/ioctl.c \
    bionic/isatty.c \
    bionic/issetugid.c \
    bionic/md5.c \
    bionic/memmem.c \
    bionic/pathconf.c \
    bionic/ptsname.c \
    bionic/ptsname_r.c \
    bionic/pututline.c \
    bionic/sched_cpualloc.c \
    bionic/sched_cpucount.c \
    bionic/semaphore.c \
    bionic/sigblock.c \
    bionic/siginterrupt.c \
    bionic/sigsetmask.c \
    bionic/system_properties_compat.c \
    bionic/unlockpt.c \
    stdio/snprintf.c\
    stdio/sprintf.c \
    stdlib/atexit.c \
    unistd/syslog.c \

# Fortify implementations of libc functions.
libc_common_src_files += \
    bionic/__FD_chk.cpp \
    bionic/__fgets_chk.cpp \
    bionic/__memmove_chk.cpp \
    bionic/__read_chk.cpp \
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

libc_bionic_src_files := \
    bionic/abort.cpp \
    bionic/accept.cpp \
    bionic/access.cpp \
    bionic/assert.cpp \
    bionic/atof.cpp \
    bionic/__bionic_name_mem.cpp \
    bionic/bionic_time_conversions.cpp \
    bionic/brk.cpp \
    bionic/chmod.cpp \
    bionic/chown.cpp \
    bionic/clearenv.cpp \
    bionic/clock.cpp \
    bionic/clone.cpp \
    bionic/cmsg_nxthdr.cpp \
    bionic/connect.cpp \
    bionic/__cxa_guard.cpp \
    bionic/__cxa_pure_virtual.cpp \
    bionic/dirent.cpp \
    bionic/dup2.cpp \
    bionic/epoll_create.cpp \
    bionic/epoll_pwait.cpp \
    bionic/epoll_wait.cpp \
    bionic/__errno.cpp \
    bionic/eventfd_read.cpp \
    bionic/eventfd_write.cpp \
    bionic/ffs.cpp \
    bionic/flockfile.cpp \
    bionic/fork.cpp \
    bionic/fpclassify.cpp \
    bionic/futimens.cpp \
    bionic/getauxval.cpp \
    bionic/getcwd.cpp \
    bionic/getpgrp.cpp \
    bionic/inotify_init.cpp \
    bionic/lchown.cpp \
    bionic/lfs64_support.cpp \
    bionic/__libc_current_sigrtmax.cpp \
    bionic/__libc_current_sigrtmin.cpp \
    bionic/libc_init_common.cpp \
    bionic/libc_logging.cpp \
    bionic/libgen.cpp \
    bionic/link.cpp \
    bionic/locale.cpp \
    bionic/lstat.cpp \
    bionic/mkdir.cpp \
    bionic/mkfifo.cpp \
    bionic/mknod.cpp \
    bionic/mntent.cpp \
    bionic/NetdClientDispatch.cpp \
    bionic/new.cpp \
    bionic/open.cpp \
    bionic/pause.cpp \
    bionic/pipe.cpp \
    bionic/poll.cpp \
    bionic/posix_fallocate.cpp \
    bionic/posix_timers.cpp \
    bionic/pthread_atfork.cpp \
    bionic/pthread_attr.cpp \
    bionic/pthread_cond.cpp \
    bionic/pthread_create.cpp \
    bionic/pthread_detach.cpp \
    bionic/pthread_equal.cpp \
    bionic/pthread_exit.cpp \
    bionic/pthread_getcpuclockid.cpp \
    bionic/pthread_getschedparam.cpp \
    bionic/pthread_internals.cpp \
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
    bionic/ptrace.cpp \
    bionic/raise.cpp \
    bionic/readlink.cpp \
    bionic/reboot.cpp \
    bionic/recv.cpp \
    bionic/rename.cpp \
    bionic/rmdir.cpp \
    bionic/scandir.cpp \
    bionic/sched_getaffinity.cpp \
    bionic/sched_getcpu.cpp \
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
    bionic/sigsuspend.cpp \
    bionic/sigwait.cpp \
    bionic/stat.cpp \
    bionic/statvfs.cpp \
    bionic/strerror.cpp \
    bionic/strerror_r.cpp \
    bionic/strsignal.cpp \
    bionic/strtold.cpp \
    bionic/stubs.cpp \
    bionic/symlink.cpp \
    bionic/sysconf.cpp \
    bionic/sys_siglist.c \
    bionic/sys_signame.c \
    bionic/system_properties.cpp \
    bionic/tdestroy.cpp \
    bionic/termios.cpp \
    bionic/thread_atexit.cpp \
    bionic/tmpfile.cpp \
    bionic/typeinfo.cpp \
    bionic/umount.cpp \
    bionic/unlink.cpp \
    bionic/utimes.cpp \
    bionic/wait.cpp \
    bionic/wchar.cpp \
    bionic/wctype.cpp \

libc_upstream_freebsd_src_files := \
    upstream-freebsd/lib/libc/gen/ldexp.c \
    upstream-freebsd/lib/libc/gen/sleep.c \
    upstream-freebsd/lib/libc/gen/usleep.c \
    upstream-freebsd/lib/libc/stdio/fclose.c \
    upstream-freebsd/lib/libc/stdio/flags.c \
    upstream-freebsd/lib/libc/stdio/fopen.c \
    upstream-freebsd/lib/libc/stdio/mktemp.c \
    upstream-freebsd/lib/libc/stdlib/abs.c \
    upstream-freebsd/lib/libc/stdlib/getopt_long.c \
    upstream-freebsd/lib/libc/stdlib/imaxabs.c \
    upstream-freebsd/lib/libc/stdlib/imaxdiv.c \
    upstream-freebsd/lib/libc/stdlib/labs.c \
    upstream-freebsd/lib/libc/stdlib/llabs.c \
    upstream-freebsd/lib/libc/stdlib/qsort.c \
    upstream-freebsd/lib/libc/stdlib/quick_exit.c \
    upstream-freebsd/lib/libc/stdlib/realpath.c \
    upstream-freebsd/lib/libc/string/wcpcpy.c \
    upstream-freebsd/lib/libc/string/wcpncpy.c \
    upstream-freebsd/lib/libc/string/wcscasecmp.c \
    upstream-freebsd/lib/libc/string/wcscspn.c \
    upstream-freebsd/lib/libc/string/wcsdup.c \
    upstream-freebsd/lib/libc/string/wcslcat.c \
    upstream-freebsd/lib/libc/string/wcsncasecmp.c \
    upstream-freebsd/lib/libc/string/wcsncat.c \
    upstream-freebsd/lib/libc/string/wcsncmp.c \
    upstream-freebsd/lib/libc/string/wcsncpy.c \
    upstream-freebsd/lib/libc/string/wcsnlen.c \
    upstream-freebsd/lib/libc/string/wcspbrk.c \
    upstream-freebsd/lib/libc/string/wcsspn.c \
    upstream-freebsd/lib/libc/string/wcstok.c \
    upstream-freebsd/lib/libc/string/wmemchr.c \
    upstream-freebsd/lib/libc/string/wmemcpy.c \
    upstream-freebsd/lib/libc/string/wmemmove.c \
    upstream-freebsd/lib/libc/string/wmemset.c \

libc_upstream_netbsd_src_files := \
    upstream-netbsd/common/lib/libc/hash/sha1/sha1.c \
    upstream-netbsd/lib/libc/gen/ftw.c \
    upstream-netbsd/lib/libc/gen/nftw.c \
    upstream-netbsd/lib/libc/gen/nice.c \
    upstream-netbsd/lib/libc/gen/popen.c \
    upstream-netbsd/lib/libc/gen/psignal.c \
    upstream-netbsd/lib/libc/gen/setjmperr.c \
    upstream-netbsd/lib/libc/gen/utime.c \
    upstream-netbsd/lib/libc/gen/utmp.c \
    upstream-netbsd/lib/libc/inet/inet_ntop.c \
    upstream-netbsd/lib/libc/isc/ev_streams.c \
    upstream-netbsd/lib/libc/isc/ev_timers.c \
    upstream-netbsd/lib/libc/regex/regcomp.c \
    upstream-netbsd/lib/libc/regex/regerror.c \
    upstream-netbsd/lib/libc/regex/regexec.c \
    upstream-netbsd/lib/libc/regex/regfree.c \
    upstream-netbsd/lib/libc/stdlib/bsearch.c \
    upstream-netbsd/lib/libc/stdlib/div.c \
    upstream-netbsd/lib/libc/stdlib/drand48.c \
    upstream-netbsd/lib/libc/stdlib/erand48.c \
    upstream-netbsd/lib/libc/stdlib/jrand48.c \
    upstream-netbsd/lib/libc/stdlib/ldiv.c \
    upstream-netbsd/lib/libc/stdlib/lldiv.c \
    upstream-netbsd/lib/libc/stdlib/lrand48.c \
    upstream-netbsd/lib/libc/stdlib/lsearch.c \
    upstream-netbsd/lib/libc/stdlib/mrand48.c \
    upstream-netbsd/lib/libc/stdlib/nrand48.c \
    upstream-netbsd/lib/libc/stdlib/_rand48.c \
    upstream-netbsd/lib/libc/stdlib/seed48.c \
    upstream-netbsd/lib/libc/stdlib/srand48.c \
    upstream-netbsd/lib/libc/stdlib/tdelete.c \
    upstream-netbsd/lib/libc/stdlib/tfind.c \
    upstream-netbsd/lib/libc/stdlib/tsearch.c \
    upstream-netbsd/lib/libc/string/memccpy.c \
    upstream-netbsd/lib/libc/string/strcasestr.c \
    upstream-netbsd/lib/libc/string/strcoll.c \
    upstream-netbsd/lib/libc/string/strxfrm.c \
    upstream-netbsd/lib/libc/thread-stub/__isthreaded.c \
    upstream-netbsd/lib/libc/unistd/killpg.c \

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

libc_upstream_openbsd_src_files := \
    upstream-openbsd/lib/libc/gen/alarm.c \
    upstream-openbsd/lib/libc/gen/ctype_.c \
    upstream-openbsd/lib/libc/gen/exec.c \
    upstream-openbsd/lib/libc/gen/fnmatch.c \
    upstream-openbsd/lib/libc/gen/ftok.c \
    upstream-openbsd/lib/libc/gen/getprogname.c \
    upstream-openbsd/lib/libc/gen/isctype.c \
    upstream-openbsd/lib/libc/gen/setprogname.c \
    upstream-openbsd/lib/libc/gen/time.c \
    upstream-openbsd/lib/libc/gen/tolower_.c \
    upstream-openbsd/lib/libc/gen/toupper_.c \
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
    upstream-openbsd/lib/libc/net/inet_addr.c \
    upstream-openbsd/lib/libc/net/inet_lnaof.c \
    upstream-openbsd/lib/libc/net/inet_makeaddr.c \
    upstream-openbsd/lib/libc/net/inet_netof.c \
    upstream-openbsd/lib/libc/net/inet_network.c \
    upstream-openbsd/lib/libc/net/inet_ntoa.c \
    upstream-openbsd/lib/libc/net/inet_pton.c \
    upstream-openbsd/lib/libc/net/ntohl.c \
    upstream-openbsd/lib/libc/net/ntohs.c \
    upstream-openbsd/lib/libc/stdio/asprintf.c \
    upstream-openbsd/lib/libc/stdio/clrerr.c \
    upstream-openbsd/lib/libc/stdio/fdopen.c \
    upstream-openbsd/lib/libc/stdio/feof.c \
    upstream-openbsd/lib/libc/stdio/ferror.c \
    upstream-openbsd/lib/libc/stdio/fflush.c \
    upstream-openbsd/lib/libc/stdio/fgetc.c \
    upstream-openbsd/lib/libc/stdio/fgetln.c \
    upstream-openbsd/lib/libc/stdio/fgetpos.c \
    upstream-openbsd/lib/libc/stdio/fgets.c \
    upstream-openbsd/lib/libc/stdio/fgetwc.c \
    upstream-openbsd/lib/libc/stdio/fgetws.c \
    upstream-openbsd/lib/libc/stdio/fileno.c \
    upstream-openbsd/lib/libc/stdio/findfp.c \
    upstream-openbsd/lib/libc/stdio/fprintf.c \
    upstream-openbsd/lib/libc/stdio/fpurge.c \
    upstream-openbsd/lib/libc/stdio/fputc.c \
    upstream-openbsd/lib/libc/stdio/fputs.c \
    upstream-openbsd/lib/libc/stdio/fputwc.c \
    upstream-openbsd/lib/libc/stdio/fputws.c \
    upstream-openbsd/lib/libc/stdio/fread.c \
    upstream-openbsd/lib/libc/stdio/freopen.c \
    upstream-openbsd/lib/libc/stdio/fscanf.c \
    upstream-openbsd/lib/libc/stdio/fseek.c \
    upstream-openbsd/lib/libc/stdio/fsetpos.c \
    upstream-openbsd/lib/libc/stdio/ftell.c \
    upstream-openbsd/lib/libc/stdio/funopen.c \
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
    upstream-openbsd/lib/libc/stdio/perror.c \
    upstream-openbsd/lib/libc/stdio/printf.c \
    upstream-openbsd/lib/libc/stdio/putc.c \
    upstream-openbsd/lib/libc/stdio/putchar.c \
    upstream-openbsd/lib/libc/stdio/puts.c \
    upstream-openbsd/lib/libc/stdio/putw.c \
    upstream-openbsd/lib/libc/stdio/putwc.c \
    upstream-openbsd/lib/libc/stdio/putwchar.c \
    upstream-openbsd/lib/libc/stdio/refill.c \
    upstream-openbsd/lib/libc/stdio/remove.c \
    upstream-openbsd/lib/libc/stdio/rewind.c \
    upstream-openbsd/lib/libc/stdio/rget.c \
    upstream-openbsd/lib/libc/stdio/scanf.c \
    upstream-openbsd/lib/libc/stdio/setbuf.c \
    upstream-openbsd/lib/libc/stdio/setbuffer.c \
    upstream-openbsd/lib/libc/stdio/setvbuf.c \
    upstream-openbsd/lib/libc/stdio/sscanf.c \
    upstream-openbsd/lib/libc/stdio/stdio.c \
    upstream-openbsd/lib/libc/stdio/swprintf.c \
    upstream-openbsd/lib/libc/stdio/swscanf.c \
    upstream-openbsd/lib/libc/stdio/tempnam.c \
    upstream-openbsd/lib/libc/stdio/tmpnam.c \
    upstream-openbsd/lib/libc/stdio/ungetc.c \
    upstream-openbsd/lib/libc/stdio/ungetwc.c \
    upstream-openbsd/lib/libc/stdio/vasprintf.c \
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
    upstream-openbsd/lib/libc/stdlib/atoi.c \
    upstream-openbsd/lib/libc/stdlib/atol.c \
    upstream-openbsd/lib/libc/stdlib/atoll.c \
    upstream-openbsd/lib/libc/stdlib/exit.c \
    upstream-openbsd/lib/libc/stdlib/getenv.c \
    upstream-openbsd/lib/libc/stdlib/setenv.c \
    upstream-openbsd/lib/libc/stdlib/strtoimax.c \
    upstream-openbsd/lib/libc/stdlib/strtol.c \
    upstream-openbsd/lib/libc/stdlib/strtoll.c \
    upstream-openbsd/lib/libc/stdlib/strtoul.c \
    upstream-openbsd/lib/libc/stdlib/strtoull.c \
    upstream-openbsd/lib/libc/stdlib/strtoumax.c \
    upstream-openbsd/lib/libc/stdlib/system.c \
    upstream-openbsd/lib/libc/string/strcasecmp.c \
    upstream-openbsd/lib/libc/string/strcspn.c \
    upstream-openbsd/lib/libc/string/strdup.c \
    upstream-openbsd/lib/libc/string/strndup.c \
    upstream-openbsd/lib/libc/string/strpbrk.c \
    upstream-openbsd/lib/libc/string/strsep.c \
    upstream-openbsd/lib/libc/string/strspn.c \
    upstream-openbsd/lib/libc/string/strstr.c \
    upstream-openbsd/lib/libc/string/strtok.c \
    upstream-openbsd/lib/libc/string/wcslcpy.c \
    upstream-openbsd/lib/libc/string/wcsstr.c \
    upstream-openbsd/lib/libc/string/wcswcs.c \
    upstream-openbsd/lib/libc/string/wcswidth.c \

libc_arch_static_src_files := \
    bionic/dl_iterate_phdr_static.cpp \

# Define some common cflags
# ========================================================
libc_common_cflags := \
    -D_LIBC=1 \
    -Wall -Wextra \

# Try to catch typical 32-bit assumptions that break with 64-bit pointers.
libc_common_cflags += \
    -Werror=pointer-to-int-cast \
    -Werror=int-to-pointer-cast \
    -Werror=type-limits \

ifeq ($(strip $(DEBUG_BIONIC_LIBC)),true)
  libc_common_cflags += -DDEBUG
endif

# To customize dlmalloc's alignment, set BOARD_MALLOC_ALIGNMENT in
# the appropriate BoardConfig.mk file.
#
ifneq ($(BOARD_MALLOC_ALIGNMENT),)
  libc_common_cflags += -DMALLOC_ALIGNMENT=$(BOARD_MALLOC_ALIGNMENT)
endif

# Define ANDROID_SMP appropriately.
ifeq ($(TARGET_CPU_SMP),true)
    libc_common_cflags += -DANDROID_SMP=1
else
    libc_common_cflags += -DANDROID_SMP=0
endif

# Define some common conlyflags
libc_common_conlyflags := \
    -std=gnu99

# Define some common cppflags
libc_common_cppflags := \
    -std=gnu++11

# Define some common includes
# ========================================================
libc_common_c_includes := \
    $(LOCAL_PATH)/stdlib  \
    $(LOCAL_PATH)/stdio   \

# ========================================================
# Add in the arch-specific flags.
# Must be called with $(eval).
# $(1): the LOCAL_ variable name
# $(2): the bionic variable name to pull in
define patch-up-arch-specific-flags
$(1)_$(TARGET_ARCH) += $($(2)_$(TARGET_ARCH))
ifdef TARGET_2ND_ARCH
$(1)_$(TARGET_2ND_ARCH) += $($(2)_$(TARGET_2ND_ARCH))
endif
endef


# ========================================================
# libc_stack_protector.a - stack protector code
# ========================================================
#
# The stack protector code needs to be compiled
# with -fno-stack-protector, since it modifies the
# stack canary.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := bionic/__stack_chk_fail.cpp
LOCAL_CFLAGS := $(libc_common_cflags) -fno-stack-protector -Werror
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_stack_protector
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_tzcode.a - upstream 'tzcode' code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-c-files-under,tzcode)
# tzcode doesn't include wcsftime, so we use the OpenBSD one.
LOCAL_SRC_FILES += upstream-openbsd/lib/libc/time/wcsftime.c

LOCAL_CFLAGS := $(libc_common_cflags)
# Don't use ridiculous amounts of stack.
LOCAL_CFLAGS += -DALL_STATE
# Include tzsetwall, timelocal, timegm, time2posix, and posix2time.
LOCAL_CFLAGS += -DSTD_INSPIRED
# The name of the tm_gmtoff field in our struct tm.
LOCAL_CFLAGS += -DTM_GMTOFF=tm_gmtoff
# Where we store our tzdata.
LOCAL_CFLAGS += -DTZDIR=\"/system/usr/share/zoneinfo\"
# Include timezone and daylight globals.
LOCAL_CFLAGS += -DUSG_COMPAT=1
LOCAL_CFLAGS += -DNO_RUN_TIME_WARNINGS_ABOUT_YEAR_2000_PROBLEMS_THANK_YOU

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes) $(LOCAL_PATH)/tzcode/
LOCAL_MODULE := libc_tzcode
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_dns.a - modified NetBSD DNS code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-c-files-under,dns)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -DANDROID_CHANGES \
    -DINET6 \
    -I$(LOCAL_PATH)/dns/include \
    -I$(LOCAL_PATH)/private \
    -I$(LOCAL_PATH)/upstream-netbsd/lib/libc/include \
    -include upstream-netbsd/android/include/netbsd-compat.h \
#    -Werror \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_dns
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

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
    -I$(LOCAL_PATH)/upstream-freebsd/android/include \
    -I$(LOCAL_PATH)/upstream-freebsd/lib/libc/include \
    -include freebsd-compat.h \
#    -Werror \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_freebsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
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
    -Wno-sign-compare -Wno-uninitialized \
    -Werror \
    -DPOSIX_MISTAKE \
    -I$(LOCAL_PATH)/upstream-netbsd/android/include \
    -I$(LOCAL_PATH)/upstream-netbsd/lib/libc/include \
    -include netbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_netbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

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
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare -Wno-uninitialized \
    -Werror \
    -I$(LOCAL_PATH)/upstream-openbsd/android/include \
    -I$(LOCAL_PATH)/upstream-openbsd/lib/libc/include \
    -I$(LOCAL_PATH)/upstream-openbsd/lib/libc/gdtoa/ \
    -include openbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_openbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
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
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -Wno-sign-compare -Wno-uninitialized \
    -Werror \
    -fvisibility=hidden \
    -I$(LOCAL_PATH)/upstream-openbsd/android/include \
    -I$(LOCAL_PATH)/upstream-openbsd/lib/libc/include \
    -include openbsd-compat.h \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_gdtoa
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_bionic.a - home-grown C library code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_bionic_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -Werror \
    -Wframe-larger-than=2048 \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_bionic
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

ifneq ($(TARGET_USES_LOGD),false)
LOCAL_CFLAGS += -DTARGET_USES_LOGD
endif

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_bionic_src_files))
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
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

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
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := \
    libc_bionic \
    libc_dns \
    libc_freebsd \
    libc_gdtoa \
    libc_netbsd \
    libc_openbsd \
    libc_stack_protector \
    libc_syscalls \
    libc_tzcode \

LOCAL_SYSTEM_SHARED_LIBRARIES :=

# TODO: split out the asflags.
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)

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
    $(libc_static_common_src_files) \
    bionic/libc_init_static.cpp

LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -DLIBC_STATIC \
    -Werror \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_MODULE := libc_nomalloc
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_static_src_files))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc.a
# ========================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(libc_arch_static_src_files) \
    $(libc_static_common_src_files) \
    bionic/dlmalloc.c \
    bionic/malloc_debug_common.cpp \
    bionic/libc_init_static.cpp \

LOCAL_CFLAGS := $(libc_common_cflags) \
    -DLIBC_STATIC \
    -Werror \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_static_src_files))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc.so
# ========================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(libc_common_cflags) -Werror
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
    $(libc_arch_dynamic_src_files) \
    $(libc_static_common_src_files) \
    bionic/dlmalloc.c \
    bionic/malloc_debug_common.cpp \
    bionic/debug_mapinfo.cpp \
    bionic/debug_stacktrace.cpp \
    bionic/libc_init_dynamic.cpp \
    bionic/NetdClient.cpp \

LOCAL_MODULE := libc
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_REQUIRED_MODULES := tzdata

# Leave the symbols in the shared library so that stack unwinders can produce
# meaningful name resolution.
LOCAL_STRIP_MODULE := keep_symbols

# WARNING: The only library libc.so should depend on is libdl.so!  If you add other libraries,
# make sure to add -Wl,--exclude-libs=libgcc.a to the LOCAL_LDFLAGS for those libraries.  This
# ensures that symbols that are pulled into those new libraries from libgcc.a are not declared
# external; if that were the case, then libc would not pull those symbols from libgcc.a as it
# should, instead relying on the external symbols from the dependent libraries.  That would
# create a "cloaked" dependency on libgcc.a in libc though the libraries, which is not what
# you wanted!

LOCAL_SHARED_LIBRARIES := libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_arch_dynamic_src_files))
$(eval $(call patch-up-arch-specific-flags,LOCAL_SRC_FILES,libc_static_common_src_files))
# special for arm
LOCAL_NO_CRT_arm := true
LOCAL_CFLAGS_arm += -DCRT_LEGACY_WORKAROUND
LOCAL_SRC_FILES_arm += \
    arch-common/bionic/crtbegin_so.c \
    arch-arm/bionic/atexit_legacy.c \
    arch-common/bionic/crtend_so.S

include $(BUILD_SHARED_LIBRARY)


# For all builds, except for the -user build we will enable memory
# allocation checking (including memory leaks, buffer overwrites, etc.)
# Note that all these checks are also controlled by env. settings
# that can enable, or disable specific checks. Note also that some of
# the checks are available only in emulator and are implemeted in
# libc_malloc_qemu_instrumented.so.
ifneq ($(TARGET_BUILD_VARIANT),user)

# ========================================================
# libc_malloc_debug_leak.so
# ========================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -DMALLOC_LEAK_CHECK \
    -Werror \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
    bionic/debug_mapinfo.cpp \
    bionic/debug_stacktrace.cpp \
    bionic/malloc_debug_leak.cpp \
    bionic/malloc_debug_check.cpp \

LOCAL_MODULE := libc_malloc_debug_leak
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)

LOCAL_SHARED_LIBRARIES := libc libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true

# Don't install on release build
LOCAL_MODULE_TAGS := eng debug

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_SHARED_LIBRARY)


# ========================================================
# libc_malloc_debug_qemu.so
# ========================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -DMALLOC_QEMU_INSTRUMENT \
    -Werror \

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
    bionic/malloc_debug_qemu.cpp

LOCAL_MODULE := libc_malloc_debug_qemu
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)

LOCAL_SHARED_LIBRARIES := libc libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

# Don't install on release build
LOCAL_MODULE_TAGS := eng debug

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_SHARED_LIBRARY)

endif  #!user


# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
