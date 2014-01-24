_LIBC_ARCH_COMMON_SRC_FILES := \
    arch-arm64/bionic/__bionic_clone.S \
    arch-arm64/bionic/bzero_arm64.c \
    arch-arm64/bionic/cacheflush_arm64.c \
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

_LIBC_ARCH_STATIC_SRC_FILES := \
    bionic/dl_iterate_phdr_static.c \

_LIBC_ARCH_DYNAMIC_SRC_FILES :=
