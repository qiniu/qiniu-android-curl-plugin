#LOCAL_PATH := $(call my-dir)
#
#include $(CLEAR_VARS)
#LOCAL_LDLIBS := -llog
#
#LOCAL_MODULE    := qn-curl

#C_SOURCE_LIST := $(wildcard $(LOCAL_PATH)/*.c)
#C_SOURCE_LIST := $(wildcard $(LOCAL_PATH)/code/*.so)
#LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

#LOCAL_SRC_FILES := $(LOCAL_PATH)/native/curl_native_test.cpp
#
#include $(BUILD_SHARED_LIBRARY)


################################################################################

LOCAL_PATH := $(call my-dir)

#cURL prebuilt
include $(CLEAR_VARS)
LOCAL_MODULE := curl-built
LOCAL_SRC_FILES := \
  $(LOCAL_PATH)/lib/$(TARGET_ARCH_ABI)/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)
################################################################################

#qn-curl
include $(CLEAR_VARS)
LOCAL_MODULE := qn-curl

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/native/*.cpp)
LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
#	$(LOCAL_PATH)/native/curl_native.cpp \
#	$(LOCAL_PATH)/native/curl_context.cpp \
#	$(LOCAL_PATH)/native/curl_transaction_metrics.cpp \
#	$(LOCAL_PATH)/native/curl_configuration.cpp

LOCAL_STATIC_LIBRARIES := curl-built

LOCAL_LDFLAGS += -Wl,--gc-sections

LOCAL_CPPFLAGS += -std=gnu++0x
#LOCAL_CPPFLAGS += -Os
#LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden

LOCAL_CFLAGS := -Werror -DANDROID
#LOCAL_CFLAGS += -Os
#LOCAL_CFLAGS += -ffunction-sections -fdata-sections
ifeq ($(TARGET_ARCH),arm)
  LOCAL_CFLAGS += -mfpu=vfp -mfloat-abi=softfp -fno-short-enums
endif

#LOCAL_LDLIBS := -lz -llog -Wl,-s
LOCAL_LDLIBS := -lz -Wl -llog

LOCAL_C_INCLUDES += \
  $(NDK_PATH)/platforms/$(TARGET_PLATFORM)/arch-$(TARGET_ARCH)/usr/include \
  $(LOCAL_PATH)/lib/include

include $(BUILD_SHARED_LIBRARY)
################################################################################