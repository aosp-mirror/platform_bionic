
#include <stdio.h>
#include <stdlib.h>

extern "C" void __cxa_pure_virtual()
{
    fprintf(stderr, "Pure virtual function called.  Are you calling virtual methods from a destructor?\n");
    abort();
}

