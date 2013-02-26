_LIBC_ARCH_COMMON_SRC_FILES := \
    arch-mips/bionic/__get_sp.S \
    arch-mips/bionic/__get_tls.c \
    arch-mips/bionic/__set_tls.c \
    arch-mips/bionic/_exit_with_stack_teardown.S \
    arch-mips/bionic/_setjmp.S \
    arch-mips/bionic/futex_mips.S \
    arch-mips/bionic/bzero.S \
    arch-mips/bionic/cacheflush.c \
    arch-mips/bionic/clone.S \
    arch-mips/bionic/ffs.S \
    arch-mips/bionic/memcmp16.S \
    arch-mips/bionic/memmove.c \
    arch-mips/bionic/pipe.S \
    arch-mips/bionic/setjmp.S \
    arch-mips/bionic/sigsetjmp.S \
    arch-mips/bionic/vfork.S \
    arch-mips/string/memset.S \
    arch-mips/string/memcpy.S \
    arch-mips/string/mips_strlen.c \

_LIBC_ARCH_STATIC_SRC_FILES := \
    bionic/dl_iterate_phdr_static.c \

_LIBC_ARCH_DYNAMIC_SRC_FILES :=
