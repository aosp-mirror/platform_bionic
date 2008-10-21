LOCAL_PATH:= $(call my-dir)

#
# static 
#

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	libthread_db.c

LOCAL_MODULE:= libthread_db

include $(BUILD_STATIC_LIBRARY)

#
# shared
#

include $(CLEAR_VARS)

LOCAL_WHOLE_STATIC_LIBRARIES := libthread_db
LOCAL_MODULE:=libthread_db
LOCAL_SHARED_LIBRARIES := libdl

# NOTE: Using --no-undefined results in a missing symbol that is defined inside
# gdbserver and is resolved at runtime. Since there is no library containing
# this symbol that we can link against, set LOCAL_ALLOW_UNDEFINED_SYMBOLS so
# that --no-undefined is removed from the linker flags.
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_SYSTEM_SHARED_LIBRARIES :=

include $(BUILD_SHARED_LIBRARY)
