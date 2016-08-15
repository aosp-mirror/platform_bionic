#if __ANDROID_API__ >= 10
int foo() __INTRODUCED_IN(10);
#endif

#if __ANDROID_API__ >= 21
int bar(int) __INTRODUCED_IN(21);
#endif

#if __ANDROID_API__ >= 10
int multiple_1() __INTRODUCED_IN(10);
int multiple_2() __INTRODUCED_IN(10);
#endif
