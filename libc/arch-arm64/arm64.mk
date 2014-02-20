# arm64 specific configs

libc_common_src_files_arm64 := \
    bionic/memchr.c   \
    bionic/memcmp.c   \
    bionic/memcpy.c   \
    bionic/memmove.c  \
    bionic/memrchr.c  \
    bionic/memset.c   \
    bionic/strchr.cpp \
    bionic/strnlen.c  \
    string/bcopy.c    \
    string/index.c    \
    string/memcmp16.c \
    string/strcat.c   \
    string/strcmp.c   \
    string/strcpy.c   \
    string/strlcat.c  \
    string/strlcpy.c  \
    string/strlen.c   \
    string/strncat.c  \
    string/strncmp.c  \
    string/strncpy.c  \
    string/strrchr.c  \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \

# Fortify implementations of libc functions.
libc_common_src_files_arm64 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \

##########################################
### CPU specific source files
libc_bionic_src_files_arm64 := \
    arch-arm64/bionic/__bionic_clone.S \
    arch-arm64/bionic/bzero_arm64.c \
    arch-arm64/bionic/_exit_with_stack_teardown.S \
    arch-arm64/bionic/futex_arm64.S \
    arch-arm64/bionic/__get_sp.S \
    arch-arm64/bionic/__rt_sigreturn.S \
    arch-arm64/bionic/_setjmp.S \
    arch-arm64/bionic/setjmp.S \
    arch-arm64/bionic/__set_tls.c \
    arch-arm64/bionic/sigsetjmp.S \
    arch-arm64/bionic/syscall.S \
    arch-arm64/bionic/vfork.S \


libc_crt_target_cflags_arm64 := \
    -I$(LOCAL_PATH)/arch-arm64/include

libc_crt_target_crtbegin_file_arm64 := \
    $(LOCAL_PATH)/arch-arm64/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_arm64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c
