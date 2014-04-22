# x86 specific configs

# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_common_src_files_x86 := \
    bionic/legacy_32_bit_support.cpp \
    bionic/ndk_cruft.cpp \
    bionic/time64.c \

# Fortify implementations of libc functions.
libc_common_src_files_x86 += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \


# These are shared by all the 32-bit targets, but not the 64-bit ones.
libc_bionic_src_files_x86 := \
    bionic/mmap.cpp

##########################################
### CPU specific source files
libc_bionic_src_files_x86 += \
    arch-x86/bionic/__bionic_clone.S \
    arch-x86/bionic/_exit_with_stack_teardown.S \
    arch-x86/bionic/futex_x86.S \
    arch-x86/bionic/__get_sp.S \
    arch-x86/bionic/_setjmp.S \
    arch-x86/bionic/setjmp.S \
    arch-x86/bionic/__set_tls.c \
    arch-x86/bionic/sigsetjmp.S \
    arch-x86/bionic/syscall.S \
    arch-x86/bionic/vfork.S \

ifeq ($(ARCH_X86_HAVE_SSSE3),true)
libc_bionic_src_files_x86 += \
    arch-x86/string/ssse3-memcpy-atom.S \
    arch-x86/string/ssse3-memmove-atom.S \
    arch-x86/string/ssse3-bcopy-atom.S \
    arch-x86/string/ssse3-strncat-atom.S \
    arch-x86/string/ssse3-strncpy-atom.S \
    arch-x86/string/ssse3-strlcat-atom.S \
    arch-x86/string/ssse3-strlcpy-atom.S \
    arch-x86/string/ssse3-strcmp-atom.S \
    arch-x86/string/ssse3-strncmp-atom.S \
    arch-x86/string/ssse3-strcat-atom.S \
    arch-x86/string/ssse3-strcpy-atom.S \
    arch-x86/string/ssse3-memcmp-atom.S \
    arch-x86/string/ssse3-wmemcmp-atom.S \
    arch-x86/string/ssse3-memcmp16-atom.S \
    arch-x86/string/ssse3-wcscat-atom.S \
    arch-x86/string/ssse3-wcscpy-atom.S
else
libc_bionic_src_files_x86 += \
    arch-x86/string/memcpy.S \
    arch-x86/string/memmove.S \
    arch-x86/string/bcopy.S \
    arch-x86/string/strcmp.S \
    arch-x86/string/strncmp.S \
    arch-x86/string/strcat.S \
    arch-x86/string/memcmp.S \
    bionic/__memcmp16.cpp \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-freebsd/lib/libc/string/wmemcmp.c \
    upstream-openbsd/lib/libc/string/strcpy.c \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c \
    upstream-openbsd/lib/libc/string/strncpy.c \

endif

libc_bionic_src_files_x86 += \
    arch-x86/string/sse2-memset-atom.S \
    arch-x86/string/sse2-bzero-atom.S \
    arch-x86/string/sse2-memchr-atom.S \
    arch-x86/string/sse2-memrchr-atom.S \
    arch-x86/string/sse2-strchr-atom.S \
    arch-x86/string/sse2-strrchr-atom.S \
    arch-x86/string/sse2-index-atom.S \
    arch-x86/string/sse2-strlen-atom.S \
    arch-x86/string/sse2-strnlen-atom.S \
    arch-x86/string/sse2-wcschr-atom.S \
    arch-x86/string/sse2-wcsrchr-atom.S \
    arch-x86/string/sse2-wcslen-atom.S \
    arch-x86/string/sse2-wcscmp-atom.S \


libc_crt_target_cflags_x86 := \
    -m32 \
    -I$(LOCAL_PATH)/arch-x86/include

libc_crt_target_ldflags_x86 := -melf_i386

libc_crt_target_crtbegin_file_x86 := \
     $(LOCAL_PATH)/arch-common/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_x86 := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c

libc_crt_target_so_cflags_x86 := \
    -fPIC
