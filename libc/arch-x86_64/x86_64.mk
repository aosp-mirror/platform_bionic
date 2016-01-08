# 64-bit x86.

#
# Remove default implementations that we have optimized versions of.
#

libc_openbsd_src_files_exclude_x86_64 += \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/stpncpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c \
    upstream-openbsd/lib/libc/string/strncmp.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

#
# Inherently architecture-specific code.
#

libc_bionic_src_files_x86_64 += \
    arch-x86_64/bionic/__bionic_clone.S \
    arch-x86_64/bionic/_exit_with_stack_teardown.S \
    arch-x86_64/bionic/__restore_rt.S \
    arch-x86_64/bionic/setjmp.S \
    arch-x86_64/bionic/syscall.S \
    arch-x86_64/bionic/vfork.S \

#
# Optimized memory/string functions.
#

libc_bionic_src_files_x86_64 += \
    arch-x86_64/string/sse2-memcpy-slm.S \
    arch-x86_64/string/sse2-memmove-slm.S \
    arch-x86_64/string/sse2-memset-slm.S \
    arch-x86_64/string/sse2-stpcpy-slm.S \
    arch-x86_64/string/sse2-stpncpy-slm.S \
    arch-x86_64/string/sse2-strcat-slm.S \
    arch-x86_64/string/sse2-strcpy-slm.S \
    arch-x86_64/string/sse2-strlcat-slm.S \
    arch-x86_64/string/sse2-strlcpy-slm.S \
    arch-x86_64/string/sse2-strlen-slm.S \
    arch-x86_64/string/sse2-strncat-slm.S \
    arch-x86_64/string/sse2-strncpy-slm.S \
    arch-x86_64/string/sse4-memcmp-slm.S \
    arch-x86_64/string/ssse3-strcmp-slm.S \
    arch-x86_64/string/ssse3-strncmp-slm.S \

libc_crt_target_cflags_x86_64 += \
    -m64 \
    -I$(LOCAL_PATH)/arch-x86_64/include \

libc_crt_target_ldflags_x86_64 := -melf_x86_64 \

libc_crt_target_crtbegin_file_x86_64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin.c \

libc_crt_target_crtbegin_so_file_x86_64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c \

libc_crt_target_so_cflags_x86_64 := \
    -fPIC \
