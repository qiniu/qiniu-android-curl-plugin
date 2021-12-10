//
// Created by yangsen on 2020/9/18.
//

#ifndef CURLDEMO_CURL_JNI_CALL_BACK_H
#define CURLDEMO_CURL_JNI_CALL_BACK_H

//
// Created by yangsen on 2020/9/18.
//
#include <jni.h>
#include <string>
#include "curl_context.h"

void receiveResponse(CurlContext *curlContext, const std::string& url, int statusCode, const std::string& httpVersion,
                     struct curl_slist *headerFields);

size_t sendData(CurlContext *curlContext, char *buffer, long long dataLength);

size_t receiveData(CurlContext *curlContext, char *buffer, size_t size);

void completeWithError(CurlContext *curlContext, int errorCode, const char *errorInfo);

void sendProgress(CurlContext *curlContext, long long bytesSent, long long totalBytesSent, long long totalBytesExpectedToSend);

void receiveProgress(CurlContext *curlContext, long long bytesReceive, long long totalBytesReceive, long long totalBytesExpectedToReceive);

void didFinishCollectingMetrics(CurlContext *curlContext);

#endif //CURLDEMO_CURL_JNI_CALL_BACK_H
