#include <dlfcn.h>
extern "C" void *dlopen_b() {
  // TODO (dimitry): this is to work around http://b/20049306
  // remove once it is fixed
  static int dummy = 0;

  // This is not supposed to succeed. Even though this library has DT_RUNPATH
  // for libtest_dt_runpath_x.so, it is not taked into account for dlopen.
  void *handle = dlopen("libtest_dt_runpath_x.so", RTLD_NOW);
  if (handle != nullptr) {
    dummy++;
    return handle;
  }
  return nullptr;
}
