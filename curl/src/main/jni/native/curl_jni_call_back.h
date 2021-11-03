//
// Created by yangsen on 2020/9/18.
//

#ifndef CURLDEMO_CURL_JNI_CALL_BACK_H
#define CURLDEMO_CURL_JNI_CALL_BACK_H

//
// Created by yangsen on 2020/9/18.
//
#include <jni.h>
#include "curl_context.h"

void receiveResponse(CurlContext *curlContext, char *url, int statusCode, char *httPVersion,
                     struct curl_slist *headerFields);

size_t sendData(struct CurlContext *curlContext, char *buffer, long long dataLength);

size_t receiveData(struct CurlContext *curlContext, char *buffer, size_t size);

void completeWithError(struct CurlContext *curlContext, int errorCode, const char *errorInfo);

void sendProgress(struct CurlContext *curlContext, long long bytesSent, long long totalBytesSent, long long totalBytesExpectedToSend);

void receiveProgress(struct CurlContext *curlContext, long long bytesReceive, long long totalBytesReceive, long long totalBytesExpectedToReceive);

void didFinishCollectingMetrics(struct CurlContext *curlContext);

#endif //CURLDEMO_CURL_JNI_CALL_BACK_H
