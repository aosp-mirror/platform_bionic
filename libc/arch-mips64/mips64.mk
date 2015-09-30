# 64-bit mips.

#
# Default implementations of functions that are commonly optimized.
#

libc_bionic_src_files_mips64 += \
    arch-mips/string/memcmp.c \
    arch-mips/string/memcpy.S \
    arch-mips/string/memset.S \
    arch-mips/string/strcmp.S \
    arch-mips/string/strlen.c \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    bionic/strrchr.cpp \

libc_freebsd_src_files_mips64 += \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-freebsd/lib/libc/string/wmemmove.c \

libc_openbsd_src_files_mips64 += \
    upstream-openbsd/lib/libc/string/memchr.c \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/memrchr.c \
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
