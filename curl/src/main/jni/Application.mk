
#APP_MODULES := qn-curl
#APP_ABI := all

APP_MODULES := qn-curl
NDK_TOOLCHAIN_VERSION := clang

APP_OPTIM := release
APP_PLATFORM := android-19
APP_STL := c++_static
#APP_STL := c++_shared
APP_CPPFLAGS += -fexceptions -frtti -DANDROID
APP_ABI := arm64-v8a x86_64 armeabi-v7a x86