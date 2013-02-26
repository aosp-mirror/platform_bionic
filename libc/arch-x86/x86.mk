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
    arch-x86/string/bcopy_wrapper.S \
    arch-x86/string/bzero_wrapper.S \
    arch-x86/string/ffs.S \
    arch-x86/string/memcmp_wrapper.S \
    arch-x86/string/memcpy_wrapper.S \
    arch-x86/string/memmove_wrapper.S \
    arch-x86/string/memset_wrapper.S \
    arch-x86/string/strcmp_wrapper.S \
    arch-x86/string/strlen_wrapper.S \
    arch-x86/string/strncmp_wrapper.S \

_LIBC_ARCH_STATIC_SRC_FILES := \
    bionic/dl_iterate_phdr_static.c \

_LIBC_ARCH_DYNAMIC_SRC_FILES :=
