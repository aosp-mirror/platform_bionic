$(call libc-add-cpu-variant-src,MEMCPY,arch-arm/cortex-a15/bionic/memcpy.S)
$(call libc-add-cpu-variant-src,MEMSET,arch-arm/cortex-a15/bionic/memset.S)
$(call libc-add-cpu-variant-src,STRCMP,arch-arm/cortex-a15/bionic/strcmp.S)
$(call libc-add-cpu-variant-src,STRLEN,arch-arm/cortex-a15/bionic/strlen.S)

include bionic/libc/arch-arm/generic/generic.mk
