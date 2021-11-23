//
// Created by yangsen on 2020/9/18.
//
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include "curl_jni_call_back.h"
#include "curl_context.h"

void receiveResponse(CurlContext *curlContext, char *url, int statusCode, char *httpVersion,
                     struct curl_slist *headerFields) {
    if (curlContext == nullptr) {
        return;
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return;
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return;
    }

    jmethodID receiveResponse_method = env->GetMethodID(handler_class,
                                                        "receiveResponse",
                                                        "(Ljava/lang/String;ILjava/lang/String;[Ljava/lang/Object;)V");
    if (receiveResponse_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return;
    }

    jstring url_string = nullptr;
    jstring httpVersion_string = nullptr;
    jobjectArray headerFieldArray = nullptr;

    if (url != nullptr) {
        url_string = env->NewStringUTF(url);
    }
    if (httpVersion != nullptr) {
        httpVersion_string = env->NewStringUTF(httpVersion);
    }
    if (headerFields != nullptr && headerFields->data != nullptr) {
        jsize size = 0;
        struct curl_slist *next_field = headerFields;
        while (next_field != nullptr) {
            size += 1;
            next_field = next_field->next;
        }

        jstring first = env->NewStringUTF(headerFields->data);
        headerFieldArray = env->NewObjectArray(size, env->FindClass("java/lang/String"), first);

        int index = 0;
        struct curl_slist *headerField = headerFields;
        while (headerField != nullptr && index < size) {
            if (headerField->data != nullptr){
                jstring value = env->NewStringUTF(headerField->data);
                if (value != nullptr){
                    env->SetObjectArrayElement(headerFieldArray, index, value);
                    index++;
                }
            }
            headerField = headerField->next;
        }
    }

    env->CallVoidMethod(curlHandler, receiveResponse_method, url_string, statusCode, httpVersion_string,
                        headerFieldArray);

    env->DeleteLocalRef(url_string);
    env->DeleteLocalRef(handler_class);
    env->DeleteLocalRef(httpVersion_string);
    if (headerFieldArray != nullptr) {
        for (int i = 0; i < env->GetArrayLength(headerFieldArray); ++i) {
            jobject e = env->GetObjectArrayElement(headerFieldArray, i);
            env->DeleteLocalRef(e);
        }
    }
    env->DeleteLocalRef(headerFieldArray);
}

size_t sendData(struct CurlContext *curlContext, char *buffer, long long dataLength) {
    if (curlContext == nullptr) {
        return static_cast<size_t>(-1);
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return static_cast<size_t>(-1);
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return static_cast<size_t>(-1);
    }

    jmethodID sendData_method = env->GetMethodID(handler_class,
                                                 "sendData",
                                                 "(J)[B");
    if (sendData_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return static_cast<size_t>(-1);
    }

    size_t readLength = 0;
    jbyteArray data = static_cast<jbyteArray>(env->CallObjectMethod(curlHandler, sendData_method,
                                                                    dataLength));
    if (data != nullptr) {
        readLength = static_cast<size_t>(env->GetArrayLength(data));

        jboolean isCopy;
        jbyte *bytes = env->GetByteArrayElements(data, &isCopy);
        memcpy(buffer, bytes, static_cast<size_t>(readLength));
        env->ReleaseByteArrayElements(data, bytes, isCopy);
    }
    env->DeleteLocalRef(handler_class);
    env->DeleteLocalRef(data);

    return readLength;
}

size_t receiveData(struct CurlContext *curlContext, char *buffer, size_t size) {
    if (curlContext == nullptr) {
        return static_cast<size_t>(-1);
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return static_cast<size_t>(-1);
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return static_cast<size_t>(-1);
    }

    jmethodID receiveData_method = env->GetMethodID(handler_class,
                                                    "receiveData",
                                                    "([B)V");
    if (receiveData_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return static_cast<size_t>(-1);
    }

    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion(data, 0, size, reinterpret_cast<const jbyte *>(buffer));

    env->CallVoidMethod(curlHandler, receiveData_method, data);

    env->DeleteLocalRef(handler_class);
    env->DeleteLocalRef(data);

    return size;
}

void completeWithError(struct CurlContext *curlContext, int errorCode, const char *errorInfo) {
    if (curlContext == nullptr) {
        return;
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return;
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return;
    }

    jmethodID completeWithError_method = env->GetMethodID(handler_class,
                                                          "completeWithError",
                                                          "(ILjava/lang/String;)V");
    if (completeWithError_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return;
    }

    jstring errorInfo_string = nullptr;
    if (errorInfo == nullptr) {
        errorInfo_string = env->NewStringUTF(errorInfo);
    }
    env->CallVoidMethod(curlHandler, completeWithError_method, errorCode, errorInfo_string);

    env->DeleteLocalRef(handler_class);
    env->DeleteLocalRef(errorInfo_string);
}

void sendProgress(struct CurlContext *curlContext, long long bytesSent, long long totalBytesSent,
                  long long totalBytesExpectedToSend) {
    if (curlContext == nullptr) {
        return;
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return;
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return;
    }

    jmethodID sendProgress_method = env->GetMethodID(handler_class,
                                                     "sendProgress",
                                                     "(JJJ)V");
    if (sendProgress_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return;
    }

    env->CallVoidMethod(curlHandler, sendProgress_method, bytesSent, totalBytesSent,
                        totalBytesExpectedToSend);

    env->DeleteLocalRef(handler_class);
}

void receiveProgress(struct CurlContext *curlContext, long long bytesReceive,
                     long long totalBytesReceive, long long totalBytesExpectedToReceive) {
    if (curlContext == nullptr) {
        return;
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return;
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return;
    }

    jmethodID receiveProgress_method = env->GetMethodID(handler_class,
                                                        "receiveProgress",
                                                        "(JJJ)V");
    if (receiveProgress_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return;
    }

    env->CallVoidMethod(curlHandler, receiveProgress_method, bytesReceive, totalBytesReceive,
                        totalBytesExpectedToReceive);

    env->DeleteLocalRef(handler_class);
}

void didFinishCollectingMetrics(struct CurlContext *curlContext) {
    if (curlContext == nullptr) {
        return;
    }

    jobject curlHandler = curlContext->curlHandler;
    JNIEnv *env = curlContext->env;
    if (env == nullptr || curlHandler == nullptr) {
        return;
    }

    jclass handler_class = env->FindClass("com/qiniu/client/curl/CurlHandler");
    if (handler_class == nullptr) {
        return;
    }

    jmethodID didFinishCollectingMetrics_method = env->GetMethodID(handler_class,
                                                                   "didFinishCollectingMetrics",
                                                                   "(Lcom/qiniu/client/curl/CurlTransactionMetrics;)V");
    if (didFinishCollectingMetrics_method == nullptr) {
        env->DeleteLocalRef(handler_class);
        return;
    }

    env->CallVoidMethod(curlHandler, didFinishCollectingMetrics_method, curlContext->metrics);

    env->DeleteLocalRef(handler_class);
}