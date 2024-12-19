# bionic documentation

[bionic](https://en.wikipedia.org/wiki/Bionic_(software)) is Android's
C library, math library, and dynamic linker.

## User documentation

* [Android bionic status](status.md) - where we are in terms of standards,
  and what changed with each OS release.
* [32-bit ABI bugs](32-bit-abi.md) - historical accidents we can never fix.
* [`EINTR`](EINTR.md) - what is the `EINTR` failure,
  and how can code deal with it?
* [When to use which `#define`](defines.md) - how to choose between
  `__ANDROID__` and `__BIONIC__` and all the other options for conditional
  compilation.
* [fdsan](fdsan.md) - bionic's file descriptor sanitizer,
  which detects use-after-close() bugs.
* [fdtrack](fdtrack.md) - bionic's file descriptor tracker,
  which helps debug file descriptor leaks.
* [C23](c23.md) - dealing with C23's breaking changes.

## Maintainer documentation

If you're trying to make changes to bionic _itself_, start with the
[bionic maintainer documentation](../README.md).

We also have more detail on several specific parts of the implementation:

* [The anatomy of bionic's `_FORTIFY_SOURCE`](clang_fortify_anatomy.md) -
  how does `_FORTIFY_SOURCE` work on bionic (primarily "with clang").
* [Android ELF TLS](elf-tls.md) - details of bionic's ELF TLS implementation.
* [Validating libc assembler](libc_assembler.md) - how to test changes to
  libc assembler routines.
* [Validating native allocator changes](native_allocator.md) - how to test
  changes to the native allocator.
