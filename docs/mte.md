# Arm Memory Tagging Extension (MTE) implementation

AOSP supports Arm MTE to detect invalid memory accesses. The implementation is
spread across multiple components, both within and out of the AOSP tree. This
document gives an overview and pointers about how the various MTE features are
implemented.

For documentation of the behavior rather than the implementation, see the
[SAC page on MTE] instead. For MTE for apps, see the [NDK page on MTE].

The relevant components are:

* [LLVM Project] (out of AOSP tree)
    * Stack tagging instrumentation pass
    * Scudo memory allocator
* bionic
    * libc
    * dynamic loader
* Zygote
* debuggerd
* [NDK]

## MTE enablement

The way MTE is requested and enabled differs between native binaries and Java
apps. This is necessarily so, because Java apps get forked from the Zygote,
while native executables get inintialized by the linker.

### Native binaries

Both AOSP and the NDK allow you to compile C/C++ code that use MTE to detect
memory safety issues. The [NDK legacy cmake toolchain] and the
[NDK new cmake toolchain] both support "memtag" as an argument for
`ANDROID_SANITIZE`. NDK make has no specific support for MTE, but the
relevant flags can be passed directly as `CFLAGS` and `LDFLAGS`.

For the OS itself, [Soong] supports "memtag_[heap|stack|globals]" as
`SANITIZE_TARGET  and as `sanitize:` attribute in Android.bp files;
[Android make] supports the same environment variables as Soong. This passes
the appropriate flags to the clang driver for both compile and link steps.

#### Linker

* For **dynamic executables** LLD has support to
  [add appropriate dynamic sections] as defined in the [ELF standard]
* For **static executables** and as a fallback for older devices, LLD
  also supports [adding the Android-specific ELF note]

Both of the above are controlled by the linker flag `--android-memtag-mode`
which is [passed in by the clang driver] if
`-fsanitize=memtag-[stack|heap|globals]` is [passed in].
`-fsanitize=memtag` [enables all three] (even for API levels that don't
implement the runtime for globals, which means builds from old versions
of clang may no work with newer platform versions that support globals).
`-fsanitize-memtag-mode` allows to choose between ASYNC and SYNC.

This information can be queried using `llvm-readelf --memtag`.

This information is [picked up by libc init] to decide whether to enable MTE.
`-fsanitize-heap` controls both whether scudo tags allocations, and whether
tag checking is enabled.

#### Runtime environment (dynamic loader, libc)

There are two different initialization sequences for libc, both of which end up
calling `__libc_init_mte`.

N.B. the linker has its own copy of libc, which is used when executing these
functions. That is why we have to use `__libc_shared_globals` to communicate
with the libc of the process we are starting.

* **static executables** `__libc_init` is called from `crtbegin.c`, which calls
                         `__libc_init_mte`
* **dynamic executables** the linker calls `__libc_init_mte`

`__libc_init_mte` figures out the appropriate MTE level that is requested by
the process, calls `prctl` to request this from the kernel, and stores data in
`__libc_shared_globals` which gets picked up later to enable MTE in scudo.

It also does work related to stack tagging and permissive mode, which will be
detailed later.

### Apps

Apps can request MTE be enabled for their process via the manifest attribute
`android:memtagMode`. This gets interpreted by Zygote, which always runs with
`ASYNC` MTE enabled, because MTE for a process can only be disabled after
it has been initialized (see [Native binaries](#native-binaries)), not enabled.

[decideTaggingLevel] in the Zygote figures out whether to enable MTE for
an app, and stores it in the `runtimeFlags`, which get picked up by
[SpecializeCommon] after forking from the Zygote.

## MTE implementation

### Heap Tagging

Heap tagging is implemented in the scudo allocator. On `malloc` and `free`,
scudo will update the memory's tags to prevent use-after-free and buffer
overflows.

[scudo's memtag.h] contains helper functions to deal with MTE tag management,
which are used in [combined.h] and [secondary.h].


### Stack Tagging

Stack tagging requires instrumenting function bodies. It is implemented as
an instrumentation pass in LLVM called [AArch64StackTagging], which sets
the tags according to the lifetime of stack objects.

The instrumentation pass also supports recording stack history, consisting of:

* PC
* Frame pointer
* Base tag

This can be used to reconstruct which stack object was referred to in an
invalid access. The logic to reconstruct this can be found in the
[stack script].


Stack tagging is enabled in one of two circumstances:
* at process startup, if the main binary or any of its dependencies are
  compiled with `memtag-stack`
* library compiled with `memtag-stack` is `dlopen`ed later, either directly or
  as a dependency of a `dlopen`ed library. In this case, the
  [__pthread_internal_remap_stack_with_mte] function is used (called from
  `memtag_stack_dlopen_callback`). Because `dlopen`
  is handled by the linker, we have to [store a function pointer] to the
  process's version of the function in `__libc_shared_globals`.

Enabling stack MTE consists of two operations:
* Remapping the stacks as `PROT_MTE`
* Allocating a stack history buffer.

The first operation is only necessary when the process is running with MTE
enabled. The second operation is also necessary when the process is not running
with MTE enabled, because the writes to the stack history buffer are
unconditional.

libc keeps track of this through two globals:

* `__libc_memtag_stack`:  whether stack MTE is enabled on the process, i.e.
  whether the stack pages are mapped with PROT\_MTE. This is always false if
  MTE is disabled for the process (i.e. `libc_globals.memtag` is false).
* `__libc_memtag_stack_abi`: whether the process contains any code that was
  compiled with memtag-stack. This is true even if the process does not have
  MTE enabled.

### Globals Tagging

TODO(fmayer): write once submitted

### Crash reporting

For MTE crashes, debuggerd serializes special information into the Tombstone
proto:

* Tags around fault address
* Scudo allocation history

This is done in [tombstone\_proto.cpp]. The information is converted to a text
proto in [tombstone\_proto\_to\_text.cpp].

## Bootloader control

The bootloader API allows userspace to enable MTE on devices that do not ship
with MTE enabled by default.

See [SAC bootloader support] for the API definition. In AOSP, this API is
implemented in [system/extras/mtectrl]. mtectrl.rc handles the property
changes and invokes mtectrl to update the misc partition to communicate
with the bootloader.

There is also an [API in Device Policy Manager] that allows the device admin
to enable or disable MTE under certain circumstances.

The device can opt in or out of these APIs by a set of system properties:

* `ro.arm64.memtag.bootctl_supported`: the system property API is supported,
  and an option is displayed in Developer Options.
* `ro.arm64.memtag.bootctl_settings_toggle`: an option is displayed in the
  normal settings. This requires `ro.arm64.memtag.bootctl_supported` to be
  true. This implies `ro.arm64.memtag.bootctl_device_policy_manager`, if it
  is not explicitely set.
* `ro.arm64.memtag.bootctl_device_policy_manager`: the Device Policy Manager
  API is supported.

## Permissive MTE

Permissive MTE refers to a mode which, instead of crashing the process on an
MTE fault, records a tombstone but then continues execution of the process.
An important caveat is that system calls with invalid pointers (where the
pointer tag does not match the memory tag) still return an error code.

This mode is only available for system services, not apps. It is implemented
in the [debugger\_signal\_handler] by disabling MTE for the faulting thread.
Optionally, the user can ask for MTE to be re-enabled after some time.
This is achieved by arming a timer that calls [enable_mte_signal_handler]
upon expiry.

## MTE Mode Upgrade

When a system service [crashes in ASYNC mode], we set an impossible signal
as an exit code (because that signal is always gracefully handled by libc),
and [in init] we set `BIONIC_MEMTAG_UPGRADE_SECS`, which gets handled by
[libc startup].

[SpecializeCommon]: https://cs.android.com/android/platform/superproject/main/+/main:frameworks/base/core/jni/com_android_internal_os_Zygote.cpp?q=f:frameworks%2Fbase%2Fcore%2Fjni%2Fcom_android_internal_os_Zygote.cpp%20%22%20mallopt(M_BIONIC_SET_HEAP_TAGGING_LEVEL,%22&ss=android%2Fplatform%2Fsuperproject%2Fmain
[LLVM Project]: https://github.com/llvm/llvm-project/
[NDK]: https://android.googlesource.com/platform/ndk/
[NDK legacy cmake toolchain]: https://android.googlesource.com/platform/ndk/+/refs/heads/main/build/cmake/android-legacy.toolchain.cmake#490
[NDK new cmake toolchain]: https://android.googlesource.com/platform/ndk/+/refs/heads/main/build/cmake/flags.cmake#56
[Soong]: https://cs.android.com/android/platform/superproject/main/+/main:build/soong/cc/sanitize.go?q=sanitize.go&ss=android%2Fplatform%2Fsuperproject%2Fmain
[decideTaggingLevel]: https://cs.android.com/android/platform/superproject/main/+/main:frameworks/base/core/java/com/android/internal/os/Zygote.java?q=symbol:decideTaggingLevel
[picked up by libc init]: https://cs.android.com/android/platform/superproject/main/+/main:bionic/libc/bionic/libc_init_static.cpp?q=symbol:__get_tagging_level%20f:bionic
[enables all three]: https://github.com/llvm/llvm-project/blob/e732d1ce86783b1d7fe30645fcb30434109505b9/clang/include/clang/Basic/Sanitizers.def#L62
[passed in]: https://github.com/llvm/llvm-project/blob/ff2e619dfcd77328812a42d2ba2b11c3ff96f410/clang/lib/Driver/SanitizerArgs.cpp#L719
[passed in by the clang driver]: https://github.com/llvm/llvm-project/blob/ff2e619dfcd77328812a42d2ba2b11c3ff96f410/clang/lib/Driver/ToolChains/CommonArgs.cpp#L1595
[adding the Android-specific ELF note]: https://github.com/llvm/llvm-project/blob/435cb0dc5eca08cdd8d9ed0d887fa1693cc2bf33/lld/ELF/Driver.cpp#L1258
[ELF standard]: https://github.com/ARM-software/abi-aa/blob/main/memtagabielf64/memtagabielf64.rst#6dynamic-section
[add appropriate dynamic sections]: https://github.com/llvm/llvm-project/blob/7022498ac2f236e411e8a0f9a48669e754000a4b/lld/ELF/SyntheticSections.cpp#L1473
[storeTags]: https://cs.android.com/android/platform/superproject/main/+/main:external/scudo/standalone/memtag.h?q=f:scudo%20f:memtag.h%20function:storeTags
[SAC page on MTE]: https://source.android.com/docs/security/test/memory-safety/arm-mte
[NDK page on MTE]: https://developer.android.com/ndk/guides/arm-mte
[AArch64StackTagging]: https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/AArch64/AArch64StackTagging.cpp
[scudo's memtag.h]: https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/scudo/standalone/memtag.h
[combined.h]: https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/scudo/standalone/combined.h
[secondary.h]: https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/scudo/standalone/secondary.h
[__pthread_internal_remap_stack_with_mte]: https://cs.android.com/android/platform/superproject/main/+/main:bionic/libc/bionic/pthread_internal.cpp?q=__pthread_internal_remap_stack_with_mte
[stack script]: https://cs.android.com/android/platform/superproject/main/+/main:development/scripts/stack?q=stack
[Android make]: https://cs.android.com/android/platform/superproject/main/+/main:build/make/core/config_sanitizers.mk
[store a function pointer]: https://cs.android.com/android/platform/superproject/main/+/main:bionic/libc/bionic/libc_init_dynamic.cpp;l=168?q=memtag_stack_dlopen_callback
[tombstone\_proto.cpp]: https://cs.android.com/android/platform/superproject/main/+/main:system/core/debuggerd/libdebuggerd/tombstone_proto.cpp?q=tombstone_proto.cpp
[tombstone\_proto\_to\_text.cpp]: https://cs.android.com/android/platform/superproject/main/+/main:system/core/debuggerd/libdebuggerd/tombstone_proto_to_text.cpp
[SAC MTE bootloader support]: https://source.android.com/docs/security/test/memory-safety/bootloader-support
[system/extras/mtectrl]: https://cs.android.com/android/platform/superproject/main/+/main:system/extras/mtectrl/
[API in Device Policy Manager]: https://cs.android.com/android/platform/superproject/main/+/main:frameworks/base/core/java/android/app/admin/DevicePolicyManager.java?q=symbol:setMtePolicy%20f:DevicePolicyManager.java
[debuggerd\_signal_handler]: https://cs.android.com/android/platform/superproject/main/+/main:system/core/debuggerd/handler/debuggerd_handler.cpp?q=f:debuggerd_handler.cpp%20symbol:debuggerd_signal_handler
[enable_mte_signal_handler]: https://cs.android.com/android/platform/superproject/main/+/main:bionic/libc/bionic/libc_init_static.cpp?q=symbol:__enable_mte_signal_handler
[in init]: https://cs.android.com/android/platform/superproject/main/+/main:system/core/init/service.cpp?q=f:system%2Fcore%2Finit%2Fservice.cpp%20should_upgrade_mte
[crashes in ASYNC mode]: https://cs.android.com/android/platform/superproject/main/+/main:system/core/debuggerd/handler/debuggerd_handler.cpp;l=799?q=BIONIC_SIGNAL_ART_PROFILER
[libc startup]: https://cs.android.com/android/platform/superproject/main/+/main:bionic/libc/bionic/libc_init_static.cpp?q=BIONIC_MEMTAG_UPGRADE_SECS
