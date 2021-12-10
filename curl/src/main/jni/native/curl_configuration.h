//
// Created by yangsen on 2020/9/18.
//

#include <curl/curl.h>
#include "curl_context.h"

#ifndef CURLDEMO_CURL_CONFIGURATION_H
#define CURLDEMO_CURL_CONFIGURATION_H

void setCurlContextWithConfiguration(JNIEnv *env, CurlContext *curlContext, jobject curlConfiguration);

#endif //CURLDEMO_CURL_CONFIGURATION_H
