*This document was originally written for a broad audience, and it was*
*determined that it'd be good to hold in Bionic's docs, too. Due to the*
*ever-changing nature of code, it tries to link to a stable tag of*
*Bionic's libc, rather than the live code in Bionic. Same for Clang.*
*Reader beware. :)*

# The Anatomy of Clang FORTIFY

## Objective

The intent of this document is to run through the minutiae of how Clang FORTIFY
actually works in Bionic at the time of writing. Other FORTIFY implementations
that target Clang should use very similar mechanics. This document exists in part
because many Clang-specific features serve multiple purposes simultaneously, so
getting up-to-speed on how things function can be quite difficult.

## Background

FORTIFY is a broad suite of extensions to libc aimed at catching misuses of
common library functions. Textually, these extensions exist purely in libc, but
all implementations of FORTIFY rely heavily on C language extensions in order
to function at all.

Broadly, FORTIFY implementations try to guard against many misuses of C
standard(-ish) libraries:
- Buffer overruns in functions where pointers+sizes are passed (e.g., `memcpy`,
  `poll`), or where sizes exist implicitly (e.g., `strcpy`).
- Arguments with incorrect values passed to libc functions (e.g.,
  out-of-bounds bits in `umask`).
- Missing arguments to functions (e.g., `open()` with `O_CREAT`, but no mode
  bits).

FORTIFY is traditionally enabled by passing `-D_FORTIFY_SOURCE=N` to your
compiler. `N==0` disables FORTIFY, whereas `N==1`, `N==2`, and `N==3` enable
increasingly strict versions of it. In general, FORTIFY doesn't require user
code changes; that said, some code patterns
are [incompatible with stricter versions of FORTIFY checking]. This is largely
because FORTIFY has significant flexibility in what it considers to be an
"out-of-bounds" access.

FORTIFY implementations use a mix of compiler diagnostics and runtime checks to
flag and/or mitigate the impacts of the misuses mentioned above.

Further, given FORTIFY's design, the effectiveness of FORTIFY is a function of
-- among other things -- the optimization level you're compiling your code at.
Many FORTIFY implementations are implicitly disabled when building with `-O0`,
since FORTIFY's design for both Clang and GCC relies on optimizations in order
to provide useful run-time checks. For the purpose of this document, all
analysis of FORTIFY functions and commentary on builtins assume that code is
being built with some optimization level > `-O0`.

### A note on GCC

This document talks specifically about Bionic's FORTIFY implementation targeted
at Clang. While GCC also provides a set of language extensions necessary to
implement FORTIFY, these tools are different from what Clang offers. This
divergence is an artifact of Clang and GCC's differing architecture as
compilers.

Textually, quite a bit can be shared between a FORTIFY implementation for GCC
and one for Clang (e.g., see [ChromeOS' Glibc patch]), but this kind of sharing
requires things like macros that expand to unbalanced braces depending on your
compiler:

```c
/*
 * Highly simplified; if you're interested in FORTIFY's actual implementation,
 * please see the patch linked above.
 */
#ifdef __clang__
# define FORTIFY_PRECONDITIONS
# define FORTIFY_FUNCTION_END
#else
# define FORTIFY_PRECONDITIONS {
# define FORTIFY_FUNCTION_END }
#endif

/*
 * FORTIFY_WARNING_ONLY_IF_SIZE_OF_BUF_LESS_THAN is not defined, due to its
 * complexity and irrelevance. It turns into a compile-time warning if the
 * compiler can determine `*buf` has fewer than `size` bytes available.
 */

char *getcwd(char *buf, size_t size)
FORTIFY_PRECONDITIONS
  FORTIFY_WARNING_ONLY_IF_SIZE_OF_BUF_LESS_THAN(buf, size, "`buf` is too smol.")
{
  // Actual shared function implementation goes here.
}
FORTIFY_FUNCTION_END
```

All talk of GCC-focused implementations and how to merge Clang and GCC
implementations is out-of-scope for this doc, however.

## The Life of a Clang FORTIFY Function

As referenced in the Background section, FORTIFY performs many different checks
for many functions. This section intends to go through real-world examples of
FORTIFY functions in Bionic, breaking down how each part of these functions
work, and how the pieces fit together to provide FORTIFY-like functionality.

While FORTIFY implementations may differ between stdlibs, they broadly follow
the same patterns when implementing their checks for Clang, and they try to
make similar promises with respect to FORTIFY compiling to be zero-overhead in
some cases, etc. Moreover, while this document specifically examines Bionic,
many stdlibs will operate _very similarly_ to Bionic in their Clang FORTIFY
implementations.

**In general, when reading the below, be prepared for exceptions, subtlety, and
corner cases. The individual function breakdowns below try to not offer
redundant information. Each one focuses on different aspects of FORTIFY.**

### Terminology

Because FORTIFY should be mostly transparent to developers, there are inherent
naming collisions here: `memcpy(x, y, z)` turns into fundamentally different
generated code depending on the value of `_FORTIFY_SOURCE`. Further, said
`memcpy` call with `_FORTIFY_SOURCE` enabled needs to be able to refer to the
`memcpy` that would have been called, had `_FORTIFY_SOURCE` been disabled.
Hence, the following convention is followed in the subsections below for all
prose (namely, multiline code blocks are exempted from this):

- Standard library function names preceded by `__builtin_` refer to the use of
  the function with `_FORTIFY_SOURCE` disabled.
- Standard library function names without a prefix refer to the use of the
  function with `_FORTIFY_SOURCE` enabled.

This convention also applies in `clang`. `__builtin_memcpy` will always call
`memcpy` as though `_FORTIFY_SOURCE` were disabled.

## Breakdown of `mempcpy`

The [FORTIFY'ed version of `mempcpy`] is a full, featureful example of a
FORTIFY'ed function from Bionic. From the user's perspective, it supports a few
things:
- Producing a compile-time error if the number of bytes to copy trivially
  exceeds the number of bytes available at the destination pointer.
- If the `mempcpy` has the potential to write to more bytes than what is
  available at the destination, a run-time check is inserted to crash the
  program if more bytes are written than what is allowed.
- Compiling away to be zero overhead when none of the buffer sizes can be
  determined at compile-time[^1].

The declaration in Bionic's headers for `__builtin_mempcpy` is:
```c
void* mempcpy(void* __dst, const void* __src, size_t __n) __INTRODUCED_IN(23);
```

Which is annotated with nothing special, so it will be ignored.

The [source for `mempcpy`] in Bionic's headers for is:
```c
__BIONIC_FORTIFY_INLINE
void* mempcpy(void* const dst __pass_object_size0, const void* src, size_t copy_amount)
        __overloadable
        __clang_error_if(__bos_unevaluated_lt(__bos0(dst), copy_amount),
                         "'mempcpy' called with size bigger than buffer") {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos_dst = __bos0(dst);
    if (!__bos_trivially_ge(bos_dst, copy_amount)) {
        return __builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);
    }
#endif
    return __builtin_mempcpy(dst, src, copy_amount);
}
```

Expanding some of the important macros here, this function expands to roughly:
```c
static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
void* mempcpy(
        void* const dst __attribute__((pass_object_size(0))),
        const void* src,
        size_t copy_amount)
        __attribute__((overloadable))
        __attribute__((diagnose_if(
            __builtin_object_size(dst, 0) != -1 && __builtin_object_size(dst, 0) <= copy_amount),
            "'mempcpy' called with size bigger than buffer"))) {
#if __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    size_t bos_dst = __builtin_object_size(dst, 0);
    if (!(__bos_trivially_ge(bos_dst, copy_amount))) {
        return __builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);
    }
#endif
    return __builtin_mempcpy(dst, src, copy_amount);
}
```

So let's walk through this step by step, to see how FORTIFY does what it says on
the tin here.

[^1]: "Zero overhead" in a way [similar to C++11's `std::unique_ptr`]: this will
turn into a direct call `__builtin_mempcpy` (or an optimized form thereof) with
no other surrounding checks at runtime. However, the additional complexity may
hinder optimizations that are performed before the optimizer can prove that the
`if (...) { ... }` can be optimized out. Depending on how late this happens,
the additional complexity may skew inlining costs, hide opportunities for e.g.,
`memcpy` coalescing, etc etc.

### How does Clang select `mempcpy`?

First, it's critical to notice that `mempcpy` is marked `overloadable`. This
function is a `static inline __attribute__((always_inline))` overload of
`__builtin_mempcpy`:
- `__attribute__((overloadable))` allows us to perform overloading in C.
- `__attribute__((overloadable))` mangles all calls to functions marked with
  `__attribute__((overloadable))`.
- `__attribute__((overloadable))` allows exactly one function signature with a
  given name to not be marked with `__attribute__((overloadable))`. Calls to
  this overload will not be mangled.

Second, one might note that this `mempcpy` implementation has the same C-level
signature as `__builtin_mempcpy`. `pass_object_size` is a Clang attribute that
is generally needed by FORTIFY, but it carries the side-effect that functions
may be overloaded simply on the presence (or lack of presence) of
`pass_object_size` attributes. Given two overloads of a function that only
differ on the presence of `pass_object_size` attributes, the candidate with
`pass_object_size` attributes is preferred.

Finally, the prior paragraph gets thrown out if one tries to take the address of
`mempcpy`. It is impossible to take the address of a function with one or more
parameters that are annotated with `pass_object_size`. Hence,
`&__builtin_mempcpy == &mempcpy`. Further, because this is an issue of overload
resolution, `(&mempcpy)(x, y, z);` is functionally identical to
`__builtin_mempcpy(x, y, z);`.

All of this accomplishes the following:
- Direct calls to `mempcpy` should call the FORTIFY-protected `mempcpy`.
- Indirect calls to `&mempcpy` should call `__builtin_mempcpy`.

### How does Clang offer compile-time diagnostics?

Once one is convinced that the FORTIFY-enabled overload of `mempcpy` will be
selected for direct calls, Clang's `diagnose_if` and `__builtin_object_size` do
all of the work from there.

Subtleties here primarily fall out of the discussion in the above section about
`&__builtin_mempcpy == &mempcpy`:
```c
#define _FORTIFY_SOURCE 2
#include <string.h>
void example_code() {
  char buf[4]; // ...Assume sizeof(char) == 1.
  const char input_buf[] = "Hello, World";
  mempcpy(buf, input_buf, 4); // Valid, no diagnostic issued.

  mempcpy(buf, input_buf, 5); // Emits a compile-time error since sizeof(buf) < 5.
  __builtin_mempcpy(buf, input_buf, 5); // No compile-time error.
  (&mempcpy)(buf, input_buf, 5); // No compile-time error, since __builtin_mempcpy is selected.
}
```

Otherwise, the rest of this subsection is dedicated to preliminary discussion
about `__builtin_object_size`.

Clang's frontend can do one of two things with `__builtin_object_size(p, n)`:
- Evaluate it as a constant.
  - This can either mean declaring that the number of bytes at `p` is definitely
    impossible to know, so the default value is used, or the number of bytes at
    `p` can be known without optimizations.
- Declare that the expression cannot form a constant, and lower it to
  `@llvm.objectsize`, which is discussed in depth later.

In the examples above, since `diagnose_if` is evaluated with context from the
caller, Clang should be able to trivially determine that `buf` refers to a
`char` array with 4 elements.

The primary consequence of the above is that diagnostics can only be emitted if
no optimizations are required to detect a broken code pattern. To be specific,
clang's constexpr evaluator must be able to determine the logical object that
any given pointer points to in order to fold `__builtin_object_size` to a
constant, non-default answer:

```c
#define _FORTIFY_SOURCE 2
#include <string.h>
void example_code() {
  char buf[4]; // ...Assume sizeof(char) == 1.
  const char input_buf[] = "Hello, World";
  mempcpy(buf, input_buf, 4); // Valid, no diagnostic issued.
  mempcpy(buf, input_buf, 5); // Emits a compile-time error since sizeof(buf) < 5.
  char *buf_ptr = buf;
  mempcpy(buf_ptr, input_buf, 5); // No compile-time error; `buf_ptr`'s target can't be determined.
}
```

### How does Clang insert run-time checks?

This section expands on the following statement: FORTIFY has zero runtime cost
in instances where there is no chance of catching a bug at run-time. Otherwise,
it introduces a tiny additional run-time cost to ensure that functions aren't
misused.

In prior sections, the following was established:
- `overloadable` and `pass_object_size` prompt Clang to always select this
  overload of `mempcpy` over `__builtin_mempcpy` for direct calls.
- If a call to `mempcpy` was trivially broken, Clang would produce a
  compile-time error, rather than producing a binary.

Hence, the case we're interested in here is one where Clang's frontend selected
a FORTIFY'ed function's implementation for a function call, but was unable to
find anything seriously wrong with said function call. Since the frontend is
powerless to detect bugs at this point, our focus shifts to the mechanisms that
LLVM uses to support FORTIFY.

Going back to Bionic's `mempcpy` implementation, we have the following (ignoring
diagnose_if and assuming run-time checks are enabled):
```c
static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
void* mempcpy(
        void* const dst __attribute__((pass_object_size(0))),
        const void* src,
        size_t copy_amount)
        __attribute__((overloadable)) {
    size_t bos_dst = __builtin_object_size(dst, 0);
    if (bos_dst != -1 &&
        !(__builtin_constant_p(copy_amount) && bos_dst >= copy_amount)) {
        return __builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);
    }
    return __builtin_mempcpy(dst, src, copy_amount);
}
```

In other words, we have a `static`, `always_inline` function which:
- If `__builtin_object_size(dst, 0)` cannot be determined (in which case, it
  returns -1), calls `__builtin_mempcpy`.
- Otherwise, if `copy_amount` can be folded to a constant, and if
  `__builtin_object_size(dst, 0) >= copy_amount`, calls `__builtin_mempcpy`.
- Otherwise, calls `__builtin___mempcpy_chk`.


How can this be "zero overhead"? Let's focus on the following part of the
function:

```c
    size_t bos_dst = __builtin_object_size(dst, 0);
    if (bos_dst != -1 &&
        !(__builtin_constant_p(copy_amount) && bos_dst >= copy_amount)) {
```

If Clang's frontend cannot determine a value for `__builtin_object_size`, Clang
lowers it to LLVM's `@llvm.objectsize` intrinsic. The `@llvm.objectsize`
invocation corresponding to `__builtin_object_size(p, 0)` is guaranteed to
always fold to a constant value by the time LLVM emits machine code.

Hence, `bos_dst` is guaranteed to be a constant; if it's -1, the above branch
can be eliminated entirely, since it folds to `if (false && ...)`. Further, the
RHS of the `&&` in this branch has us call `__builtin_mempcpy` if `copy_amount`
is a known value less than `bos_dst` (yet another constant value). Therefore,
the entire condition is always knowable when LLVM is done with LLVM IR-level
optimizations, so no condition is ever emitted to machine code in practice.

#### Why is "zero overhead" in quotes? Why is `unique_ptr` relevant?

`__builtin_object_size` and `__builtin_constant_p` are forced to be constants
after most optimizations take place. Until LLVM replaces both of these with
constants and optimizes them out, we have additional branches and function calls
in our IR. This can have negative effects, such as distorting inlining costs and
inhibiting optimizations that are conservative around branches in control-flow.

So FORTIFY is free in these cases _in isolation of any of the code around it_.
Due to its implementation, it may impact the optimizations that occur on code
around the literal call to the FORTIFY-hardened libc function.

`unique_ptr` was just the first thing that came to the author's mind for "the
type should be zero cost with any level of optimization enabled, but edge-cases
might make it only-mostly-free to use."

### How is checking actually performed?

In cases where checking can be performed (e.g., where we call
`__builtin___mempcpy_chk(dst, src, copy_amount, bos_dst);`), Bionic provides [an
implementation for `__mempcpy_chk`]. This is:

```c
extern "C" void* __mempcpy_chk(void* dst, const void* src, size_t count, size_t dst_len) {
  __check_count("mempcpy", "count", count);
  __check_buffer_access("mempcpy", "write into", count, dst_len);
  return mempcpy(dst, src, count);
}
```
This function itself boils down to a few small branches which abort the program
if they fail, and a direct call to `__builtin_mempcpy`.

### Wrapping up

In the above breakdown, it was shown how Clang and Bionic work together to:
- represent FORTIFY-hardened overloads of functions,
- report misuses of stdlib functions at compile-time, and
- insert run-time checks for uses of functions that might be incorrect, but only
  if we have the potential of proving the incorrectness of these.

## Breakdown of open

In Bionic, the [FORTIFY'ed implementation of `open`] is quite large. Much like
`mempcpy`, the `__builtin_open` declaration is simple:

```c
int open(const char* __path, int __flags, ...);
```

With some macros expanded, the FORTIFY-hardened header implementation is:
```c
int __open_2(const char*, int);
int __open_real(const char*, int, ...) __asm__(open);

#define __open_modes_useful(flags) (((flags) & O_CREAT) || ((flags) & O_TMPFILE) == O_TMPFILE)

static
int open(const char* pathname, int flags, mode_t modes, ...) __overloadable
        __attribute__((diagnose_if(1, "error", "too many arguments")));

static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
int open(const char* const __attribute__((pass_object_size(1))) pathname, int flags)
        __attribute__((overloadable))
        __attribute__((diagnose_if(
            __open_modes_useful(flags),
            "error",
            "'open' called with O_CREAT or O_TMPFILE, but missing mode"))) {
#if __ANDROID_API__ >= 17 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __open_2(pathname, flags);
#else
    return __open_real(pathname, flags);
#endif
}
static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
int open(const char* const __attribute__((pass_object_size(1))) pathname, int flags, mode_t modes)
        __attribute__((overloadable))
        __clang_warning_if(!__open_modes_useful(flags) && modes,
                           "'open' has superfluous mode bits; missing O_CREAT?") {
    return __open_real(pathname, flags, modes);
}
```

Which may be a lot to take in.

Before diving too deeply, please note that the remainder of these subsections
assume that the programmer didn't make any egregious typos. Moreover, there's no
real way that Bionic tries to prevent calls to `open` like
`open("foo", 0, "how do you convert a const char[N] to mode_t?");`. The only
real C-compatible solution the author can think of is "stamp out many overloads
to catch sort-of-common instances of this very uncommon typo." This isn't great.

More directly, no effort is made below to recognize calls that, due to
incompatible argument types, cannot go to any `open` implementation other than
`__builtin_open`, since it's recognized right here. :)

### Implementation breakdown

This `open` implementation does a few things:
- Turns calls to `open` with too many arguments into a compile-time error.
- Diagnoses calls to `open` with missing modes at compile-time and run-time
  (both cases turn into errors).
- Emits warnings on calls to `open` with useless mode bits, unless the mode bits
  are all 0.

One common bit of code not explained below is the `__open_real` declaration above:
```c
int __open_real(const char*, int, ...) __asm__(open);
```

This exists as a way for us to call `__builtin_open` without needing clang to
have a pre-defined `__builtin_open` function.

#### Compile-time error on too many arguments

```c
static
int open(const char* pathname, int flags, mode_t modes, ...) __overloadable
        __attribute__((diagnose_if(1, "error", "too many arguments")));
```

Which matches most calls to open that supply too many arguments, since
`int(const char *, int, ...)` matches less strongly than
`int(const char *, int, mode_t, ...)` for calls where the 3rd arg can be
converted to `mode_t` without too much effort. Because of the `diagnose_if`
attribute, all of these calls turn into compile-time errors.

#### Compile-time or run-time error on missing arguments
The following overload handles all two-argument calls to `open`.
```c
static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
int open(const char* const __attribute__((pass_object_size(1))) pathname, int flags)
        __attribute__((overloadable))
        __attribute__((diagnose_if(
            __open_modes_useful(flags),
            "error",
            "'open' called with O_CREAT or O_TMPFILE, but missing mode"))) {
#if __ANDROID_API__ >= 17 && __BIONIC_FORTIFY_RUNTIME_CHECKS_ENABLED
    return __open_2(pathname, flags);
#else
    return __open_real(pathname, flags);
#endif
}
```

Like `mempcpy`, `diagnose_if` handles emitting a compile-time error if the call
to `open` is broken in a way that's visible to Clang's frontend. This
essentially boils down to "`open` is being called with a `flags` value that
requires mode bits to be set."

If that fails to catch a bug, we [unconditionally call `__open_2`], which
performs a run-time check:
```c
int __open_2(const char* pathname, int flags) {
  if (needs_mode(flags)) __fortify_fatal("open: called with O_CREAT/O_TMPFILE but no mode");
  return FDTRACK_CREATE_NAME("open", __openat(AT_FDCWD, pathname, force_O_LARGEFILE(flags), 0));
}
```

#### Compile-time warning if modes are pointless

Finally, we have the following `open` call:
```c
static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
int open(const char* const __attribute__((pass_object_size(1))) pathname, int flags, mode_t modes)
        __attribute__((overloadable))
        __clang_warning_if(!__open_modes_useful(flags) && modes,
                           "'open' has superfluous mode bits; missing O_CREAT?") {
    return __open_real(pathname, flags, modes);
}
```

This simply issues a warning if Clang's frontend can determine that `flags`
isn't necessary. Due to conventions in existing code, a `modes` value of `0` is
not diagnosed.

#### What about `&open`?
One yet-unaddressed aspect of the above is how `&open` works. This is thankfully
a short answer:
- It happens that `open` takes a parameter of type `const char*`.
- It happens that `pass_object_size` -- an attribute only applicable to
  parameters of type `T*` --  makes it impossible to take the address of a
  function.

Since clang doesn't support a "this function should never have its address
taken," attribute, Bionic uses the next best thing: `pass_object_size`. :)

## Breakdown of poll

(Preemptively: at the time of writing, Clang has no literal `__builtin_poll`
builtin. `__builtin_poll` is referenced below to remain consistent with the
convention established in the Terminology section.)

Bionic's `poll` implementation is closest to `mempcpy` above, though it has a
few interesting aspects worth examining.

The [full header implementation of `poll`] is, with some macros expanded:
```c
#define __bos_fd_count_trivially_safe(bos_val, fds, fd_count) \
  ((bos_val) == -1) || \
    (__builtin_constant_p(fd_count) && \
    (bos_val) >= sizeof(*fds) * (fd_count)))

static
__inline__
__attribute__((no_stack_protector))
__attribute__((always_inline))
int poll(struct pollfd* const fds __attribute__((pass_object_size(1))), nfds_t fd_count, int timeout)
    __attribute__((overloadable))
    __attriubte__((diagnose_if(
       __builtin_object_size(fds, 1) != -1 && __builtin_object_size(fds, 1) < sizeof(*fds) * fd_count,
        "error",
        "in call to 'poll', fd_count is larger than the given buffer"))) {
  size_t bos_fds = __builtin_object_size(fds, 1);
  if (!__bos_fd_count_trivially_safe(bos_fds, fds, fd_count)) {
    return __poll_chk(fds, fd_count, timeout, bos_fds);
  }
  return (&poll)(fds, fd_count, timeout);
}
```

To get the commonality with `mempcpy` and `open` out of the way:
- This function is an overload with `__builtin_poll`.
- The signature is the same, modulo the presence of a `pass_object_size`
  attribute. Hence, for direct calls, overload resolution will always prefer it
  over `__builtin_poll`. Taking the address of `poll` is forbidden, so all
  references to `&poll` actually reference `__builtin_poll`.
- When `fds` is too small to hold `fd_count` `pollfd`s, Clang will emit a
  compile-time error if possible using `diagnose_if`.
- If this can't be observed until run-time, `__poll_chk` verifies this.
- When `fds` is a constant according to `__builtin_constant_p`, this always
  compiles into `__poll_chk` for always-broken calls to `poll`, or
  `__builtin_poll` for always-safe calls to `poll`.

The critical bits to highlight here are on this line:
```c
int poll(struct pollfd* const fds __attribute__((pass_object_size(1))), nfds_t fd_count, int timeout)
```

And this line:
```c
  return (&poll)(fds, fd_count, timeout);
```

Starting with the simplest, we call `__builtin_poll` with `(&poll)(...);`. As
referenced above, taking the address of an overloaded function where all but one
overload has a `pass_object_size` attribute on one or more parameters always
resolves to the function without any `pass_object_size` attributes.

The other line deserves a section. The subtlety of it is almost entirely in the
use of `pass_object_size(1)` instead of `pass_object_size(0)`. on the `fds`
parameter, and the corresponding use of `__builtin_object_size(fds, 1);` in the
body of `poll`.

### Subtleties of __builtin_object_size(p, N)

Earlier in this document, it was said that a full description of each
attribute/builtin necessary to power FORTIFY was out of scope. This is... only
somewhat the case when we talk about `__builtin_object_size` and
`pass_object_size`, especially when their second argument is `1`.

#### tl;dr
`__builtin_object_size(p, N)` and `pass_object_size(N)`, where `(N & 1) == 1`,
can only be accurately determined by Clang. LLVM's `@llvm.objectsize` intrinsic
ignores the value of `N & 1`, since handling `(N & 1) == 1` accurately requires
data that's currently entirely inaccessible to LLVM, and that is difficult to
preserve through LLVM's optimization passes.

`pass_object_size`'s "lifting" of the evaluation of
`__builtin_object_size(p, N)` to the caller is critical, since it allows Clang
full visibility into the expression passed to e.g., `poll(&foo->bar, baz, qux)`.
It's not a perfect solution, but it allows `N == 1` to be fully accurate in at
least some cases.

#### Background
Clang's implementation of `__builtin_object_size` aims to be compatible with
GCC's, which has [a decent bit of documentation]. Put simply,
`__builtin_object_size(p, N)` is intended to evaluate at compile-time how many
bytes can be accessed after `p` in a well-defined way. Straightforward examples
of this are:
```c
char buf[8];
assert(__builtin_object_size(buf, N) == 8);
assert(__builtin_object_size(buf + 1, N) == 7);
```

This should hold for all values of N that are valid to pass to
`__builtin_object_size`. The `N` value of `__builtin_object_size` is a mask of
settings.

##### (N & 2) == ?

This is mostly for completeness sake; in Bionic's FORTIFY implementation, N is
always either 0 or 1.

If there are multiple possible values of `p` in a call to
`__builtin_object_size(p, N)`, the second bit in `N` determines the behavior of
the compiler. If `(N & 2) == 0`, `__builtin_object_size` should return the
greatest possible size for each possible value of `p`. Otherwise, it should
return the least possible value. For example:

```c
char smol_buf[7];
char buf[8];
char *p = rand() ? smol_buf : buf;
assert(__builtin_object_size(p, 0) == 8);
assert(__builtin_object_size(p, 2) == 7);
```

##### (N & 1) == 0

`__builtin_object_size(p, 0)` is more or less as simple as the example in the
Background section directly above. When Clang attempts to evaluate
`__builtin_object_size(p, 0);` and when LLVM tries to determine the result of a
corresponding `@llvm.objectsize` call to, they search for the storage underlying
the pointer in question. If that can be determined, Clang or LLVM can provide an
answer; otherwise, they cannot.

##### (N & 1) == 1, and the true magic of pass_object_size

`__builtin_object_size(p, 1)` has a less uniform implementation between LLVM and
Clang. According to GCC's documentation, "If the least significant bit [of
__builtin_object_size's second argument] is clear, objects are whole variables,
if it is set, a closest surrounding subobject is considered the object a pointer
points to."

The "closest surrounding subobject," means that `(N & 1) == 1` depends on type
information in order to operate in many cases. Consider the following examples:
```c
struct Foo {
  int a;
  int b;
};

struct Foo foo;
assert(__builtin_object_size(&foo, 0) == sizeof(foo));
assert(__builtin_object_size(&foo, 1) == sizeof(foo));
assert(__builtin_object_size(&foo->a, 0) == sizeof(foo));
assert(__builtin_object_size(&foo->a, 1) == sizeof(int));

struct Foo foos[2];
assert(__builtin_object_size(&foos[0], 0) == 2 * sizeof(foo));
assert(__builtin_object_size(&foos[0], 1) == sizeof(foo));
assert(__builtin_object_size(&foos[0]->a, 0) == 2 * sizeof(foo));
assert(__builtin_object_size(&foos[0]->a, 1) == sizeof(int));
```

...And perhaps somewhat surprisingly:
```c
void example(struct Foo *foo) {
  // (As a reminder, `-1` is "I don't know" when `(N & 2) == 0`.)
  assert(__builtin_object_size(foo, 0) == -1);
  assert(__builtin_object_size(foo, 1) == -1);
  assert(__builtin_object_size(foo->a, 0) == -1);
  assert(__builtin_object_size(foo->a, 1) == sizeof(int));
}
```

In Clang, [this type-aware requirement poses problems for us]: Clang's frontend
knows everything we could possibly want about the types of variables, but
optimizations are only performed by LLVM. LLVM has no reliable source for C or
C++ data types, so calls to `__builtin_object_size(p, N)` that cannot be
resolved by clang are lowered to the equivalent of
`__builtin_object_size(p, N & ~1)` in LLVM IR.

Moreover, Clang's frontend is the best-equipped part of the compiler to
accurately determine the answer for `__builtin_object_size(p, N)`, given we know
what `p` is. LLVM is the best-equipped part of the compiler to determine the
value of `p`. This ordering issue is unfortunate.

This is where `pass_object_size(N)` comes in. To summarize [the docs for
`pass_object_size`], it evaluates `__builtin_object_size(p, N)` within the
context of the caller of the function annotated with `pass_object_size`, and
passes the value of that into the callee as an invisible parameter. All calls to
`__builtin_object_size(parameter, N)` are substituted with references to this
invisible parameter.

Putting this plainly, Clang's frontend struggles to evaluate the following:
```c
int foo(void *p) {
  return __builtin_object_size(p, 1);
}

void bar() {
  struct { int i, j } k;
  // The frontend can't figure this interprocedural objectsize out, so it gets lowered to
  // LLVM, which determines that the answer here is sizeof(k).
  int baz = foo(&k.i);
}
```

However, with the magic of `pass_object_size`, we get one level of inlining to
look through:
```c
int foo(void *const __attribute__((pass_object_size(1))) p) {
  return __builtin_object_size(p, 1);
}

void bar() {
  struct { int i, j } k;
  // Due to pass_object_size, this is equivalent to:
  // int baz = foo(&k.i, __builtin_object_size(&k.i, 1));
  // ...and `int foo(void *)` is actually equivalent to:
  // int foo(void *const, size_t size) {
  //   return size;
  // }
  int baz = foo(&k.i);
}
```

So we can obtain an accurate result in this case.

##### What about pass_object_size(0)?
It's sort of tangential, but if you find yourself wondering about the utility of
`pass_object_size(0)` ... it's somewhat split. `pass_object_size(0)` in Bionic's
FORTIFY exists mostly for visual consistency, simplicity, and as a useful way to
have e.g., `&mempcpy` == `&__builtin_mempcpy`.

Outside of these fringe benefits, all of the functions with
`pass_object_size(0)` on parameters are marked with `always_inline`, so
"lifting" the `__builtin_object_size` call isn't ultimately very helpful. In
theory, users can always have something like:

```c
// In some_header.h
// This function does cool and interesting things with the `__builtin_object_size` of its parameter,
// and is able to work with that as though the function were defined inline.
void out_of_line_function(void *__attribute__((pass_object_size(0))));
```

Though the author isn't aware of uses like this in practice, beyond a few folks
on LLVM's mailing list seeming interested in trying it someday.

#### Wrapping up
In the (long) section above, two things were covered:
- The use of `(&poll)(...);` is a convenient shorthand for calling
  `__builtin_poll`.
- `__builtin_object_size(p, N)` with `(N & 1) == 1` is not easy for Clang to
  answer accurately, since it relies on type info only available in the
  frontend, and it sometimes relies on optimizations only available in the
  middle-end. `pass_object_size` helps mitigate this.

## Miscellaneous Notes
The above should be a roughly comprehensive view of how FORTIFY works in the
real world. The main thing it fails to mention is the use of [the `diagnose_as_builtin` attribute] in Clang.

As time has moved on, Clang has increasingly gained support for emitting
warnings that were previously emitted by FORTIFY machinery.
`diagnose_as_builtin` allows us to remove the `diagnose_if`s from some of the
`static inline` overloads of stdlib functions above, so Clang may diagnose them
instead.

Clang's built-in diagnostics are often better than `diagnose_if` diagnostics,
since Clang can format its diagnostics to include e.g., information about the
sizes of buffers in a suspect call to a function. `diagnose_if` can only have
the compiler output constant strings.

[ChromeOS' Glibc patch]: https://chromium.googlesource.com/chromiumos/overlays/chromiumos-overlay/+/90fa9b27731db10a6010c7f7c25b24028145b091/sys-libs/glibc/files/local/glibc-2.33/0007-glibc-add-clang-style-FORTIFY.patch
[FORTIFY'ed implementation of `open`]: https://android.googlesource.com/platform/bionic/+/refs/heads/android12-release/libc/include/bits/fortify/fcntl.h#41
[FORTIFY'ed version of `mempcpy`]: https://android.googlesource.com/platform/bionic/+/refs/heads/android12-release/libc/include/bits/fortify/string.h#45
[a decent bit of documentation]: https://gcc.gnu.org/onlinedocs/gcc/Object-Size-Checking.html
[an implementation for `__mempcpy_chk`]: https://android.googlesource.com/platform/bionic/+/refs/heads/android12-release/libc/bionic/fortify.cpp#501
[full header implementation of `poll`]: https://android.googlesource.com/platform/bionic/+/refs/heads/android12-release/libc/include/bits/fortify/poll.h#43
[incompatible with stricter versions of FORTIFY checking]: https://godbolt.org/z/fGfEYxfnf
[similar to C++11's `std::unique_ptr`]: https://stackoverflow.com/questions/58339165/why-can-a-t-be-passed-in-register-but-a-unique-ptrt-cannot
[source for `mempcpy`]: https://android.googlesource.com/platform/bionic/+/refs/heads/android12-release/libc/include/string.h#55
[the `diagnose_as_builtin` attribute]: https://releases.llvm.org/14.0.0/tools/clang/docs/AttributeReference.html#diagnose-as-builtin
[the docs for `pass_object_size`]: https://releases.llvm.org/14.0.0/tools/clang/docs/AttributeReference.html#pass-object-size-pass-dynamic-object-size
[this type-aware requirement poses problems for us]: https://github.com/llvm/llvm-project/issues/55742
[unconditionally call `__open_2`]: https://android.googlesource.com/platform/bionic/+/refs/heads/android12-release/libc/bionic/open.cpp#70
