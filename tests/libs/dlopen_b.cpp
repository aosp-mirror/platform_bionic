#include <dlfcn.h>
extern "C" void *dlopen_b() {
  // This is not supposed to succeed. Even though this library has DT_RUNPATH
  // for libtest_dt_runpath_x.so, it is not taked into account for dlopen.
  void *handle = dlopen("libtest_dt_runpath_x.so", RTLD_NOW);
  return handle;
}
