#if __ANDROID_API__ <= 9
int foo() __attribute__((availability(android, introduced = 9)));
#else
int foo() __attribute__((availability(android, introduced = 10)));
#endif
