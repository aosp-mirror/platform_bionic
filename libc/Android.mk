LOCAL_PATH:= $(call my-dir)

include $(LOCAL_PATH)/arch-$(TARGET_ARCH)/syscalls.mk

# Define the common source files for all the libc instances
# =========================================================
libc_common_src_files := \
	$(syscall_src) \
	unistd/abort.c \
	unistd/alarm.c \
	unistd/brk.c \
	unistd/daemon.c \
	unistd/eventfd.c \
	unistd/exec.c \
	unistd/fcntl.c \
	unistd/fnmatch.c \
	unistd/fstatfs.c \
	unistd/ftime.c \
	unistd/ftok.c \
	unistd/getdtablesize.c \
	unistd/gethostname.c \
	unistd/getopt_long.c \
	unistd/getpgrp.c \
	unistd/getpriority.c \
	unistd/getpt.c \
	unistd/initgroups.c \
	unistd/isatty.c \
	unistd/issetugid.c \
	unistd/killpg.c \
	unistd/lseek64.c \
	unistd/mmap.c \
	unistd/open.c \
	unistd/openat.c \
	unistd/opendir.c \
	unistd/pathconf.c \
	unistd/perror.c \
	unistd/popen.c \
	unistd/pread.c \
	unistd/pselect.c \
	unistd/ptsname.c \
	unistd/ptsname_r.c \
	unistd/pwrite.c \
	unistd/raise.c \
	unistd/reboot.c \
	unistd/recv.c \
	unistd/sbrk.c \
	unistd/send.c \
	unistd/setegid.c \
	unistd/setuid.c \
	unistd/seteuid.c \
	unistd/setreuid.c \
	unistd/setresuid.c \
	unistd/setpgrp.c \
	unistd/sigblock.c \
	unistd/siginterrupt.c \
	unistd/siglist.c \
	unistd/signal.c \
	unistd/signame.c \
	unistd/sigsetmask.c \
	unistd/sigsuspend.c \
	unistd/sigwait.c \
	unistd/sleep.c \
	unistd/statfs.c \
	unistd/syslog.c \
	unistd/system.c \
	unistd/tcgetpgrp.c \
	unistd/tcsetpgrp.c \
	unistd/time.c \
	unistd/umount.c \
	unistd/unlockpt.c \
	unistd/usleep.c \
	unistd/wait.c \
	stdio/asprintf.c \
	stdio/clrerr.c \
	stdio/fclose.c \
	stdio/fdopen.c \
	stdio/feof.c \
	stdio/ferror.c \
	stdio/fflush.c \
	stdio/fgetc.c \
	stdio/fgetln.c \
	stdio/fgetpos.c \
	stdio/fgets.c \
	stdio/fileno.c \
	stdio/findfp.c \
	stdio/flags.c \
	stdio/flockfile.c \
	stdio/fopen.c \
	stdio/fprintf.c \
	stdio/fpurge.c \
	stdio/fputc.c \
	stdio/fputs.c \
	stdio/fread.c \
	stdio/freopen.c \
	stdio/fscanf.c \
	stdio/fseek.c \
	stdio/fsetpos.c \
	stdio/ftell.c \
	stdio/funopen.c \
	stdio/fvwrite.c \
	stdio/fwalk.c \
	stdio/fwrite.c \
	stdio/getc.c \
	stdio/getchar.c \
	stdio/gets.c \
	stdio/makebuf.c \
	stdio/mktemp.c \
	stdio/printf.c \
	stdio/putc.c \
	stdio/putchar.c \
	stdio/puts.c \
	stdio/putw.c \
	stdio/refill.c \
	stdio/remove.c \
	stdio/rewind.c \
	stdio/rget.c \
	stdio/scanf.c \
	stdio/setbuf.c \
	stdio/setbuffer.c \
	stdio/setvbuf.c \
	stdio/snprintf.c\
	stdio/sprintf.c \
	stdio/sscanf.c \
	stdio/stdio.c \
	stdio/tempnam.c \
	stdio/tmpfile.c \
	stdio/tmpnam.c \
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
	stdio/wsetup.c \
	stdio/__fgets_chk.c \
	stdio/__snprintf_chk.c \
	stdio/__sprintf_chk.c \
	stdio/__vsnprintf_chk.c \
	stdio/__vsprintf_chk.c \
	stdlib/_rand48.c \
	stdlib/assert.c \
	stdlib/atexit.c \
	stdlib/atoi.c \
	stdlib/atol.c \
	stdlib/atoll.c \
	stdlib/bsearch.c \
	stdlib/ctype_.c \
	stdlib/div.c \
	stdlib/exit.c \
	stdlib/getenv.c \
	stdlib/jrand48.c \
	stdlib/ldiv.c \
	stdlib/lldiv.c \
	stdlib/locale.c \
	stdlib/lrand48.c \
	stdlib/mrand48.c \
	stdlib/nrand48.c \
	stdlib/putenv.c \
	stdlib/qsort.c \
	stdlib/seed48.c \
	stdlib/setenv.c \
	stdlib/setjmperr.c \
	stdlib/srand48.c \
	stdlib/strntoimax.c \
	stdlib/strntoumax.c \
	stdlib/strtod.c \
	stdlib/strtoimax.c \
	stdlib/strtol.c \
	stdlib/strtoll.c \
	stdlib/strtoul.c \
	stdlib/strtoull.c \
	stdlib/strtoumax.c \
	stdlib/tolower_.c \
	stdlib/toupper_.c \
	stdlib/wchar.c \
	string/index.c \
	string/memccpy.c \
	string/memchr.c \
	string/memmem.c \
	string/memrchr.c \
	string/memswap.c \
	string/strcasecmp.c \
	string/strcasestr.c \
	string/strcat.c \
	string/strchr.c \
	string/strcoll.c \
	string/strcspn.c \
	string/strdup.c \
	string/strlcat.c \
	string/strlcpy.c \
	string/strncat.c \
	string/strncpy.c \
	string/strndup.c \
	string/strnlen.c \
	string/strpbrk.c \
	string/strrchr.c \
	string/strsep.c \
	string/strspn.c \
	string/strstr.c \
	string/strtok.c \
	string/strtotimeval.c \
	string/__memcpy_chk.c \
	string/__memmove_chk.c \
	string/__memset_chk.c \
	string/__strcat_chk.c \
	string/__strcpy_chk.c \
	string/__strlcat_chk.c \
	string/__strlcpy_chk.c \
	string/__strlen_chk.c \
	string/__strncat_chk.c \
	string/__strncpy_chk.c \
	wchar/wcpcpy.c \
	wchar/wcpncpy.c \
	wchar/wcscasecmp.c \
	wchar/wcscat.c \
	wchar/wcschr.c \
	wchar/wcscmp.c \
	wchar/wcscoll.c \
	wchar/wcscpy.c \
	wchar/wcscspn.c \
	wchar/wcsdup.c \
	wchar/wcslcat.c \
	wchar/wcslcpy.c \
	wchar/wcslen.c \
	wchar/wcsncasecmp.c \
	wchar/wcsncat.c \
	wchar/wcsncmp.c \
	wchar/wcsncpy.c \
	wchar/wcsnlen.c \
	wchar/wcspbrk.c \
	wchar/wcsrchr.c \
	wchar/wcsspn.c \
	wchar/wcsstr.c \
	wchar/wcstok.c \
	wchar/wcswidth.c \
	wchar/wcsxfrm.c \
	wchar/wmemchr.c \
	wchar/wmemcmp.c \
	wchar/wmemcpy.c \
	wchar/wmemmove.c \
	wchar/wmemset.c \
	inet/bindresvport.c \
	inet/inet_addr.c \
	inet/inet_aton.c \
	inet/inet_ntoa.c \
	inet/inet_ntop.c \
	inet/inet_pton.c \
	inet/ether_aton.c \
	inet/ether_ntoa.c \
	tzcode/asctime.c \
	tzcode/difftime.c \
	tzcode/localtime.c \
	tzcode/strftime.c \
	tzcode/strptime.c \
	bionic/__set_errno.c \
	bionic/bionic_clone.c \
	bionic/cpuacct.c \
	bionic/arc4random.c \
	bionic/basename.c \
	bionic/basename_r.c \
	bionic/clearenv.c \
	bionic/dirname.c \
	bionic/dirname_r.c \
	bionic/drand48.c \
	bionic/erand48.c \
	bionic/err.c \
	bionic/fdprintf.c \
	bionic/fork.c \
	bionic/fts.c \
	bionic/getcwd.cpp \
	bionic/if_nametoindex.c \
	bionic/if_indextoname.c \
	bionic/ioctl.c \
	bionic/ldexp.c \
	bionic/libc_init_common.c \
	bionic/logd_write.c \
	bionic/md5.c \
	bionic/memmove_words.c \
	bionic/pututline.c \
	bionic/realpath.c \
	bionic/sched_getaffinity.c \
	bionic/sched_getcpu.c \
	bionic/sched_cpualloc.c \
	bionic/sched_cpucount.c \
	bionic/semaphore.c \
	bionic/sha1.c \
	bionic/strerror.cpp \
	bionic/strerror_r.cpp \
	bionic/strsignal.cpp \
	bionic/stubs.cpp \
	bionic/system_properties.c \
	bionic/tdestroy.c \
	bionic/time64.c \
	bionic/thread_atexit.c \
	bionic/utime.c \
	bionic/utmp.c \
	netbsd/gethnamaddr.c \
	netbsd/isc/ev_timers.c \
	netbsd/isc/ev_streams.c \
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

libc_upstream_netbsd_src_files := \
	upstream-netbsd/libc/compat-43/creat.c \
	upstream-netbsd/libc/gen/ftw.c \
	upstream-netbsd/libc/gen/nftw.c \
	upstream-netbsd/libc/gen/nice.c \
	upstream-netbsd/libc/gen/psignal.c \
	upstream-netbsd/libc/regex/regcomp.c \
	upstream-netbsd/libc/regex/regerror.c \
	upstream-netbsd/libc/regex/regexec.c \
	upstream-netbsd/libc/regex/regfree.c \
	upstream-netbsd/libc/stdlib/tdelete.c \
	upstream-netbsd/libc/stdlib/tfind.c \
	upstream-netbsd/libc/stdlib/tsearch.c \
	upstream-netbsd/libc/string/strxfrm.c \

# The following files are common, but must be compiled
# with different C flags when building a static C library.
#
# The reason for this is the implementation of __get_tls()
# that will differ between the shared and static versions
# of the library.
#
# See comments in private/bionic_tls.h for more details.
#
# NOTE: bionic/pthread.c is added later to this list
#       because it needs special handling on ARM, see
#       below.
#
libc_static_common_src_files := \
        unistd/sysconf.c \
        bionic/__errno.c \

# Architecture specific source files go here
# =========================================================
ifeq ($(TARGET_ARCH),arm)
libc_common_src_files += \
	arch-arm/bionic/__get_sp.S \
	arch-arm/bionic/_exit_with_stack_teardown.S \
	arch-arm/bionic/_setjmp.S \
	arch-arm/bionic/abort_arm.S \
	arch-arm/bionic/atomics_arm.c \
	arch-arm/bionic/clone.S \
	arch-arm/bionic/eabi.c \
	arch-arm/bionic/ffs.S \
	arch-arm/bionic/futex_arm.S \
	arch-arm/bionic/kill.S \
	arch-arm/bionic/libgcc_compat.c \
	arch-arm/bionic/tkill.S \
	arch-arm/bionic/tgkill.S \
	arch-arm/bionic/memcmp.S \
	arch-arm/bionic/memcmp16.S \
	arch-arm/bionic/memcpy.S \
	arch-arm/bionic/memset.S \
	arch-arm/bionic/setjmp.S \
	arch-arm/bionic/sigsetjmp.S \
	arch-arm/bionic/strlen.c.arm \
	arch-arm/bionic/strcpy.S \
	arch-arm/bionic/strcmp.S \
	arch-arm/bionic/syscall.S \
	string/memmove.c.arm \
	string/bcopy.c \
	string/strncmp.c \
	unistd/socketcalls.c

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

# these are used by the static and dynamic versions of the libc
# respectively
libc_arch_static_src_files := \
	arch-arm/bionic/exidx_static.c

libc_arch_dynamic_src_files := \
	arch-arm/bionic/exidx_dynamic.c
endif # arm

ifeq ($(TARGET_ARCH),x86)
libc_common_src_files += \
	arch-x86/bionic/__get_sp.S \
	arch-x86/bionic/__get_tls.c \
	arch-x86/bionic/__set_tls.c \
	arch-x86/bionic/clone.S \
	arch-x86/bionic/_exit_with_stack_teardown.S \
	arch-x86/bionic/futex_x86.S \
	arch-x86/bionic/setjmp.S \
	arch-x86/bionic/_setjmp.S \
	arch-x86/bionic/sigsetjmp.S \
	arch-x86/bionic/vfork.S \
	arch-x86/bionic/syscall.S \
	arch-x86/string/bcopy_wrapper.S \
	arch-x86/string/memcpy_wrapper.S \
	arch-x86/string/memmove_wrapper.S \
	arch-x86/string/bzero_wrapper.S \
	arch-x86/string/memcmp_wrapper.S \
	arch-x86/string/memset_wrapper.S \
	arch-x86/string/strcmp_wrapper.S \
	arch-x86/string/strncmp_wrapper.S \
	arch-x86/string/strlen_wrapper.S \
	string/strcpy.c \
	bionic/pthread-atfork.c \
	bionic/pthread-rwlocks.c \
	bionic/pthread-timers.c \
	bionic/ptrace.c

libc_static_common_src_files += \
        bionic/pthread.c \

libc_arch_static_src_files := \
	bionic/dl_iterate_phdr_static.c

libc_arch_dynamic_src_files :=
endif # x86

ifeq ($(TARGET_ARCH),mips)
libc_common_src_files += \
	arch-mips/bionic/__get_sp.S \
	arch-mips/bionic/__get_tls.c \
	arch-mips/bionic/__set_tls.c \
	arch-mips/bionic/_exit_with_stack_teardown.S \
	arch-mips/bionic/_setjmp.S \
	arch-mips/bionic/futex_mips.S \
	arch-mips/bionic/bzero.S \
	arch-mips/bionic/cacheflush.c \
	arch-mips/bionic/clone.S \
	arch-mips/bionic/ffs.S \
	arch-mips/bionic/memcmp16.S \
	arch-mips/bionic/memmove.c \
	arch-mips/bionic/pipe.S \
	arch-mips/bionic/setjmp.S \
	arch-mips/bionic/sigsetjmp.S \
	arch-mips/bionic/vfork.S

libc_common_src_files += \
	arch-mips/string/memset.S \
	arch-mips/string/memcpy.S \
	arch-mips/string/mips_strlen.c

libc_common_src_files += \
	string/bcopy.c \
	string/memcmp.c \
	string/strcmp.c \
	string/strcpy.c \
	string/strncmp.c

libc_common_src_files += \
	bionic/pthread-atfork.c \
	bionic/pthread-rwlocks.c \
	bionic/pthread-timers.c \
	bionic/ptrace.c

libc_static_common_src_files += \
	bionic/pthread.c

libc_arch_static_src_files := \
	bionic/dl_iterate_phdr_static.c

libc_arch_dynamic_src_files :=
endif # mips

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
    -std=gnu99 \
    -Wall -Wextra

# these macro definitions are required to implement the
# 'timezone' and 'daylight' global variables, as well as
# properly update the 'tm_gmtoff' field in 'struct tm'.
#
libc_common_cflags += \
    -DTM_GMTOFF=tm_gmtoff \
    -DUSG_COMPAT=1

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
  #
  # Define HAVE_ARM_TLS_REGISTER macro to indicate to the C library
  # that it should access the hardware TLS register directly in
  # private/bionic_tls.h
  #
  # The value must match your kernel configuration
  #
  ifeq ($(ARCH_ARM_HAVE_TLS_REGISTER),true)
    libc_common_cflags += -DHAVE_ARM_TLS_REGISTER
  endif
  #
  # Define HAVE_32_BYTE_CACHE_LINES to indicate to C
  # library it should use to 32-byte version of memcpy, and not
  # the 64-byte version.
  #
  ifeq ($(ARCH_ARM_HAVE_32_BYTE_CACHE_LINES),true)
    libc_common_cflags += -DHAVE_32_BYTE_CACHE_LINE
  endif
  ifeq ($(ARCH_ARM_USE_NON_NEON_MEMCPY),true)
    libc_common_cflags += -DARCH_ARM_USE_NON_NEON_MEMCPY
  endif
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
    libc_crtbegin_extension := c
    libc_crt_target_so_cflags :=
endif
ifeq ($(TARGET_ARCH),mips)
    libc_crtbegin_extension := S
    libc_crt_target_so_cflags := -fPIC
endif
ifeq ($(TARGET_ARCH),x86)
    libc_crtbegin_extension := S
    libc_crt_target_so_cflags := -fPIC
endif
ifeq ($(libc_crtbegin_extension),)
    $(error $(TARGET_ARCH) not supported)
endif
libc_crt_target_so_cflags += $(libc_crt_target_cflags)
libc_crt_target_crtbegin_file := $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin.$(libc_crtbegin_extension)
libc_crt_target_crtbegin_so_file := $(LOCAL_PATH)/arch-$(TARGET_ARCH)/bionic/crtbegin_so.$(libc_crtbegin_extension)

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

LOCAL_SRC_FILES := bionic/ssp.c
LOCAL_CFLAGS := $(libc_common_cflags) -fno-stack-protector
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libbionic_ssp
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
    -include upstream-netbsd/netbsd-compat.h
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_netbsd
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_common.a
# ========================================================

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libc_common_src_files)
LOCAL_CFLAGS := $(libc_common_cflags)
LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_MODULE := libc_common
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_WHOLE_STATIC_LIBRARIES := libbionic_ssp libc_netbsd
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_STATIC_LIBRARY)


# ========================================================
# libc_nomalloc.a
# ========================================================
#
# This is a version of the static C library that does not
# include malloc. It's useful in situations when calling
# the user wants to provide their own malloc implementation,
# or wants to explicitly disallow the use of the use of malloc,
# like the dynamic loader.

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	$(libc_arch_static_src_files) \
	$(libc_static_common_src_files) \
	bionic/libc_init_static.c

LOCAL_C_INCLUDES := $(libc_common_c_includes)
LOCAL_CFLAGS := $(libc_common_cflags) \
                -DLIBC_STATIC

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
	bionic/libc_init_static.c

LOCAL_CFLAGS := $(libc_common_cflags) \
                -DLIBC_STATIC
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

LOCAL_CFLAGS := $(libc_common_cflags) -DPTHREAD_DEBUG -DPTHREAD_DEBUG_ENABLED=0
LOCAL_C_INCLUDES := $(libc_common_c_includes)

LOCAL_SRC_FILES := \
	$(libc_arch_dynamic_src_files) \
	$(libc_static_common_src_files) \
	bionic/dlmalloc.c \
	bionic/malloc_debug_common.cpp \
	bionic/pthread_debug.c \
	bionic/libc_init_dynamic.c

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
	bionic/malloc_debug_leak.cpp \
	bionic/malloc_debug_check.cpp \
	bionic/malloc_debug_check_mapinfo.cpp \
	bionic/malloc_debug_stacktrace.cpp

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
