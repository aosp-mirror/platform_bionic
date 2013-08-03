_LIBC_ARCH_COMMON_SRC_FILES := \
    arch-x86/bionic/clone.S \
    arch-x86/bionic/_exit_with_stack_teardown.S \
    arch-x86/bionic/futex_x86.S \
    arch-x86/bionic/__get_sp.S \
    arch-x86/bionic/__get_tls.c \
    arch-x86/bionic/_setjmp.S \
    arch-x86/bionic/setjmp.S \
    arch-x86/bionic/__set_tls.c \
    arch-x86/bionic/sigsetjmp.S \
    arch-x86/bionic/syscall.S \
    arch-x86/bionic/vfork.S \
    arch-x86/string/ffs.S

ifeq ($(ARCH_X86_HAVE_SSSE3),true)
_LIBC_ARCH_COMMON_SRC_FILES += \
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
_LIBC_ARCH_COMMON_SRC_FILES += \
	arch-x86/string/memcpy.S \
	arch-x86/string/memmove.S \
	arch-x86/string/bcopy.S \
	arch-x86/string/strcmp.S \
	arch-x86/string/strncmp.S \
	arch-x86/string/strcat.S \
	arch-x86/string/memcmp.S \
	string/memcmp16.c \
	string/strcpy.c \
	string/strncat.c \
	string/strncpy.c \
	string/strlcat.c \
	string/strlcpy.c \
	upstream-freebsd/lib/libc/string/wcscpy.c \
	upstream-freebsd/lib/libc/string/wcscat.c \
	upstream-freebsd/lib/libc/string/wmemcmp.c
endif

ifeq ($(ARCH_X86_HAVE_SSE2),true)
_LIBC_ARCH_COMMON_SRC_FILES += \
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
	arch-x86/string/sse2-wcscmp-atom.S
else
_LIBC_ARCH_COMMON_SRC_FILES += \
	arch-x86/string/memset.S \
	arch-x86/string/strlen.S \
	arch-x86/string/bzero.S \
	bionic/memrchr.c \
	bionic/memchr.c \
	string/strchr.cpp \
	string/strrchr.c \
	string/index.c \
	bionic/strnlen.c \
	upstream-freebsd/lib/libc/string/wcschr.c \
	upstream-freebsd/lib/libc/string/wcsrchr.c \
	upstream-freebsd/lib/libc/string/wcslen.c \
	upstream-freebsd/lib/libc/string/wcscmp.c
endif

_LIBC_ARCH_STATIC_SRC_FILES := \
    bionic/dl_iterate_phdr_static.c \

_LIBC_ARCH_DYNAMIC_SRC_FILES :=
