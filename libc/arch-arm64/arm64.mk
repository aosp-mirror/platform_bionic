# arm64 specific configs

libc_common_src_files_arm64 := \
    bionic/index.cpp \
    bionic/memchr.c \
    bionic/__memcmp16.cpp \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strrchr.cpp \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-openbsd/lib/libc/string/bcopy.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/stpncpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

# Fortify implementations of libc functions.
libc_common_src_files_arm64 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \

##########################################
### CPU specific source files
libc_bionic_src_files_arm64 := \
    arch-arm64/bionic/__bionic_clone.S \
    arch-arm64/bionic/bzero_arm64.c \
    arch-arm64/bionic/_exit_with_stack_teardown.S \
    arch-arm64/bionic/futex_arm64.S \
    arch-arm64/bionic/__get_sp.S \
    arch-arm64/bionic/__rt_sigreturn.S \
    arch-arm64/bionic/_setjmp.S \
    arch-arm64/bionic/setjmp.S \
    arch-arm64/bionic/__set_tls.c \
    arch-arm64/bionic/sigsetjmp.S \
    arch-arm64/bionic/syscall.S \
    arch-arm64/bionic/vfork.S \


libc_crt_target_cflags_arm64 := \
    -I$(LOCAL_PATH)/arch-arm64/include

libc_crt_target_crtbegin_file_arm64 := \
    $(LOCAL_PATH)/arch-arm64/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_arm64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

## CPU variant specific source files
ifeq ($(strip $(TARGET_CPU_VARIANT)),)
  $(warning TARGET_ARCH is arm64, but TARGET_CPU_VARIANT is not defined)
endif
cpu_variant_mk := $(LOCAL_PATH)/arch-arm64/$(TARGET_CPU_VARIANT)/$(TARGET_CPU_VARIANT).mk
ifeq ($(wildcard $(cpu_variant_mk)),)
$(error "TARGET_CPU_VARIANT not set or set to an unknown value. Possible values are generic, generic-neon. Use generic for devices that do not have a CPU similar to any of the supported cpu variants.")
endif
include $(cpu_variant_mk)
libc_common_additional_dependencies += $(cpu_variank_mk)

cpu_variant_mk :=
