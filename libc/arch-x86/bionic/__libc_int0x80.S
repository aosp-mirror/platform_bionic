#include <private/bionic_asm.h>

// void __libc_int0x80();
ENTRY_PRIVATE(__libc_int0x80)
  int $0x80
  ret
END(__libc_int0x80)
