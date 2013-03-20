$(call libc-add-cpu-variant-src,MEMCPY,arch-arm/cortex-a9/bionic/memcpy.S)
$(call libc-add-cpu-variant-src,MEMSET,arch-arm/cortex-a9/bionic/memset.S)
$(call libc-add-cpu-variant-src,STRCMP,arch-arm/cortex-a9/bionic/strcmp.S)

include bionic/libc/arch-arm/generic/generic.mk
