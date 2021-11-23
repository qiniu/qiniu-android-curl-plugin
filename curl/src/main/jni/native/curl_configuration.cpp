//
// Created by yangsen on 2020/9/18.
//
#include <curl/curl.h>
#include "curl_utils.h"
#include "curl_context.h"
#include <stdlib.h>
#include <string.h>

struct curl_slist * getJavaCurlConfigurationDnsResolverArray(JNIEnv *env, jobject curlConfiguration) {
    if (env == nullptr || curlConfiguration == nullptr) {
        return nullptr;
    }

    jclass config_class = env->FindClass("com/qiniu/client/curl/CurlConfiguration");
    if (config_class == nullptr) {
        return nullptr;
    }

    jmethodID getDnsResolverArray_method = env->GetMethodID(config_class,
                                                            "getDnsResolverArray",
                                                            "()[Ljava/lang/String;");
    if (getDnsResolverArray_method == nullptr) {
        env->DeleteLocalRef(config_class);
        return nullptr;
    }
    jobjectArray dnsResolverArray = (jobjectArray) env->CallObjectMethod(curlConfiguration,
                                                                         getDnsResolverArray_method);

    struct curl_slist *dnsResolverList = nullptr;
    int dnsResolverListSize = 0;
    if (dnsResolverArray != nullptr) {
        dnsResolverListSize = env->GetArrayLength(dnsResolverArray);
    }
    for (int i = 0; i < dnsResolverListSize; ++i) {
        jstring dnsResolver = (jstring) env->GetObjectArrayElement(dnsResolverArray, i);

        jboolean isCopy;
        const char *dnsResolver_char = env->GetStringUTFChars(dnsResolver, &isCopy);
        if (dnsResolver_char != nullptr) {
            size_t dnsResolver_char_size = strlen(dnsResolver_char);
            char *dnsResolver_char_cp = (char *) malloc(dnsResolver_char_size);
            memset(dnsResolver_char_cp, '\0', dnsResolver_char_size);
            strcpy(dnsResolver_char_cp, dnsResolver_char);

            dnsResolverList = curl_slist_append(dnsResolverList, dnsResolver_char_cp);

            env->ReleaseStringUTFChars(dnsResolver, dnsResolver_char);
        }

        env->DeleteLocalRef(dnsResolver);
    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(dnsResolverArray);

    return dnsResolverList;
}

char * getJavaCurlConfigurationProxy(JNIEnv *env, jobject curlConfiguration) {
    if (env == nullptr || curlConfiguration == nullptr) {
        return nullptr;
    }

    jclass config_class = env->FindClass("com/qiniu/client/curl/CurlConfiguration");
    if (config_class == nullptr) {
        return nullptr;
    }

    jmethodID getProxy_method = env->GetMethodID(config_class,
                                                 "getProxy",
                                                 "()Ljava/lang/String;");
    if (getProxy_method == nullptr) {
        env->DeleteLocalRef(config_class);
        return nullptr;
    }

    char *proxy_char = nullptr;
    jstring proxy = (jstring) env->CallObjectMethod(curlConfiguration, getProxy_method);
    if (proxy != nullptr) {
        jboolean isCopy;
        char *proxy_char_p = const_cast<char *>(env->GetStringUTFChars(proxy, &isCopy));
        if (proxy_char_p != nullptr) {
            size_t proxy_char_size = strlen(proxy_char_p);
            proxy_char = (char *) malloc(proxy_char_size);
            memset(proxy_char, '\0', proxy_char_size);
            strcpy(proxy_char, proxy_char_p);

            env->ReleaseStringUTFChars(proxy, proxy_char_p);
        }

    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(proxy);

    return proxy_char;
}

char *getJavaCurlConfigurationProxyUserPwd(JNIEnv *env, jobject curlConfiguration) {
    if (env == nullptr || curlConfiguration == nullptr) {
        return nullptr;
    }

    jclass config_class = env->FindClass("com/qiniu/client/curl/CurlConfiguration");
    if (config_class == nullptr) {
        return nullptr;
    }

    jmethodID getProxyUserPwd_method = env->GetMethodID(config_class,
                                                        "getProxyUserPwd",
                                                        "()Ljava/lang/String;");
    if (getProxyUserPwd_method == nullptr) {
        env->DeleteLocalRef(config_class);
        return nullptr;
    }

    char *userPwd_char = nullptr;
    jstring userPwd = (jstring) env->CallObjectMethod(curlConfiguration, getProxyUserPwd_method);
    if (userPwd != nullptr) {
        jboolean isCopy;
        char *userPwd_char_p = const_cast<char *>(env->GetStringUTFChars(userPwd, &isCopy));
        if (userPwd_char_p != nullptr) {
            size_t userPwd_char_size = strlen(userPwd_char_p);
            userPwd_char = (char *) malloc(userPwd_char_size);
            memset(userPwd_char, '\0', userPwd_char_size);
            strcpy(userPwd_char, userPwd_char_p);

            env->ReleaseStringUTFChars(userPwd, userPwd_char_p);
        }

    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(userPwd);

    return userPwd_char;
}


char *getJavaCurlConfigurationCAPath(JNIEnv *env, jobject curlConfiguration) {
    if (env == nullptr || curlConfiguration == nullptr) {
        return nullptr;
    }

    jclass config_class = env->FindClass("com/qiniu/client/curl/CurlConfiguration");
    if (config_class == nullptr) {
        return nullptr;
    }

    jmethodID getCAPath_method = env->GetMethodID(config_class,
                                                  "getCAPath",
                                                  "()Ljava/lang/String;");
    if (getCAPath_method == nullptr) {
        env->DeleteLocalRef(config_class);
        return nullptr;
    }

    char *caPath_char = nullptr;
    jstring caPath = (jstring) env->CallObjectMethod(curlConfiguration, getCAPath_method);
    if (caPath != nullptr) {
        jboolean isCopy;
        char *caPath_char_p = const_cast<char *>(env->GetStringUTFChars(caPath, &isCopy));
        if (caPath_char_p != nullptr) {
            size_t userPwd_char_size = strlen(caPath_char_p);
            caPath_char = (char *) malloc(userPwd_char_size);
            memset(caPath_char, '\0', userPwd_char_size);
            strcpy(caPath_char, caPath_char_p);

            env->ReleaseStringUTFChars(caPath, caPath_char_p);
        }
    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(caPath);

    return caPath_char;
}

void setCurlContextWithConfiguration(JNIEnv *env, CurlContext *curlContext, jobject curlConfiguration){
    if (env == nullptr) {
        return;
    }

    curlContext->proxy = getJavaCurlConfigurationProxy(env, curlConfiguration);
    curlContext->proxyUserPwd = getJavaCurlConfigurationProxyUserPwd(env, curlConfiguration);
    curlContext->caPath = getJavaCurlConfigurationCAPath(env, curlConfiguration);
    curlContext->dnsResolverArray = getJavaCurlConfigurationDnsResolverArray(env, curlConfiguration);
}