//
// Created by yangsen on 2020/10/22.
//

#include "curl_utils.h"
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <curl/curl.h>

char *getCurlVersion() {
    curl_version_info_data *versionInfo = curl_version_info(CURLVERSION_NOW);
    const char *libcurlLabel = "libcurl:";
    const char *libcurlVersion = versionInfo->version;
    const char *nghttp2Label = " nghttp2:";
    const char *nghttp2Version = versionInfo->nghttp2_version;
    const char *quicLabel = " ";
    const char *quicVersion = versionInfo->quic_version;

    size_t len = 0;
    len += strlen(libcurlLabel) + strlen(libcurlVersion);
    len += strlen(nghttp2Label) + strlen(nghttp2Version);
    len += strlen(quicLabel) + strlen(quicVersion);
    char *version = static_cast<char *>(malloc(len));
    if (version == nullptr) {
        return nullptr;
    }
    version = strcpy(version, libcurlLabel);
    version = strcat(version, libcurlVersion);
    version = strcat(version, nghttp2Label);
    version = strcat(version, nghttp2Version);
    version = strcat(version, quicLabel);
    version = strcat(version, quicVersion);
    return version;
}

// 返回的char *需要delete
char* curlUtilConvertJByteArrayToChars(JNIEnv *env, jbyteArray byteArray) {
    if (env == NULL) {
        return 0;
    }

    char *chars = NULL;
    jbyte *bytes = env->GetByteArrayElements(byteArray, 0);

    int chars_len = env->GetArrayLength(byteArray);
    chars = new char[chars_len + 1];
    memset(chars, 0, static_cast<size_t>(chars_len + 1));
    memcpy(chars, bytes, static_cast<size_t>(chars_len));
    chars[chars_len] = 0;

    env->ReleaseByteArrayElements(byteArray, bytes, 0);

    return chars;
}
