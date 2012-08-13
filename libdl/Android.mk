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
ifeq ($(TARGET_ARCH),x86)
LOCAL_LDFLAGS += -Wl,--exclude-libs=libgcc_eh.a
endif

LOCAL_SRC_FILES:= libdl.c

LOCAL_MODULE:= libdl
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

# NOTE: libdl needs __aeabi_unwind_cpp_pr0 from libgcc.a but libgcc.a needs a
# few symbols from libc. Using --no-undefined here results in having to link
# against libc creating a circular dependency which is removed and we end up
# with missing symbols. Since this library is just a bunch of stubs, we set
# LOCAL_ALLOW_UNDEFINED_SYMBOLS to remove --no-undefined from the linker flags.
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_SHARED_LIBRARY)

BUILD_DLTEST:=0
ifeq ($(BUILD_DLTEST),1)

#
# dltest
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= dltest.c

LOCAL_MODULE:= dltest
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk

LOCAL_SHARED_LIBRARIES := libdl

include $(BUILD_EXECUTABLE)

endif
