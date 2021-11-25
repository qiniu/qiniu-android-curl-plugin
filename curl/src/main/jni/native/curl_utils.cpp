//
// Created by yangsen on 2020/10/22.
//

#include "curl_utils.h"
#include <curl/curl.h>

std::string getCurlVersion() {
    curl_version_info_data *versionInfo = curl_version_info(CURLVERSION_NOW);
    std::string version = "libcurl/";
    version.append(versionInfo->version);
    version.append(";nghttp2/");
    version.append(versionInfo->nghttp2_version);
    version.append(";");
    version.append(versionInfo->quic_version);
    return std::move(version);
}

// std::vector<char> to char*:   char *chars = (char*)char_vector.data()
std::vector<char> curlUtilConvertJByteArrayToChars(JNIEnv *env, jbyteArray byteArray) {
    if (env == nullptr) {
        return std::vector<char>();
    }

    jbyte *bytes = env->GetByteArrayElements(byteArray, 0);
    char *chars = (char *)bytes;
    int chars_len = env->GetArrayLength(byteArray);
    std::vector<char> char_vector(chars, chars + chars_len);

    env->ReleaseByteArrayElements(byteArray, bytes, 0);

    return char_vector;
}
