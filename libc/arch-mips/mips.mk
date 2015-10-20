# 32-bit mips.

libc_bionic_src_files_mips += \
    arch-mips/string/memcmp.c \
    arch-mips/string/memcpy.S \
    arch-mips/string/memset.S \
    arch-mips/string/strcmp.S \

#
# Inherently architecture-specific code.
#

libc_bionic_src_files_mips += \
    arch-mips/bionic/__bionic_clone.S \
    arch-mips/bionic/bzero.S \
    arch-mips/bionic/cacheflush.cpp \
    arch-mips/bionic/_exit_with_stack_teardown.S \
    arch-mips/bionic/setjmp.S \
    arch-mips/bionic/syscall.S \
    arch-mips/bionic/vfork.S \

ifndef ARCH_MIPS_REV6
libc_bionic_src_files_mips += \
    arch-mips/string/mips_strlen.c \

else
libc_bionic_src_files_mips += \
    arch-mips/string/strlen.c \

endif

libc_crt_target_cflags_mips := \
    $($(my_2nd_arch_prefix)TARGET_GLOBAL_CFLAGS) \
    -I$(LOCAL_PATH)/arch-mips/include

libc_crt_target_crtbegin_file_mips := \
    $(LOCAL_PATH)/arch-mips/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_mips := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

libc_crt_target_so_cflags_mips := \
    -fPIC

libc_crt_target_ldflags_mips := \
    -melf32ltsmip
