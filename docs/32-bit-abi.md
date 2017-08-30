32-bit ABI bugs
===============

`off_t` is 32-bit
-----------------

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
[linker map](libc/libc.map.txt) for full details.

In the 64-bit ABI, `off_t` is always 64-bit.


`sigset_t` is too small for real-time signals
---------------------------------------------

On 32-bit Android, `sigset_t` is too small for ARM and x86 (but correct for
MIPS). This means that there is no support for real-time signals in 32-bit
code.

In the 64-bit ABI, `sigset_t` is the correct size for every architecture.


`time_t` is 32-bit
------------------

On 32-bit Android, `time_t` is 32-bit. The header `<time64.h>` and type
`time64_t` exist as a workaround, but the kernel interfaces exposed on 32-bit
Android all use the 32-bit `time_t`.

In the 64-bit ABI, `time_t` is 64-bit.
