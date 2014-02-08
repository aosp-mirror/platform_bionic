LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/syscalls.mk

# Make everything depend on any changes to included makefiles.
libc_common_additional_dependencies := \
    $(LOCAL_PATH)/arch-$(TARGET_ARCH)/syscalls.mk \
    $(LOCAL_PATH)/Android.mk \

# Define the common source files for all the libc instances
# =========================================================
libc_common_src_files := \
	$(syscall_src) \
	unistd/alarm.c \
	unistd/exec.c \
	unistd/fnmatch.c \
	unistd/syslog.c \
	unistd/system.c \
	unistd/time.c \
	stdio/asprintf.c \
	stdio/fflush.c \
	stdio/fgetc.c \
	stdio/findfp.c \
	stdio/fprintf.c \
	stdio/fputc.c \
	stdio/fread.c \
	stdio/freopen.c \
	stdio/fscanf.c \
	stdio/fseek.c \
	stdio/ftell.c \
	stdio/fvwrite.c \
	stdio/gets.c \
	stdio/printf.c \
	stdio/refill.c \
	stdio/rewind.c \
	stdio/scanf.c \
	stdio/snprintf.c\
	stdio/sprintf.c \
	stdio/sscanf.c \
	stdio/stdio.c \
	stdio/ungetc.c \
	stdio/vasprintf.c \
	stdio/vfprintf.c \
	stdio/vfscanf.c \
	stdio/vprintf.c \
	stdio/vsnprintf.c \
	stdio/vsprintf.c \
	stdio/vscanf.c \
	stdio/vsscanf.c \
	stdio/wbuf.c \
	stdlib/atexit.c \
	stdlib/ctype_.c \
	stdlib/getenv.c \
	stdlib/putenv.c \
	stdlib/setenv.c \
	stdlib/strtod.c \
	stdlib/strtoimax.c \
	stdlib/strtol.c \
	stdlib/strtoll.c \
	stdlib/strtoul.c \
	stdlib/strtoull.c \
	stdlib/strtoumax.c \
	stdlib/tolower_.c \
	stdlib/toupper_.c \
	string/strcasecmp.c \
	string/strcspn.c \
	string/strdup.c \
	string/strpbrk.c \
	string/strsep.c \
	string/strspn.c \
	string/strstr.c \
	string/strtok.c \
	wchar/wcswidth.c \
	wchar/wcsxfrm.c \
	bionic/arc4random.c \
	bionic/atoi.c \
	bionic/atol.c \
	bionic/atoll.c \
	bionic/bindresvport.c \
	bionic/clearenv.c \
	bionic/daemon.c \
	bionic/err.c \
	bionic/ether_aton.c \
	bionic/ether_ntoa.c \
	bionic/fdprintf.c \
	bionic/flockfile.c \
	bionic/ftime.c \
	bionic/ftok.c \
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
	bionic/name_mem.c \
	bionic/pathconf.c \
	bionic/perror.c \
	bionic/ptsname.c \
	bionic/ptsname_r.c \
	bionic/pututline.c \
	bionic/reboot.c \
	bionic/recv.c \
	bionic/sched_cpualloc.c \
	bionic/sched_cpucount.c \
	bionic/semaphore.c \
	bionic/send.c \
	bionic/setpgrp.c \
	bionic/sigblock.c \
	bionic/siginterrupt.c \
	bionic/siglist.c \
	bionic/signame.c \
	bionic/sigsetmask.c \
	bionic/strndup.c \
	bionic/strntoimax.c \
	bionic/strntoumax.c \
	bionic/strtotimeval.c \
	bionic/system_properties.c \
	bionic/system_properties_compat.c \
	bionic/tcgetpgrp.c \
	bionic/tcsetpgrp.c \
	bionic/time64.c \
	bionic/umount.c \
	bionic/unlockpt.c \
	bionic/utmp.c \
	bionic/wcscoll.c \


libc_dns_src_files += \
    netbsd/gethnamaddr.c \
    netbsd/inet/nsap_addr.c \
    netbsd/nameser/ns_name.c \
    netbsd/nameser/ns_netint.c \
    netbsd/nameser/ns_parse.c \
    netbsd/nameser/ns_print.c \
    netbsd/nameser/ns_samedomain.c \
    netbsd/nameser/ns_ttl.c \
    netbsd/net/base64.c \
    netbsd/net/getaddrinfo.c \
    netbsd/net/getnameinfo.c \
    netbsd/net/getservbyname.c \
    netbsd/net/getservbyport.c \
    netbsd/net/getservent.c \
    netbsd/net/nsdispatch.c \
    netbsd/resolv/__dn_comp.c \
    netbsd/resolv/herror.c \
    netbsd/resolv/res_cache.c \
    netbsd/resolv/__res_close.c \
    netbsd/resolv/res_comp.c \
    netbsd/resolv/res_data.c \
    netbsd/resolv/res_debug.c \
    netbsd/resolv/res_init.c \
    netbsd/resolv/res_mkquery.c \
    netbsd/resolv/res_query.c \
    netbsd/resolv/__res_send.c \
    netbsd/resolv/res_send.c \
    netbsd/resolv/res_state.c \


# These are used by the 32-bit targets, but not the 64-bit ones.
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),arm mips x86))
libc_common_src_files += \
    bionic/legacy_32_bit_support.cpp \
    bionic/ndk_cruft.cpp \

endif

# Fortify implementations of libc functions.
libc_common_src_files += \
    bionic/__FD_chk.cpp \
    bionic/__fgets_chk.cpp \
    bionic/__memcpy_chk.cpp \
    bionic/__memmove_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__read_chk.cpp \
    bionic/__recvfrom_chk.cpp \
    bionic/__strcat_chk.cpp \
    bionic/__strchr_chk.cpp \
    bionic/__strcpy_chk.cpp \
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
    bionic/bionic_time_conversions.cpp \
    bionic/brk.cpp \
    bionic/chmod.cpp \
    bionic/chown.cpp \
    bionic/clone.cpp \
    bionic/dirent.cpp \
    bionic/dup2.cpp \
    bionic/epoll_create.cpp \
    bionic/epoll_wait.cpp \
    bionic/epoll_pwait.cpp \
    bionic/__errno.cpp \
    bionic/eventfd_read.cpp \
    bionic/eventfd_write.cpp \
    bionic/ffs.cpp \
    bionic/fork.cpp \
    bionic/futimens.cpp \
    bionic/getauxval.cpp \
    bionic/getcwd.cpp \
    bionic/inotify_init.cpp \
    bionic/lchown.cpp \
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
    bionic/signalfd.cpp \
    bionic/sigaction.cpp \
    bionic/sigaddset.cpp \
    bionic/sigdelset.cpp \
    bionic/sigemptyset.cpp \
    bionic/sigfillset.cpp \
    bionic/sigismember.cpp \
    bionic/signal.cpp \
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
    bionic/thread_atexit.cpp \
    bionic/tdestroy.cpp \
    bionic/timer.cpp \
    bionic/tmpfile.cpp \
    bionic/unlink.cpp \
    bionic/utimes.cpp \
    bionic/wait.cpp \
    bionic/wchar.cpp \

# These are shared by all the 32-bit targets, but not the 64-bit ones.
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),arm mips x86))
libc_bionic_src_files += \
    bionic/mmap.cpp \

endif

libc_tzcode_src_files := \
    tzcode/asctime.c \
    tzcode/difftime.c \
    tzcode/localtime.c \
    tzcode/strftime.c \
    tzcode/strptime.c \

libc_upstream_freebsd_src_files := \
    upstream-freebsd/lib/libc/gen/sleep.c \
    upstream-freebsd/lib/libc/gen/usleep.c \
    upstream-freebsd/lib/libc/stdio/clrerr.c \
    upstream-freebsd/lib/libc/stdio/fclose.c \
    upstream-freebsd/lib/libc/stdio/fdopen.c \
    upstream-freebsd/lib/libc/stdio/feof.c \
    upstream-freebsd/lib/libc/stdio/ferror.c \
    upstream-freebsd/lib/libc/stdio/fgetln.c \
    upstream-freebsd/lib/libc/stdio/fgetpos.c \
    upstream-freebsd/lib/libc/stdio/fgets.c \
    upstream-freebsd/lib/libc/stdio/fileno.c \
    upstream-freebsd/lib/libc/stdio/flags.c \
    upstream-freebsd/lib/libc/stdio/fopen.c \
    upstream-freebsd/lib/libc/stdio/fpurge.c \
    upstream-freebsd/lib/libc/stdio/fputs.c \
    upstream-freebsd/lib/libc/stdio/fsetpos.c \
    upstream-freebsd/lib/libc/stdio/funopen.c \
    upstream-freebsd/lib/libc/stdio/fwalk.c \
    upstream-freebsd/lib/libc/stdio/fwrite.c \
    upstream-freebsd/lib/libc/stdio/getc.c \
    upstream-freebsd/lib/libc/stdio/getchar.c \
    upstream-freebsd/lib/libc/stdio/makebuf.c \
    upstream-freebsd/lib/libc/stdio/mktemp.c \
    upstream-freebsd/lib/libc/stdio/putc.c \
    upstream-freebsd/lib/libc/stdio/putchar.c \
    upstream-freebsd/lib/libc/stdio/puts.c \
    upstream-freebsd/lib/libc/stdio/putw.c \
    upstream-freebsd/lib/libc/stdio/remove.c \
    upstream-freebsd/lib/libc/stdio/rget.c \
    upstream-freebsd/lib/libc/stdio/setbuf.c \
    upstream-freebsd/lib/libc/stdio/setbuffer.c \
    upstream-freebsd/lib/libc/stdio/setvbuf.c \
    upstream-freebsd/lib/libc/stdio/tempnam.c \
    upstream-freebsd/lib/libc/stdio/tmpnam.c \
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
    upstream-freebsd/lib/libc/string/wcslcpy.c \
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
    upstream-netbsd/libc/compat-43/creat.c \
    upstream-netbsd/libc/gen/ftw.c \
    upstream-netbsd/libc/gen/nftw.c \
    upstream-netbsd/libc/gen/nice.c \
    upstream-netbsd/libc/gen/popen.c \
    upstream-netbsd/libc/gen/psignal.c \
    upstream-netbsd/libc/gen/setjmperr.c \
    upstream-netbsd/libc/gen/utime.c \
    upstream-netbsd/libc/inet/inet_ntoa.c \
    upstream-netbsd/libc/inet/inet_ntop.c \
    upstream-netbsd/libc/inet/inet_pton.c \
    upstream-netbsd/libc/isc/ev_streams.c \
    upstream-netbsd/libc/isc/ev_timers.c \
    upstream-netbsd/libc/regex/regcomp.c \
    upstream-netbsd/libc/regex/regerror.c \
    upstream-netbsd/libc/regex/regexec.c \
    upstream-netbsd/libc/regex/regfree.c \
    upstream-netbsd/libc/stdio/getdelim.c \
    upstream-netbsd/libc/stdio/getline.c \
    upstream-netbsd/libc/stdlib/bsearch.c \
    upstream-netbsd/libc/stdlib/div.c \
    upstream-netbsd/libc/stdlib/drand48.c \
    upstream-netbsd/libc/stdlib/erand48.c \
    upstream-netbsd/libc/stdlib/exit.c \
    upstream-netbsd/libc/stdlib/jrand48.c \
    upstream-netbsd/libc/stdlib/ldiv.c \
    upstream-netbsd/libc/stdlib/lldiv.c \
    upstream-netbsd/libc/stdlib/lrand48.c \
    upstream-netbsd/libc/stdlib/mrand48.c \
    upstream-netbsd/libc/stdlib/nrand48.c \
    upstream-netbsd/libc/stdlib/_rand48.c \
    upstream-netbsd/libc/stdlib/seed48.c \
    upstream-netbsd/libc/stdlib/srand48.c \
    upstream-netbsd/libc/stdlib/tdelete.c \
    upstream-netbsd/libc/stdlib/tfind.c \
    upstream-netbsd/libc/stdlib/tsearch.c \
    upstream-netbsd/libc/string/memccpy.c \
    upstream-netbsd/libc/string/strcasestr.c \
    upstream-netbsd/libc/string/strcoll.c \
    upstream-netbsd/libc/string/strxfrm.c \
    upstream-netbsd/libc/thread-stub/__isthreaded.c \
    upstream-netbsd/libc/unistd/killpg.c \

# Architecture specific source files go here
# =========================================================
ifeq ($(TARGET_ARCH),arm)
libc_common_src_files += \
    bionic/memchr.c \
    bionic/memmove.c.arm \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    string/bcopy.c \
    string/index.c \
    string/strlcat.c \
    string/strlcpy.c \
    string/strncat.c \
    string/strncmp.c \
    string/strncpy.c \
    string/strrchr.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \

endif # arm

ifeq ($(TARGET_ARCH), arm64)
#TODO: Replace C stubs with optimised assembly
libc_common_src_files += \
    bionic/memchr.c   \
    bionic/memcmp.c   \
    bionic/memcpy.c   \
    bionic/memmove.c  \
    bionic/memrchr.c  \
    bionic/memset.c   \
    bionic/strchr.cpp \
    bionic/strnlen.c  \
    string/bcopy.c    \
    string/index.c    \
    string/memcmp16.c \
    string/strcat.c   \
    string/strcmp.c   \
    string/strcpy.c   \
    string/strlcat.c  \
    string/strlcpy.c  \
    string/strlen.c   \
    string/strncat.c  \
    string/strncmp.c  \
    string/strncpy.c  \
    string/strrchr.c  \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \

endif # arm64

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),mips mips64))
libc_common_src_files += \
    bionic/memchr.c \
    bionic/memcmp.c \
    bionic/memmove.c \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    string/bcopy.c \
    string/index.c \
    string/strcat.c \
    string/strcmp.c \
    string/strcpy.c \
    string/strlcat.c \
    string/strlcpy.c \
    string/strncat.c \
    string/strncmp.c \
    string/strncpy.c \
    string/strrchr.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \

endif # mips || mips64

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86_64))
libc_common_src_files += \
    bionic/memchr.c \
    bionic/memcmp.c \
    bionic/memcpy.c \
    bionic/memmove.c \
    bionic/memrchr.c \
    bionic/memset.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    string/bcopy.c \
    string/index.c \
    string/strcat.c \
    string/strcmp.c \
    string/strcpy.c \
    string/strlcat.c \
    string/strlcpy.c \
    string/strlen.c \
    string/strncat.c \
    string/strncmp.c \
    string/strncpy.c \
    string/strrchr.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \

endif # x86_64

ifeq ($(TARGET_ARCH),arm)
  ifeq ($(strip $(TARGET_CPU_VARIANT)),)
    $(warning TARGET_ARCH is arm, but TARGET_CPU_VARIANT is not defined)
  endif
endif

###########################################################
## Add cpu specific source files.
##
## This can be called multiple times, but it will only add
## the first source file for each unique $(1).
## $(1): Unique identifier to identify the cpu variant
##       implementation.
## $(2): Cpu specific source file.
###########################################################

define libc-add-cpu-variant-src
$(if $(filter true,$(_LIBC_ARCH_CPU_VARIANT_HAS_$(1))), \
	, \
     $(eval _LIBC_ARCH_CPU_VARIANT_HAS_$(1) := true) \
     $(eval _LIBC_ARCH_CPU_VARIANT_SRC_FILE.$(1) := $(2)) \
     $(eval _LIBC_ARCH_CPU_VARIANT_SRC_FILES += $(2)) \
)
endef

_LIBC_ARCH_COMMON_SRC_FILES :=
_LIBC_ARCH_CPU_VARIANT_SRC_FILES :=
_LIBC_ARCH_STATIC_SRC_FILES :=
_LIBC_ARCH_DYNAMIC_SRC_FILES :=
_LIBC_ARCH_ADDITIONAL_DEPENDENCIES :=

libc_common_additional_dependencies += \
    $(LOCAL_PATH)/arch-$(TARGET_ARCH)/$(TARGET_ARCH).mk
include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/$(TARGET_ARCH).mk

libc_bionic_src_files += $(_LIBC_ARCH_COMMON_SRC_FILES)
libc_bionic_src_files += $(_LIBC_ARCH_CPU_VARIANT_SRC_FILES)
libc_arch_static_src_files := $(_LIBC_ARCH_STATIC_SRC_FILES)
libc_arch_dynamic_src_files := $(_LIBC_ARCH_DYNAMIC_SRC_FILES)
libc_common_additional_dependencies += $(_LIBC_ARCH_ADDITIONAL_DEPENDENCIES)

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

# crtbrand.c needs <stdint.h> and a #define for the platform SDK version.
libc_crt_target_cflags := \
    -I$(LOCAL_PATH)/include \
    -I$(LOCAL_PATH)/arch-$(TARGET_ARCH)/include \
    -DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION) \

ifeq ($(TARGET_ARCH),arm)
  libc_common_cflags += -DSOFTFLOAT
  libc_common_cflags += -fstrict-aliasing
  libc_crt_target_cflags += -mthumb-interwork
endif # arm

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),mips mips64))
  ifneq ($(ARCH_MIPS_HAS_FPU),true)
    libc_common_cflags += -DSOFTFLOAT
  endif
  libc_common_cflags += -fstrict-aliasing
  libc_crt_target_cflags += $(TARGET_GLOBAL_CFLAGS)
endif # mips

ifeq ($(TARGET_ARCH),x86)
  libc_crt_target_cflags += -m32
  libc_crt_target_ldflags := -melf_i386
endif # x86

ifeq ($(TARGET_ARCH),x86_64)
  libc_crt_target_cflags += -m64
  libc_crt_target_ldflags := -melf_x86_64
endif # x86_64

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

# Define the libc run-time (crt) support object files that must be built,
# which are needed to build all other objects (shared/static libs and
# executables)
# ==========================================================================
# ARM, Arm64, MIPS, and x86 all need crtbegin_so/crtend_so.
#
# For x86, the .init section must point to a function that calls all
# entries in the .ctors section. (on ARM this is done through the
# .init_array section instead).
#
# For all the platforms, the .fini_array section must point to a function
# that will call __cxa_finalize(&__dso_handle) in order to ensure that
# static C++ destructors are properly called on dlclose().
#
libc_crt_target_crtbegin_file := $(LOCAL_PATH)/arch-common/bionic/crtbegin.c
libc_crt_target_crtbegin_so_file := $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

ifeq ($(TARGET_ARCH),arm)
    libc_crt_target_so_cflags :=
endif
ifeq ($(TARGET_ARCH),arm64)
    libc_crt_target_so_cflags :=
    libc_crt_target_crtbegin_file := $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin.c
endif
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),mips mips64))
    libc_crt_target_so_cflags := -fPIC
    libc_crt_target_crtbegin_file := $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin.c
endif
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),x86 x86_64))
    libc_crt_target_so_cflags := -fPIC
endif
libc_crt_target_so_cflags += $(libc_crt_target_cflags)

# See the comment in crtbrand.c for the reason why we need to generate
# crtbrand.s before generating crtbrand.o.
GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.s
$(GEN): $(LOCAL_PATH)/bionic/crtbrand.c
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_so_cflags) -S \
		-MD -MF $(@:%.s=%.d) -o $@ $<
	$(hide) sed -i -e '/\.note\.ABI-tag/s/progbits/note/' $@
	$(call transform-d-to-p-args,$(@:%.s=%.d),$(@:%.s=%.P))
-include $(GEN:%.s=%.P)
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
$(GEN): $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.s
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_so_cflags) -o $@ -c $<
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_so.o
$(GEN): $(libc_crt_target_crtbegin_so_file)
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_so_cflags) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
-include $(GEN:%.o=%.P)
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_so.o
$(GEN): $(LOCAL_PATH)/arch-common/bionic/crtend_so.S
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_so_cflags) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
-include $(GEN:%.o=%.P)
ALL_GENERATED_SOURCES += $(GEN)

# The following two are installed to device
GEN := $(TARGET_OUT_SHARED_LIBRARIES)/crtbegin_so.o
$(GEN): $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_so.o
	$(hide) mkdir -p $(dir $@) && cp -f $< $@
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_SHARED_LIBRARIES)/crtend_so.o
$(GEN): $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_so.o
	$(hide) mkdir -p $(dir $@) && cp -f $< $@
ALL_GENERATED_SOURCES += $(GEN)


GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_static1.o
$(GEN): $(libc_crt_target_crtbegin_file)
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_cflags) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
-include $(GEN:%.o=%.P)
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_static.o
$(GEN): $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_static1.o $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_LD) $(libc_crt_target_ldflags) -r -o $@ $^
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_dynamic1.o
$(GEN): $(libc_crt_target_crtbegin_file)
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_cflags) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
-include $(GEN:%.o=%.P)
ALL_GENERATED_SOURCES += $(GEN)

GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_dynamic.o
$(GEN): $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_dynamic1.o $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_LD) $(libc_crt_target_ldflags) -r -o $@ $^
ALL_GENERATED_SOURCES += $(GEN)

# We rename crtend.o to crtend_android.o to avoid a
# name clash between gcc and bionic.
GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_android.o
$(GEN): $(LOCAL_PATH)/arch-common/bionic/crtend.S
	@mkdir -p $(dir $@)
	$(hide) $(TARGET_CC) $(libc_crt_target_cflags) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
-include $(GEN:%.o=%.P)
ALL_GENERATED_SOURCES += $(GEN)


# To enable malloc leak check for statically linked programs, add
# "WITH_MALLOC_CHECK_LIBC_A := true" to buildspec.mk
WITH_MALLOC_CHECK_LIBC_A := $(strip $(WITH_MALLOC_CHECK_LIBC_A))

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

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_tzcode.a - upstream 'tzcode' code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_tzcode_src_files)
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

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_dns.a - modified NetBSD DNS code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_dns_src_files)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -DINET6 \
    -I$(LOCAL_PATH)/private \
    -I$(LOCAL_PATH)/upstream-netbsd/libc/include # for NetBSD private headers

LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_dns
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

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
    -I$(LOCAL_PATH)/upstream-netbsd/libc/include \
    -include upstream-netbsd/netbsd-compat.h
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_netbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_SYSTEM_SHARED_LIBRARIES :=

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
LOCAL_CFLAGS += $(libc_common_cflags) -DPTHREAD_DEBUG_ENABLED=false

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
    libc_tzcode \

LOCAL_SYSTEM_SHARED_LIBRARIES :=

# TODO: split out the asflags.
LOCAL_ASFLAGS := $(LOCAL_CFLAGS)

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
	bionic/libc_init_static.cpp

LOCAL_CFLAGS := $(libc_common_cflags) \
                -DLIBC_STATIC
LOCAL_CONLYFLAGS := $(libc_common_conlyflags)
LOCAL_CPPFLAGS := $(libc_common_cppflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc
LOCAL_ADDITIONAL_DEPENDENCIES := $(libc_common_additional_dependencies)
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

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

ifeq ($(TARGET_ARCH),arm)
	LOCAL_NO_CRT := true
	LOCAL_CFLAGS += -DCRT_LEGACY_WORKAROUND

	LOCAL_SRC_FILES := \
		arch-common/bionic/crtbegin_so.c \
		arch-arm/bionic/atexit_legacy.c \
		$(LOCAL_SRC_FILES) \
		arch-common/bionic/crtend_so.S
endif

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

include $(BUILD_SHARED_LIBRARY)

endif	#!user


# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
