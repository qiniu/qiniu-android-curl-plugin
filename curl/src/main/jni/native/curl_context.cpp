//
// Created by yangsen on 2020/10/26.
//
#include <jni.h>
#include "stdlib.h"
#include "curl_context.h"

void releaseCurlContext(CurlContext * context){

    if (context->dnsResolverArray != nullptr){
        curl_slist_free_all(context->dnsResolverArray);
        context->dnsResolverArray = nullptr;
    }

    if (context->requestHeaderFields != nullptr){
        curl_slist_free_all(context->requestHeaderFields);
        context->requestHeaderFields = nullptr;
    }

    if (context->responseHeaderFields != nullptr){
        curl_slist_free_all(context->responseHeaderFields);
        context->responseHeaderFields = nullptr;
    }

    if (context->metrics != nullptr){
        context->env->DeleteLocalRef(context->metrics);
        context->metrics = nullptr;
    }

    // C
    if (context->url != nullptr) {
        free(context->url);
    }

    if (context->proxy != nullptr) {
        free(context->proxy);
    }

    if (context->proxyUserPwd != nullptr) {
        free(context->proxyUserPwd);
    }

    if (context->caPath != nullptr) {
        free(context->caPath);
    }

    // jni
    if (context->env != nullptr) {
        JNIEnv *env = context->env;
        if (context->body != nullptr) {
            env->DeleteLocalRef(context->body);
        }
    }
}