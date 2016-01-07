# 64-bit arm.

#
# Generic arm64 optimizations, may be overriden by CPU variants.
#

libc_bionic_src_files_arm64 += \
    arch-arm64/generic/bionic/memchr.S \
    arch-arm64/generic/bionic/memcmp.S \
    arch-arm64/generic/bionic/memcpy.S \
    arch-arm64/generic/bionic/memmove.S \
    arch-arm64/generic/bionic/memset.S \
    arch-arm64/generic/bionic/stpcpy.S \
    arch-arm64/generic/bionic/strchr.S \
    arch-arm64/generic/bionic/strcmp.S \
    arch-arm64/generic/bionic/strcpy.S \
    arch-arm64/generic/bionic/strlen.S \
    arch-arm64/generic/bionic/strncmp.S \
    arch-arm64/generic/bionic/strnlen.S \
    arch-arm64/generic/bionic/wmemmove.S \

libc_bionic_src_files_exclude_arm64 += \
    bionic/__memcpy_chk.cpp \
    bionic/strchr.cpp \
    bionic/strnlen.c \

libc_freebsd_src_files_exclude_arm64 += \
    upstream-freebsd/lib/libc/string/wmemmove.c \

libc_openbsd_src_files_exclude_arm64 += \
    upstream-openbsd/lib/libc/string/memchr.c \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strncmp.c \

#
# Inherently architecture-specific code.
#

libc_bionic_src_files_arm64 += \
    arch-arm64/bionic/__bionic_clone.S \
    arch-arm64/bionic/_exit_with_stack_teardown.S \
    arch-arm64/bionic/setjmp.S \
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
ifneq ($(TARGET_CPU_VARIANT),generic)
cpu_variant_mk := $(LOCAL_PATH)/arch-arm64/$(TARGET_CPU_VARIANT)/$(TARGET_CPU_VARIANT).mk
ifeq ($(wildcard $(cpu_variant_mk)),)
$(error "TARGET_CPU_VARIANT not set or set to an unknown value. Possible values are generic, denver64. Use generic for devices that do not have a CPU similar to any of the supported cpu variants.")
endif
include $(cpu_variant_mk)
libc_common_additional_dependencies += $(cpu_variant_mk)

cpu_variant_mk :=
endif
