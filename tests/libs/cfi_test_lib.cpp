#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// This library is built for all targets, including host tests, so __cfi_slowpath may not be
// present. But it is only used in the bionic loader tests.
extern "C" __attribute__((weak)) void __cfi_slowpath(uint64_t, void*);

static int g_count;
static uint64_t g_last_type_id;
static void* g_last_address;
static void* g_last_diag;

extern "C" {

// Mock a CFI-enabled library without relying on the compiler.
__attribute__((aligned(4096))) void __cfi_check(uint64_t CallSiteTypeId, void* TargetAddr,
                                                void* Diag) {
  ++g_count;
  g_last_type_id = CallSiteTypeId;
  g_last_address = TargetAddr;
  g_last_diag = Diag;
}

int get_count() {
  return g_count;
}

uint64_t get_last_type_id() {
  return g_last_type_id;
}

void* get_last_address() {
  return g_last_address;
}

void* get_last_diag() {
  return g_last_diag;
}

void* get_global_address() {
  return &g_count;
}
}

// Check that CFI is set up in module constructors and destructors.
struct A {
  void check_cfi_self() {
    g_last_type_id = 0;
    assert(&__cfi_slowpath);
    // CFI check for an invalid address. Normally, this would kill the process by routing the call
    // back to the calling module's __cfi_check, which does the right thing based on
    // -fsanitize-recover / -fsanitize-trap. But this module has custom __cfi_check that does not do
    // any of that, so the result looks like a passing check.
    int zz;
    __cfi_slowpath(13, static_cast<void*>(&zz));
    assert(g_last_type_id == 13);
    // CFI check for a libc function. This never goes into this module's __cfi_check, and must pass.
    __cfi_slowpath(14, reinterpret_cast<void*>(&exit));
    assert(g_last_type_id == 13);
  }
  A() {
    check_cfi_self();
  }
  ~A() {
    check_cfi_self();
  }
} a;
