//
// Created by yangsen on 2020/9/18.
//

#include <jni.h>
#include "curl_context.h"

#ifndef CURLDEMO_CURL_TRANSACTION_METRICS_H
#define CURLDEMO_CURL_TRANSACTION_METRICS_H

jobject createJavaMetrics(CurlContext *curlContext);

void setJavaMetricsCountOfRequestHeaderBytesSent(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsCountOfRequestBodyBytesSent(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsCountOfResponseHeaderBytesReceived(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsCountOfResponseBodyBytesReceived(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsLocalAddress(CurlContext *curlContext, char *fieldValue);

void setJavaMetricsLocalPort(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsRemoteAddress(CurlContext *curlContext, char *fieldValue);

void setJavaMetricsRemotePort(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsStartTimestamp(CurlContext *curlContext);

void setJavaMetricsNameLookupTime(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsConnectTime(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsAppConnectTime(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsPreTransferTime(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsStartTransferTime(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsTotalTime(CurlContext *curlContext, long long fieldValue);

void setJavaMetricsRedirectTime(CurlContext *curlContext, long long fieldValue);

#endif //CURLDEMO_CURL_TRANSACTION_METRICS_H
