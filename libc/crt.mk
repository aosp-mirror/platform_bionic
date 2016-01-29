# Define the libc run-time (crt) support object files that must be built,
# which are needed to build all other objects (shared/static libs and
# executables)
# ==========================================================================
# AArch64, ARM, MIPS, and x86 all need crtbegin_so/crtend_so.
#
# For x86, the .init section must point to a function that calls all
# entries in the .ctors section. (on ARM this is done through the
# .init_array section instead).
#
# For all the platforms, the .fini_array section must point to a function
# that will call __cxa_finalize(&__dso_handle) in order to ensure that
# static C++ destructors are properly called on dlclose().
#
# Args:
#     my_2nd_arch_prefix: set to $(TARGET_2ND_ARCH_VAR_PREFIX) if it's
#                         for the 2nd arch; otherwise empty.

my_arch := $(TARGET_$(my_2nd_arch_prefix)ARCH)

my_libc_crt_target_crtbegin_file := $(libc_crt_target_crtbegin_file_$(my_arch))
my_libc_crt_target_crtbegin_so_file := $(libc_crt_target_crtbegin_so_file_$(my_arch))

my_libc_crt_target_cflags := \
    $(libc_crt_target_cflags) \
    $(libc_crt_target_cflags_$(my_arch))

my_libc_crt_target_so_cflags := \
    $(libc_crt_target_so_cflags_$(my_arch)) \
    $(my_libc_crt_target_cflags)

my_libc_crt_target_ldflags := $(libc_crt_target_ldflags_$(my_arch))

# crtbrand.S -> crtbrand.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
$(GEN): PRIVATE_CC := $($(my_2nd_arch_prefix)TARGET_CC)
$(GEN): PRIVATE_CFLAGS := $(my_libc_crt_target_so_cflags)
$(GEN): $(LOCAL_PATH)/arch-common/bionic/crtbrand.S
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_CC) $(PRIVATE_CFLAGS) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
$(call include-depfile,$(GEN:%.o=%.P),$(GEN))

# crtbegin_so.c -> crtbegin_so1.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_so1.o
$(GEN): PRIVATE_CC := $($(my_2nd_arch_prefix)TARGET_CC)
$(GEN): PRIVATE_CFLAGS := $(my_libc_crt_target_so_cflags)
$(GEN): $(my_libc_crt_target_crtbegin_so_file)
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_CC) $(PRIVATE_CFLAGS) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
$(call include-depfile,$(GEN:%.o=%.P),$(GEN))

# crtbegin_so1.o + crtbrand.o -> crtbegin_so.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_so.o
$(GEN): PRIVATE_LD := $($(my_2nd_arch_prefix)TARGET_LD)
$(GEN): PRIVATE_LDFLAGS := $(my_libc_crt_target_ldflags)
$(GEN): $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_so1.o \
    $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_LD) $(PRIVATE_LDFLAGS) -r -o $@ $^

# crtend_so.S -> crtend_so.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_so.o
$(GEN): PRIVATE_CC := $($(my_2nd_arch_prefix)TARGET_CC)
$(GEN): PRIVATE_CFLAGS := $(my_libc_crt_target_so_cflags)
$(GEN): $(LOCAL_PATH)/arch-common/bionic/crtend_so.S
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_CC) $(PRIVATE_CFLAGS) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
$(call include-depfile,$(GEN:%.o=%.P),$(GEN))

# crtbegin_so.o and crtend_so.o are installed to device
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_SHARED_LIBRARIES)/crtbegin_so.o
$(GEN): $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_so.o
	$(hide) mkdir -p $(dir $@) && cp -f $< $@
ALL_GENERATED_SOURCES += $(GEN)

GEN := $($(my_2nd_arch_prefix)TARGET_OUT_SHARED_LIBRARIES)/crtend_so.o
$(GEN): $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_so.o
	$(hide) mkdir -p $(dir $@) && cp -f $< $@
ALL_GENERATED_SOURCES += $(GEN)

# crtbegin.c -> crtbegin_static1.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_static1.o
$(GEN): PRIVATE_CC := $($(my_2nd_arch_prefix)TARGET_CC)
$(GEN): PRIVATE_CFLAGS := $(my_libc_crt_target_cflags)
$(GEN): $(my_libc_crt_target_crtbegin_file)
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_CC) $(PRIVATE_CFLAGS) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
$(call include-depfile,$(GEN:%.o=%.P),$(GEN))

# crtbegin_static1.o + crtbrand.o -> crtbegin_static.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_static.o
$(GEN): PRIVATE_LD := $($(my_2nd_arch_prefix)TARGET_LD)
$(GEN): PRIVATE_LDFLAGS := $(my_libc_crt_target_ldflags)
$(GEN): $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_static1.o \
    $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_LD) $(PRIVATE_LDFLAGS) -r -o $@ $^

# crtbegin.c -> crtbegin_dynamic1.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_dynamic1.o
$(GEN): PRIVATE_CC := $($(my_2nd_arch_prefix)TARGET_CC)
$(GEN): PRIVATE_CFLAGS := $(my_libc_crt_target_cflags)
$(GEN): $(my_libc_crt_target_crtbegin_file)
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_CC) $(PRIVATE_CFLAGS) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
$(call include-depfile,$(GEN:%.o=%.P),$(GEN))

# crtbegin_dynamic1.o + crtbrand.o -> crtbegin_dynamic.o
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_dynamic.o
$(GEN): PRIVATE_LD := $($(my_2nd_arch_prefix)TARGET_LD)
$(GEN): PRIVATE_LDFLAGS := $(my_libc_crt_target_ldflags)
$(GEN): $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbegin_dynamic1.o \
    $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtbrand.o
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_LD) $(PRIVATE_LDFLAGS) -r -o $@ $^

# crtend.S -> crtend_android.o
# We rename crtend.o to crtend_android.o to avoid a
# name clash between gcc and bionic.
GEN := $($(my_2nd_arch_prefix)TARGET_OUT_INTERMEDIATE_LIBRARIES)/crtend_android.o
$(GEN): PRIVATE_CC := $($(my_2nd_arch_prefix)TARGET_CC)
$(GEN): PRIVATE_CFLAGS := $(my_libc_crt_target_cflags)
$(GEN): $(LOCAL_PATH)/arch-common/bionic/crtend.S
	@mkdir -p $(dir $@)
	$(hide) $(PRIVATE_CC) $(PRIVATE_CFLAGS) \
		-MD -MF $(@:%.o=%.d) -o $@ -c $<
	$(transform-d-to-p)
$(call include-depfile,$(GEN:%.o=%.P),$(GEN))

# Clear temp vars
my_libc_crt_target_ldflags :=
my_libc_crt_target_so_cflags :=
my_libc_crt_target_cflags :=
my_libc_crt_target_crtbegin_so_file :=
my_libc_crt_target_crtbegin_file :=
my_arch :=
