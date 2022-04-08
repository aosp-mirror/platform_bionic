Malloc Hooks
============

Malloc hooks allows a program to intercept all allocation/free calls that
happen during execution. It is only available in Android P and newer versions
of the OS.

There are two ways to enable these hooks, set a special system
property, or set a special environment variable and run your app/program.

When malloc hooks is enabled, it works by adding a shim layer that replaces
the normal allocation calls. The replaced calls are:

* `malloc`
* `free`
* `calloc`
* `realloc`
* `posix_memalign`
* `memalign`
* `aligned_alloc`
* `malloc_usable_size`

On 32 bit systems, these two deprecated functions are also replaced:

* `pvalloc`
* `valloc`

These four hooks are defined in malloc.h:

    void* (*volatile __malloc_hook)(size_t, const void*);
    void* (*volatile __realloc_hook)(void*, size_t, const void*);
    void (*volatile __free_hook)(void*, const void*);
    void* (*volatile __memalign_hook)(size_t, size_t, const void*);

When malloc is called and \_\_malloc\_hook has been set, then the hook
function is called instead.

When realloc is called and \_\_realloc\_hook has been set, then the hook
function is called instead.

When free is called and \_\_free\_hook has been set, then the hook
function is called instead.

When memalign is called and \_\_memalign\_hook has been set, then the hook
function is called instead.

For posix\_memalign, if \_\_memalign\_hook has been set, then the hook is
called, but only if alignment is a power of 2.

For aligned\_alloc, if \_\_memalign\_hook has been set, then the hook is
called, but only if alignment is a power of 2.

For calloc, if \_\_malloc\_hook has been set, then the hook function is
called, then the allocated memory is set to zero.

For the two deprecated functions pvalloc and valloc, if \_\_memalign\_hook
has been set, then the hook is called with an appropriate alignment value.

There is no hook for malloc\_usable\_size as of now.

These hooks can be set at any time, but there is no thread safety, so
the caller must guarantee that it does not depend on allocations/frees
occurring at the same time.

Implementation Details
======================
When malloc hooks is enabled, then the hook pointers are set to
the current default allocation functions. It is expected that if an
app does intercept the allocation/free calls, it will eventually call
the original hook function to do allocations. If the app does not do this,
it runs the risk of crashing whenever a malloc\_usable\_size call is made.

Example Implementation
======================
Below is a simple implementation intercepting only malloc/calloc calls.

    void* new_malloc_hook(size_t bytes, const char* arg) {
      return orig_malloc_hook(bytes, arg);
    }

    void orig_malloc_hook = __malloc_hook;
    __malloc_hook = new_malloc_hook;

Enabling Examples
=================

### For platform developers

Enable the hooks for all processes:

    adb shell stop
    adb shell setprop libc.debug.hooks.enable 1
    adb shell start

Enable malloc hooks using an environment variable:

    adb shell
    # export LIBC_HOOKS_ENABLE=1
    # ls

Any process spawned from this shell will run with malloc hooks enabled.

### For app developers

Enable malloc hooks for a specific program/application:

    adb shell setprop wrap.<APP> '"LIBC_HOOKS_ENABLE=1"'

For example, to enable malloc hooks for the google search box:

    adb shell setprop wrap.com.google.android.googlequicksearchbox '"LIBC_HOOKS_ENABLE=1 logwrapper"'
    adb shell am force-stop com.google.android.googlequicksearchbox

NOTE: On pre-O versions of the Android OS, property names had a length limit
of 32. This meant that to create a wrap property with the name of the app, it
was necessary to truncate the name to fit. On O, property names can be
an order of magnitude larger, so there should be no need to truncate the name
at all.
