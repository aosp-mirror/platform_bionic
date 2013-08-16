$(call libc-add-cpu-variant-src,MEMCPY,arch-arm/cortex-a9/bionic/memcpy.S)
$(call libc-add-cpu-variant-src,MEMSET,arch-arm/cortex-a9/bionic/memset.S)
$(call libc-add-cpu-variant-src,STRCAT,arch-arm/cortex-a9/bionic/strcat.S)
$(call libc-add-cpu-variant-src,STRCMP,arch-arm/cortex-a9/bionic/strcmp.S)
$(call libc-add-cpu-variant-src,STRCPY,arch-arm/cortex-a9/bionic/strcpy.S)
$(call libc-add-cpu-variant-src,STRLEN,arch-arm/cortex-a9/bionic/strlen.S)
$(call libc-add-cpu-variant-src,__STRCAT_CHK,arch-arm/cortex-a9/bionic/__strcat_chk.S)
$(call libc-add-cpu-variant-src,__STRCPY_CHK,arch-arm/cortex-a9/bionic/__strcpy_chk.S)

include bionic/libc/arch-arm/generic/generic.mk
