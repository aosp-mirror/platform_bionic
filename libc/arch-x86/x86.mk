# 32-bit x86.

#
# Various kinds of LP32 cruft.
#

libc_bionic_src_files_x86 += \
    bionic/mmap.cpp \

libc_common_src_files_x86 += \
    bionic/legacy_32_bit_support.cpp \
    bionic/ndk_cruft.cpp \
    bionic/time64.c \

libc_netbsd_src_files_x86 += \
    upstream-netbsd/common/lib/libc/hash/sha1/sha1.c \

libc_openbsd_src_files_x86 += \
    upstream-openbsd/lib/libc/stdio/putw.c \

#
# Default implementations of functions that are commonly optimized.
#

libc_common_src_files_x86 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \

libc_freebsd_src_files_x86 += \
    upstream-freebsd/lib/libc/string/wmemmove.c \

#
# Inherently architecture-specific functions.
#

libc_bionic_src_files_x86 += \
    arch-x86/bionic/__bionic_clone.S \
    arch-x86/bionic/_exit_with_stack_teardown.S \
    arch-x86/bionic/libgcc_compat.c \
    arch-x86/bionic/__restore.S \
    arch-x86/bionic/_setjmp.S \
    arch-x86/bionic/setjmp.S \
    arch-x86/bionic/__set_tls.c \
    arch-x86/bionic/sigsetjmp.S \
    arch-x86/bionic/syscall.S \
    arch-x86/bionic/vfork.S \

## ARCH variant specific source files
arch_variant_mk := $(LOCAL_PATH)/arch-x86/$(TARGET_ARCH_VARIANT)/$(TARGET_ARCH_VARIANT).mk
ifeq ($(wildcard $(arch_variant_mk)),)
    arch_variant_mk := $(LOCAL_PATH)/arch-x86/generic/generic.mk
endif
include $(arch_variant_mk)
libc_common_additional_dependencies += $(arch_variant_mk)

arch_variant_mk :=

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
