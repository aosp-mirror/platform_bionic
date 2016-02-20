LOCAL_PATH := $(call my-dir)

libc_malloc_debug_src_files := \
    BacktraceData.cpp \
    Config.cpp \
    DebugData.cpp \
    debug_disable.cpp \
    FreeTrackData.cpp \
    GuardData.cpp \
    malloc_debug.cpp \
    TrackData.cpp \

# ==============================================================
# libc_malloc_debug_backtrace.a
# ==============================================================
# Used by libmemunreachable
include $(CLEAR_VARS)

LOCAL_MODULE := libc_malloc_debug_backtrace

LOCAL_SRC_FILES := \
    backtrace.cpp \
    MapData.cpp \

LOCAL_CXX_STL := libc++_static

LOCAL_STATIC_LIBRARIES += \
    libc_logging \

LOCAL_C_INCLUDES += bionic/libc
LOCAL_EXPORT_C_INCLUDE_DIRS += $(LOCAL_PATH)

LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := false

# -Wno-error=format-zero-length needed for gcc to compile.
LOCAL_CFLAGS := \
    -Wall \
    -Werror \
    -Wno-error=format-zero-length \

include $(BUILD_STATIC_LIBRARY)

# ==============================================================
# libc_malloc_debug.so
# ==============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := libc_malloc_debug

LOCAL_SRC_FILES := \
    $(libc_malloc_debug_src_files) \

LOCAL_CXX_STL := libc++_static

# Only need this for arm since libc++ uses its own unwind code that
# doesn't mix with the other default unwind code.
LOCAL_STATIC_LIBRARIES_arm := libunwind_llvm

LOCAL_STATIC_LIBRARIES += \
    libc_malloc_debug_backtrace \
    libc_logging \

LOCAL_LDFLAGS_32 := -Wl,--version-script,$(LOCAL_PATH)/exported32.map
LOCAL_LDFLAGS_64 := -Wl,--version-script,$(LOCAL_PATH)/exported64.map
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_C_INCLUDES += bionic/libc

LOCAL_SANITIZE := never
LOCAL_NATIVE_COVERAGE := false

# -Wno-error=format-zero-length needed for gcc to compile.
LOCAL_CFLAGS := \
    -Wall \
    -Werror \
    -fno-stack-protector \
    -Wno-error=format-zero-length \

include $(BUILD_SHARED_LIBRARY)

# ==============================================================
# Unit Tests
# ==============================================================
include $(CLEAR_VARS)

LOCAL_MODULE := malloc_debug_unit_tests
LOCAL_MODULE_STEM_32 := $(LOCAL_MODULE)32
LOCAL_MODULE_STEM_64 := $(LOCAL_MODULE)64

LOCAL_SRC_FILES := \
    tests/backtrace_fake.cpp \
    tests/log_fake.cpp \
    tests/libc_fake.cpp \
    tests/property_fake.cpp \
    tests/malloc_debug_config_tests.cpp \
    tests/malloc_debug_unit_tests.cpp \
    $(libc_malloc_debug_src_files) \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/tests
LOCAL_C_INCLUDES += bionic/libc

LOCAL_SHARED_LIBRARIES := libbase

LOCAL_CFLAGS := \
    -Wall \
    -Werror \
    -Wno-error=format-zero-length \

include $(BUILD_NATIVE_TEST)
