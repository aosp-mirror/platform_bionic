# 64-bit mips.

libc_bionic_src_files_mips64 += \
    arch-mips/string/memcmp.c \
    arch-mips/string/memcpy.S \
    arch-mips/string/memset.S \
    arch-mips/string/strcmp.S \
    arch-mips/string/strlen.c \

#
# Inherently architecture-specific code.
#

libc_bionic_src_files_mips64 += \
    arch-mips64/bionic/__bionic_clone.S \
    arch-mips64/bionic/_exit_with_stack_teardown.S \
    arch-mips64/bionic/setjmp.S \
    arch-mips64/bionic/syscall.S \
    arch-mips64/bionic/vfork.S \
    arch-mips64/bionic/stat.cpp \

libc_crt_target_cflags_mips64 := \
    $($(my_2nd_arch_prefix)TARGET_GLOBAL_CFLAGS) \
    -I$(LOCAL_PATH)/arch-mips64/include \

libc_crt_target_crtbegin_file_mips64 := \
    $(LOCAL_PATH)/arch-mips64/bionic/crtbegin.c \

libc_crt_target_crtbegin_so_file_mips64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c \

libc_crt_target_so_cflags_mips64 := \
    -fPIC \

libc_crt_target_ldflags_mips64 := \
    -melf64ltsmip \
