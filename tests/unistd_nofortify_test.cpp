
#ifdef _FORTIFY_SOURCE
#undef _FORTIFY_SOURCE
#endif

#define NOFORTIFY

#include "unistd_test.cpp"

#if defined(_FORTIFY_SOURCE)
#error "_FORTIFY_SOURCE has been redefined, fix the code to remove this redefinition."
#endif
