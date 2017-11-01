// Mock an invalid CFI-enabled library.
__attribute__((aligned(4096))) extern "C" char dummy[16] = {};
__asm__(".globl __cfi_check");
__asm__("__cfi_check = dummy + 3"); // Not aligned to anything.
