libc_bionic_src_files_x86 += \
    arch-x86/atom/string/sse2-memchr-atom.S \
    arch-x86/atom/string/sse2-memrchr-atom.S \
    arch-x86/atom/string/sse2-strchr-atom.S \
    arch-x86/atom/string/sse2-strnlen-atom.S \
    arch-x86/atom/string/sse2-strrchr-atom.S \
    arch-x86/atom/string/sse2-wcschr-atom.S \
    arch-x86/atom/string/sse2-wcsrchr-atom.S \
    arch-x86/atom/string/sse2-wcslen-atom.S \
    arch-x86/atom/string/sse2-wcscmp-atom.S \
    arch-x86/silvermont/string/sse2-bcopy-slm.S \
    arch-x86/silvermont/string/sse2-bzero-slm.S \
    arch-x86/silvermont/string/sse2-memcpy-slm.S \
    arch-x86/silvermont/string/sse2-memmove-slm.S \
    arch-x86/silvermont/string/sse2-memset-slm.S \
    arch-x86/silvermont/string/sse2-stpcpy-slm.S \
    arch-x86/silvermont/string/sse2-stpncpy-slm.S \
    arch-x86/silvermont/string/sse2-strcpy-slm.S \
    arch-x86/silvermont/string/sse2-strlen-slm.S \
    arch-x86/silvermont/string/sse2-strncpy-slm.S

ifeq ($(ARCH_X86_HAVE_SSSE3),true)
libc_bionic_src_files_x86 += \
    arch-x86/atom/string/ssse3-strncat-atom.S \
    arch-x86/atom/string/ssse3-strlcat-atom.S \
    arch-x86/atom/string/ssse3-strlcpy-atom.S \
    arch-x86/atom/string/ssse3-strcmp-atom.S \
    arch-x86/atom/string/ssse3-strncmp-atom.S \
    arch-x86/atom/string/ssse3-strcat-atom.S \
    arch-x86/atom/string/ssse3-wcscat-atom.S \
    arch-x86/atom/string/ssse3-wcscpy-atom.S
else
libc_bionic_src_files_x86 += \
    arch-x86/generic/string/strcmp.S \
    arch-x86/generic/string/strncmp.S \
    arch-x86/generic/string/strcat.S \
    upstream-freebsd/lib/libc/string/wcscpy.c \
    upstream-freebsd/lib/libc/string/wcscat.c \
    upstream-openbsd/lib/libc/string/strlcat.c \
    upstream-openbsd/lib/libc/string/strlcpy.c \
    upstream-openbsd/lib/libc/string/strncat.c
endif

ifeq ($(ARCH_X86_HAVE_SSE4),true)
 libc_bionic_src_files_x86 += \
    arch-x86/silvermont/string/sse4-memcmp-slm.S \
    arch-x86/silvermont/string/sse4-wmemcmp-slm.S
else
libc_bionic_src_files_x86 += \
    arch-x86/generic/string/memcmp.S \
    upstream-freebsd/lib/libc/string/wmemcmp.c
endif
