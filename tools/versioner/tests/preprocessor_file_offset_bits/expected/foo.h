typedef int off_t;
typedef int ssize_t;
typedef unsigned size_t;

#if !defined(__LP64__) && defined(_FILE_OFFSET_BITS)
#if _FILE_OFFSET_BITS == 64
#define __USE_FILE_OFFSET64 1
#endif
#endif

#define __RENAME(x) __asm__(#x)

#if defined(__USE_FILE_OFFSET64) && __ANDROID_API__ >= 21
int truncate(const char* __path, off_t __length) __RENAME(truncate64) __INTRODUCED_IN(21);
#else
int truncate(const char* __path, off_t __length);
#endif

#if defined(__USE_FILE_OFFSET64)

#if __ANDROID_API__ >= 12
ssize_t pread(int __fd, void* __buf, size_t __count, off_t __offset) __RENAME(pread64)
    __INTRODUCED_IN(12);
#endif /* __ANDROID_API__ >= 12 */

#else
ssize_t pread(int __fd, void* __buf, size_t __count, off_t __offset);
#endif

#if defined(__USE_FILE_OFFSET64)
off_t lseek(int __fd, off_t __offset, int __whence) __RENAME(lseek64);
#else
off_t lseek(int __fd, off_t __offset, int __whence);
#endif
