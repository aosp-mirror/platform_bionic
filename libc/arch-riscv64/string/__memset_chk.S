#include <private/bionic_asm.h>

ENTRY(__memset_chk)
    bleu    a2, a3, 1f
    call    __memset_chk_fail

1:
    tail   memset
END(__memset_chk)

