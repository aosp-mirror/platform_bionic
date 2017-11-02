#if defined(__cplusplus)
extern "C" {
#endif

#define __RENAME(x) __asm__(#x)

#if defined(__cplusplus)
extern "C++" char* basename(char*) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
extern "C++" const char* basename(const char*) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
#else
char* basename(const char*) __RENAME(__gnu_basename) __INTRODUCED_IN(23);
#endif

char* foo() __INTRODUCED_IN(8);

#if defined(__cplusplus)
}
#endif
