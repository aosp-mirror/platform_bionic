# mips64 specific configs

libc_common_src_files_mips64 := \
    bionic/index.cpp \
    bionic/memchr.c \
    bionic/memcmp.c \
    bionic/memmove.c \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    bionic/strrchr.cpp \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-openbsd/lib/libc/string/bcopy.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/stpncpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \
    upstream-openbsd/lib/libc/string/strcmp.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strlen.c \
    upstream-openbsd/lib/libc/string/strncat.c \
    upstream-openbsd/lib/libc/string/strncmp.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

# Fortify implementations of libc functions.
libc_common_src_files_mips64 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \


##########################################
### CPU specific source files
libc_bionic_src_files_mips64 := \
    arch-mips64/bionic/__bionic_clone.S \
    arch-mips64/bionic/bzero.S \
    arch-mips64/bionic/_exit_with_stack_teardown.S \
    arch-mips64/bionic/futex_mips.S \
    arch-mips64/bionic/__get_sp.S \
    arch-mips64/bionic/getdents.cpp \
    arch-mips64/bionic/memcmp16.S \
    arch-mips64/bionic/_setjmp.S \
    arch-mips64/bionic/setjmp.S \
    arch-mips64/bionic/sigsetjmp.S \
    arch-mips64/bionic/syscall.S \
    arch-mips64/bionic/vfork.S \

# FIXME TODO
## libc_bionic_src_files_mips64 += arch-mips64/string/memcpy.S
## libc_bionic_src_files_mips64 += arch-mips64/string/memset.S
libc_bionic_src_files_mips64 += bionic/memcpy.cpp
libc_bionic_src_files_mips64 += bionic/memset.c


libc_crt_target_cflags_mips64 := \
    $($(my_2nd_arch_prefix)TARGET_GLOBAL_CFLAGS) \
    -I$(LOCAL_PATH)/arch-mips64/include

libc_crt_target_crtbegin_file_mips64 := \
    $(LOCAL_PATH)/arch-mips64/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_mips64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

libc_crt_target_so_cflags_mips64 := \
    -fPIC

libc_crt_target_ldflags_mips64 := \
    -melf64ltsmip
