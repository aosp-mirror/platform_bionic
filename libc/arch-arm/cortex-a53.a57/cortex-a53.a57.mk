# This file represents the best optimized routines that are the middle
# ground when running on a big/little system that is cortex-a57/cortex-a53.
# The cortex-a7 optimized routines, and the cortex-a53 optimized routines
# decrease performance on cortex-a57 processors by as much as 20%.

libc_openbsd_src_files_exclude_arm += \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \

libc_bionic_src_files_exclude_arm += \
    arch-arm/generic/bionic/memcpy.S \
    arch-arm/generic/bionic/memset.S \
    arch-arm/generic/bionic/strcmp.S \
    arch-arm/generic/bionic/strcpy.S \
    arch-arm/generic/bionic/strlen.c \
    bionic/__strcat_chk.cpp \
    bionic/__strcpy_chk.cpp \

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
    arch-arm/denver/bionic/memmove.S \
