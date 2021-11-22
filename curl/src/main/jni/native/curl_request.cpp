//
// Created by yangsen on 2020/10/26.
//

#include "stdlib.h"
#include "string.h"
#include "curl_request.h"

char * getJavaCurlRequestURL(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return NULL;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return NULL;
    }

    jmethodID getUrlString_method = env->GetMethodID(request_class,
                                                     "getUrlString",
                                                     "()Ljava/lang/String;");
    if (getUrlString_method == NULL) {
        env->DeleteLocalRef(request_class);
        return NULL;
    }


    char *url_char = NULL;
    jstring url = (jstring) env->CallObjectMethod(curlRequest, getUrlString_method);
    if (url != NULL) {
        jboolean isCopy;
        char *url_char_p = const_cast<char *>(env->GetStringUTFChars(url, &isCopy));
        if (url_char_p != NULL) {
            size_t userPwd_char_size = strlen(url_char_p);
            url_char = (char *) malloc(userPwd_char_size);
            memset(url_char, '\0', userPwd_char_size);
            strcpy(url_char, url_char_p);

            env->ReleaseStringUTFChars(url, url_char_p);
        }
    }

    env->DeleteLocalRef(url);
    env->DeleteLocalRef(request_class);

    return url_char;
}

int getJavaCurlRequestHttpVersion(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return Curl_Request_Http_version_default;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return Curl_Request_Http_version_default;
    }

    jmethodID getHttpVersion_method = env->GetMethodID(request_class,
                                                      "getHttpVersion",
                                                      "()I");
    if (getHttpVersion_method == NULL) {
        env->DeleteLocalRef(request_class);
        return Curl_Request_Http_version_default;
    }

    int httpVersion = env->CallIntMethod(curlRequest, getHttpVersion_method);

    env->DeleteLocalRef(request_class);

    return httpVersion;
}

int getJavaCurlRequestHttpMethod(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return Curl_Request_Http_Method_GET;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return Curl_Request_Http_Method_GET;
    }

    jmethodID getHttpMethod_method = env->GetMethodID(request_class,
                                                      "getHttpMethod",
                                                      "()I");
    if (getHttpMethod_method == NULL) {
        env->DeleteLocalRef(request_class);
        return Curl_Request_Http_Method_GET;
    }

    int httpMethod = env->CallIntMethod(curlRequest, getHttpMethod_method);

    env->DeleteLocalRef(request_class);

    return httpMethod;
}

jobjectArray getJavaCurlRequestHeaderFields(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return NULL;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return NULL;
    }

    jmethodID getAllHeaderList_method = env->GetMethodID(request_class,
                                                         "getAllHeaderList",
                                                         "()[Ljava/lang/String;");
    if (getAllHeaderList_method == NULL) {
        env->DeleteLocalRef(request_class);
        return NULL;
    }

    jobjectArray headerList = (jobjectArray) env->CallObjectMethod(curlRequest,
                                                                   getAllHeaderList_method);

    env->DeleteLocalRef(request_class);

    return headerList;
}

struct curl_slist * getJavaCurlRequestHeaderCList(JNIEnv *env, jobject curlRequest){
    if (env == NULL || curlRequest == NULL) {
        return NULL;
    }

    jobjectArray requestHeader = getJavaCurlRequestHeaderFields(env, curlRequest);
    struct curl_slist *headerList = NULL;
    int headSize = 0;
    if (requestHeader != NULL) {
        headSize = env->GetArrayLength(requestHeader);
    }
    for (int i = 0; i < headSize; ++i) {
        jstring headerField = (jstring) env->GetObjectArrayElement(requestHeader, i);
        const char *headerField_char = env->GetStringUTFChars(headerField, NULL);
        if (headerField_char != NULL) {
            size_t headerField_char_size = strlen(headerField_char);
            char *headerField_char_cp = (char *) malloc(headerField_char_size);
            memset(headerField_char_cp, '\0', headerField_char_size);
            strcpy(headerField_char_cp, headerField_char);

            headerList = curl_slist_append(headerList, headerField_char_cp);

            env->ReleaseStringUTFChars(headerField, headerField_char);
        }
    }

    if (requestHeader != NULL) {
        env->DeleteLocalRef(requestHeader);
    }

    return headerList;
}

jbyteArray getJavaCurlRequestBody(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return NULL;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return NULL;
    }

    jmethodID getHttpBody_method = env->GetMethodID(request_class,
                                                    "getHttpBody",
                                                    "()[B");
    if (getHttpBody_method == NULL) {
        env->DeleteLocalRef(request_class);
        return NULL;
    }

    jbyteArray body = (jbyteArray) env->CallObjectMethod(curlRequest, getHttpBody_method);

    env->DeleteLocalRef(request_class);

    return body;
}

long long getJavaCurlRequestBodyContentLength(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return 0;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return 0;
    }

    jmethodID getContentLength_method = env->GetMethodID(request_class,
                                                    "getContentLength",
                                                    "()J");
    if (getContentLength_method == NULL) {
        env->DeleteLocalRef(request_class);
        return 0;
    }

    jlong contentLength = env->CallLongMethod(curlRequest, getContentLength_method);

    env->DeleteLocalRef(request_class);

    return contentLength;
}

int getJavaCurlRequestTimeout(JNIEnv *env, jobject curlRequest) {
    if (env == NULL || curlRequest == NULL) {
        return 60;
    }

    jclass request_class = env->FindClass("com/qiniu/client/curl/CurlRequest");
    if (request_class == NULL) {
        return 60;
    }

    jmethodID getTimeout_method = env->GetMethodID(request_class,
                                                   "getTimeout",
                                                   "()I");
    if (getTimeout_method == NULL) {
        env->DeleteLocalRef(request_class);
        return 60;
    }

    int timeout = env->CallIntMethod(curlRequest, getTimeout_method);

    env->DeleteLocalRef(request_class);

    return timeout;
}

void setCurlContextWithRequest(JNIEnv *env, CurlContext *curlContext, jobject curlRequest){
    if (env == NULL || curlRequest == NULL) {
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