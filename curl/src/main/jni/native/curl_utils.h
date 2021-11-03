//
// Created by yangsen on 2020/10/22.
//

#include <jni.h>
#include "curl_context.h"

#ifndef UPLOADDEMO_ANDROID_CURL_UTILS_H
#define UPLOADDEMO_ANDROID_CURL_UTILS_H

char * curlUtilStrToLower(char *str);

char * curlUtilStrStr(char * ps,char *pd);

int curlUtilStrReplace(char *p_result,char* p_source,char* p_seach,char *p_repstr);

long long int curlUtilGetRequestContentLength(JNIEnv *env, jbyteArray body, jobjectArray header);

// 返回的char *需要delete
char* curlUtilConvertJByteArrayToChars(JNIEnv *env, jbyteArray byteArray);

#endif //UPLOADDEMO_ANDROID_CURL_UTILS_H
