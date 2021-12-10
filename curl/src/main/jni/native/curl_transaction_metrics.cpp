//
// Created by yangsen on 2020/9/18.
//

#include "curl_transaction_metrics.h"
#include <jni.h>
#include "curl_context.h"

jobject createJavaMetrics(CurlContext *curlContext) {
    if (curlContext == nullptr) {
        return nullptr;
    }

    JNIEnv *env = curlContext->env;
    if (env == nullptr) {
        return nullptr;
    }

    jclass metrics_class = env->FindClass("com/qiniu/client/curl/CurlTransactionMetrics");
    if (metrics_class == nullptr) {
        return nullptr;
    }

    jmethodID init_method = env->GetMethodID(metrics_class,
                                             "<init>",
                                             "()V");
    if (init_method == nullptr) {
        env->DeleteLocalRef(metrics_class);
        return nullptr;
    }

    jobject object = env->NewObject(metrics_class, init_method);

    env->DeleteLocalRef(metrics_class);

    return object;
}

void setJavaMetricsStringField(CurlContext *curlContext, const char *fieldName, char *fieldValue) {
    if (curlContext == nullptr || fieldValue == nullptr) {
        return;
    }

    JNIEnv *env = curlContext->env;
    jobject metrics = curlContext->metrics;
    if (env == nullptr || metrics == nullptr) {
        return;
    }

    jclass metrics_class = env->FindClass("com/qiniu/client/curl/CurlTransactionMetrics");
    if (metrics_class == nullptr) {
        return;
    }

    jmethodID set_method = env->GetMethodID(metrics_class, fieldName, "(Ljava/lang/String;)V");

    if (set_method == nullptr) {
        env->DeleteLocalRef(metrics_class);
        return;
    }

    env->CallVoidMethod(metrics, set_method, env->NewStringUTF(fieldValue));

    env->DeleteLocalRef(metrics_class);
}

void
setJavaMetricsLongField(CurlContext *curlContext, const char *fieldName, long long fieldValue) {
    if (curlContext == nullptr) {
        return;
    }

    JNIEnv *env = curlContext->env;
    jobject metrics = curlContext->metrics;
    if (env == nullptr || metrics == nullptr) {
        return;
    }

    jclass metrics_class = env->FindClass("com/qiniu/client/curl/CurlTransactionMetrics");
    if (metrics_class == nullptr) {
        return;
    }

    jmethodID set_method = env->GetMethodID(metrics_class, fieldName, "(J)V");

    if (set_method == nullptr) {
        env->DeleteLocalRef(metrics_class);
        return;
    }

    env->CallVoidMethod(metrics, set_method, fieldValue);

    env->DeleteLocalRef(metrics_class);
}

void setJavaMetricsVoidField(CurlContext *curlContext, const char *fieldName) {
    if (curlContext == nullptr) {
        return;
    }

    JNIEnv *env = curlContext->env;
    jobject metrics = curlContext->metrics;
    if (env == nullptr || metrics == nullptr) {
        return;
    }

    jclass metrics_class = env->FindClass("com/qiniu/client/curl/CurlTransactionMetrics");
    if (metrics_class == nullptr) {
        return;
    }

    jmethodID set_method = env->GetMethodID(metrics_class, fieldName, "()V");

    if (set_method == nullptr) {
        env->DeleteLocalRef(metrics_class);
        return;
    }

    env->CallVoidMethod(metrics, set_method);

    env->DeleteLocalRef(metrics_class);
}

void setJavaMetricsCountOfRequestHeaderBytesSent(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setCountOfRequestHeaderBytesSent", fieldValue);
}

void setJavaMetricsCountOfRequestBodyBytesSent(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setCountOfRequestBodyBytesSent", fieldValue);
}

void
setJavaMetricsCountOfResponseHeaderBytesReceived(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setCountOfResponseHeaderBytesReceived", fieldValue);
}

void
setJavaMetricsCountOfResponseBodyBytesReceived(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setCountOfResponseBodyBytesReceived", fieldValue);
}

void setJavaMetricsLocalAddress(CurlContext *curlContext, char *fieldValue) {
    setJavaMetricsStringField(curlContext, "setLocalAddress", fieldValue);
}

void setJavaMetricsLocalPort(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setLocalPort", fieldValue);
}

void setJavaMetricsRemoteAddress(CurlContext *curlContext, char *fieldValue) {
    setJavaMetricsStringField(curlContext, "setRemoteAddress", fieldValue);
}

void setJavaMetricsRemotePort(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setRemotePort", fieldValue);
}

void setJavaMetricsStartTimestamp(CurlContext *curlContext) {
    setJavaMetricsVoidField(curlContext, "setStartTimestamp");
}

void setJavaMetricsNameLookupTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setNameLookupTime", fieldValue);
}

void setJavaMetricsConnectTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setConnectTime", fieldValue);
}

void setJavaMetricsAppConnectTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setAppConnectTime", fieldValue);
}

void setJavaMetricsPreTransferTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setPreTransferTime", fieldValue);
}

void setJavaMetricsStartTransferTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setStartTransferTime", fieldValue);
}

void setJavaMetricsTotalTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setTotalTime", fieldValue);
}

void setJavaMetricsRedirectTime(CurlContext *curlContext, long long fieldValue) {
    setJavaMetricsLongField(curlContext, "setRedirectTime", fieldValue);
}