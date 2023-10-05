## fdsan

[TOC]

fdsan is a file descriptor sanitizer added to Android in API level 29.
In API level 29, fdsan warns when it finds a bug.
In API level 30, fdsan aborts when it finds a bug.

### Background
*What problem is fdsan trying to solve? Why should I care?*

fdsan (file descriptor sanitizer) detects mishandling of file descriptor ownership, which tend to manifest as *use-after-close* and *double-close*. These errors are direct analogues of the memory allocation *use-after-free* and *double-free* bugs, but tend to be much more difficult to diagnose and fix. With `malloc` and `free`, implementations have free reign to detect errors and abort on double free. File descriptors, on the other hand, are mandated by the POSIX standard to be allocated with the lowest available number being returned for new allocations. As a result, many file descriptor bugs can *never* be noticed on the thread on which the error occurred, and will manifest as "impossible" behavior on another thread.

For example, given two threads running the following code:
```cpp
void thread_one() {
    int fd = open("/dev/null", O_RDONLY);
    close(fd);
    close(fd);
}

void thread_two() {
    while (true) {
        int fd = open("log", O_WRONLY | O_APPEND);
        if (write(fd, "foo", 3) != 3) {
            err(1, "write failed!");
        }
    }
}
```
the following interleaving is possible:
```cpp
thread one                                thread two
open("/dev/null", O_RDONLY) = 123
close(123) = 0
                                          open("log", O_WRONLY | APPEND) = 123
close(123) = 0
                                          write(123, "foo", 3) = -1 (EBADF)
                                          err(1, "write failed!")
```

Assertion failures are probably the most innocuous result that can arise from these bugs: silent data corruption [[1](#footnotes), [2](#footnotes)] or security vulnerabilities are also possible (e.g. suppose thread two was saving user data to disk when a third thread came in and opened a socket to the Internet).

### Design
*What does fdsan do?*

fdsan attempts to detect and/or prevent file descriptor mismanagement by enforcing file descriptor ownership. Like how most memory allocations can have their ownership handled by types such as `std::unique_ptr`, almost all file descriptors can be associated with a unique owner which is responsible for their closure. fdsan provides functions to associate a file descriptor with an owner; if someone tries to close a file descriptor that they don't own, depending on configuration, either a warning is emitted, or the process aborts.

The way this is implemented is by providing functions to set a 64-bit closure tag on a file descriptor. The tag consists of an 8-bit type byte that identifies the type of the owner (`enum android_fdan_owner_type` in [`<android/fdsan.h>`](https://android.googlesource.com/platform/bionic/+/main/libc/include/android/fdsan.h)), and a 56-bit value. The value should ideally be something that uniquely identifies the object (object address for native objects and `System.identityHashCode` for Java objects), but in cases where it's hard to derive an identifier for the "owner" that should close a file descriptor, even using the same value for all file descriptors in the module can be useful, since it'll catch other code that closes your file descriptors.

If a file descriptor that's been marked with a tag is closed with an incorrect tag, or without a tag, we know something has gone wrong, and can generate diagnostics or abort.

### Enabling fdsan (as a user)
*How do I use fdsan?*

fdsan has four severity levels:
 - disabled (`ANDROID_FDSAN_ERROR_LEVEL_DISABLED`)
 - warn-once (`ANDROID_FDSAN_ERROR_LEVEL_WARN_ONCE`)
   - Upon detecting an error, emit a warning to logcat, generate a tombstone, and then continue execution with fdsan disabled.
 - warn-always (`ANDROID_FDSAN_ERROR_LEVEL_WARN_ALWAYS`)
   - Same as warn-once, except without disabling after the first warning.
 - fatal (`ANDROID_FDSAN_ERROR_LEVEL_FATAL`)
   - Abort upon detecting an error.

In Android Q, fdsan has a global default of warn-once. fdsan can be made more or less strict at runtime via the `android_fdsan_set_error_level` function in [`<android/fdsan.h>`](https://android.googlesource.com/platform/bionic/+/main/libc/include/android/fdsan.h).

The likelihood of fdsan catching a file descriptor error is proportional to the percentage of file descriptors in your process that are tagged with an owner.

### Using fdsan to fix a bug
*No, really, how do I use fdsan?*

Let's look at a simple contrived example that uses sleeps to force a particular interleaving of thread execution.

```cpp
#include <err.h>
#include <unistd.h>

#include <chrono>
#include <thread>
#include <vector>

#include <android-base/unique_fd.h>

using namespace std::chrono_literals;
using std::this_thread::sleep_for;

void victim() {
  sleep_for(300ms);
  int fd = dup(STDOUT_FILENO);
  sleep_for(200ms);
  ssize_t rc = write(fd, "good\n", 5);
  if (rc == -1) {
    err(1, "good failed to write?!");
  }
  close(fd);
}

void bystander() {
  sleep_for(100ms);
  int fd = dup(STDOUT_FILENO);
  sleep_for(300ms);
  close(fd);
}

void offender() {
  int fd = dup(STDOUT_FILENO);
  close(fd);
  sleep_for(200ms);
  close(fd);
}

int main() {
  std::vector<std::thread> threads;
  for (auto function : { victim, bystander, offender }) {
    threads.emplace_back(function);
  }
  for (auto& thread : threads) {
    thread.join();
  }
}
```

When running the program, the threads' executions will be interleaved as follows:

```cpp
// victim                         bystander                       offender
                                                                  int fd = dup(1); // 3
                                                                  close(3);
                                  int fd = dup(1); // 3
                                                                  close(3);
int fd = dup(1); // 3
                                  close(3);
write(3, "good\n") = 😞;
```

which results in the following output:

    fdsan_test: good failed to write?!: Bad file descriptor

This implies that either we're accidentally closing out file descriptor too early, or someone else is helpfully closing it for us. Let's use `android::base::unique_fd` in `victim` to guard the file descriptor with fdsan:

```diff
--- a/fdsan_test.cpp
+++ b/fdsan_test.cpp
@@ -12,13 +12,12 @@ using std::this_thread::sleep_for;

 void victim() {
   sleep_for(200ms);
-  int fd = dup(STDOUT_FILENO);
+  android::base::unique_fd fd(dup(STDOUT_FILENO));
   sleep_for(200ms);
   ssize_t rc = write(fd, "good\n", 5);
   if (rc == -1) {
     err(1, "good failed to write?!");
   }
-  close(fd);
 }

 void bystander() {
```

Now that we've guarded the file descriptor with fdsan, we should be able to find where the double close is:

```
pid: 25587, tid: 25589, name: fdsan_test  >>> fdsan_test <<<
signal 35 (<debuggerd signal>), code -1 (SI_QUEUE), fault addr --------
Abort message: 'attempted to close file descriptor 3, expected to be unowned, actually owned by unique_fd 0x7bf15dc448'
    x0  0000000000000000  x1  00000000000063f5  x2  0000000000000023  x3  0000007bf14de338
    x4  0000007bf14de3b8  x5  3463643531666237  x6  3463643531666237  x7  3834346364353166
    x8  00000000000000f0  x9  0000000000000000  x10 0000000000000059  x11 0000000000000035
    x12 0000007bf1bebcfa  x13 0000007bf14ddf0a  x14 0000007bf14ddf0a  x15 0000000000000000
    x16 0000007bf1c33048  x17 0000007bf1ba9990  x18 0000000000000000  x19 00000000000063f3
    x20 00000000000063f5  x21 0000007bf14de588  x22 0000007bf1f1b864  x23 0000000000000001
    x24 0000007bf14de130  x25 0000007bf13e1000  x26 0000007bf1f1f580  x27 0000005ab43ab8f0
    x28 0000000000000000  x29 0000007bf14de400
    sp  0000007bf14ddff0  lr  0000007bf1b5fd6c  pc  0000007bf1b5fd90

backtrace:
    #00 pc 0000000000008d90  /system/lib64/libc.so (fdsan_error(char const*, ...)+384)
    #01 pc 0000000000008ba8  /system/lib64/libc.so (android_fdsan_close_with_tag+632)
    #02 pc 00000000000092a0  /system/lib64/libc.so (close+16)
    #03 pc 00000000000003e4  /system/bin/fdsan_test (bystander()+84)
    #04 pc 0000000000000918  /system/bin/fdsan_test
    #05 pc 000000000006689c  /system/lib64/libc.so (__pthread_start(void*)+36)
    #06 pc 000000000000712c  /system/lib64/libc.so (__start_thread+68)
```

...in the obviously correct bystander? What's going on here?

The reason for this is (hopefully!) not a bug in fdsan, and will commonly be seen when tracking down double-closes in processes that have sparse fdsan coverage. What actually happened is that the culprit closed `bystander`'s file descriptor between its open and close, which resulted in `bystander` being blamed for closing `victim`'s fd. If we store `bystander`'s fd in a `unique_fd` as well, we should get something more useful:
```diff
--- a/tmp/fdsan_test.cpp
+++ b/tmp/fdsan_test.cpp
@@ -23,9 +23,8 @@ void victim() {

 void bystander() {
   sleep_for(100ms);
-  int fd = dup(STDOUT_FILENO);
+  android::base::unique_fd fd(dup(STDOUT_FILENO));
   sleep_for(200ms);
-  close(fd);
 }
```
giving us:
```
pid: 25779, tid: 25782, name: fdsan_test  >>> fdsan_test <<<
signal 35 (<debuggerd signal>), code -1 (SI_QUEUE), fault addr --------
Abort message: 'attempted to close file descriptor 3, expected to be unowned, actually owned by unique_fd 0x6fef9ff448'
    x0  0000000000000000  x1  00000000000064b6  x2  0000000000000023  x3  0000006fef901338
    x4  0000006fef9013b8  x5  3466663966656636  x6  3466663966656636  x7  3834346666396665
    x8  00000000000000f0  x9  0000000000000000  x10 0000000000000059  x11 0000000000000039
    x12 0000006ff0055cfa  x13 0000006fef900f0a  x14 0000006fef900f0a  x15 0000000000000000
    x16 0000006ff009d048  x17 0000006ff0013990  x18 0000000000000000  x19 00000000000064b3
    x20 00000000000064b6  x21 0000006fef901588  x22 0000006ff04ff864  x23 0000000000000001
    x24 0000006fef901130  x25 0000006fef804000  x26 0000006ff0503580  x27 0000006368aa18f8
    x28 0000000000000000  x29 0000006fef901400
    sp  0000006fef900ff0  lr  0000006feffc9d6c  pc  0000006feffc9d90

backtrace:
    #00 pc 0000000000008d90  /system/lib64/libc.so (fdsan_error(char const*, ...)+384)
    #01 pc 0000000000008ba8  /system/lib64/libc.so (android_fdsan_close_with_tag+632)
    #02 pc 00000000000092a0  /system/lib64/libc.so (close+16)
    #03 pc 000000000000045c  /system/bin/fdsan_test (offender()+68)
    #04 pc 0000000000000920  /system/bin/fdsan_test
    #05 pc 000000000006689c  /system/lib64/libc.so (__pthread_start(void*)+36)
    #06 pc 000000000000712c  /system/lib64/libc.so (__start_thread+68)
```

Hooray!

In a real application, things are probably not going to be as detectable or reproducible as our toy example, which is a good reason to try to maximize the usage of fdsan-enabled types like `unique_fd` and `ParcelFileDescriptor`, to improve the odds that double closes in other code get detected.

### Enabling fdsan (as a C++ library implementer)

fdsan operates via two main primitives. `android_fdsan_exchange_owner_tag` modifies a file descriptor's close tag, and `android_fdsan_close_with_tag` closes a file descriptor with its tag. In the `<android/fdsan.h>` header, these are marked with `__attribute__((weak))`, so instead of passing down the platform version from JNI, availability of the functions can be queried directly. An example implementation of unique_fd follows:

```cpp
/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <android/fdsan.h>
#include <unistd.h>

#include <utility>

struct unique_fd {
    unique_fd() = default;

    explicit unique_fd(int fd) {
        reset(fd);
    }

    unique_fd(const unique_fd& copy) = delete;
    unique_fd(unique_fd&& move) {
        *this = std::move(move);
    }

    ~unique_fd() {
        reset();
    }

    unique_fd& operator=(const unique_fd& copy) = delete;
    unique_fd& operator=(unique_fd&& move) {
        if (this == &move) {
            return *this;
        }

        reset();

        if (move.fd_ != -1) {
            fd_ = move.fd_;
            move.fd_ = -1;

            // Acquire ownership from the moved-from object.
            exchange_tag(fd_, move.tag(), tag());
        }

        return *this;
    }

    int get() { return fd_; }

    int release() {
        if (fd_ == -1) {
            return -1;
        }

        int fd = fd_;
        fd_ = -1;

        // Release ownership.
        exchange_tag(fd, tag(), 0);
        return fd;
    }

    void reset(int new_fd = -1) {
        if (fd_ != -1) {
            close(fd_, tag());
            fd_ = -1;
        }

        if (new_fd != -1) {
            fd_ = new_fd;

            // Acquire ownership of the presumably unowned fd.
            exchange_tag(fd_, 0, tag());
        }
    }

  private:
    int fd_ = -1;

    // The obvious choice of tag to use is the address of the object.
    uint64_t tag() {
        return reinterpret_cast<uint64_t>(this);
    }

    // These functions are marked with __attribute__((weak)), so that their
    // availability can be determined at runtime. These wrappers will use them
    // if available, and fall back to no-ops or regular close on pre-Q devices.
    static void exchange_tag(int fd, uint64_t old_tag, uint64_t new_tag) {
        if (android_fdsan_exchange_owner_tag) {
            android_fdsan_exchange_owner_tag(fd, old_tag, new_tag);
        }
    }

    static int close(int fd, uint64_t tag) {
        if (android_fdsan_close_with_tag) {
            return android_fdsan_close_with_tag(fd, tag);
        } else {
            return ::close(fd);
        }
    }
};
```

### Frequently seen bugs
 * Native APIs not making it clear when they take ownership of a file descriptor. <br/>
   * Solution: accept `unique_fd` instead of `int` in functions that take ownership.
   * [Example one](https://android-review.googlesource.com/c/platform/system/core/+/721985), [two](https://android-review.googlesource.com/c/platform/frameworks/native/+/709451)
 * Receiving a `ParcelFileDescriptor` via Intent, and then passing it into JNI code that ends up calling close on it. <br/>
   * Solution: ¯\\\_(ツ)\_/¯. Use fdsan?
   * [Example one](https://android-review.googlesource.com/c/platform/system/bt/+/710104), [two](https://android-review.googlesource.com/c/platform/frameworks/base/+/732305)

### Footnotes
1. [How To Corrupt An SQLite Database File](https://www.sqlite.org/howtocorrupt.html#_continuing_to_use_a_file_descriptor_after_it_has_been_closed)

2. [<b><i>50%</i></b> of Facebook's iOS crashes caused by a file descriptor double close leading to SQLite database corruption](https://code.fb.com/ios/debugging-file-corruption-on-ios/)
