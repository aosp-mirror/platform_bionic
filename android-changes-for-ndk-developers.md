# Android linker changes for NDK developers

This document details important changes related to native code
loading in various Android releases.

See also [bionic status](docs/status.md) for general libc/libm/libdl
behavior changes.

See also the
[unwinder documentation](https://android.googlesource.com/platform/system/unwinding/+/refs/heads/main/libunwindstack/AndroidVersions.md)
for details about changes in stack unwinding (crash dumps) between
different releases.

Required tools: the NDK has an `llvm-readelf` binary that understands all the
architecture-specific details of all Android's supported architectures. Recent
versions of Android also have toybox readelf on the device.


## How we manage incompatible changes

Our general practice with dynamic linker behavior changes is that they
will be tied to an app's target API level:

* Below the affected API level we'll preserve the old behavior or issue
a warning, as appropriate.

* At the affected API level and above, we’ll refuse to load the library.

* Warnings about any behavior change that will affect a library if you
increase your target API level will appear in logcat when that library
is loaded, even if you're not yet targeting that API level.

* On a developer preview build, dynamic linker warnings will also show up
as toasts. Experience has shown that many developers don’t habitually
check logcat for warnings until their app stops functioning, so the
toasts help bring some visibility to the issues before it's too late.


## Changes to library dependency resolution

Until it was [fixed](https://issuetracker.google.com/36950617) in
API level 18, Android didn't include the application library directory
on the dynamic linker's search path. This meant that apps
had to call `dlopen` or `System.loadLibrary` on all transitive
dependencies before loading their main library. Worse, until it was
[fixed](https://issuetracker.google.com/36935779) in API level 18, the
dynamic linker's caching code cached failures too, so it was necessary
to topologically sort your libraries and load them in reverse order.

If you need to support Android devices running OS versions older than
API level 23, you might want to consider
[ReLinker](https://github.com/KeepSafe/ReLinker) which claims to solve
these and other problems automatically.

Alternatively, if you don't have too many dependencies, it can be easiest to
simply link all of your code into one big library and sidestep the details of
library and symbol lookup changes on all past (and future) Android versions.


## Changes to library search order

We have made various fixes to library search order when resolving symbols.

With API level 22, load order switched from depth-first to breadth-first to
fix dlsym(3).

Before API level 23, the default search order was to try the main executable,
LD_PRELOAD libraries, the library itself, and its DT_NEEDED libraries
in that order. For API level 23 and later, for any given library, the dynamic
linker divides other libraries into the global group and the local
group. The global group is shared by all libraries and contains the main
executable, LD_PRELOAD libraries, and any library with the DF_1_GLOBAL
flag set (by passing “-z global” to ld(1)). The local group is
the breadth-first transitive closure of the library and its DT_NEEDED
libraries. The API level 23 dynamic linker searches the global group followed by
the local group. This allows ASAN, for example, to ensure that it can
intercept any symbol.


## LD_PRELOAD and 32/64 bit

LD_PRELOAD applies to both 32- and 64-bit processes. This means that you
should avoid saying something like `/system/lib/libfoo.so` and just say
`libfoo.so` instead, letting the dynamic linker find the correct library
on its search path.


## RTLD_LOCAL (Available in API level >= 23)

The dlopen(3) RTLD_LOCAL flag used to be ignored but is implemented
correctly in API level 23 and later. Note that RTLD_LOCAL is the default,
so even calls to dlopen(3) that didn’t explicitly use RTLD_LOCAL will
be affected (unless they explicitly used RTLD_GLOBAL). With RTLD_LOCAL,
symbols will not be made available to libraries loaded by later calls
to dlopen(3) (as opposed to being referenced by DT_NEEDED entries).


## GNU hashes (Availible in API level >= 23)

The GNU hash style available with `--hash-style=gnu` allows faster
symbol lookup and is supported by Android's dynamic linker in API level 23 and
above. Use `--hash-style=both` if you want to build code that uses this
feature in new enough releases but still works on older releases.
If you're using the NDK, clang chooses the right option
(automatically)[https://github.com/android/ndk/issues/2005].


## Correct soname/path handling (Available in API level >= 23)

The dynamic linker now understands the difference
between a library’s soname and its path (public bug
https://code.google.com/p/android/issues/detail?id=6670). API level 23
is the first release where search by soname is implemented. Earlier
releases would assume that the basename of the library was the soname,
and used that to search for already-loaded libraries. For example,
`dlopen("/this/directory/does/not/exist/libc.so", RTLD_NOW)` would
find `/system/lib/libc.so` because it’s already loaded. This also meant
that it was impossible to have two libraries `"dir1/libx.so"` and
`"dir2/libx.so"` --- the dynamic linker couldn’t tell the difference
and would always use whichever was loaded first, even if you explicitly
tried to load both. This also applied to DT_NEEDED entries.

Some apps have bad DT_NEEDED entries (usually absolute paths on the build
machine’s file system) that used to work because we ignored everything
but the basename. These apps will fail to load on API level 23 and above.


## Symbol versioning (Available in API level >= 23)

Symbol versioning allows libraries to provide better backwards
compatibility. For example, if a library author knowingly changes
the behavior of a function, they can provide two versions in the same
library so that old code gets the old version and new code gets the new
version. This is supported in API level 23 and above.


## Opening shared libraries directly from an APK

In API level 23 and above, it’s possible to open a .so file directly from
your APK. Just use `System.loadLibrary("foo")` exactly as normal but set
`android:extractNativeLibs="false"` in your `AndroidManifest.xml`. In
older releases, the .so files were extracted from the APK file
at install time. This meant that they took up space in your APK and
again in your installation directory (and this was counted against you
and reported to the user as space taken up by your app). Any .so file
that you want to load directly from your APK must be page aligned
(on a 4096-byte boundary) in the zip file and stored uncompressed.
Current versions of the zipalign tool take care of alignment.

Note that in API level 23 and above dlopen(3) can open a library from
any zip file, not just an APK. Just give dlopen(3) a path of the form
"my_zip_file.zip!/libs/libstuff.so". As with APKs, the library must be
page-aligned and stored uncompressed for this to work.


## Private API (Enforced for API level >= 24)

Native libraries must use only public API, and must not link against
non-NDK platform libraries. On devices running API level 24 or later,
this rule is enforced and applications are no longer able to load all
non-NDK platform libraries. This was to prevent future issues similar
to the disruption caused when Android switched from OpenSSL to BoringSSL
at API level 23.

The rule is enforced by the dynamic linker, so non-public libraries
are not accessible regardless of the way code tries to load them:
System.loadLibrary(), DT_NEEDED entries, and direct calls to dlopen(3)
will all work exactly the same.

In order to reduce the user impact of this transition, we identified
a set of libraries that saw significant use from Google Play's
most-installed apps and were feasible for us to support in the
short term (including libandroid_runtime.so, libcutils.so, libcrypto.so,
and libssl.so). In order to give app developers more time to transition,
we allowed access to these libraries for apps with a target API level < 24.
On devices running API level 26 to API level 30, this compatibility mode could be
disabled by setting a system property (`debug.ld.greylist_disabled`).
This property is ignored on devices running API level 31 and later.

```
$ readelf --dynamic libBroken.so | grep NEEDED
 0x00000001 (NEEDED)                     Shared library: [libnativehelper.so]
 0x00000001 (NEEDED)                     Shared library: [libutils.so]
 0x00000001 (NEEDED)                     Shared library: [libstagefright_foundation.so]
 0x00000001 (NEEDED)                     Shared library: [libmedia_jni.so]
 0x00000001 (NEEDED)                     Shared library: [liblog.so]
 0x00000001 (NEEDED)                     Shared library: [libdl.so]
 0x00000001 (NEEDED)                     Shared library: [libz.so]
 0x00000001 (NEEDED)                     Shared library: [libstdc++.so]
 0x00000001 (NEEDED)                     Shared library: [libm.so]
 0x00000001 (NEEDED)                     Shared library: [libc.so]
```

*Potential problems*: starting from API level 24 the dynamic linker will not
load private libraries, preventing the application from loading.

*Resolution*: rewrite your native code to rely only on public API. As a
short term workaround, platform libraries without complex dependencies
(libcutils.so) can be copied to the project. As a long term solution
the relevant code must be copied to the project tree. SSL/Media/JNI
internal/binder APIs should not be accessed from the native code. When
necessary, native code should call appropriate public Java API methods.

A complete list of public libraries is available within the NDK, under
platforms/android-API/usr/lib.

Note: SSL/crypto is a special case, applications must NOT use platform
libcrypto and libssl libraries directly, even on older platforms. All
applications should use GMS Security Provider to ensure they are protected
from known vulnerabilities.


## Missing Section Headers (Enforced for API level >= 24)

Each ELF file has additional information contained in the section
headers. These headers must be present now, because the dynamic linker
uses them for validity checking. Some developers strip them in an
attempt to obfuscate the binary and prevent reverse engineering. (This
doesn't really help because it is possible to reconstruct the stripped
information using widely-available tools.)

```
$ readelf --headers libBroken.so | grep 'section headers'
  Start of section headers:          0 (bytes into file)
  Size of section headers:           0 (bytes)
  Number of section headers:         0
```

*Resolution*: remove the extra steps from your build that strip section
headers.


## Text Relocations (Enforced for API level >= 23)

Apps with a target API level >= 23 cannot load shared objects that contain text
relocations. Such an approach reduces load time and improves security. This was
only a change for 32-bit, because 64-bit never supported text relocations.

The usual reason for text relocations was non-position independent
hand-written assembler. This is not common. You can use the scanelf tool
from the pax-utils debian package for further diagnostics:

```
$ scanelf -qT libTextRel.so
  libTextRel.so: (memory/data?) [0x15E0E2] in (optimized out: previous simd_broken_op1) [0x15E0E0]
  libTextRel.so: (memory/data?) [0x15E3B2] in (optimized out: previous simd_broken_op2) [0x15E3B0]
  ...
```

If you have no scanelf tool available, it is possible to do a basic
check with readelf instead. Look for either a TEXTREL entry or the
TEXTREL flag. Either alone is sufficient. (The value corresponding to the
TEXTREL entry is irrelevant and typically 0 --- simply the presence of
the TEXTREL entry declares that the .so contains text relocations.) This
example has both indicators present:

```
$ readelf --dynamic libTextRel.so | grep TEXTREL
 0x00000016 (TEXTREL)                    0x0
 0x0000001e (FLAGS)                      SYMBOLIC TEXTREL BIND_NOW
```

Note: it is technically possible to have a shared object with the TEXTREL
entry/flag but without any actual text relocations. This doesn't happen
with the NDK, but if you're generating ELF files yourself make sure
you're not generating ELF files that claim to have text relocations,
because the Android dynamic linker trusts the entry/flag.

*Potential problems*: Relocations enforce code pages being writable, and
wastefully increase the number of dirty pages in memory. The dynamic
linker issued warnings about text relocations from API level 19, but on API
level 23 and above refuses to load code with text relocations.

*Resolution*: rewrite assembler to be position independent to ensure
no text relocations are necessary. The
[Gentoo Textrels guide](https://wiki.gentoo.org/wiki/Hardened/Textrels_Guide)
has instructions for fixing text relocations, and more detailed
[scanelf documentation](https://wiki.gentoo.org/wiki/Hardened/PaX_Utilities).


## Invalid DT_NEEDED Entries (Enforced for API level >= 23)

While library dependencies (DT_NEEDED entries in the ELF headers) can be
absolute paths, that doesn't make sense on Android because you have
no control over where your library will be installed by the system. A
DT_NEEDED entry should be the same as the needed library's SONAME,
leaving the business of finding the library at runtime to the dynamic
linker.

Before API level 23, Android's dynamic linker ignored the full path, and
used only the basename (the part after the last ‘/') when looking
up the required libraries. Since API level 23 the runtime linker will honor
the DT_NEEDED exactly and so it won't be able to load the library if
it is not present in that exact location on the device.

Even worse, some build systems have bugs that cause them to insert
DT_NEEDED entries that point to a file on the build host, something that
cannot be found on the device.

```
$ readelf --dynamic libSample.so | grep NEEDED
 0x00000001 (NEEDED)                     Shared library: [libm.so]
 0x00000001 (NEEDED)                     Shared library: [libc.so]
 0x00000001 (NEEDED)                     Shared library: [libdl.so]
 0x00000001 (NEEDED)                     Shared library:
[C:\Users\build\Android\ci\jni\libBroken.so]
```

*Potential problems*: before API level 23 the DT_NEEDED entry's basename was
used, but starting from API level 23 the Android runtime will try to load the
library using the path specified, and that path won't exist on the
device. There are broken third-party toolchains/build systems that use
a path on a build host instead of the SONAME.

*Resolution*: make sure all required libraries are referenced by SONAME
only. It is better to let the runtime linker to find and load those
libraries as the location may change from device to device.


## Missing SONAME (Enforced for API level >= 23)

Each ELF shared object (“native library”) must have a SONAME
(Shared Object Name) attribute. The NDK build systems add this
attribute by default, so its absence (or an incorrect soname) indicates
a misconfiguration in your build system. A missing SONAME may lead to
runtime issues such as the wrong library being loaded: the filename is
used instead when this attribute is missing.

```
$ readelf --dynamic libWithSoName.so | grep SONAME
 0x0000000e (SONAME)                     Library soname: [libWithSoName.so]
```

*Potential problems*: namespace conflicts may lead to the wrong library
being loaded at runtime, which leads to crashes when required symbols
are not found, or you try to use an ABI-incompatible library that isn't
the library you were expecting.

*Resolution*: the current NDK generates the correct SONAME by
default. Ensure you're using the current NDK and that you haven't
configured your build system to generate incorrect SONAME entries (using
the `-soname` linker option).


## `__register_atfork` (Available in API level >= 23)

To allow `atfork` and `pthread_atfork` handlers to be unregistered on
`dlclose`, API level 23 added a new libc function `__register_atfork`.
This means that code using `atfork` or `pthread_atfork` functions that is
built with a `minSdkVersion` >= 23 will not load on earlier versions of
Android, with an error referencing `__register_atfork`.

*Resolution*: build your code with `minSdkVersion` that matches the minimum
API level you actually support, or avoid using `atfork`/`pthread_atfork`.


## DT_RUNPATH support (Available in API level >= 24)

If an ELF file contains a DT_RUNPATH entry, the directories listed there
will be searched to resolve DT_NEEDED entries. The string `${ORIGIN}` will
be rewritten at runtime to the directory containing the ELF file. This
allows the use of relative paths. The `${LIB}` and `${PLATFORM}`
substitutions supported on some systems are not currently implemented on
Android.


## Writable and Executable Segments (Enforced for API level >= 26)

Each segment in an ELF file has associated flags that tell the
dynamic linker what permissions to give the corresponding page in
memory. For security, data shouldn't be executable and code shouldn't be
writable. This means that the W (for Writable) and E (for Executable)
flags should be mutually exclusive. This wasn't historically enforced,
but is now.

```
$ readelf --program-headers -W libBadFlags.so | grep WE
  LOAD           0x000000 0x00000000 0x00000000 0x4c01d 0x4c01d RWE 0x1000
```

*Resolution*: we're aware of one middleware product that introduces these
into your app. The middleware vendor is aware of the problem and has a fix
available.


## Invalid ELF header/section headers (Enforced for API level >= 26)

In API level 26 and above the dynamic linker checks more values in
the ELF header and section headers and fails if they are invalid.

*Example error*
```
dlopen failed: "/data/data/com.example.bad/lib.so" has unsupported e_shentsize: 0x0 (expected 0x28)
```

*Resolution*: don't use tools that produce invalid/malformed
ELF files. Note that using them puts application under high risk of
being incompatible with future versions of Android.


## Enable logging of dlopen/dlsym and library loading errors for apps (Available for API level >= 26)

On devices running API level 26 or later you can enable logging of dynamic
linker activity for debuggable apps by setting a property corresponding
to the fully-qualified name of the specific app:
```
adb shell setprop debug.ld.app.com.example.myapp dlerror,dlopen,dlsym
adb logcat
```

Any combination of `dlerror`, `dlopen`, and `dlsym` can be used. There's
no separate `dlclose` option: `dlopen` covers both loading and unloading
of libraries. Note also that `dlerror` doesn't correspond to actual
calls of dlerror(3) but to any time the dynamic linker writes to its
internal error buffer, so you'll see any errors the dynamic linker would
have reported, even if the code you're debugging doesn't actually call
dlerror(3) itself.

On userdebug and eng builds it is possible to enable tracing for the
whole system by using the `debug.ld.all` system property instead of
app-specific one. For example, to enable logging of all dlopen(3)
(and thus dclose(3)) calls, and all failures, but not dlsym(3) calls:
```
adb shell setprop debug.ld.all dlerror,dlopen
```


## dlclose interacts badly with thread local variables with non-trivial destructors

Android allows `dlclose` to unload a library even if there are still
thread-local variables with non-trivial destructors. This leads to
crashes when a thread exits and attempts to call the destructor, the
code for which has been unloaded (as in [issue 360], fixed in API level 28).

[issue 360]: https://github.com/android-ndk/ndk/issues/360

Not calling `dlclose` or ensuring that your library has `RTLD_NODELETE`
set (so that calls to `dlclose` don't actually unload the library)
are possible workarounds.

|                   | API level < 23             | >= 23   | >= 28 |
| ----------------- | -------------------------- | ------- | ----- |
| No workaround     | Works for static STL       | Broken  | Works |
| `-Wl,-z,nodelete` | Works for static STL       | Works   | Works |
| No `dlclose`      | Works                      | Works   | Works |


## ELF TLS (Available for API level >= 29)

Android supports [ELF TLS](docs/elf-tls.md) starting at API level 29. Since
NDK r26, clang will automatically enable ELF TLS for `minSdkVersion 29` or
higher. Otherwise, the existing emutls implementation (which uses
`pthread_key_create()` behind the scenes) will continue to be used. This
means that convenient C/C++ thread-local syntax is available at any API level;
at worst it will perform similarly to "roll your own" thread locals using
`pthread_key_create()` but at best you'll get the performance benefit of
ELF TLS, and the NDK will take care of the details.


## Use of IFUNC in libc (True for all API levels on devices running Android 10)

On devices running API level 29, libc uses
[IFUNC](https://sourceware.org/glibc/wiki/GNU_IFUNC)
functionality in the dynamic linker to choose optimized assembler routines at
run time rather than at build time. This lets us use the same `libc.so` on all
devices, and is similar to what other OSes already did. Because the zygote
uses the C library, this decision is made long before we know what API
level an app targets, so all code sees the new IFUNC-using C library.
Most apps should be unaffected by this change, but apps that hook or try to
detect hooking of C library functions might need to fix their code to cope
with IFUNC relocations. The affected functions are from `<string.h>`, but
may expand to include more functions (and more libraries) in future.


## Relative relocations (RELR)

Android added experimental support for RELR relative relocations
in API level 28, but using `SHT_` and `DT_` constants in the space
reserved for OS private use.

API level 30 added support for ELF files using the official `SHT_` and
`DT_` constants.

The RELR encoding is unrelated to the earlier "packed relocations"
format available from API level 23.

There are no plans to remove support for ELF files using the older
OS private use constants for RELR, nor for ELF files using packed
relocations.

Prior to API level 35, there was a bug that caused RELR relocations to
be applied after packed relocations. This meant that ifunc resolvers
referenced by `R_*_IRELATIVE` relocations in the packed relocation
section would have been able to read globals with RELR relocations
before they were relocated. The version of `lld` in the NDK has never
produced binaries affected by this bug, but third-party toolchains
should make sure not to store `R_*_IRELATIVE` relocations in packed
relocation sections in order to maintain compatibility with API levels
below 35.

You can read more about relative relocations
and their long and complicated history at
https://maskray.me/blog/2021-10-31-relative-relocations-and-relr.


## No more sentinels in .preinit_array/.init_array/.fini_array sections of executables (in All API levels)

In Android <= API level 34 and NDK <= r26, Android used sentinels in the
`.preinit_array`/`.init_array`/`.fini_array` sections of executables to locate
the start and end of these arrays. When building with LTO, the function pointers
in the arrays can be reordered, making sentinels no longer work. This prevents
constructors for global C++ variables from being called in static executables
when using LTO.

To fix this, in Android >= API level 35 and NDK >= r27, we removed sentinels
and switched to using symbols inserted by LLD (like `__init_array_start`,
`__init_array_end`) to locate the arrays. This also avoids the need for an
empty section when there are no corresponding functions.

For dynamic executables, we kept sentinel support in `crtbegin_dynamic.o` and
`libc.so`. This ensures that executables built with newer `crtbegin_dynamic.o`
(in NDK >= r27) work with older `libc.so` (in Android <= API level 34), and
vice versa.
