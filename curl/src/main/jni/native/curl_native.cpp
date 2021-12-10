//
// Created by yangsen on 2020/9/9.
//
#include "curl_native.h"
#include "curl_context.h"
#include "curl_transaction_metrics.h"
#include "curl_configuration.h"
#include "curl_request.h"
#include "curl_jni_call_back.h"
#include "curl_error.h"
#include "curl_utils.h"

#include <jni.h>
#include <mutex>
#include <android/log.h>
#include <curl/curl.h>
#include <ctime>

#ifdef ANDROID
#define TAG "CurlLibrary"
#define kCurlLogD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__)
#define kCurlLogI(...) __android_log_print(ANDROID_LOG_INFO,TAG  ,__VA_ARGS__)
#define kCurlLogW(...) __android_log_print(ANDROID_LOG_WARN,TAG  ,__VA_ARGS__)
#define kCurlLogE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__)
#define kCurlLogF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__)
#else
#define kCurlLogD(...) printf(__VA_ARGS__)
#define kCurlLogI(...) printf(__VA_ARGS__)
#define kCurlLogW(...) printf(__VA_ARGS__)
#define kCurlLogE(...) printf(__VA_ARGS__)
#define kCurlLogF(...) printf(__VA_ARGS__)
#endif

#define qn_curl_easy_setopt(handle, opt, param, errorCode, errorInfo, error_desc) \
{ \
    CURLcode codeNew = curl_easy_setopt(handle,opt,param); \
    if (codeNew != CURLE_OK) { \
        *errorCode = codeNew; \
        *errorInfo = error_desc; \
        return; \
    } \
}

//-------------------------------------------- Curl filed ------------------------------------------
bool curlJavaIsCancel(CurlContext *curlContext) {
    if (curlContext == nullptr) {
        return false;
    }

    JNIEnv *env = curlContext->env;
    jobject curl = curlContext->curlObj;
    if (env == nullptr || curl == nullptr) {
        return false;
    }

    jclass curl_class = env->FindClass("com/qiniu/client/curl/Curl");
    if (curl_class == nullptr) {
        return false;
    }

    jmethodID is_cancel_method = env->GetMethodID(curl_class, "isCancelled", "()Z");
    jboolean isCancel = env->CallBooleanMethod(curl, is_cancel_method);

    env->DeleteLocalRef(curl_class);

    return isCancel == JNI_TRUE;
}

//--------------------------------------------- CallBack -------------------------------------------
int CurlDebugCallback(CURL *curl, curl_infotype infoType, char *info, size_t infoLen,
                      void *contextInfo) {

    (void) curl; /* prevent compiler warning */
    (void) contextInfo;

    switch (infoType) {
        case CURLINFO_TEXT:
//            fprintf(stderr, "=> Text: %s", info);
            break;
        case CURLINFO_HEADER_IN:
//            kCurlLogD("<= Recv header: %s", info);
            break;
        case CURLINFO_HEADER_OUT:
//            kCurlLogD("==> Send header: %s", info);
            break;
        case CURLINFO_DATA_IN:
//            kCurlLogD("=> Recv data");
            break;
        case CURLINFO_DATA_OUT:
//            kCurlLogD("=> Send data");
            break;
        case CURLINFO_SSL_DATA_IN:
//            kCurlLogD("<= Recv SSL data: %s", info);
            break;
        case CURLINFO_SSL_DATA_OUT:
//            kCurlLogD("=> Send SSL data: %s", info);
            break;
        default: /* in case a new one is introduced to shock us */
            return 0;
    }

    return 0;
}

size_t CurlReceiveHeaderCallback(char *buffer, size_t size, size_t nitems, void *userData) {
    const size_t sizeInBytes = size * nitems;
    auto *curlContext = (CurlContext *) userData;
    curlContext->responseHeaderFields = curl_slist_append(curlContext->responseHeaderFields,
                                                          buffer);
    return sizeInBytes;
}

size_t CurlReadCallback(void *ptr, size_t size, size_t nmemb, void *userData) {
    size_t sizeInBytes = size * nmemb;
    CurlContext *curlContext = (CurlContext *) userData;
    sizeInBytes = sendData(curlContext, (char *) ptr, sizeInBytes);
    return sizeInBytes;
}

size_t CurlWriteCallback(char *ptr, size_t size, size_t nmemb, void *userData) {
    const size_t sizeInBytes = size * nmemb;
    auto *curlContext = (CurlContext *) userData;
    return receiveData(curlContext, ptr, sizeInBytes);
}

int CurlProgressCallback(void *client, double downloadTotal, double downloadNow, double uploadTotal,
                         double uploadNow) {
    CurlContext *curlContext = (CurlContext *) client;

    curlContext->totalBytesExpectedToSend = (long long) uploadTotal;
    auto sendBodyLength = (long long) ((long long) uploadNow - curlContext->totalBytesSent);
    curlContext->totalBytesSent = (long long) uploadNow;
    if (sendBodyLength > 0) {
        sendProgress(curlContext, sendBodyLength, (long long) uploadNow, (long long) uploadTotal);
    }

    curlContext->totalBytesExpectedToReceive = (long long) downloadTotal;
    auto receiveBodyLength = (long long) ((long long) downloadNow - curlContext->totalBytesReceive);
    curlContext->totalBytesReceive = (long long) downloadNow;
    if (receiveBodyLength > 0) {
        receiveProgress(curlContext, receiveBodyLength, (long long) downloadNow,
                        (long long) downloadTotal);
    }

    if (curlJavaIsCancel(curlContext)) {
        return -999;
    } else {
        return 0;
    }
}

//--------------------------------------------- INTER ----------------------------------------------
void initCurlRequestDefaultOptions(CURL *curl, CurlContext *curlContext, CURLcode *errorCode,
                                   const char **errorInfo) {

    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

    // 注：与库对接需要
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);
    curl_easy_setopt(curl, CURLOPT_ACCEPTTIMEOUT_MS, 5000L);
    curl_easy_setopt(curl, CURLOPT_HAPPY_EYEBALLS_TIMEOUT_MS, 300L);

    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPIDLE, 10L);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPINTVL, 2L);
    curl_easy_setopt(curl, CURLOPT_TCP_FASTOPEN, 1L);

    curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    if (curlContext != nullptr) {
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, curlContext->requestVersion);
    }


    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, CurlDebugCallback);
    curl_easy_setopt(curl, CURLOPT_DEBUGDATA, curlContext);

    qn_curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, CurlReceiveHeaderCallback, errorCode,
                        errorInfo, "header function set 0 error");
    qn_curl_easy_setopt(curl, CURLOPT_HEADERDATA, curlContext, errorCode, errorInfo,
                        "header function set 1 error");

    qn_curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L, errorCode, errorInfo,
                        "progress function set 0 error");
    qn_curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, CurlProgressCallback, errorCode, errorInfo,
                        "progress function set 1 error");
    qn_curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, curlContext, errorCode, errorInfo,
                        "progress function set 2 error");
}

void initCurlRequestDownloadData(CURL *curl, CurlContext *curlContext, CURLcode *errorCode,
                                 const char **errorInfo) {
    if (curlContext == nullptr) {
        return;
    }
    qn_curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback, errorCode, errorInfo,
                        "write function set 0 error");
    qn_curl_easy_setopt(curl, CURLOPT_WRITEDATA, curlContext, errorCode, errorInfo,
                        "write function set 1 error");
}

void initCurlRequestCustomOptions(CURL *curl, CurlContext *curlContext) {
    if (curlContext == nullptr) {
        return;
    }

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, curlContext->requestTimeout);

    std::string caPath = curlContext->caPath;
    if (!caPath.empty()) {
        curl_easy_setopt(curl, CURLOPT_CAINFO, caPath.c_str());
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
        curl_easy_setopt(curl, CURLOPT_SSL_CIPHER_LIST, "ALL");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    } else {
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
}

void initCurlDnsResolver(CURL *curl, CurlContext *curlContext) {
    if (curlContext != nullptr && curlContext->dnsResolverArray != nullptr) {
//        kCurlLogD("== Dns resolver:%s", curlContext->dnsResolverArray->data);
        curl_easy_setopt(curl, CURLOPT_RESOLVE, curlContext->dnsResolverArray);
    }
}

void initCurlRequestHeader(CURL *curl, CurlContext *curlContext, CURLcode *errorCode,
                           const char **errorInfo) {
    if (curlContext != nullptr && curlContext->requestHeaderFields != nullptr) {
        qn_curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlContext->requestHeaderFields, errorCode,
                            errorInfo,
                            "header set error");
    }
}

void initCurlRequestUrl(CURL *curl, CurlContext *curlContext, CURLcode *errorCode,
                        const char **errorInfo) {
    if (curlContext != nullptr && !curlContext->url.empty()) {
        qn_curl_easy_setopt(curl, CURLOPT_URL, curlContext->url.c_str(), errorCode, errorInfo,
                            "url set error");
    }
}

void
initCurlRequestMethodAndRequestData(CURL *curl, CurlContext *curlContext,
                                    CURLcode *errorCode, const char **errorInfo) {
    if (curlContext == nullptr) {
        return;
    }
    long long totalBytesExpectedToSend = curlContext->totalBytesExpectedToSend;
    int httpMethod = curlContext->requestMethod;
    if (httpMethod == Curl_Request_Http_Method_HEAD) {
        qn_curl_easy_setopt(curl, CURLOPT_NOBODY, 1, errorCode, errorInfo, "HEAD Option set error");
    } else if (httpMethod == Curl_Request_Http_Method_GET) {
        qn_curl_easy_setopt(curl, CURLOPT_HTTPGET, 1, errorCode, errorInfo, "GET Option set error");
    } else if (httpMethod == Curl_Request_Http_Method_POST) {
        qn_curl_easy_setopt(curl, CURLOPT_POST, 1, errorCode, errorInfo, "POST Option set error");
        if (totalBytesExpectedToSend > 0) {
            qn_curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, totalBytesExpectedToSend, errorCode,
                                errorInfo,
                                "body length set error");
//        qn_curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlUtilConvertJByteArrayToChars(curlContext->env, curlContext->body), errorCode, errorInfo,
//                            "body set error");
        }
        qn_curl_easy_setopt(curl, CURLOPT_READFUNCTION, CurlReadCallback, errorCode, errorInfo,
                            "read function set 1 error");
        qn_curl_easy_setopt(curl, CURLOPT_READDATA, curlContext, errorCode, errorInfo,
                            "read function set 2 error");
    } else if (httpMethod == Curl_Request_Http_Method_PUT) {
        qn_curl_easy_setopt(curl, CURLOPT_UPLOAD, 1, errorCode, errorInfo,
                            "PUT Option set error");
        if (totalBytesExpectedToSend > 0) {
            qn_curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, totalBytesExpectedToSend, errorCode,
                                errorInfo,
                                "body length set error");
        }
        qn_curl_easy_setopt(curl, CURLOPT_READFUNCTION, CurlReadCallback, errorCode, errorInfo,
                            "read function set 1 error");
        qn_curl_easy_setopt(curl, CURLOPT_READDATA, curlContext, errorCode, errorInfo,
                            "read function set 2 error");
    } else if (httpMethod == Curl_Request_Http_Method_DELETE) {
        qn_curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE", errorCode, errorInfo,
                            "DELETE Option set error");
    } else {
        *errorCode = CURLE_FAILED_INIT;
        *errorInfo = "method set error";
    }
}

void initCurlRequestProxy(CURL *curl, CurlContext *curlContext) {
    if (curlContext != nullptr && !curlContext->proxy.empty()) {
        curl_easy_setopt(curl, CURLOPT_PROXY, curlContext->proxy.c_str());
    }
}

void performRequest(CURL *curl, CURLcode *errorCode, const char **errorInfo) {
    char errBuffer[CURL_ERROR_SIZE];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errBuffer);

    CURLcode code = curl_easy_perform(curl);
    if (code != CURLE_OK) {
        *errorInfo = "curl request perform error";
    }
    *errorCode = code;
}

void handleResponse(CurlContext *curlContext, CURL *curl) {

    if (curlContext == nullptr || curl == nullptr) {
        return;
    }

    long statusCode = 0;
    long httpVersion = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    curl_easy_getinfo(curl, CURLINFO_HTTP_VERSION, &httpVersion);
    bool isCancel = curlJavaIsCancel(curlContext);
    if (isCancel) {
        statusCode = -999;
    }

    std::string httpVersionString;
    if (httpVersion == CURL_HTTP_VERSION_1_0) {
        httpVersionString = "HTTP/1.0";
    } else if (httpVersion == CURL_HTTP_VERSION_1_1) {
        httpVersionString = "HTTP/1.1";
    } else if (httpVersion == CURL_HTTP_VERSION_2_0 ||
               httpVersion == CURL_HTTP_VERSION_2TLS ||
               httpVersion == CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE) {
        httpVersionString = "HTTP/2";
    } else if (httpVersion == CURL_HTTP_VERSION_3) {
        httpVersionString = "HTTP/3";
    } else {
        httpVersionString = "";
    }
    receiveResponse(curlContext, curlContext->url, statusCode, httpVersionString,
                    curlContext->responseHeaderFields);
}

void handleMetrics(CurlContext *curlContext, CURL *curl) {
    if (curl == nullptr) {
        return;
    }

    long localPort;
    long remotePort;
    char *localIP = nullptr;
    char *remoteIP = nullptr;
    curl_easy_getinfo(curl, CURLINFO_LOCAL_PORT, &localPort);
    curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &localIP);
    curl_easy_getinfo(curl, CURLINFO_PRIMARY_PORT, &remotePort);
    curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &remoteIP);

    setJavaMetricsLocalPort(curlContext, localPort);
    setJavaMetricsLocalAddress(curlContext, localIP);
    setJavaMetricsRemotePort(curlContext, remotePort);
    setJavaMetricsRemoteAddress(curlContext, remoteIP);
    free(localIP);
    free(remoteIP);

    curl_off_t total_time, name_lookup_time, connect_time, app_connect_time,
            pre_transfer_time, start_transfer_time, redirect_time, redirect_count;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total_time);
    curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME_T, &name_lookup_time);
    curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME_T, &connect_time);
    curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME_T, &app_connect_time);
    curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME_T, &pre_transfer_time);
    curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME_T, &start_transfer_time);
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_TIME_T, &redirect_time);
    curl_easy_getinfo(curl, CURLINFO_REDIRECT_COUNT, &redirect_count);

//    kCurlLogD("total_time:%ld, name_lookup_time:%ld, connect_time:%ld, app_connect_time:%ld, pre_transfer_time:%ld, start_transfer_time:%ld, redirect_time:%ld, redirect_count:%ld",
//              (long)total_time, (long)name_lookup_time, (long)connect_time, (long)app_connect_time, (long)pre_transfer_time, (long)start_transfer_time, (long)redirect_time, (long)redirect_count);

    setJavaMetricsTotalTime(curlContext, total_time);
    setJavaMetricsNameLookupTime(curlContext, name_lookup_time);
    setJavaMetricsConnectTime(curlContext, connect_time);
    setJavaMetricsAppConnectTime(curlContext, app_connect_time);
    setJavaMetricsPreTransferTime(curlContext, pre_transfer_time);
    setJavaMetricsStartTransferTime(curlContext, start_transfer_time);
    setJavaMetricsRedirectTime(curlContext, redirect_time);

    curl_off_t request_header_size, request_body_size, response_header_size, response_body_size;
    if (curlContext->requestHeaderFields != nullptr) {
        struct curl_slist *next_headerField = curlContext->requestHeaderFields;
        long long size = 0;
        while (next_headerField != nullptr) {
            if (next_headerField->data != nullptr) {
                size += strlen(next_headerField->data);
            }
            next_headerField = next_headerField->next;
        }
        request_header_size = size;
    } else {
        request_header_size = 0;
    }

    curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &request_body_size);
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &response_body_size);
    curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &response_header_size);

    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_UPLOAD_T, &request_body_size);
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &response_body_size);

    setJavaMetricsCountOfRequestHeaderBytesSent(curlContext, request_header_size);
    setJavaMetricsCountOfRequestBodyBytesSent(curlContext, request_body_size);
    setJavaMetricsCountOfResponseHeaderBytesReceived(curlContext, response_header_size);
    setJavaMetricsCountOfResponseBodyBytesReceived(curlContext, response_body_size);

    curl_off_t protocol;
    curl_easy_getinfo(curl, CURLINFO_PROTOCOL, &protocol);

    didFinishCollectingMetrics(curlContext);
}

//---------------------------------------------- JNI -----------------------------------------------
extern "C" JNIEXPORT jlong JNICALL
Java_com_qiniu_client_curl_Curl_globalInit(JNIEnv *env, jobject obj) {
    return curl_global_init(CURL_GLOBAL_ALL);
}

static std::once_flag version_once_flag;
static std::string version;
extern "C" JNIEXPORT jstring JNICALL
Java_com_qiniu_client_curl_Curl_getCurlVersion(JNIEnv *env, jclass clazz) {
    if (env == nullptr) {
        return nullptr;
    }

    std::call_once(version_once_flag, []() {
        version = getCurlVersion();
    });

    if (version.empty()) {
        return env->NewStringUTF("");
    } else {
        return env->NewStringUTF(version.c_str());
    }
}

extern "C" JNIEXPORT void JNICALL Java_com_qiniu_client_curl_Curl_requestNative(JNIEnv *env,
                                                                                jobject curlObj,
                                                                                jobject curlRequest,
                                                                                jobject configure,
                                                                                jobject curlHandler) {


    CURLcode errorCode = CURLE_OK;
    const char *errorInfo = nullptr;

    // context
    CurlContext curlContext;
    curlContext.env = env;
    curlContext.curlObj = curlObj;
    curlContext.curlHandler = curlHandler;
    curlContext.responseHeaderFields = nullptr;
    curlContext.metrics = createJavaMetrics(&curlContext);
    curlContext.totalBytesSent = 0;
    curlContext.totalBytesReceive = 0;
    curlContext.totalBytesExpectedToSend = 0;
    curlContext.totalBytesExpectedToReceive = 0;

    setCurlContextWithRequest(env, &curlContext, curlRequest);
    setCurlContextWithConfiguration(env, &curlContext, configure);

    // start time
    setJavaMetricsStartTimestamp(&curlContext);

    CURL *curl = curl_easy_init();
    if (curl == nullptr) {
        goto curl_perform_complete_error;
    }

    initCurlRequestDefaultOptions(curl, &curlContext, &errorCode,
                                  reinterpret_cast<const char **>(&errorInfo));
    if (errorInfo != nullptr) {
        goto curl_perform_complete_error;
    }
    initCurlRequestCustomOptions(curl, &curlContext);

    initCurlRequestDownloadData(curl, &curlContext, &errorCode,
                                reinterpret_cast<const char **>(&errorInfo));
    if (errorInfo != nullptr) {
        goto curl_perform_complete_error;
    }
    initCurlDnsResolver(curl, &curlContext);
    initCurlRequestProxy(curl, &curlContext);
    initCurlRequestHeader(curl, &curlContext, &errorCode,
                          reinterpret_cast<const char **>(&errorInfo));
    if (errorInfo != nullptr) {
        goto curl_perform_complete_error;
    }
    initCurlRequestUrl(curl, &curlContext, &errorCode, reinterpret_cast<const char **>(&errorInfo));
    if (errorInfo != nullptr) {
        goto curl_perform_complete_error;
    }
    initCurlRequestMethodAndRequestData(curl, &curlContext, &errorCode,
                                        reinterpret_cast<const char **>(&errorInfo));
    if (errorInfo != nullptr) {
        goto curl_perform_complete_error;
    }

    performRequest(curl, &errorCode, reinterpret_cast<const char **>(&errorInfo));

    handleResponse(&curlContext, curl);

    curl_perform_complete_error:
    handleMetrics(&curlContext, curl);

    completeWithError(&curlContext, transformCurlStatusCode(errorCode),
                      reinterpret_cast<const char *>(&errorInfo));

    if (curl != nullptr) {
        curl_easy_cleanup(curl);
    }
}

