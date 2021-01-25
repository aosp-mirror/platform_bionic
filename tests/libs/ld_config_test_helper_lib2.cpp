#include <dlfcn.h>
#include <stdio.h>

// Mark foo and bar weak so that Clang allows the run-time linker to decide which DSO's symbol to
// use.

__attribute__((weak)) extern "C" void bar() {
  printf("bar lib2\n");
  void (*next)(void) = reinterpret_cast<void (*)()>(dlsym(RTLD_NEXT, "bar"));
  if (next != nullptr) next();
}
