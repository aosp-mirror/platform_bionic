_LIBC_ARCH_COMMON_SRC_FILES := \
    arch-arm/bionic/abort_arm.S \
    arch-arm/bionic/atomics_arm.c \
    arch-arm/bionic/clone.S \
    arch-arm/bionic/eabi.c \
    arch-arm/bionic/_exit_with_stack_teardown.S \
    arch-arm/bionic/ffs.S \
    arch-arm/bionic/futex_arm.S \
    arch-arm/bionic/__get_sp.S \
    arch-arm/bionic/kill.S \
    arch-arm/bionic/libgcc_compat.c \
    arch-arm/bionic/memcmp16.S \
    arch-arm/bionic/memcmp.S \
    arch-arm/bionic/_setjmp.S \
    arch-arm/bionic/setjmp.S \
    arch-arm/bionic/sigsetjmp.S \
    arch-arm/bionic/syscall.S \
    arch-arm/bionic/tgkill.S \
    arch-arm/bionic/tkill.S \

# These are used by the static and dynamic versions of the libc
# respectively.
_LIBC_ARCH_STATIC_SRC_FILES := \
    arch-arm/bionic/exidx_static.c

_LIBC_ARCH_DYNAMIC_SRC_FILES := \
    arch-arm/bionic/exidx_dynamic.c

# Remove the C++ fortify function implementations for which there is an
# arm assembler version.
_LIBC_FORTIFY_FILES_TO_REMOVE := \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \
    bionic/__strcpy_chk.cpp \
    bionic/__strcat_chk.cpp \

libc_common_src_files := \
    $(filter-out $(_LIBC_FORTIFY_FILES_TO_REMOVE),$(libc_common_src_files))

ifeq ($(strip $(wildcard bionic/libc/arch-arm/$(TARGET_CPU_VARIANT)/$(TARGET_CPU_VARIANT).mk)),)
$(error "TARGET_CPU_VARIANT not set or set to an unknown value. Possible values are cortex-a7, cortex-a8, cortex-a9, cortex-a15, krait. Use generic for devices that do not have a CPU similar to any of the supported cpu variants.")
endif

include bionic/libc/arch-arm/$(TARGET_CPU_VARIANT)/$(TARGET_CPU_VARIANT).mk
