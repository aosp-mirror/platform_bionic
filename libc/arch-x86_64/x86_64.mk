_LIBC_ARCH_COMMON_SRC_FILES := \
    arch-x86_64/bionic/clone.S \
    arch-x86_64/bionic/_exit_with_stack_teardown.S \
    arch-x86_64/bionic/futex_x86_64.S \
    arch-x86_64/bionic/__get_sp.S \
    arch-x86_64/bionic/__rt_sigreturn.S \
    arch-x86_64/bionic/_setjmp.S \
    arch-x86_64/bionic/setjmp.S \
    arch-x86_64/bionic/__set_tls.c \
    arch-x86_64/bionic/sigaction.c \
    arch-x86_64/bionic/sigsetjmp.S \
    arch-x86_64/bionic/syscall.S \

_LIBC_ARCH_STATIC_SRC_FILES := \
    bionic/dl_iterate_phdr_static.c \

_LIBC_ARCH_DYNAMIC_SRC_FILES :=
