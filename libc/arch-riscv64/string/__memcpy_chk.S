#include <private/bionic_asm.h>

ENTRY(__memcpy_chk)
  bleu a2, a3, 1f
  call __memcpy_chk_fail

1:
   tail memcpy
END(__memcpy_chk)
