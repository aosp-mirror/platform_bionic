libc_bionic_src_files_arm += \
    arch-arm/generic/bionic/memcmp.S \
    arch-arm/generic/bionic/memcpy.S \
    arch-arm/generic/bionic/memset.S \
    arch-arm/generic/bionic/strcmp.S \
    arch-arm/generic/bionic/strcpy.S \
    arch-arm/generic/bionic/strlen.c \
    bionic/__strcat_chk.cpp \
    bionic/__strcpy_chk.cpp \

libc_openbsd_src_files_arm += \
    upstream-openbsd/lib/libc/string/memmove.c \
    upstream-openbsd/lib/libc/string/stpcpy.c \
    upstream-openbsd/lib/libc/string/strcat.c \
