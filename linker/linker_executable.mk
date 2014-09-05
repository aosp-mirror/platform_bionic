
#
# Instead of including $(BUILD_EXECUTABLE), we execute the steps to create an executable by
# hand, as we want to insert an extra objcopy step that is not supported by the build
# system, and is probably specific the linker only, so there's no need to modify the build
# system for the purpose.
#

LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_MODULE_SUFFIX := $(TARGET_EXECUTABLE_SUFFIX)

# Clang calls /usr/bin/ld: unrecognized option '--icf=safe', http://b/17403674.
LOCAL_CLANG := false
include $(BUILD_SYSTEM)/dynamic_binary.mk

# See build/core/executable_internal.mk
$(linked_module): PRIVATE_TARGET_GLOBAL_LD_DIRS := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_GLOBAL_LD_DIRS)
$(linked_module): PRIVATE_TARGET_GLOBAL_LDFLAGS := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_GLOBAL_LDFLAGS)
$(linked_module): PRIVATE_TARGET_FDO_LIB := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_FDO_LIB)
$(linked_module): PRIVATE_TARGET_LIBGCC := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_LIBGCC)
$(linked_module): PRIVATE_TARGET_CRTBEGIN_DYNAMIC_O := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_CRTBEGIN_DYNAMIC_O)
$(linked_module): PRIVATE_TARGET_CRTBEGIN_STATIC_O := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_CRTBEGIN_STATIC_O)
$(linked_module): PRIVATE_TARGET_CRTEND_O := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_CRTEND_O)
$(linked_module): PRIVATE_TARGET_OBJCOPY := $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_OBJCOPY)
$(linked_module): $(TARGET_CRTBEGIN_STATIC_O) $(all_objects) $(all_libraries) $($(LOCAL_2ND_ARCH_VAR_PREFIX)TARGET_CRTEND_O)
	$(transform-o-to-static-executable)
	@echo "target PrefixSymbols: $(PRIVATE_MODULE) ($@)"
	$(hide) $(PRIVATE_TARGET_OBJCOPY) --prefix-symbols=__dl_ $@
