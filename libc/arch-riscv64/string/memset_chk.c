#include <stdint.h>

extern void* memset(void*, int, size_t);
extern void* __memset_chk_fail(void*, int, size_t, size_t);

void* __memset_chk(void* dst, int c, size_t n, size_t dst_len) {
  if (dst_len < n) __memset_chk_fail(dst, c, n, dst_len);
  return memset(dst, c, n);
}
