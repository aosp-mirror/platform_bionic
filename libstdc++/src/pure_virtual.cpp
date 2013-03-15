#undef NDEBUG
#include <assert.h>

extern "C" void __cxa_pure_virtual() {
  // We can't call __libc_format_log from libstdc++ because it's hidden and in libc, so cheat.
  assert(!"Pure virtual function called. Are you calling virtual methods from a destructor?");
  /* NOTREACHED */
}
