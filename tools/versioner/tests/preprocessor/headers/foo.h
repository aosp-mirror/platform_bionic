int always_available();

int also_always_available() __INTRODUCED_IN(9);

int needs_guard() __INTRODUCED_IN(13);

#if __ANDROID_API__ >= 12
int needs_guard_2() __INTRODUCED_IN(13);
#endif

#if __ANDROID_API__ >= 13
int already_guarded() __INTRODUCED_IN(13);
#endif

#if __ANDROID_API__ > 13
int already_guarded_2() __INTRODUCED_IN(13);
#endif

#if defined(__arm__)
int specific_arch() __INTRODUCED_IN(14);

#if __ANDROID_API__ >= 14
int specific_arch_already_guarded() __INTRODUCED_IN(14);
#endif

#if __ANDROID_API__ > 14
int specific_arch_already_guarded_2() __INTRODUCED_IN(14);
#endif
#endif

#if defined(__arm__) || defined(__i386__)
int multiple_archs() __INTRODUCED_IN(14);
#endif

// __INTRODUCED_IN_64(21) should be ignored.
int multiple_introduced_1() __INTRODUCED_IN_ARM(13) __INTRODUCED_IN_MIPS(14) __INTRODUCED_IN_X86(13)
    __INTRODUCED_IN_64(21);

int multiple_introduced_2() __INTRODUCED_IN_ARM(13) __INTRODUCED_IN_MIPS(14) __INTRODUCED_IN_X86(13)
    __INTRODUCED_IN_64(22);

int group_lp32() __INTRODUCED_IN_ARM(12) __INTRODUCED_IN_X86(12) __INTRODUCED_IN_MIPS(12);

int future() __INTRODUCED_IN_FUTURE;
