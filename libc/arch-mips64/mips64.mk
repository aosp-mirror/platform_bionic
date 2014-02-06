_LIBC_ARCH_COMMON_SRC_FILES := \
    arch-mips64/bionic/__bionic_clone.S \
    arch-mips64/bionic/bzero.S \
    arch-mips64/bionic/cacheflush.cpp \
    arch-mips64/bionic/_exit_with_stack_teardown.S \
    arch-mips64/bionic/futex_mips.S \
    arch-mips64/bionic/__get_sp.S \
    arch-mips64/bionic/getdents.cpp \
    arch-mips64/bionic/memcmp16.S \
    arch-mips64/bionic/_setjmp.S \
    arch-mips64/bionic/setjmp.S \
    arch-mips64/bionic/__set_tls.c \
    arch-mips64/bionic/sigsetjmp.S \
    arch-mips64/bionic/syscall.S \
    arch-mips64/bionic/vfork.S \

# FIXME TODO
## _LIBC_ARCH_COMMON_SRC_FILES += arch-mips64/string/memcpy.S
## _LIBC_ARCH_COMMON_SRC_FILES += arch-mips64/string/memset.S
## _LIBC_ARCH_COMMON_SRC_FILES += arch-mips64/string/mips_strlen.c
_LIBC_ARCH_COMMON_SRC_FILES += bionic/memcpy.c
_LIBC_ARCH_COMMON_SRC_FILES += bionic/memset.c
_LIBC_ARCH_COMMON_SRC_FILES += string/strlen.c

_LIBC_ARCH_STATIC_SRC_FILES := \
    bionic/dl_iterate_phdr_static.c \

_LIBC_ARCH_DYNAMIC_SRC_FILES :=
