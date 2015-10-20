# 32-bit arm.

libc_bionic_src_files_arm += \
    arch-arm/generic/bionic/memcmp.S \
    arch-arm/generic/bionic/memcpy.S \
    arch-arm/generic/bionic/memset.S \
    arch-arm/generic/bionic/strcmp.S \
    arch-arm/generic/bionic/strcpy.S \
    arch-arm/generic/bionic/strlen.c \

libc_bionic_src_files_exclude_arm += \
    bionic/__memcpy_chk.cpp \
    bionic/__memset_chk.cpp \

libc_openbsd_src_files_exclude_arm += \
    upstream-openbsd/lib/libc/string/strcpy.c \

#
# Inherently architecture-specific code.
#

libc_bionic_src_files_arm += \
    arch-arm/bionic/abort_arm.S \
    arch-arm/bionic/atomics_arm.c \
    arch-arm/bionic/__bionic_clone.S \
    arch-arm/bionic/_exit_with_stack_teardown.S \
    arch-arm/bionic/libgcc_compat.c \
    arch-arm/bionic/popcount_tab.c \
    arch-arm/bionic/__restore.S \
    arch-arm/bionic/setjmp.S \
    arch-arm/bionic/syscall.S \
    arch-arm/bionic/vfork.S \

libc_arch_static_src_files_arm := arch-arm/bionic/exidx_static.c
libc_arch_dynamic_src_files_arm := arch-arm/bionic/exidx_dynamic.c

## CPU variant specific source files
ifeq ($(strip $(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT)),)
  $(warning TARGET_$(my_2nd_arch_prefix)ARCH is arm, but TARGET_$(my_2nd_arch_prefix)CPU_VARIANT is not defined)
endif
ifneq ($(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT),generic)
cpu_variant_mk := $(LOCAL_PATH)/arch-arm/$(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT)/$(TARGET_$(my_2nd_arch_prefix)CPU_VARIANT).mk
ifeq ($(wildcard $(cpu_variant_mk)),)
$(error "TARGET_$(my_2nd_arch_prefix)CPU_VARIANT not set or set to an unknown value. Possible values are cortex-a7, cortex-a8, cortex-a9, cortex-a15, krait, denver. Use generic for devices that do not have a CPU similar to any of the supported cpu variants.")
endif
include $(cpu_variant_mk)
libc_common_additional_dependencies += $(cpu_variant_mk)

cpu_variant_mk :=
endif


libc_crt_target_cflags_arm := \
    -I$(LOCAL_PATH)/arch-arm/include \
    -mthumb-interwork

libc_crt_target_so_cflags_arm :=

libc_crt_target_crtbegin_file_arm := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin.c

libc_crt_target_crtbegin_so_file_arm := \
    $(LOCAL_PATH)/arch-common/bionic/crtbegin_so.c
