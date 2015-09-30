# This file represents the best optimized routines that are the middle
# ground when running on a big/little system that is cortex-a57/cortex-a53.
# The cortex-a7 optimized routines, and the cortex-a53 optimized routines
# decrease performance on cortex-a57 processors by as much as 20%.

libc_bionic_src_files_arm += \
    arch-arm/cortex-a15/bionic/memcpy.S \
    arch-arm/cortex-a15/bionic/memset.S \
    arch-arm/cortex-a15/bionic/stpcpy.S \
    arch-arm/cortex-a15/bionic/strcat.S \
    arch-arm/cortex-a15/bionic/__strcat_chk.S \
    arch-arm/cortex-a15/bionic/strcmp.S \
    arch-arm/cortex-a15/bionic/strcpy.S \
    arch-arm/cortex-a15/bionic/__strcpy_chk.S \
    arch-arm/cortex-a15/bionic/strlen.S \

libc_bionic_src_files_arm += \
    arch-arm/generic/bionic/memcmp.S \

libc_bionic_src_files_arm += \
    arch-arm/denver/bionic/memmove.S \
