#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__arm__)
int foo() __INTRODUCED_IN(9);
#else
int foo() __INTRODUCED_IN_FUTURE;
#endif

#if defined(__cplusplus)
}
#endif
