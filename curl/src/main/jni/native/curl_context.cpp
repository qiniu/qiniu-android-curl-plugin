//
// Created by yangsen on 2020/10/26.
//
#include <jni.h>
#include "stdlib.h"
#include "curl_context.h"

void releaseCurlContext(CurlContext * context){

    if (context->dnsResolverArray != NULL){
        curl_slist_free_all(context->dnsResolverArray);
        context->dnsResolverArray = NULL;
    }

    if (context->requestHeaderFields != NULL){
        curl_slist_free_all(context->requestHeaderFields);
        context->requestHeaderFields = NULL;
    }

    if (context->responseHeaderFields != NULL){
        curl_slist_free_all(context->responseHeaderFields);
        context->responseHeaderFields = NULL;
    }

    if (context->metrics != NULL){
        context->env->DeleteLocalRef(context->metrics);
        context->metrics = NULL;
    }

    // C
    if (context->url != NULL) {
        free(context->url);
    }

    if (context->proxy != NULL) {
        free(context->proxy);
    }

    if (context->proxyUserPwd != NULL) {
        free(context->proxyUserPwd);
    }

    if (context->caPath != NULL) {
        free(context->caPath);
    }

    // jni
    if (context->env != NULL) {
        JNIEnv *env = context->env;
        if (context->body != NULL) {
            env->DeleteLocalRef(context->body);
        }
    }
}