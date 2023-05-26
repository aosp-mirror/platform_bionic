# When to use which `#define`

Using `#ifdef` or equivalents is common when writing portable code. Which to use
when can be quite tricky. This document describes the most common choices
related to Android.

## `__BIONIC__`

If your code is specific to Android's C library, bionic, use `__BIONIC__`. This
is typically a good choice when you use libc API that's only in bionic, such as
the system property functions. Common alternatives on this dimension are
`__GLIBC__`, `__APPLE__`, or `_WIN32`. Note that although bionic is most often
seen on Android devices, it is possible to use bionic on the host too.

## `__ANDROID__`

If your code is specific to Android devices, use `__ANDROID__`. This isn't
useful as you might think, and one of the other choices on this page is usually
more appropriate. This is typically a good choice if you have code that's part
of the OS and needs to behave differently on the host than on the device.
Genuine cases are quite rare, and `__BIONIC__` is often more specific (but
remember that it is possible -- if unusual -- to use bionic on the host).

## `__ANDROID_API__`

If your code can be built targeting a variety of different OS versions, use
`__ANDROID_API__` to test which version you're building against. This is
typically useful if you can use new NDK APIs when available, but don't require
them if not.

One thing to note (if your code may also be built as part of the OS itself) is
that for most of the year, the OS builds with this set to 10,000 rather than the
obvious "next" API level such as 19. Once the API level has been decided, the
value of `__ANDROID_API__` drops to that number.

## `__linux__`

If your code requires a Linux kernel, use `__linux__`. This is typically a good
choice when you use Linux-specific API, such as a Linux-specific system call or
a file in `/proc`, but aren't restricted to just Android and would work equally
well on a desktop Linux distro, say. Common alternatives on this dimension
are `__APPLE__` or `_WIN32`.

## `__ANDROID_NDK__`

If your code can be built either as part of an app _or_ as part of the OS
itself, use `__ANDROID_NDK__` to differentiate between those two circumstances.
This is typically a good choice when your code uses non-NDK API if it's built as
part of the OS, but sticks to just the NDK APIs otherwise.

## `__NDK_MAJOR__`, `__NDK_MINOR__`, `__NDK_BETA__`, `__NDK_BUILD__`, `__NDK_CANARY__`

If your code can be built with a variety of different NDK versions, and needs to
work around issues with some of them, use these macros to detect the versinon of
the NDK you're being built with. Usually only `__NDK_MAJOR__` will be necessary.

## `__arm__`/`__aarch64__`, `__i386__`/`__x86_64__`, `__riscv`

If your code is specific to a particular processor architecture, use
these macros to conditionally compile. Note that the ABI usually called
`arm64` uses the macro `__aarch64__` and the ABI usually called `x86` uses
`__i386__`. Android only supports riscv64, so `__riscv` is a sufficient
check for Android-only code. If you need to write code portable to other
operating systems that do support riscv32, you'll also need to check
whether `__riscv_xlen` is 32 or 64.

## `__ILP32__` and `__LP64__`

If your code depends on "bitness" -- whether `long` and pointers are 32-
or 64-bit -- use these macros to conditionally compile. Note the extra
"I" in the 32-bit macro (since `int`, `long`, and pointers are all 32-bit
on such systems, with `long long` being needed for a 64-bit type).
