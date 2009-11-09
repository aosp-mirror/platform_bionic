LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	arch/$(TARGET_ARCH)/begin.S \
	linker.c \
	rt.c \
	dlfcn.c \
	debugger.c \
	ba.c

ifeq ($(TARGET_ARCH),sh)
# SH-4A series virtual address range from 0x00000000 to 0x7FFFFFFF.
LINKER_TEXT_BASE := 0x70000100
else
LINKER_TEXT_BASE := 0xB0000100
endif

# The maximum size set aside for the linker, from
# LINKER_TEXT_BASE rounded down to a megabyte.
LINKER_AREA_SIZE := 0x01000000

LOCAL_LDFLAGS := -Wl,-Ttext,$(LINKER_TEXT_BASE)

LOCAL_CFLAGS += -DPRELINK
LOCAL_CFLAGS += -DLINKER_TEXT_BASE=$(LINKER_TEXT_BASE)
LOCAL_CFLAGS += -DLINKER_AREA_SIZE=$(LINKER_AREA_SIZE)

# we need to access the Bionic private header <bionic_tls.h>
# in the linker
LOCAL_CFLAGS += -I$(LOCAL_PATH)/../libc/private

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -DANDROID_ARM_LINKER
else
  ifeq ($(TARGET_ARCH),x86)
    LOCAL_CFLAGS += -DANDROID_X86_LINKER
  else
    ifeq ($(TARGET_ARCH),sh)
      LOCAL_CFLAGS += -DANDROID_SH_LINKER
    else
      $(error Unsupported TARGET_ARCH $(TARGET_ARCH))
    endif
  endif
endif

LOCAL_MODULE:= linker

LOCAL_STATIC_LIBRARIES := libcutils libc_nomalloc

#LOCAL_FORCE_STATIC_EXECUTABLE := true # not necessary when not including BUILD_EXECUTABLE

#
# include $(BUILD_EXECUTABLE)
#
# Instead of including $(BUILD_EXECUTABLE), we execute the steps to create an executable by
# hand, as we want to insert an extra step that is not supported by the build system, and
# is probably specific the linker only, so there's no need to modify the build system for
# the purpose.

LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_SUFFIX := $(TARGET_EXECUTABLE_SUFFIX)

# Executables are not prelinked.
LOCAL_PRELINK_MODULE := false

include $(BUILD_SYSTEM)/dynamic_binary.mk

$(linked_module): $(TARGET_CRTBEGIN_STATIC_O) $(all_objects) $(all_libraries) $(TARGET_CRTEND_O)
	$(transform-o-to-static-executable)
	@echo "target PrefixSymbols: $(PRIVATE_MODULE) ($@)"
	$(hide) $(TARGET_OBJCOPY) --prefix-symbols=__dl_ $@

#
# end of BUILD_EXECUTABLE hack
#

# we don't want crtbegin.o (because we have begin.o), so unset it
# just for this module
$(LOCAL_BUILT_MODULE): TARGET_CRTBEGIN_STATIC_O :=
# This line is not strictly necessary because the dynamic linker is built
# as a static executable, but it won't hurt if in the future we start 
# building the linker as a dynamic one.
$(LOCAL_BUILT_MODULE): TARGET_CRTBEGIN_DYNAMIC_O :=
