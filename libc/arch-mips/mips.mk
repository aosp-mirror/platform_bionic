# mips specific configs

# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_common_src_files_mips := \
    bionic/legacy_32_bit_support.cpp \
    bionic/ndk_cruft.cpp \

# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_bionic_src_files_mips += \
     bionic/mmap.cpp

libc_common_src_files_mips += \
    bionic/memchr.c \
    bionic/memcmp.c \
    bionic/memmove.c \
    bionic/memrchr.c \
    bionic/strchr.cpp \
    bionic/strnlen.c \
    string/bcopy.c \
    string/index.c \
    string/strcat.c \
    string/strcmp.c \
    string/strcpy.c \
    string/strlcat.c \
    string/strlcpy.c \
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
libc_common_src_files_mips += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \


# cflags
ifneq ($(ARCH_MIPS_HAS_FPU),true)
libc_common_cflags_mips := \
    -DSOFTFLOAT
endif
libc_common_cflags_mips += \
    -fstrict-aliasing

##########################################
### CPU specific source files
libc_bionic_src_files_mips += \
    arch-mips/bionic/__bionic_clone.S \
    arch-mips/bionic/bzero.S \
    arch-mips/bionic/cacheflush.cpp \
    arch-mips/bionic/_exit_with_stack_teardown.S \
    arch-mips/bionic/futex_mips.S \
    arch-mips/bionic/__get_sp.S \
    arch-mips/bionic/memcmp16.S \
    arch-mips/bionic/_setjmp.S \
    arch-mips/bionic/setjmp.S \
    arch-mips/bionic/__set_tls.c \
    arch-mips/bionic/sigsetjmp.S \
    arch-mips/bionic/syscall.S \
    arch-mips/bionic/vfork.S \
    arch-mips/string/memcpy.S \
    arch-mips/string/memset.S \
    arch-mips/string/mips_strlen.c \

# These are used by the static and dynamic versions of the libc
# respectively.
libc_arch_static_src_files_mips :=

libc_arch_dynamic_src_files_mips :=


##########################################
# crt-related
libc_crt_target_cflags_mips := \
    $($(my_2nd_arch_prefix)TARGET_GLOBAL_CFLAGS) \
    -I$(LOCAL_PATH)/arch-mips/include

libc_crt_target_crtbegin_file_mips := \
    $(LOCAL_PATH)/arch-mips/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_mips := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

libc_crt_target_so_cflags_mips := \
    -fPIC
