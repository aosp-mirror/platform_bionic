# 32-bit x86.

#
# Generic x86 optimizations, may be overriden by CPU variants.
#

libc_bionic_src_files_x86 += \
    arch-x86/atom/string/sse2-memchr-atom.S \
    arch-x86/atom/string/sse2-memrchr-atom.S \
    arch-x86/atom/string/sse2-strchr-atom.S \
    arch-x86/atom/string/sse2-strnlen-atom.S \
    arch-x86/atom/string/sse2-strrchr-atom.S \
    arch-x86/atom/string/sse2-wcschr-atom.S \
    arch-x86/atom/string/sse2-wcsrchr-atom.S \
    arch-x86/atom/string/sse2-wcslen-atom.S \
    arch-x86/atom/string/sse2-wcscmp-atom.S \
    arch-x86/silvermont/string/sse2-bcopy-slm.S \
    arch-x86/silvermont/string/sse2-bzero-slm.S \
    arch-x86/silvermont/string/sse2-memcpy-slm.S \
    arch-x86/silvermont/string/sse2-memmove-slm.S \
    arch-x86/silvermont/string/sse2-memset-slm.S \
    arch-x86/silvermont/string/sse2-stpcpy-slm.S \
    arch-x86/silvermont/string/sse2-stpncpy-slm.S \
    arch-x86/silvermont/string/sse2-strcpy-slm.S \
    arch-x86/silvermont/string/sse2-strlen-slm.S \
    arch-x86/silvermont/string/sse2-strncpy-slm.S

libc_bionic_src_files_x86 += \
    arch-x86/generic/string/memcmp.S \
    arch-x86/generic/string/strcmp.S \
    arch-x86/generic/string/strncmp.S \
    arch-x86/generic/string/strcat.S

ifeq ($(ARCH_X86_HAVE_SSSE3),true)
libc_bionic_src_files_x86 += \
    arch-x86/atom/string/ssse3-strncat-atom.S \
    arch-x86/atom/string/ssse3-strlcat-atom.S \
    arch-x86/atom/string/ssse3-strlcpy-atom.S \
    arch-x86/atom/string/ssse3-strcmp-atom.S \
    arch-x86/atom/string/ssse3-strncmp-atom.S \
    arch-x86/atom/string/ssse3-strcat-atom.S \
    arch-x86/atom/string/ssse3-wcscat-atom.S \
    arch-x86/atom/string/ssse3-wcscpy-atom.S
libc_bionic_src_files_exclude_x86 += \
    arch-x86/generic/string/strcmp.S \
    arch-x86/generic/string/strncmp.S \
    arch-x86/generic/string/strcat.S
endif

ifeq ($(ARCH_X86_HAVE_SSE4),true)
libc_bionic_src_files_x86 += \
    arch-x86/silvermont/string/sse4-memcmp-slm.S \
    arch-x86/silvermont/string/sse4-wmemcmp-slm.S
libc_bionic_src_files_exclude_x86 += \
    arch-x86/generic/string/memcmp.S
endif

#
# Remove default implementations that we have optimized versions of.
#

libc_freebsd_src_files_exclude_x86 += \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \

ifeq ($(ARCH_X86_HAVE_SSSE3),true)
libc_freebsd_src_files_exclude_x86 += \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcscpy.c
endif

ifeq ($(ARCH_X86_HAVE_SSE4),true)
libc_freebsd_src_files_exclude_x86 += \
    upstream-freebsd/lib/libc/string/wmemcmp.c
endif

libc_openbsd_src_files_exclude_x86 += \
    upstream-openbsd/lib/libc/string/memchr.c \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/memrchr.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/stpncpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strncmp.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

ifeq ($(ARCH_X86_HAVE_SSSE3),true)
libc_openbsd_src_files_exclude_x86 += \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c
endif

libc_bionic_src_files_exclude_x86 += \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    bionic/strrchr.cpp \

#
# Inherently architecture-specific functions.
#

libc_bionic_src_files_x86 += \
    arch-x86/bionic/__bionic_clone.S \
    arch-x86/bionic/_exit_with_stack_teardown.S \
    arch-x86/bionic/libgcc_compat.c \
    arch-x86/bionic/__restore.S \
    arch-x86/bionic/setjmp.S \
    arch-x86/bionic/syscall.S \
    arch-x86/bionic/vfork.S \

## ARCH variant specific source files
arch_variant_mk := $(LOCAL_PATH)/arch-x86/$(TARGET_ARCH_VARIANT)/$(TARGET_ARCH_VARIANT).mk
ifeq ($(wildcard $(arch_variant_mk)),)
    arch_variant_mk :=
endif
ifneq ($(arch_variant_mk),)
include $(arch_variant_mk)
libc_common_additional_dependencies += $(arch_variant_mk)

arch_variant_mk :=
endif

libc_crt_target_cflags_x86 := \
    -m32 \
    -I$(LOCAL_PATH)/arch-x86/include

libc_crt_target_ldflags_x86 := -melf_i386

libc_crt_target_crtbegin_file_x86 := \
     $(LOCAL_PATH)/arch-common/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_x86 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

libc_crt_target_so_cflags_x86 := \
    -fPIC
