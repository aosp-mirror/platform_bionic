LOCAL_PATH:= $(call my-dir)

#
# libdl
#

include $(CLEAR_VARS)

# NOTE: --exclude-libs=libgcc.a makes sure that any symbols libdl.so pulls from
# libgcc.a are made static to libdl.so.  This in turn ensures that libraries that
# a) pull symbols from libgcc.a and b) depend on libdl.so will not rely on libdl.so
# to provide those symbols, but will instead pull them from libgcc.a.  Specifically,
# we use this property to make sure libc.so has its own copy of the code from
# libgcc.a it uses.
#
# DO NOT REMOVE --exclude-libs!

LOCAL_LDFLAGS := -Wl,--exclude-libs=libgcc.a

# for x86, exclude libgcc_eh.a for the same reasons as above
LOCAL_LDFLAGS_x86 := -Wl,--exclude-libs=libgcc_eh.a
LOCAL_LDFLAGS_x86_64 := $(LOCAL_LDFLAGS_x86)

LOCAL_LDFLAGS_arm    += -Wl,--version-script=$(LOCAL_PATH)/libdl.arm.map
LOCAL_LDFLAGS_arm64  += -Wl,--version-script=$(LOCAL_PATH)/libdl.arm64.map
LOCAL_LDFLAGS_mips   += -Wl,--version-script=$(LOCAL_PATH)/libdl.mips.map
LOCAL_LDFLAGS_mips64 += -Wl,--version-script=$(LOCAL_PATH)/libdl.mips64.map
LOCAL_LDFLAGS_x86    += -Wl,--version-script=$(LOCAL_PATH)/libdl.x86.map
LOCAL_LDFLAGS_x86_64 += -Wl,--version-script=$(LOCAL_PATH)/libdl.x86_64.map

LOCAL_SRC_FILES:= libdl.c
LOCAL_CFLAGS := -Wall -Wextra -Wunused -Werror
LOCAL_CXX_STL := none

LOCAL_MODULE := libdl
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk \
                                 $(LOCAL_PATH)/libdl.arm.map \
                                 $(LOCAL_PATH)/libdl.arm64.map \
                                 $(LOCAL_PATH)/libdl.mips.map \
                                 $(LOCAL_PATH)/libdl.mips64.map \
                                 $(LOCAL_PATH)/libdl.x86.map \
                                 $(LOCAL_PATH)/libdl.x86_64.map \

# NOTE: libdl needs __aeabi_unwind_cpp_pr0 from libgcc.a but libgcc.a needs a
# few symbols from libc. Using --no-undefined here results in having to link
# against libc creating a circular dependency which is removed and we end up
# with missing symbols. Since this library is just a bunch of stubs, we set
# LOCAL_ALLOW_UNDEFINED_SYMBOLS to remove --no-undefined from the linker flags.
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_SYSTEM_SHARED_LIBRARIES :=

LOCAL_SANITIZE := never
include $(BUILD_SHARED_LIBRARY)

# A dummy libdl.a. Need for static executables using the LLVM unwinder. Most
# functions default to failure, others use a sensible default (dl_iterate_phdr()
# returns 0, as would happen if the user iterated over every phdr).
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= libdl.c
LOCAL_CFLAGS := -Wall -Wextra -Wunused -Werror
LOCAL_CXX_STL := none

LOCAL_MODULE := libdl
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_SANITIZE := never
include $(BUILD_STATIC_LIBRARY)
