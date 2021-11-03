//
// Created by yangsen on 2020/10/26.
//
#include <jni.h>
#include "curl_context.h"

void releaseCurlContext(CurlContext * context){

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
}