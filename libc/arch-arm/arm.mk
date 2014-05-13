# arm specific configs

# These are used by the 32-bit targets, but not the 64-bit ones.
libc_common_src_files_arm := \
    bionic/legacy_32_bit_support.cpp \
    bionic/ndk_cruft.cpp \
    bionic/time64.c \

# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_bionic_src_files_arm := \
    bionic/mmap.cpp

libc_common_src_files_arm += \
    bionic/index.cpp \
    bionic/memchr.c \
    bionic/memmove.c.arm \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
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
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c \
    upstream-openbsd/lib/libc/string/strncmp.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

# The C++ fortify function implementations for which there is an
# arm assembler version.
#
# Fortify implementations of libc functions.
# libc_common_src_files_arm +=
#    bionic/__memcpy_chk.cpp \
#    bionic/__memset_chk.cpp \
#    bionic/__strcpy_chk.cpp \
#    bionic/__strcat_chk.cpp \

libc_common_cflags_arm := -DSOFTFLOAT

##########################################
### CPU specific source files
libc_bionic_src_files_arm += \
    arch-arm/bionic/abort_arm.S \
    arch-arm/bionic/atomics_arm.c \
    arch-arm/bionic/__bionic_clone.S \
    arch-arm/bionic/eabi.c \
    arch-arm/bionic/_exit_with_stack_teardown.S \
    arch-arm/bionic/futex_arm.S \
    arch-arm/bionic/__get_sp.S \
    arch-arm/bionic/libgcc_compat.c \
    arch-arm/bionic/memcmp16.S \
    arch-arm/bionic/memcmp.S \
    arch-arm/bionic/_setjmp.S \
    arch-arm/bionic/setjmp.S \
    arch-arm/bionic/sigsetjmp.S \
    arch-arm/bionic/syscall.S \

libc_arch_static_src_files_arm := arch-arm/bionic/exidx_static.c
libc_arch_dynamic_src_files_arm := arch-arm/bionic/exidx_dynamic.c

## CPU variant specific source files
ifeq ($(strip $(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT)),)
  $(warning TARGET_$(my_2nd_arch_prefix)ARCH is arm, but TARGET_$(my_2nd_arch_prefix)CPU_VARIANT is not defined)
endif
cpu_variant_mk := $(LOCAL_PATH)/arch-arm/$(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT)/$(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT).mk
ifeq ($(wildcard $(cpu_variant_mk)),)
$(error "TARGET_$(my_2nd_arch_prefix)CPU_VARIANT not set or set to an unknown value. Possible values are cortex-a7, cortex-a8, cortex-a9, cortex-a15, krait, denver. Use generic for devices that do not have a CPU similar to any of the supported cpu variants.")
endif
include $(cpu_variant_mk)
libc_common_additional_dependencies += $(cpu_variant_mk)

cpu_variant_mk :=


libc_crt_target_cflags_arm := \
    -I$(LOCAL_PATH)/arch-arm/include \
    -mthumb-interwork

libc_crt_target_so_cflags_arm :=

libc_crt_target_crtbegin_file_arm := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_arm := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c
