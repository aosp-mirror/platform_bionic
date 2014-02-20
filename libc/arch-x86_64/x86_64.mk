# x86_64 specific configs

libc_common_src_files_x86_64 := \
    bionic/memchr.c \
    bionic/memcmp.c \
    bionic/memcpy.c \
    bionic/memmove.c \
    bionic/memrchr.c \
    bionic/memset.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    string/bcopy.c \
    string/index.c \
    string/strcat.c \
    string/strcmp.c \
    string/strcpy.c \
    string/strlcat.c \
    string/strlcpy.c \
    string/strlen.c \
    string/strncat.c \
    string/strncmp.c \
    string/strncpy.c \
    string/strrchr.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wcschr.c \
    upstream-freebsd/lib/libc/string/wcscmp.c \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcslen.c \
    upstream-freebsd/lib/libc/string/wcsrchr.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \

# Fortify implementations of libc functions.
libc_common_src_files_x86_64 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \


##########################################
### CPU specific source files
libc_bionic_src_files_x86_64 := \
    arch-x86_64/bionic/__bionic_clone.S \
    arch-x86_64/bionic/_exit_with_stack_teardown.S \
    arch-x86_64/bionic/futex_x86_64.S \
    arch-x86_64/bionic/__get_sp.S \
    arch-x86_64/bionic/__rt_sigreturn.S \
    arch-x86_64/bionic/_setjmp.S \
    arch-x86_64/bionic/setjmp.S \
    arch-x86_64/bionic/__set_tls.c \
    arch-x86_64/bionic/sigsetjmp.S \
    arch-x86_64/bionic/syscall.S \
    arch-x86_64/bionic/vfork.S \
    string/memcmp16.c \


libc_crt_target_cflags_x86_64 += \
    -m64 \
    -I$(LOCAL_PATH)/arch-x86_64/include

libc_crt_target_ldflags_x86_64 := -melf_x86_64

libc_crt_target_crtbegin_file_x86_64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_x86_64 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

libc_crt_target_so_cflags_x86_64 := \
    -fPIC
