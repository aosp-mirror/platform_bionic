# x86 specific configs

# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_common_src_files_x86 := \
    bionic/legacy_32_bit_support.cpp \
    bionic/ndk_cruft.cpp \
    bionic/time64.c \

# Fortify implementations of libc functions.
libc_common_src_files_x86 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \


# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_bionic_src_files_x86 := \
    bionic/mmap.cpp

##########################################
### CPU specific source files
libc_bionic_src_files_x86 += \
    arch-x86/bionic/__bionic_clone.S \
    arch-x86/bionic/_exit_with_stack_teardown.S \
    arch-x86/bionic/futex_x86.S \
    arch-x86/bionic/__get_sp.S \
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
