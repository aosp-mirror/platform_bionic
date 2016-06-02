#include <archdep.h>
#include <foodep.h>

#if defined(__i386__)
x86_t foo(foo_t);
#elif defined(__arm__)
arm_t foo(foo_t);
#endif
