# 32-bit ABI bugs

## 32-bit `off_t` and `_FILE_OFFSET_BITS=64`

On 32-bit Android, `off_t` is a signed 32-bit integer. This limits functions
that use `off_t` to working on files no larger than 2GiB.

Android does not require the `_LARGEFILE_SOURCE` macro to be used to make
`fseeko` and `ftello` available. Instead they're always available from API
level 24 where they were introduced, and never available before then.

Android also does not require the `_LARGEFILE64_SOURCE` macro to be used
to make `off64_t` and corresponding functions such as `ftruncate64` available.
Instead, whatever subset of those functions was available at your target API
level will be visible.

There are a couple of exceptions to note. Firstly, `off64_t` and the single
function `lseek64` were available right from the beginning in API 3. Secondly,
Android has always silently inserted `O_LARGEFILE` into any open call, so if
all you need are functions like `read` that don't take/return `off_t`, large
files have always worked.

Android support for `_FILE_OFFSET_BITS=64` (which turns `off_t` into `off64_t`
and replaces each `off_t` function with its `off64_t` counterpart, such as
`lseek` in the source becoming `lseek64` at runtime) was added late. Even when
it became available for the platform, it wasn't available from the NDK until
r15. Before NDK r15, `_FILE_OFFSET_BITS=64` silently did nothing: all code
compiled with that was actually using a 32-bit `off_t`. With a new enough NDK,
the situation becomes complicated. If you're targeting an API before 21, almost
all functions that take an `off_t` become unavailable. You've asked for their
64-bit equivalents, and none of them (except `lseek`/`lseek64`) exist. As you
increase your target API level, you'll have more and more of the functions
available. API 12 adds some of the `<unistd.h>` functions, API 21 adds `mmap`,
and by API 24 you have everything including `<stdio.h>`. See the
[linker map](libc/libc.map.txt) for full details. Note also that in NDK r16 and
later, if you're using Clang we'll inline an `mmap64` implementation in the
headers when you target an API before 21 because it's an easy special case
that's often needed. This means that code using `_FILE_OFFSET_BITS=64`
and `mmap` (but no other functions that are unavailable at your target
API level) will always compile.

If your code stops compiling when you move to NDK r15 or later, removing every
definition of `_FILE_OFFSET_BITS=64` will restore the behavior you used to have:
you'll have a 32-bit `off_t` and use the 32-bit functions. Make sure you
grep thoroughly in both your source and your build system: many people
aren't aware that `_FILE_OFFSET_BITS` is set. You might also have to
remove references to `__USE_FILE_OFFSET64` --- this is the internal
flag that should never be set by user code but sometimes is (by zlib,
for example). If you think you have removed these but your code still
doesn't compile, you can insert this just before the line that's failing
to double check:
```
#if _FILE_OFFSET_BITS == 64
#error "oops, file _FILE_OFFSET_BITS == 64"
#elif defined(__USE_FILE_OFFSET64)
#error "oops, __USE_FILE_OFFSET64 is defined"
#endif
```

In the 64-bit ABI, `off_t` is always 64-bit.

For source compatibility, the names containing `64` are also available
in the 64-bit ABI even though they're identical to the non-`64` names.


## `sigset_t` is too small for real-time signals

On 32-bit Android, `sigset_t` is too small for ARM and x86. This means that
there is no support for real-time signals in 32-bit code. Android P (API
level 28) adds `sigset64_t` and a corresponding function for every function
that takes a `sigset_t` (so `sigprocmask64` takes a `sigset64_t` where
`sigprocmask` takes a `sigset_t`).

On 32-bit Android, `struct sigaction` is also too small because it contains
a `sigset_t`. We also offer a `struct sigaction64` and `sigaction64` function
to work around this.

In the 64-bit ABI, `sigset_t` is the correct size for every architecture.

For source compatibility, the names containing `64` are also available
in the 64-bit ABI even though they're identical to the non-`64` names.


## `time_t` is 32-bit

On 32-bit Android, `time_t` is 32-bit. The header `<time64.h>` and type
`time64_t` exist as a workaround, but the kernel interfaces exposed on 32-bit
Android all use the 32-bit `time_t`.

In the 64-bit ABI, `time_t` is 64-bit.


## `pthread_mutex_t` is too small for large pids

This doesn't generally affect Android devices, because on devices
`/proc/sys/kernel/pid_max` is usually too small to hit our 16-bit limit,
but 32-bit bionic's `pthread_mutex` is a total of 32 bits, leaving just
16 bits for the owner thread id. This means bionic isn't able to support
mutexes for tids that don't fit in 16 bits. This typically manifests as
a hang in `pthread_mutex_lock` if the libc startup code doesn't detect
this condition and abort.
