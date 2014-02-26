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
    bionic/clearenv.c \
    bionic/daemon.c \
    bionic/err.c \
    bionic/ether_aton.c \
    bionic/ether_ntoa.c \
    bionic/fdprintf.c \
    bionic/ftime.c \
    bionic/fts.c \
    bionic/getdtablesize.c \
    bionic/gethostname.c \
    bionic/getpgrp.c \
    bionic/getpriority.c \
    bionic/getpt.c \
    bionic/if_indextoname.c \
    bionic/if_nametoindex.c \
    bionic/initgroups.c \
    bionic/ioctl.c \
    bionic/isatty.c \
    bionic/issetugid.c \
    bionic/ldexp.c \
    bionic/md5.c \
    bionic/memmem.c \
    bionic/memswap.c \
    bionic/pathconf.c \
    bionic/ptsname.c \
    bionic/ptsname_r.c \
    bionic/pututline.c \
    bionic/recv.c \
    bionic/sched_cpualloc.c \
    bionic/sched_cpucount.c \
    bionic/semaphore.c \
    bionic/send.c \
    bionic/setpgrp.c \
    bionic/sigblock.c \
    bionic/siginterrupt.c \
    bionic/sigsetmask.c \
    bionic/strntoimax.c \
    bionic/strntoumax.c \
    bionic/strtotimeval.c \
    bionic/system_properties_compat.c \
    bionic/tcgetpgrp.c \
    bionic/tcsetpgrp.c \
    bionic/time64.c \
    bionic/umount.c \
    bionic/unlockpt.c \
    bionic/utmp.c \
    bionic/wcscoll.c \
    stdio/asprintf.c \
    stdio/findfp.c \
    stdio/fprintf.c \
    stdio/fread.c \
    stdio/freopen.c \
    stdio/fvwrite.c \
    stdio/snprintf.c\
    stdio/sprintf.c \
    stdio/sscanf.c \
    stdio/vfprintf.c \
    stdio/vfscanf.c \
    stdio/vsscanf.c \
    stdlib/atexit.c \
    stdlib/ctype_.c \
    stdlib/getenv.c \
    stdlib/putenv.c \
    stdlib/setenv.c \
    stdlib/strtod.c \
    unistd/alarm.c \
    unistd/syslog.c \
    unistd/time.c \

# Fortify implementations of libc functions.
libc_common_src_files += \
    bionic/__FD_chk.cpp \
    bionic/__fgets_chk.cpp \
    bionic/__memmove_chk.cpp \
    bionic/__read_chk.cpp \
    bionic/__recvfrom_chk.cpp \
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
    bionic/access.cpp \
    bionic/assert.cpp \
    bionic/__bionic_name_mem.cpp \
    bionic/bionic_time_conversions.cpp \
    bionic/brk.cpp \
    bionic/chmod.cpp \
    bionic/chown.cpp \
    bionic/clone.cpp \
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
    bionic/futimens.cpp \
    bionic/getauxval.cpp \
    bionic/getcwd.cpp \
    bionic/inotify_init.cpp \
    bionic/lchown.cpp \
    bionic/lfs64_support.cpp \
    bionic/libc_init_common.cpp \
    bionic/libc_logging.cpp \
    bionic/libgen.cpp \
    bionic/link.cpp \
    bionic/lstat.cpp \
    bionic/mkdir.cpp \
    bionic/mkfifo.cpp \
    bionic/mknod.cpp \
    bionic/open.cpp \
    bionic/pause.cpp \
    bionic/pipe.cpp \
    bionic/poll.cpp \
    bionic/posix_fallocate.cpp \
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
    bionic/rename.cpp \
    bionic/rmdir.cpp \
    bionic/sbrk.cpp \
    bionic/scandir.cpp \
    bionic/sched_getaffinity.cpp \
    bionic/sched_getcpu.cpp \
    bionic/setegid.cpp \
    bionic/__set_errno.cpp \
    bionic/seteuid.cpp \
    bionic/setlocale.cpp \
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
    bionic/stubs.cpp \
    bionic/symlink.cpp \
    bionic/sysconf.cpp \
    bionic/system_properties.cpp \
    bionic/sys_siglist.c \
    bionic/sys_signame.c \
    bionic/tdestroy.cpp \
    bionic/thread_atexit.cpp \
    bionic/timer.cpp \
    bionic/tmpfile.cpp \
    bionic/unlink.cpp \
    bionic/utimes.cpp \
    bionic/wait.cpp \
    bionic/wchar.cpp \

libc_upstream_freebsd_src_files := \
    upstream-freebsd/lib/libc/gen/sleep.c \
    upstream-freebsd/lib/libc/gen/usleep.c \
    upstream-freebsd/lib/libc/stdio/fclose.c \
    upstream-freebsd/lib/libc/stdio/flags.c \
    upstream-freebsd/lib/libc/stdio/fopen.c \
    upstream-freebsd/lib/libc/stdio/fwrite.c \
    upstream-freebsd/lib/libc/stdio/makebuf.c \
    upstream-freebsd/lib/libc/stdio/mktemp.c \
    upstream-freebsd/lib/libc/stdio/putw.c \
    upstream-freebsd/lib/libc/stdio/setvbuf.c \
    upstream-freebsd/lib/libc/stdio/wsetup.c \
    upstream-freebsd/lib/libc/stdlib/abs.c \
    upstream-freebsd/lib/libc/stdlib/getopt_long.c \
    upstream-freebsd/lib/libc/stdlib/imaxabs.c \
    upstream-freebsd/lib/libc/stdlib/imaxdiv.c \
    upstream-freebsd/lib/libc/stdlib/labs.c \
    upstream-freebsd/lib/libc/stdlib/llabs.c \
    upstream-freebsd/lib/libc/stdlib/qsort.c \
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
    upstream-freebsd/lib/libc/string/wcsstr.c \
    upstream-freebsd/lib/libc/string/wcstok.c \
    upstream-freebsd/lib/libc/string/wmemchr.c \
    upstream-freebsd/lib/libc/string/wmemcpy.c \
    upstream-freebsd/lib/libc/string/wmemmove.c \
    upstream-freebsd/lib/libc/string/wmemset.c \

libc_upstream_netbsd_src_files := \
    upstream-netbsd/common/lib/libc/hash/sha1/sha1.c \
    upstream-netbsd/common/lib/libc/inet/inet_addr.c \
    upstream-netbsd/lib/libc/gen/ftw.c \
    upstream-netbsd/lib/libc/gen/nftw.c \
    upstream-netbsd/lib/libc/gen/nice.c \
    upstream-netbsd/lib/libc/gen/popen.c \
    upstream-netbsd/lib/libc/gen/psignal.c \
    upstream-netbsd/lib/libc/gen/setjmperr.c \
    upstream-netbsd/lib/libc/gen/utime.c \
    upstream-netbsd/lib/libc/inet/inet_ntoa.c \
    upstream-netbsd/lib/libc/inet/inet_ntop.c \
    upstream-netbsd/lib/libc/inet/inet_pton.c \
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
    upstream-netbsd/lib/libc/stdlib/exit.c \
    upstream-netbsd/lib/libc/stdlib/jrand48.c \
    upstream-netbsd/lib/libc/stdlib/ldiv.c \
    upstream-netbsd/lib/libc/stdlib/lldiv.c \
    upstream-netbsd/lib/libc/stdlib/lrand48.c \
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

libc_upstream_openbsd_src_files := \
    upstream-openbsd/lib/libc/gen/exec.c \
    upstream-openbsd/lib/libc/gen/ftok.c \
    upstream-openbsd/lib/libc/gen/fnmatch.c \
    upstream-openbsd/lib/libc/gen/toupper_.c \
    upstream-openbsd/lib/libc/gen/tolower_.c \
    upstream-openbsd/lib/libc/string/strstr.c \
    upstream-openbsd/lib/libc/string/strsep.c \
    upstream-openbsd/lib/libc/string/wcslcpy.c \
    upstream-openbsd/lib/libc/string/strcasecmp.c \
    upstream-openbsd/lib/libc/string/strdup.c \
    upstream-openbsd/lib/libc/string/strcspn.c \
    upstream-openbsd/lib/libc/string/strtok.c \
    upstream-openbsd/lib/libc/string/strpbrk.c \
    upstream-openbsd/lib/libc/string/strspn.c \
    upstream-openbsd/lib/libc/string/wcswidth.c \
    upstream-openbsd/lib/libc/string/strndup.c \
    upstream-openbsd/lib/libc/stdio/ungetc.c \
    upstream-openbsd/lib/libc/stdio/vsnprintf.c \
    upstream-openbsd/lib/libc/stdio/tmpnam.c \
    upstream-openbsd/lib/libc/stdio/setbuffer.c \
    upstream-openbsd/lib/libc/stdio/fsetpos.c \
    upstream-openbsd/lib/libc/stdio/setbuf.c \
    upstream-openbsd/lib/libc/stdio/vprintf.c \
    upstream-openbsd/lib/libc/stdio/perror.c \
    upstream-openbsd/lib/libc/stdio/remove.c \
    upstream-openbsd/lib/libc/stdio/getc.c \
    upstream-openbsd/lib/libc/stdio/funopen.c \
    upstream-openbsd/lib/libc/stdio/ferror.c \
    upstream-openbsd/lib/libc/stdio/putchar.c \
    upstream-openbsd/lib/libc/stdio/vscanf.c \
    upstream-openbsd/lib/libc/stdio/fflush.c \
    upstream-openbsd/lib/libc/stdio/fpurge.c \
    upstream-openbsd/lib/libc/stdio/fputs.c \
    upstream-openbsd/lib/libc/stdio/tempnam.c \
    upstream-openbsd/lib/libc/stdio/printf.c \
    upstream-openbsd/lib/libc/stdio/fileno.c \
    upstream-openbsd/lib/libc/stdio/stdio.c \
    upstream-openbsd/lib/libc/stdio/getdelim.c \
    upstream-openbsd/lib/libc/stdio/fseek.c \
    upstream-openbsd/lib/libc/stdio/fputc.c \
    upstream-openbsd/lib/libc/stdio/fgetln.c \
    upstream-openbsd/lib/libc/stdio/fdopen.c \
    upstream-openbsd/lib/libc/stdio/fgets.c \
    upstream-openbsd/lib/libc/stdio/fgetc.c \
    upstream-openbsd/lib/libc/stdio/feof.c \
    upstream-openbsd/lib/libc/stdio/vasprintf.c \
    upstream-openbsd/lib/libc/stdio/refill.c \
    upstream-openbsd/lib/libc/stdio/getline.c \
    upstream-openbsd/lib/libc/stdio/rewind.c \
    upstream-openbsd/lib/libc/stdio/wbuf.c \
    upstream-openbsd/lib/libc/stdio/gets.c \
    upstream-openbsd/lib/libc/stdio/fgetpos.c \
    upstream-openbsd/lib/libc/stdio/fscanf.c \
    upstream-openbsd/lib/libc/stdio/putc.c \
    upstream-openbsd/lib/libc/stdio/vsprintf.c \
    upstream-openbsd/lib/libc/stdio/rget.c \
    upstream-openbsd/lib/libc/stdio/clrerr.c \
    upstream-openbsd/lib/libc/stdio/scanf.c \
    upstream-openbsd/lib/libc/stdio/puts.c \
    upstream-openbsd/lib/libc/stdio/getchar.c \
    upstream-openbsd/lib/libc/stdio/ftell.c \
    upstream-openbsd/lib/libc/stdio/fwalk.c \
    upstream-openbsd/lib/libc/stdlib/strtoul.c \
    upstream-openbsd/lib/libc/stdlib/strtoull.c \
    upstream-openbsd/lib/libc/stdlib/atoi.c \
    upstream-openbsd/lib/libc/stdlib/atoll.c \
    upstream-openbsd/lib/libc/stdlib/atol.c \
    upstream-openbsd/lib/libc/stdlib/strtoimax.c \
    upstream-openbsd/lib/libc/stdlib/strtoumax.c \
    upstream-openbsd/lib/libc/stdlib/strtoll.c \
    upstream-openbsd/lib/libc/stdlib/strtol.c \
    upstream-openbsd/lib/libc/stdlib/system.c \
    upstream-openbsd/lib/libc/locale/wcsxfrm.c \

libc_arch_static_src_files := \
    bionic/dl_iterate_phdr_static.cpp \

# Define some common cflags
# ========================================================
libc_common_cflags := \
    -DANDROID_CHANGES \
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
    $(LOCAL_PATH)/string  \
    $(LOCAL_PATH)/stdio   \
    external/safe-iop/include

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
# libbionic_ssp.a - stack protector code
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
LOCAL_MODULE := libbionic_ssp
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_tzcode.a - upstream 'tzcode' code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-c-files-under,tzcode)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -DSTD_INSPIRED=1 \
    -DTZDIR=\"/system/usr/share/zoneinfo\" \
    -DTM_GMTOFF=tm_gmtoff \
    -DUSG_COMPAT=1
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_tzcode
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_dns.a - modified NetBSD DNS code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-c-files-under,netbsd)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -DINET6 \
    -I$(LOCAL_PATH)/private \
    -I$(LOCAL_PATH)/upstream-netbsd/lib/libc/include # for NetBSD private headers
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
    -I$(LOCAL_PATH)/upstream-freebsd \
    -I$(LOCAL_PATH)/upstream-freebsd/libc/include \
    -include upstream-freebsd/freebsd-compat.h
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
    -DPOSIX_MISTAKE \
    -I$(LOCAL_PATH)/upstream-netbsd \
    -I$(LOCAL_PATH)/upstream-netbsd/lib/libc/include \
    -include upstream-netbsd/netbsd-compat.h
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
    -I$(LOCAL_PATH)/upstream-openbsd \
    -I$(LOCAL_PATH)/upstream-openbsd/lib/libc/include \
    -include upstream-openbsd/openbsd-compat.h
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_openbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

$(eval $(call patch-up-arch-specific-flags,LOCAL_CFLAGS,libc_common_cflags))
include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_bionic.a - home-grown C library code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_bionic_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) -Werror
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_bionic
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

# Set -DPTHREAD_DEBUG_ENABLED=true to enable support for pthread deadlock prediction.
# Since this code is experimental it is disabled by default.
LOCAL_CFLAGS += -DPTHREAD_DEBUG_ENABLED=false

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
    libbionic_ssp \
    libc_bionic \
    libc_dns \
    libc_freebsd \
    libc_netbsd \
    libc_openbsd \
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
# explicitly disallow the use of the use of malloc,
# such as in the dynamic loader.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    $(libc_arch_static_src_files) \
    $(libc_static_common_src_files) \
    bionic/libc_init_static.cpp

LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_CFLAGS := $(libc_common_cflags) \
                -DLIBC_STATIC
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
                -DLIBC_STATIC
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

LOCAL_CFLAGS := $(libc_common_cflags)
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
    bionic/pthread_debug.cpp \
    bionic/libc_init_dynamic.cpp \

LOCAL_MODULE:= libc
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_REQUIRED_MODULES := tzdata

# WARNING: The only library libc.so should depend on is libdl.so!  If you add other libraries,
# make sure to add -Wl,--exclude-libs=libgcc.a to the LOCAL_LDFLAGS for those libraries.  This
# ensures that symbols that are pulled into those new libraries from libgcc.a are not declared
# external; if that were the case, then libc would not pull those symbols from libgcc.a as it
# should, instead relying on the external symbols from the dependent libraries.  That would
# create an "cloaked" dependency on libgcc.a in libc though the libraries, which is not what
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
    -DMALLOC_LEAK_CHECK
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
    bionic/debug_mapinfo.cpp \
    bionic/debug_stacktrace.cpp \
    bionic/malloc_debug_leak.cpp \
    bionic/malloc_debug_check.cpp \

LOCAL_MODULE:= libc_malloc_debug_leak
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
    -DMALLOC_QEMU_INSTRUMENT
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
    bionic/malloc_debug_qemu.cpp

LOCAL_MODULE:= libc_malloc_debug_qemu
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
