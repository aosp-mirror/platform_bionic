# Android changes for NDK developers

This document details important changes related to native code
loading in various Android releases.

Required tools: the NDK has an _arch_-linux-android-readelf binary
(e.g. arm-linux-androideabi-readelf or i686-linux-android-readelf)
for each architecture (under toolchains/), but you can use readelf for
any architecture, as we will be doing basic inspection only. On Linux
you need to have the “binutils” package installed for readelf,
and “pax-utils” for scanelf.


## Private API (Enforced for API level >= 24)

Native libraries must use only public API, and must not link against
non-NDK platform libraries. Starting with API 24 this rule is enforced and
applications are no longer able to load non-NDK platform libraries. The
rule is enforced by the dynamic linker, so non-public libraries
are not accessible regardless of the way code tries to load them:
System.loadLibrary, DT_NEEDED entries, and direct calls to dlopen(3)
will all work exactly the same.

Users should have a consistent app experience across updates,
and developers shouldn't have to make emergency app updates to
handle platform changes. For that reason, we recommend against using
private C/C++ symbols. Private symbols aren't tested as part of the
Compatibility Test Suite (CTS) that all Android devices must pass. They
may not exist, or they may behave differently. This makes apps that use
them more likely to fail on specific devices, or on future releases ---
as many developers found when Android 6.0 Marshmallow switched from
OpenSSL to BoringSSL.

In order to reduce the user impact of this transition, we've identified
a set of libraries that see significant use from Google Play's
most-installed apps, and that are feasible for us to support in the
short term (including libandroid_runtime.so, libcutils.so, libcrypto.so,
and libssl.so). In order to give you more time to transition, we will
temporarily support these libraries; so if you see a warning that means
your code will not work in a future release -- please fix it now!

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

*Potential problems*: starting from API 24 the dynamic linker will not
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
uses them for sanity checking. Some developers strip them in an
attempt to obfuscate the binary and prevent reverse engineering. (This
doesn't really help because it is possible to reconstruct the stripped
information using widely-available tools.)

```
$ readelf --header libBroken.so | grep 'section headers'
  Start of section headers:          0 (bytes into file)
  Size of section headers:           0 (bytes)
  Number of section headers:         0
```

*Resolution*: remove the extra steps from your build that strip section
headers.

## Text Relocations (Enforced for API level >= 23)

Starting with API 23, shared objects must not contain text
relocations. That is, the code must be loaded as is and must not be
modified. Such an approach reduces load time and improves security.

The usual reason for text relocations is non-position independent
hand-written assembler. This is not common. Use the scanelf tool as
described in our documentation for further diagnostics:

```
$ scanelf -qT libTextRel.so
  libTextRel.so: (memory/data?) [0x15E0E2] in (optimized out: previous simd_broken_op1) [0x15E0E0]
  libTextRel.so: (memory/data?) [0x15E3B2] in (optimized out: previous simd_broken_op2) [0x15E3B0]
  ...
```

If you have no scanelf tool available, it is possible to do a basic
check with readelf instead, look for either a TEXTREL entry or the
TEXTREL flag. Either alone is sufficient. (The value corresponding to the
TEXTREL entry is irrelevant and typically 0 --- simply the presence of
the TEXTREL entry declares that the .so contains text relocations). This
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
linker has issued warnings about text relocations since Android K
(API 19), but on API 23 and above it refuses to load code with text
relocations.

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

Before API 23, Android's dynamic linker ignored the full path, and
used only the basename (the part after the last ‘/') when looking
up the required libraries. Since API 23 the runtime linker will honor
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

*Potential problems*: before API 23 the DT_NEEDED entry's basename was
used, but starting from API 23 the Android runtime will try to load the
library using the path specified, and that path won't exist on the
device. There are broken third-party toolchains/build systems that use
a path on a build host instead of the SONAME.

*Resolution*: make sure all required libraries are referenced by SONAME
only. It is better to let the runtime linker to find and load those
libraries as the location may change from device to device.


## Missing SONAME (Enforced for API level >= 23)

Each ELF shared object (“native library”) must have a SONAME (Shared
Object Name) attribute. The NDK toolchain adds this attribute by default,
so its absence indicates either a misconfigured alternative toolchain
or a misconfiguration in your build system. A missing SONAME may lead
to runtime issues such as the wrong library being loaded: the filename
is used instead when this attribute is missing.

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
the -soname linker option).
