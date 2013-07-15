$(call libc-add-cpu-variant-src,MEMCPY,arch-arm/cortex-a9/bionic/memcpy.S)
$(call libc-add-cpu-variant-src,MEMSET,arch-arm/cortex-a9/bionic/memset.S)
$(call libc-add-cpu-variant-src,STRCAT,arch-arm/cortex-a9/bionic/strcat.S)
$(call libc-add-cpu-variant-src,STRCMP,arch-arm/cortex-a9/bionic/strcmp.S)
$(call libc-add-cpu-variant-src,STRCPY,arch-arm/cortex-a9/bionic/strcpy.S)
$(call libc-add-cpu-variant-src,STRLEN,arch-arm/cortex-a9/bionic/strlen.S)

include bionic/libc/arch-arm/generic/generic.mk
