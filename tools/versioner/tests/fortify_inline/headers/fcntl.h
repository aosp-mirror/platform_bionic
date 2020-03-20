#if defined(__cplusplus)
extern "C" {
#endif

extern int open_real(const char* name, int flags, ...) __asm__("open");

#define O_CREAT 00000100

typedef unsigned int mode_t;

static inline __attribute__((always_inline))
int open(const char* name, int flags)
    __attribute__((annotate("versioner_fortify_inline")))
    __attribute__((overloadable))
    __attribute__((enable_if(!(flags & O_CREAT), ""))) {
  return open_real(name, flags);
}

static inline __attribute__((always_inline))
int open(const char* name, int flags, mode_t mode)
    __attribute__((annotate("versioner_fortify_inline")))
    __attribute__((overloadable))
    __attribute__((enable_if(flags & O_CREAT, ""))) {
  return open_real(name, flags, mode);
}

#if defined(__cplusplus)
}
#endif
