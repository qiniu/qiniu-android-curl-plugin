//
// Created by yangsen on 2020/9/18.
//

#include <jni.h>
#include "string.h"
#include "curl/curl.h"
#ifndef CURLDEMO_CURL_CONTEXT_H
#define CURLDEMO_CURL_CONTEXT_H

struct CurlContext {
    // 外部引入，内存无需释放
    JNIEnv *env;
    jobject curlObj;
    jobject curlHandler;
    jbyteArray body;
    int requestVersion;
    int requestMethod;
    int requestTimeout;

    long long totalBytesSent;
    long long totalBytesExpectedToSend;
    long long totalBytesReceive;
    long long totalBytesExpectedToReceive;

    // 内部创建，内存需要释放
    char * url;
    char * proxy;
    char * proxyUserPwd;
    char * caPath;
    struct curl_slist * dnsResolverArray;
    struct curl_slist *requestHeaderFields;
    struct curl_slist *responseHeaderFields;

    jobject metrics;
};

void releaseCurlContext(CurlContext * context);

#endif //CURLDEMO_CURL_CONTEXT_H
