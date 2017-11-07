#define __RENAME(x) asm(#x)

#if defined(__cplusplus)
extern "C++" const char* strchrnul(const char*, int) __RENAME(strchrnul) __INTRODUCED_IN(24);
#endif

#if defined(__cplusplus)
extern "C" int foo();
#endif

#if defined(__cplusplus)
extern "C" {
#endif

char* strchrnul(char*, int) __INTRODUCED_IN(24);

#if defined(__cplusplus)
}
#endif
