#include <dlfcn.h>

// Work around for http://b/20049306, which isn't going to be fixed.
int defeat_sibling_call_optimization = 0;

extern "C" void* dlopen_b() {
  // This is supposed to succeed because this library has DT_RUNPATH
  // for libtest_dt_runpath_x.so which should be taken into account
  // by dlopen.
  void *handle = dlopen("libtest_dt_runpath_x.so", RTLD_NOW);
  if (handle != nullptr) {
    defeat_sibling_call_optimization++;
    return handle;
  }
  return nullptr;
}
