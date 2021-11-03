
#APP_MODULES := qn-curl
#APP_ABI := all


APP_OPTIM := release
APP_PLATFORM := android-19
APP_STL := c++_static
APP_CPPFLAGS += -frtti
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -DANDROID
APP_ABI := arm64-v8a x86_64 armeabi-v7a x86
APP_MODULES := qn-curl
NDK_TOOLCHAIN_VERSION := clang