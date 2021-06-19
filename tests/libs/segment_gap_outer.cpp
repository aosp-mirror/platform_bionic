#include <android/dlext.h>
#include <dlfcn.h>
#include <stdlib.h>

extern "C" void __attribute__((section(".custom_text"))) text_before_start_of_gap() {}
char __attribute__((section(".custom_bss"))) end_of_gap[0x1000];

extern "C" void* get_inner() {
  android_dlextinfo info = {};
  info.flags = ANDROID_DLEXT_RESERVED_ADDRESS;

  char* start_of_gap =
      reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(text_before_start_of_gap) & ~0xfffull) +
      0x1000;
  info.reserved_addr = start_of_gap;
  info.reserved_size = end_of_gap - start_of_gap;

  void *handle = android_dlopen_ext("libsegment_gap_inner.so", RTLD_NOW, &info);
  if (!handle) {
    __builtin_trap();
  }

  return dlsym(handle, "inner");
}
