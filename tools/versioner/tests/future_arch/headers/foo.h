#if defined(__arm__)
int foo() __INTRODUCED_IN(9);
#else
int foo() __INTRODUCED_IN_FUTURE;
#endif
