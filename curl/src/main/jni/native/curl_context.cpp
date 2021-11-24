//
// Created by yangsen on 2020/10/26.
//
#include <jni.h>
#include "curl_context.h"

CurlContext::CurlContext() {
}

CurlContext::~CurlContext() {
    if (dnsResolverArray != nullptr) {
        curl_slist_free_all(dnsResolverArray);
        dnsResolverArray = nullptr;
    }

    if (requestHeaderFields != nullptr) {
        curl_slist_free_all(requestHeaderFields);
        requestHeaderFields = nullptr;
    }

    if (responseHeaderFields != nullptr) {
        curl_slist_free_all(responseHeaderFields);
        responseHeaderFields = nullptr;
    }

    // jni
    if (env != nullptr) {
        if (metrics != nullptr) {
            env->DeleteLocalRef(metrics);
            metrics = nullptr;
        }

        if (body != nullptr) {
            env->DeleteLocalRef(body);
            body = nullptr;
        }
    }
}