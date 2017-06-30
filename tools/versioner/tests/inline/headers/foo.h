#if defined(__cplusplus)
extern "C" {
#endif

#if __ANDROID_API__ < 12
static int foo() {
  return 0;
}
#else
int foo() __INTRODUCED_IN(12);
#endif

#if defined(__cplusplus)
}
#endif
