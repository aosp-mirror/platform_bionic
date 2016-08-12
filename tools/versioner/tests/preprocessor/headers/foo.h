int always_available();

int also_always_available() __INTRODUCED_IN(9);

int needs_guard() __INTRODUCED_IN(10);

#if __ANDROID_API__ >= 10
int already_guarded() __INTRODUCED_IN(10);
#endif

#if defined(__arm__)
int specific_arch() __INTRODUCED_IN(11);
#endif

#if defined(__arm__) || defined(__i386__)
int multiple_archs() __INTRODUCED_IN(11);
#endif

// __INTRODUCED_IN_64(21) should be ignored.
int multiple_introduced_1() __INTRODUCED_IN_ARM(10) __INTRODUCED_IN_MIPS(11) __INTRODUCED_IN_X86(10)
    __INTRODUCED_IN_64(21);

int multiple_introduced_2() __INTRODUCED_IN_ARM(10) __INTRODUCED_IN_MIPS(11) __INTRODUCED_IN_X86(10)
    __INTRODUCED_IN_64(22);

int group_lp32() __INTRODUCED_IN_ARM(12) __INTRODUCED_IN_X86(12) __INTRODUCED_IN_MIPS(12);
