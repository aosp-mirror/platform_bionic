#if defined(__arm__)
int foo() __attribute__((availability(android, introduced = 9)));
#else
int foo() __attribute__((availability(android, introduced = 10000)));
#endif
