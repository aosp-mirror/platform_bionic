#if __ANDROID_API__ <= 9
static int foo() __attribute__((availability(android, introduced = 9))) {
  return 0;
}
#else
int foo() __attribute__((availability(android, introduced = 10)));
#endif
