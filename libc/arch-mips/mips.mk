# 32-bit mips.

#
# Default implementations of functions that are commonly optimized.
#

libc_bionic_src_files_mips += \
    arch-mips/string/memcmp.c \
    arch-mips/string/memcpy.S \
    arch-mips/string/memset.S \
    arch-mips/string/strcmp.S \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    bionic/strrchr.cpp \

libc_freebsd_src_files_mips += \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-freebsd/lib/libc/string/wmemmove.c \

libc_openbsd_src_files_mips += \
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
