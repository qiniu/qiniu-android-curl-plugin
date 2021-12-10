//
// Created by yangsen on 2020/10/22.
//

#include <jni.h>
#include <string>
#include <vector>
#include "curl_context.h"

#ifndef UPLOADDEMO_ANDROID_CURL_UTILS_H
#define UPLOADDEMO_ANDROID_CURL_UTILS_H

// 获取 curl 版本信息
std::string getCurlVersion();

// 返回的char *需要delete
std::vector<char> curlUtilConvertJByteArrayToChars(JNIEnv *env, jbyteArray byteArray);

#endif //UPLOADDEMO_ANDROID_CURL_UTILS_H
