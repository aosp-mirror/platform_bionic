LOCAL_PATH:= $(call my-dir)

include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/syscalls.mk

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
	stdlib/exit.c \
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
	bionic/bionic_clone.c \
	bionic/clearenv.c \
	bionic/cpuacct.c \
	bionic/daemon.c \
	bionic/err.c \
	bionic/ether_aton.c \
	bionic/ether_ntoa.c \
	bionic/fcntl.c \
	bionic/fdprintf.c \
	bionic/flockfile.c \
	bionic/fork.c \
	bionic/fstatfs.c \
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
	bionic/lseek64.c \
	bionic/md5.c \
	bionic/memmem.c \
	bionic/memswap.c \
	bionic/name_mem.c \
	bionic/openat.c \
	bionic/open.c \
	bionic/pathconf.c \
	bionic/perror.c \
	bionic/pread.c \
	bionic/pselect.c \
	bionic/ptsname.c \
	bionic/ptsname_r.c \
	bionic/pututline.c \
	bionic/pwrite.c \
	bionic/reboot.c \
	bionic/recv.c \
	bionic/sched_cpualloc.c \
	bionic/sched_cpucount.c \
	bionic/sched_getcpu.c \
	bionic/semaphore.c \
	bionic/send.c \
	bionic/setegid.c \
	bionic/seteuid.c \
	bionic/setpgrp.c \
	bionic/setresuid.c \
	bionic/setreuid.c \
	bionic/setuid.c \
	bionic/sigblock.c \
	bionic/siginterrupt.c \
	bionic/siglist.c \
	bionic/signal.c \
	bionic/signame.c \
	bionic/sigsetmask.c \
	bionic/sigsuspend.c \
	bionic/sleep.c \
	bionic/statfs.c \
	bionic/strndup.c \
	bionic/strntoimax.c \
	bionic/strntoumax.c \
	bionic/strtotimeval.c \
	bionic/system_properties.c \
	bionic/system_properties_compat.c \
	bionic/tcgetpgrp.c \
	bionic/tcsetpgrp.c \
	bionic/thread_atexit.c \
	bionic/time64.c \
	bionic/umount.c \
	bionic/unlockpt.c \
	bionic/usleep.c \
	bionic/utmp.c \
	bionic/wcscoll.c \
	netbsd/gethnamaddr.c \
	netbsd/inet/nsap_addr.c \
	netbsd/resolv/__dn_comp.c \
	netbsd/resolv/__res_close.c \
	netbsd/resolv/__res_send.c \
	netbsd/resolv/herror.c \
	netbsd/resolv/res_comp.c \
	netbsd/resolv/res_data.c \
	netbsd/resolv/res_debug.c \
	netbsd/resolv/res_init.c \
	netbsd/resolv/res_mkquery.c \
	netbsd/resolv/res_query.c \
	netbsd/resolv/res_send.c \
	netbsd/resolv/res_state.c \
	netbsd/resolv/res_cache.c \
	netbsd/net/nsdispatch.c \
	netbsd/net/getaddrinfo.c \
	netbsd/net/getnameinfo.c \
	netbsd/net/getservbyname.c \
	netbsd/net/getservent.c \
	netbsd/net/base64.c \
	netbsd/net/getservbyport.c \
	netbsd/nameser/ns_name.c \
	netbsd/nameser/ns_parse.c \
	netbsd/nameser/ns_ttl.c \
	netbsd/nameser/ns_netint.c \
	netbsd/nameser/ns_print.c \
	netbsd/nameser/ns_samedomain.c \

# Fortify implementations of libc functions.
libc_common_src_files += \
    bionic/__fgets_chk.cpp \
    bionic/__memcpy_chk.cpp \
    bionic/__memmove_chk.cpp \
    bionic/__memset_chk.cpp \
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
    bionic/assert.cpp \
    bionic/brk.cpp \
    bionic/dirent.cpp \
    bionic/__errno.c \
    bionic/eventfd_read.cpp \
    bionic/eventfd_write.cpp \
    bionic/futimens.cpp \
    bionic/getauxval.cpp \
    bionic/getcwd.cpp \
    bionic/libc_init_common.cpp \
    bionic/libc_logging.cpp \
    bionic/libgen.cpp \
    bionic/mmap.cpp \
    bionic/pthread_attr.cpp \
    bionic/pthread_detach.cpp \
    bionic/pthread_equal.cpp \
    bionic/pthread_getcpuclockid.cpp \
    bionic/pthread_getschedparam.cpp \
    bionic/pthread_internals.cpp \
    bionic/pthread_join.cpp \
    bionic/pthread_kill.cpp \
    bionic/pthread_self.cpp \
    bionic/pthread_setname_np.cpp \
    bionic/pthread_setschedparam.cpp \
    bionic/pthread_sigmask.cpp \
    bionic/raise.cpp \
    bionic/sbrk.cpp \
    bionic/scandir.cpp \
    bionic/sched_getaffinity.cpp \
    bionic/__set_errno.cpp \
    bionic/setlocale.cpp \
    bionic/signalfd.cpp \
    bionic/sigwait.cpp \
    bionic/statvfs.cpp \
    bionic/strerror.cpp \
    bionic/strerror_r.cpp \
    bionic/strsignal.cpp \
    bionic/stubs.cpp \
    bionic/sysconf.cpp \
    bionic/tdestroy.cpp \
    bionic/tmpfile.cpp \
    bionic/wait.cpp \
    bionic/wchar.cpp \

libc_tzcode_src_files := \
    tzcode/asctime.c \
    tzcode/difftime.c \
    tzcode/localtime.c \
    tzcode/strftime.c \
    tzcode/strptime.c \

libc_upstream_freebsd_src_files := \
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
    upstream-netbsd/libc/unistd/killpg.c \

# Architecture specific source files go here
# =========================================================
ifeq ($(TARGET_ARCH),arm)
libc_common_src_files += \
	bionic/memmove.c.arm \
	string/bcopy.c \
	string/strncmp.c \
	string/strncat.c \
	string/strncpy.c \
	bionic/strchr.cpp \
	string/strrchr.c \
	bionic/memchr.c \
	bionic/memrchr.c \
	string/index.c \
	bionic/strnlen.c \
	string/strlcat.c \
	string/strlcpy.c \
	upstream-freebsd/lib/libc/string/wcschr.c \
	upstream-freebsd/lib/libc/string/wcsrchr.c \
	upstream-freebsd/lib/libc/string/wcscmp.c \
	upstream-freebsd/lib/libc/string/wcscpy.c \
	upstream-freebsd/lib/libc/string/wmemcmp.c \
	upstream-freebsd/lib/libc/string/wcslen.c \
	upstream-freebsd/lib/libc/string/wcscat.c

# These files need to be arm so that gdbserver
# can set breakpoints in them without messing
# up any thumb code.
libc_common_src_files += \
	bionic/pthread-atfork.c.arm \
	bionic/pthread-rwlocks.c.arm \
	bionic/pthread-timers.c.arm \
	bionic/ptrace.c.arm

libc_static_common_src_files += \
    bionic/pthread.c.arm \
    bionic/pthread_create.cpp.arm \
    bionic/pthread_key.cpp.arm \

endif # arm

ifeq ($(TARGET_ARCH),x86)
libc_common_src_files += \
    bionic/pthread-atfork.c \
    bionic/pthread-rwlocks.c \
    bionic/pthread-timers.c \
    bionic/ptrace.c \

libc_static_common_src_files += \
    bionic/pthread.c \
    bionic/pthread_create.cpp \
    bionic/pthread_key.cpp \

endif # x86

ifeq ($(TARGET_ARCH),mips)
libc_common_src_files += \
	bionic/memcmp.c \
	string/bcopy.c \
	string/strcmp.c \
	string/strcpy.c \
	string/strncmp.c \
	string/strcat.c \
	string/strncat.c \
	string/strncpy.c \
	bionic/strchr.cpp \
	string/strrchr.c \
	bionic/memchr.c \
	bionic/memrchr.c \
	string/index.c \
	bionic/strnlen.c \
	string/strlcat.c \
	string/strlcpy.c \
	upstream-freebsd/lib/libc/string/wcschr.c \
	upstream-freebsd/lib/libc/string/wcsrchr.c \
	upstream-freebsd/lib/libc/string/wcscmp.c \
	upstream-freebsd/lib/libc/string/wcscpy.c \
	upstream-freebsd/lib/libc/string/wmemcmp.c \
	upstream-freebsd/lib/libc/string/wcslen.c \
	upstream-freebsd/lib/libc/string/wcscat.c

libc_common_src_files += \
	bionic/pthread-atfork.c \
	bionic/pthread-rwlocks.c \
	bionic/pthread-timers.c \
	bionic/ptrace.c

libc_static_common_src_files += \
    bionic/pthread.c \
    bionic/pthread_create.cpp \
    bionic/pthread_key.cpp \

endif # mips

ifeq ($(strip $(TARGET_CPU_VARIANT)),)
$(warning TARGET_CPU_VARIANT is not defined)
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
include bionic/libc/arch-$(TARGET_ARCH)/$(TARGET_ARCH).mk

libc_common_src_files += $(_LIBC_ARCH_COMMON_SRC_FILES)
libc_common_src_files += $(_LIBC_ARCH_CPU_VARIANT_SRC_FILES)
libc_arch_static_src_files := $(_LIBC_ARCH_STATIC_SRC_FILES)
libc_arch_dynamic_src_files := $(_LIBC_ARCH_DYNAMIC_SRC_FILES)

# Define some common cflags
# ========================================================
libc_common_cflags := \
    -DWITH_ERRLIST \
    -DANDROID_CHANGES \
    -D_LIBC=1 \
    -DFLOATING_POINT \
    -DINET6 \
    -I$(LOCAL_PATH)/private \
    -DPOSIX_MISTAKE \
    -DLOG_ON_HEAP_ERROR \
    -Wall -Wextra

ifeq ($(strip $(DEBUG_BIONIC_LIBC)),true)
  libc_common_cflags += -DDEBUG
endif

# To customize dlmalloc's alignment, set BOARD_MALLOC_ALIGNMENT in
# the appropriate BoardConfig.mk file.
#
ifneq ($(BOARD_MALLOC_ALIGNMENT),)
  libc_common_cflags += -DMALLOC_ALIGNMENT=$(BOARD_MALLOC_ALIGNMENT)
endif

ifeq ($(TARGET_ARCH),arm)
  libc_common_cflags += -DSOFTFLOAT
  libc_common_cflags += -fstrict-aliasing
  libc_crt_target_cflags := -mthumb-interwork
endif # !arm

ifeq ($(TARGET_ARCH),x86)
  libc_common_cflags += -DSOFTFLOAT
  libc_crt_target_cflags :=
  ifeq ($(ARCH_X86_HAVE_SSE2),true)
      libc_crt_target_cflags += -DUSE_SSE2=1
  endif
  ifeq ($(ARCH_X86_HAVE_SSSE3),true)
      libc_crt_target_cflags += -DUSE_SSSE3=1
  endif
endif # x86

ifeq ($(TARGET_ARCH),mips)
  ifneq ($(ARCH_MIPS_HAS_FPU),true)
    libc_common_cflags += -DSOFTFLOAT
  endif
  libc_common_cflags += -fstrict-aliasing
  libc_crt_target_cflags := $(TARGET_GLOBAL_CFLAGS)
endif # mips

# Define ANDROID_SMP appropriately.
ifeq ($(TARGET_CPU_SMP),true)
    libc_common_cflags += -DANDROID_SMP=1
else
    libc_common_cflags += -DANDROID_SMP=0
endif

# crtbrand.c needs <stdint.h> and a #define for the platform SDK version.
libc_crt_target_cflags += \
    -I$(LOCAL_PATH)/include  \
    -DPLATFORM_SDK_VERSION=$(PLATFORM_SDK_VERSION)

# Define some common includes
# ========================================================
libc_common_c_includes := \
		$(LOCAL_PATH)/stdlib  \
		$(LOCAL_PATH)/string  \
		$(LOCAL_PATH)/stdio   \
		external/safe-iop/include

# Needed to access private/__dso_handle.h from
# crtbegin_xxx.S and crtend_xxx.S
libc_crt_target_cflags += \
    -I$(LOCAL_PATH)/private \
    -I$(LOCAL_PATH)/arch-$(TARGET_ARCH)/include

# Define the libc run-time (crt) support object files that must be built,
# which are needed to build all other objects (shared/static libs and
# executables)
# ==========================================================================
# ARM, MIPS, and x86 all need crtbegin_so/crtend_so.
#
# For x86, the .init section must point to a function that calls all
# entries in the .ctors section. (on ARM this is done through the
# .init_array section instead).
#
# For all the platforms, the .fini_array section must point to a function
# that will call __cxa_finalize(&__dso_handle) in order to ensure that
# static C++ destructors are properly called on dlclose().
#
ifeq ($(TARGET_ARCH),arm)
    libc_crt_target_so_cflags :=
endif
ifeq ($(TARGET_ARCH),mips)
    libc_crt_target_so_cflags := -fPIC
endif
ifeq ($(TARGET_ARCH),x86)
    libc_crt_target_so_cflags := -fPIC
endif
libc_crt_target_so_cflags += $(libc_crt_target_cflags)
libc_crt_target_crtbegin_file := $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin.c
libc_crt_target_crtbegin_so_file := $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin_so.c

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
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtend_so.S
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
	$(hide) $(TARGET_LD) -r -o $@ $^
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
	$(hide) $(TARGET_LD) -r -o $@ $^
ALL_GENERATED_SOURCES += $(GEN)

# We rename crtend.o to crtend_android.o to avoid a
# name clash between gcc and bionic.
GEN := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_android.o
$(GEN): $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtend.S
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
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libbionic_ssp
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_tzcode.a - upstream 'tzcode' code
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_tzcode_src_files)
LOCAL_CFLAGS := \
    $(libc_common_cflags) \
    -std=gnu99 \
    -DSTD_INSPIRED=1 \
    -DTZDIR=\"/system/usr/share/zoneinfo\" \
    -DTM_GMTOFF=tm_gmtoff \
    -DUSG_COMPAT=1
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_tzcode
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
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
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_freebsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
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
    -I$(LOCAL_PATH)/upstream-netbsd \
    -I$(LOCAL_PATH)/upstream-netbsd/libc/include \
    -include upstream-netbsd/netbsd-compat.h
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_netbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_bionic.a - home-grown C library code
# ========================================================
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_bionic_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) -Werror
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_bionic
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_common.a
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_common_src_files)
LOCAL_CFLAGS := $(libc_common_cflags) \
    -std=gnu99 \
    -I$(LOCAL_PATH)/upstream-netbsd/libc/include # for netbsd private headers
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_common
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_WHOLE_STATIC_LIBRARIES := \
    libbionic_ssp \
    libc_bionic \
    libc_freebsd \
    libc_netbsd \
    libc_tzcode
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
                -DLIBC_STATIC \
                -std=gnu99

LOCAL_MODULE := libc_nomalloc
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
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
                -DLIBC_STATIC \
                -std=gnu99
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc.so
# ========================================================
include $(CLEAR_VARS)

# pthread deadlock prediction:
# set -DPTHREAD_DEBUG -DPTHREAD_DEBUG_ENABLED=1 to enable support for
# pthread deadlock prediction.
# Since this code is experimental it is disabled by default.
# see libc/bionic/pthread_debug.c for details

LOCAL_CFLAGS := $(libc_common_cflags) -std=gnu99 -DPTHREAD_DEBUG -DPTHREAD_DEBUG_ENABLED=0
LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
	$(libc_arch_dynamic_src_files) \
	$(libc_static_common_src_files) \
	bionic/dlmalloc.c \
	bionic/malloc_debug_common.cpp \
	bionic/pthread_debug.cpp \
	bionic/libc_init_dynamic.cpp

ifeq ($(TARGET_ARCH),arm)
	LOCAL_NO_CRT := true
	LOCAL_CFLAGS += -DCRT_LEGACY_WORKAROUND

	LOCAL_SRC_FILES := \
		arch-arm/bionic/crtbegin_so.c \
		arch-arm/bionic/atexit_legacy.c \
		$(LOCAL_SRC_FILES) \
		arch-arm/bionic/crtend_so.S
endif

LOCAL_MODULE:= libc
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
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

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
	bionic/debug_mapinfo.cpp \
	bionic/debug_stacktrace.cpp \
	bionic/malloc_debug_leak.cpp \
	bionic/malloc_debug_check.cpp \

LOCAL_MODULE:= libc_malloc_debug_leak
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

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

LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
	bionic/malloc_debug_qemu.cpp

LOCAL_MODULE:= libc_malloc_debug_qemu
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_SHARED_LIBRARIES := libc libdl
LOCAL_WHOLE_STATIC_LIBRARIES := libc_common
LOCAL_SYSTEM_SHARED_LIBRARIES :=

# Don't install on release build
LOCAL_MODULE_TAGS := eng debug

include $(BUILD_SHARED_LIBRARY)

endif	#!user


# ========================================================
include $(call all-makefiles-under,$(LOCAL_PATH))
