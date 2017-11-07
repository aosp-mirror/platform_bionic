#define __RENAME(x) asm(#x)

#if defined(__cplusplus)

#if __ANDROID_API__ >= 24
extern "C++" const char* strchrnul(const char*, int) __RENAME(strchrnul) __INTRODUCED_IN(24);
#endif /* __ANDROID_API__ >= 24 */

#endif

#if defined(__cplusplus)
extern "C" int foo();
#endif

#if defined(__cplusplus)
extern "C" {
#endif


#if __ANDROID_API__ >= 24
char* strchrnul(char*, int) __INTRODUCED_IN(24);
#endif /* __ANDROID_API__ >= 24 */


#if defined(__cplusplus)
}
#endif
