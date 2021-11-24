//
// Created by yangsen on 2020/10/26.
//

#include <string>
#include "curl_request.h"

std::string getJavaCurlRequestURL(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return nullptr;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return nullptr;
    }

    jmethodID getUrlString_method = env->GetMethodID(request_class,
                                                     "getUrlString",
                                                     "()Ljava/lang/String;");
    if (getUrlString_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return nullptr;
    }


    std::string urlString;
    auto url = (jstring) env->CallObjectMethod(curlRequest, getUrlString_method);
    if (url != nullptr) {
        char *urlChar = const_cast<char *>(env->GetStringUTFChars(url, nullptr));
        if (urlChar != nullptr) {
            urlString = urlChar;
            env->ReleaseStringUTFChars(url, urlChar);
        }
    }

    env->DeleteLocalRef(url);
    env->DeleteLocalRef(request_class);

    return urlString;
}

int getJavaCurlRequestHttpVersion(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return Curl_Request_Http_version_default;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return Curl_Request_Http_version_default;
    }

    jmethodID getHttpVersion_method = env->GetMethodID(request_class,
                                                      "getHttpVersion",
                                                      "()I");
    if (getHttpVersion_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return Curl_Request_Http_version_default;
    }

    int httpVersion = env->CallIntMethod(curlRequest, getHttpVersion_method);

    env->DeleteLocalRef(request_class);

    return httpVersion;
}

int getJavaCurlRequestHttpMethod(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return Curl_Request_Http_Method_GET;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return Curl_Request_Http_Method_GET;
    }

    jmethodID getHttpMethod_method = env->GetMethodID(request_class,
                                                      "getHttpMethod",
                                                      "()I");
    if (getHttpMethod_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return Curl_Request_Http_Method_GET;
    }

    int httpMethod = env->CallIntMethod(curlRequest, getHttpMethod_method);

    env->DeleteLocalRef(request_class);

    return httpMethod;
}

jobjectArray getJavaCurlRequestHeaderFields(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return nullptr;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return nullptr;
    }

    jmethodID getAllHeaderList_method = env->GetMethodID(request_class,
                                                         "getAllHeaderList",
                                                         "()[Ljava/lang/String;");
    if (getAllHeaderList_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return nullptr;
    }

    jobjectArray headerList = (jobjectArray) env->CallObjectMethod(curlRequest,
                                                                   getAllHeaderList_method);

    env->DeleteLocalRef(request_class);

    return headerList;
}

struct curl_slist * getJavaCurlRequestHeaderCList(JNIEnv *env, jobject curlRequest){
    if (env == nullptr || curlRequest == nullptr) {
        return nullptr;
    }

    jobjectArray requestHeader = getJavaCurlRequestHeaderFields(env, curlRequest);
    struct curl_slist *headerList = nullptr;
    int headSize = 0;
    if (requestHeader != nullptr) {
        headSize = env->GetArrayLength(requestHeader);
    }
    for (int i = 0; i < headSize; ++i) {
        auto headerField = (jstring) env->GetObjectArrayElement(requestHeader, i);
        const char *headerFieldChar = env->GetStringUTFChars(headerField, nullptr);
        if (headerFieldChar != nullptr) {
            // curl_slist_append 会 copy
            headerList = curl_slist_append(headerList, headerFieldChar);

            env->ReleaseStringUTFChars(headerField, headerFieldChar);
        }
    }

    if (requestHeader != nullptr) {
        env->DeleteLocalRef(requestHeader);
    }

    return headerList;
}

jbyteArray getJavaCurlRequestBody(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return nullptr;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return nullptr;
    }

    jmethodID getHttpBody_method = env->GetMethodID(request_class,
                                                    "getHttpBody",
                                                    "()[B");
    if (getHttpBody_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return nullptr;
    }

    jbyteArray body = (jbyteArray) env->CallObjectMethod(curlRequest, getHttpBody_method);

    env->DeleteLocalRef(request_class);

    return body;
}

long long getJavaCurlRequestBodyContentLength(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return 0;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return 0;
    }

    jmethodID getContentLength_method = env->GetMethodID(request_class,
                                                    "getContentLength",
                                                    "()J");
    if (getContentLength_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return 0;
    }

    jlong contentLength = env->CallLongMethod(curlRequest, getContentLength_method);

    env->DeleteLocalRef(request_class);

    return contentLength;
}

int getJavaCurlRequestTimeout(JNIEnv *env, jobject curlRequest) {
    if (env == nullptr || curlRequest == nullptr) {
        return 60;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == nullptr) {
        return 60;
    }

    jmethodID getTimeout_method = env->GetMethodID(request_class,
                                                   "getTimeout",
                                                   "()I");
    if (getTimeout_method == nullptr) {
        env->DeleteLocalRef(request_class);
        return 60;
    }

    int timeout = env->CallIntMethod(curlRequest, getTimeout_method);

    env->DeleteLocalRef(request_class);

    return timeout;
}

void setCurlContextWithRequest(JNIEnv *env, CurlContext *curlContext, jobject curlRequest){
    if (env == nullptr || curlRequest == nullptr) {
        return;
    }
    curlContext->url = getJavaCurlRequestURL(env, curlRequest);
    curlContext->requestMethod = getJavaCurlRequestHttpMethod(env, curlRequest);
    curlContext->requestTimeout = getJavaCurlRequestTimeout(env, curlRequest);
    curlContext->requestVersion = getJavaCurlRequestHttpVersion(env, curlRequest);
    curlContext->requestHeaderFields = getJavaCurlRequestHeaderCList(env, curlRequest);
    curlContext->body = getJavaCurlRequestBody(env, curlRequest);
    curlContext->totalBytesExpectedToSend = getJavaCurlRequestBodyContentLength(env, curlRequest);
}