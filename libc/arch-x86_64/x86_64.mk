# 64-bit x86.

#
# Default implementations of functions that are commonly optimized.
#

libc_bionic_src_files_x86_64 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \
    bionic/memchr.c \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    bionic/strrchr.cpp \

libc_freebsd_src_files_x86_64 += \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-freebsd/lib/libc/string/wmemmove.c \

libc_openbsd_src_files_x86_64 += \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \

#
# Inherently architecture-specific code.
#

libc_bionic_src_files_x86_64 += \
    arch-x86_64/bionic/__bionic_clone.S \
    arch-x86_64/bionic/_exit_with_stack_teardown.S \
    arch-x86_64/bionic/__restore_rt.S \
    arch-x86_64/bionic/_setjmp.S \
    arch-x86_64/bionic/setjmp.S \
    arch-x86_64/bionic/__set_tls.c \
    arch-x86_64/bionic/sigsetjmp.S \
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
