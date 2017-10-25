#if defined(__cplusplus)
extern "C" {
#endif

#if __ANDROID_API__ <= 9
int foo() __INTRODUCED_IN(9);
#else
int foo() __INTRODUCED_IN(10);
#endif

#if defined(__cplusplus)
}
#endif
