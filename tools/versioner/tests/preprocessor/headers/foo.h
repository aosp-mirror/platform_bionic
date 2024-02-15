#if defined(__cplusplus)
extern "C" {
#endif

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
int multiple_introduced_1() __INTRODUCED_IN_32(24) __INTRODUCED_IN_64(21);

// This needs different guards for 32 vs 64.
int multiple_introduced_2() __INTRODUCED_IN_32(24) __INTRODUCED_IN_64(26);

// This produces both an LP64 and a not-LP64 check, but it doesn't need to check for all 64-bit
// targets separately.
int multiple_introduced_3() __INTRODUCED_IN_32(23) __INTRODUCED_IN_64(23);

int added_to_lp64_late() __INTRODUCED_IN_64(28);

#if defined(__cplusplus)
}
#endif
