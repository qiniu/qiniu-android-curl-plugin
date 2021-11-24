//
// Created by yangsen on 2020/9/18.
//
#include <curl/curl.h>
#include "curl_utils.h"
#include "curl_context.h"

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

        const char *dnsResolverChar = env->GetStringUTFChars(dnsResolver, nullptr);
        if (dnsResolverChar != nullptr) {
            dnsResolverList = curl_slist_append(dnsResolverList, dnsResolverChar);
            env->ReleaseStringUTFChars(dnsResolver, dnsResolverChar);
        }

        env->DeleteLocalRef(dnsResolver);
    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(dnsResolverArray);

    return dnsResolverList;
}

std::string getJavaCurlConfigurationProxy(JNIEnv *env, jobject curlConfiguration) {
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

    std::string proxyString;
    jstring proxy = (jstring) env->CallObjectMethod(curlConfiguration, getProxy_method);
    if (proxy != nullptr) {
        char *proxyChar = const_cast<char *>(env->GetStringUTFChars(proxy, nullptr));
        if (proxyChar != nullptr) {
            proxyString = proxyChar;
            env->ReleaseStringUTFChars(proxy, proxyChar);
        }

    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(proxy);

    return proxyString;
}

std::string getJavaCurlConfigurationProxyUserPwd(JNIEnv *env, jobject curlConfiguration) {
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

    std::string userPwdString;
    jstring userPwd = (jstring) env->CallObjectMethod(curlConfiguration, getProxyUserPwd_method);
    if (userPwd != nullptr) {
        char *userPwdChar = const_cast<char *>(env->GetStringUTFChars(userPwd, nullptr));
        if (userPwdChar != nullptr) {
            userPwdString = userPwdChar;
            env->ReleaseStringUTFChars(userPwd, userPwdChar);
        }

    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(userPwd);

    return userPwdString;
}


std::string getJavaCurlConfigurationCAPath(JNIEnv *env, jobject curlConfiguration) {
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

    std::string caPathString;
    jstring caPath = (jstring) env->CallObjectMethod(curlConfiguration, getCAPath_method);
    if (caPath != nullptr) {
        char *caPathChar = const_cast<char *>(env->GetStringUTFChars(caPath, nullptr));
        if (caPathChar != nullptr) {
            caPathString = caPathChar;
            env->ReleaseStringUTFChars(caPath, caPathChar);
        }
    }

    env->DeleteLocalRef(config_class);
    env->DeleteLocalRef(caPath);

    return caPathString;
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